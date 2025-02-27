#include <box2d/box2d.h>
#include <fmt/core.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>
#include <memory>
#include <raylib.h>
#include <ranges>

#include <Constants.hpp>
#include <utils/DebugUtils.hpp>

#include "../../physics/PhysicsTypes.hpp"
#include "../Scenes.hpp"
#include "GameScene.hpp"

#include "./entities/BaseEntity.hpp"
#include"./entities/Enemies/EnemySpawner.hpp"
#include <entities/Camera/Camera.hpp>

#include"GameSceneUI.hpp"
#include"utils/SoundManager.hpp"

using namespace std;

std::unique_ptr<Player> GameScene::player = nullptr;
std::unique_ptr<b2World> GameScene::world = nullptr;
GameSceneUI gameSceneUI;


GameScene::GameScene()
{
	player = std::make_unique<Player>();
	ldtkProject = std::make_unique<ldtk::Project>();

	ldtkProject->loadFromFile(AppConstants::GetAssetPath("BladeAndStoneMap.ldtk"));

	ldtkWorld = &ldtkProject->getWorld();

	// Initialize sound manager and play start sound
	auto soundManager = SoundManager::getInstance();
	soundManager->initialize();
	soundManager->playSoundEffect("gameStart");  // Play start sound when scene first loads

	current_level = -1;
	setSelectedLevel(0);
}

GameScene::~GameScene()
{
	UnloadTexture(renderedLevelTexture);
	UnloadTexture(currentTilesetTexture);
}

Scenes GameScene::update(float dt)
{
	const float timeStep = 1.0f / 60.0f;
	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;
	auto soundManager = SoundManager::getInstance();

	// Update physics world
	world->Step(timeStep, velocityIterations, positionIterations);

	// Store previous player state
	bool wasAlive = isPlayerAlive;
	isPlayerAlive = player->isAlive();

	// Check if player just died this frame
	bool playerJustDied = wasAlive && !isPlayerAlive;
	if (playerJustDied)
	{
		soundManager->playSoundEffect("gameOver");
	}

	// Handle dead player state
	if (!isPlayerAlive)
	{
		if (IsKeyDown(KEY_SPACE))
		{
			return Scenes::GAME;
		}
	}

	player->update(dt);

	for (auto& enemySpawner : enemySpawners)
	{
		enemySpawner->update(dt, player->getPosition());
		CheckCollisions(enemySpawner);
	}

	

	return Scenes::NONE;
}

void GameScene::draw()
{
	ClearBackground(RAYWHITE);

	

	BeginMode2D(player->get_camera().get_camera());



	DrawTextureRec(renderedLevelTexture,
		{ 0, 0, (float)renderedLevelTexture.width, (float)-renderedLevelTexture.height },
		{ 0, 0 }, WHITE);
	
	// Draw solid blocks debug outlines
	if (GameConstants::debugModeCollision)
	{
		if (world)
		{
			for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
			{
				if (body->GetUserData().pointer &&
					(const char*)body->GetUserData().pointer == PhysicsTypes::SolidBlock)
				{
					b2Fixture* fixture = body->GetFixtureList();
					if (fixture)
					{
						b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();
						if (polygonShape)
						{
							b2Vec2 position = body->GetPosition();
							float posX = position.x * GameConstants::PhysicsWorldScale;
							float posY = position.y * GameConstants::PhysicsWorldScale;


							float width = polygonShape->m_vertices[2].x * 2 * GameConstants::PhysicsWorldScale;
							float height = polygonShape->m_vertices[2].y * 2 * GameConstants::PhysicsWorldScale;

						
							DrawRectangleLines(
								posX - (width / 2),
								posY - (height / 2),
								width,
								height,
								GREEN 
							);
						}
					}
				}
			}
		}
	}

	
	// Draw EnnemySpawner
	for (auto& enemySpawner : enemySpawners)
	{
		enemySpawner->DrawEnemies();
	}

	// Draw Player (Draw player at last to render always on top)
	player->draw();

	EndMode2D();

	// Draw Scene UI (Draw UI outside 2D mode to not affect by camera)
	gameSceneUI.drawGameUI(player->getHealth(), killedEnemies);

	if (!isPlayerAlive)
	{
		gameSceneUI.drawGameEndUI();

	}
}

