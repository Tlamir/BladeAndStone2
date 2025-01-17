#include <memory>
#include <raylib.h>
#include <box2d/box2d.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>
#include <fmt/core.h>

#include <Constants.hpp>
#include <utils/DebugUtils.hpp>

#include "GameScene.hpp"
#include "../../physics/PhysicsTypes.hpp"
#include "../Scenes.hpp"

#include "./entities/BaseEntity.hpp"
#include <entities/Camera/Camera.hpp>
#include"./entities/Enemies/EnemySpawner.hpp"

using namespace std;

std::unique_ptr<Player> GameScene::player = nullptr;
std::unique_ptr<b2World> GameScene::world = nullptr;


GameScene::GameScene()
{
	player = std::make_unique<Player>();
	ldtkProject = std::make_unique<ldtk::Project>();

	ldtkProject->loadFromFile(AppConstants::GetAssetPath("BladeAndStoneMap.ldtk"));

	ldtkWorld = &ldtkProject->getWorld();

	current_level = -1;
	set_selected_level(0);
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

	world->Step(timeStep, velocityIterations, positionIterations);
	player->update(dt);

	for (auto& enemySpawner : enemySpawners)
	{
		enemySpawner->update(dt, player->get_position());
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
				// Check if this is a solid block by checking the user data
				if (body->GetUserData().pointer &&
					(const char*)body->GetUserData().pointer == PhysicsTypes::SolidBlock)
				{
					// Get the fixture (assuming one fixture per body for solid blocks)
					b2Fixture* fixture = body->GetFixtureList();
					if (fixture)
					{
						b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();
						if (polygonShape)
						{
							// Calculate the world position of the body
							b2Vec2 position = body->GetPosition();
							float posX = position.x * GameConstants::PhysicsWorldScale;
							float posY = position.y * GameConstants::PhysicsWorldScale;

							// Get the box dimensions
							float width = polygonShape->m_vertices[2].x * 2 * GameConstants::PhysicsWorldScale;
							float height = polygonShape->m_vertices[2].y * 2 * GameConstants::PhysicsWorldScale;

							// Draw the debug rectangle
							DrawRectangleLines(
								posX - (width / 2),  // Center the rectangle on the body position
								posY - (height / 2),
								width,
								height,
								GREEN  // Using green to distinguish from player's debug box
							);
						}
					}
				}
			}
		}
	}

	// Draw Player
	player->draw();
	// Draw EnnemySpawner
	for (auto& enemySpawner : enemySpawners)
	{
		enemySpawner->DrawEnemies();
	}


	EndMode2D();
}
void GameScene::set_selected_level(int lvl)
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

	for (auto&& layer : currentLdtkLevel->allLayers())
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
							// Draw Enum locations if mode
							if (GameConstants::debugModeCollision) DrawRectangle(target_pos.x, target_pos.y, tile_size, tile_size, Fade(RED, 0.5f));
							create_solid_block(target_pos.x, target_pos.y, tile_size);
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
			player->init_for_level(&entity, world.get());
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
			//cout << "RATE: " << spawnRate << " AMOUNT: " << spawnAmount << " TYPE: " << spawnType << endl;
			// Extract position from the entity
			float posX = entity.getPosition().x;
			float posY = entity.getPosition().y;
			DebugUtils::println("EnemySpawner created at position ({}, {})", posX, posY);
			// Pass the position to the EnemySpawner
			 // Create the spawner with 
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

void GameScene::create_solid_block(float targetX, float targetY, float tileSize)
{
	// Convert to Box2D coordinates (center-based)
	float halfSize = tileSize / 2.0f;

	// Adjust position to be at the center of the tile
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
	groundBox.SetAsBox(
		(tileSize / 2.0f) / GameConstants::PhysicsWorldScale,
		(tileSize / 2.0f) / GameConstants::PhysicsWorldScale
	);

	body->CreateFixture(&groundBox, 0.0f);
}