void GameScene::setSelectedLevel(int lvl)
{
	if (current_level >= 0)
	{
		UnloadTexture(currentTilesetTexture);
	}

	if (world != nullptr)
	{
		world = nullptr;
	}

	b2Vec2 gravity(0.0f, 0.0f);
	world = std::make_unique<b2World>(gravity);

	current_level = lvl;

	currentLdtkLevel = &ldtkWorld->getLevel(current_level);

	DebugUtils::println("----------------------------------------------");
	DebugUtils::println("Loaded LDTK map with {} levels in it", ldtkWorld->allLevels().size());
	DebugUtils::println("The loaded level is {} and it has {} layers", current_level, currentLdtkLevel->allLayers().size());
	for (auto&& layer : currentLdtkLevel->allLayers())
	{
		DebugUtils::print("  - {}", layer.getName());
	}

	auto testTileLayerTileset = currentLdtkLevel->getLayer("Ground").getTileset();

	DebugUtils::println("The path to the tile layer tileset is: {}", testTileLayerTileset.path);
	DebugUtils::println("----------------------------------------------");

	auto levelSize = currentLdtkLevel->size;
	auto renderTexture = LoadRenderTexture(levelSize.x, levelSize.y);

	BeginTextureMode(renderTexture);

	if (currentLdtkLevel->hasBgImage())
	{
		DebugUtils::println("Drawing background image");
		auto backgroundPath = currentLdtkLevel->getBgImage();
		auto backgroundTexture = LoadTexture(AppConstants::GetAssetPath(backgroundPath.path.c_str()).c_str());
		SetTextureFilter(backgroundTexture, TEXTURE_FILTER_TRILINEAR);

		for (int i = 0; i <= (GameConstants::WorldWidth / backgroundTexture.width); i++)
		{
			for (int j = 0; j <= (GameConstants::WorldHeight / backgroundTexture.height); j++)
			{
				DrawTextureV(backgroundTexture, { float(i * backgroundTexture.width), float(j * backgroundTexture.height) }, WHITE);
			}
		}
	}

	for (auto&& layer : std::ranges::reverse_view(currentLdtkLevel->allLayers()))
	{
		if (layer.hasTileset())
		{
			currentTilesetTexture = LoadTexture(AppConstants::GetAssetPath(layer.getTileset().path).c_str());

			for (auto&& tile : layer.allTiles())
			{
				auto source_pos = tile.getTextureRect();
				auto tile_size = float(layer.getTileset().tile_size);

				Rectangle source_rect = {
					.x = float(source_pos.x),
					.y = float(source_pos.y),
					.width = tile.flipX ? -tile_size : tile_size,
					.height = tile.flipY ? -tile_size : tile_size,
				};

				Vector2 target_pos = {
					(float)tile.getPosition().x,
					(float)tile.getPosition().y,
				};
				DrawTextureRec(currentTilesetTexture, source_rect, target_pos, WHITE);

				if (layer.getTileset().hasTagsEnum())
				{
					const ldtk::Enum& tags_enum = layer.getTileset().getTagsEnum();

					const std::string tag_to_find = "Ground";
					if (tags_enum.hasTag(tag_to_find))
					{
						const ldtk::EnumValue& enum_value = tags_enum["Solid"];
						const std::vector<int>& tile_ids = layer.getTileset().getTilesWithTagEnum(enum_value);

						if (std::find(tile_ids.begin(), tile_ids.end(), tile.tileId) != tile_ids.end())
						{
							
							if (GameConstants::debugModeCollision) DrawRectangle(target_pos.x, target_pos.y, tile_size, tile_size, Fade(RED, 0.5f));
							createSolidBlock(target_pos.x, target_pos.y, tile_size);
						}
					}
				}
			}
		}
	}

	EndTextureMode();
	renderedLevelTexture = renderTexture.texture;
	DebugUtils::println("Entities in level:");
	for (auto&& entity : currentLdtkLevel->getLayer("Actors").allEntities())
	{
		DebugUtils::println("  - {}", entity.getName());
		if (entity.getName() == "Player")
		{
			player->initForLevel(&entity, world.get());
		}

		if (entity.getName() == "Portal")
		{
			float target_lvl = entity.getField<float>("level_destination").value();
			DebugUtils::println("Portal goes to level: {}", target_lvl);
		}

		if (entity.getName() == "EnemySpawner")
		{
			//Create Enemy Spawners
			int spawnRate = entity.getField<int>("spawnRate").value();
			int spawnAmount = entity.getField<int>("spawnAmount").value();
			int spawnType = entity.getField<int>("spawnType").value();
			float posX = entity.getPosition().x;
			float posY = entity.getPosition().y;
			DebugUtils::println("EnemySpawner created at position ({}, {})", posX, posY);

			enemySpawners.emplace_back(std::make_unique<EnemySpawner>(spawnRate, spawnAmount, spawnType, posX, posY, world.get()));

		}
	}
	DebugUtils::println("Loading solid blocks in level:");
	for (auto&& entity : currentLdtkLevel->getLayer("Collision").allEntities())
	{
		auto b2width = entity.getSize().x / 2.0f;
		auto b2height = entity.getSize().y / 2.0f;

		auto centerX = entity.getPosition().x + b2width;
		auto centerY = entity.getPosition().y + b2height;

		//create_solid_block(centerX, centerY, b2width, b2height);
	}
}

void GameScene::createSolidBlock(float targetX, float targetY, float tileSize)
{
	float halfSize = tileSize / 2.0f;
	float centerX = targetX + halfSize;
	float centerY = targetY + halfSize;

	b2BodyDef bodyDef;
	bodyDef.userData.pointer = (uintptr_t)PhysicsTypes::SolidBlock.c_str();
	bodyDef.position.Set(
		centerX / GameConstants::PhysicsWorldScale,
		centerY / GameConstants::PhysicsWorldScale
	);

	b2Body* body = world->CreateBody(&bodyDef);
	b2PolygonShape groundBox;
	// Slightly reduce the collision box size to prevent edge catching
	groundBox.SetAsBox(
		(tileSize / 2.0f - 0.1f) / GameConstants::PhysicsWorldScale,
		(tileSize / 2.0f - 0.1f) / GameConstants::PhysicsWorldScale
	);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &groundBox;
	fixtureDef.density = 1.f;
	// Reduced friction to make sliding along walls smoother
	fixtureDef.friction = 0.1f;
	// Add some restitution to prevent sticking
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = PhysicsTypes::Categories::SOLID;
	fixtureDef.filter.maskBits = PhysicsTypes::Categories::PLAYER |
		PhysicsTypes::Categories::ENEMY;

	body->CreateFixture(&fixtureDef);
}

void GameScene::CheckCollisions(std::unique_ptr<EnemySpawner>& enemySpawner)
{

	if (!isPlayerAlive)	return;

	auto& enemies = enemySpawner->getEnemies();

	for (auto it = enemies.begin(); it != enemies.end();)
	{
		auto& enemy = *it;

		// Check collision with the player
		if (player->checkCollisionWithEnemy(enemy->getHitbox()))
		{
			player->getDamage(GameConstants::enemyDamage);
		}

		// Check collision with the player's weapon
		if (enemy->checkCollisionWithWeapon(player->getWeapon()->getHitbox()))
		{
			enemy->onHit(GameConstants::swordDamage);
			if (!enemy->isAlive())
			{
				it = enemies.erase(it);
				killedEnemies++;
				continue;
			}
		}

		// Check collision with bullets
		for (const auto& bulletHitbox : player->getBulletManager()->getBulletHitboxes())
		{
			if (enemy->checkCollisionWithWeapon(*bulletHitbox))
			{
				enemy->onHit(GameConstants::magicDamage);
				if (!enemy->isAlive())
				{
					it = enemies.erase(it);
					killedEnemies++;
					goto next_enemy;
				}
			}
		}

		++it;
	next_enemy:;
	}
}