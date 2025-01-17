#include <math.h>
#include <unordered_map>
#include <vector>
#include <memory>


#include <raylib.h>

#include <LDtkLoader/World.hpp>

#include <Constants.hpp>
#include <utils/DebugUtils.hpp>

#include "Player.hpp"
#include "../../physics/PhysicsTypes.hpp"
#include "../../scenes/GameScene/GameScene.hpp"
#include "../../physics/RaycastUtils.hpp"

using namespace std;

std::unique_ptr<Weapon> Player::Sword = nullptr;
std::unique_ptr<Weapon> Player::Magic = nullptr;


Player::Player()
{
	// Load player sprite
	this->sprite = LoadTexture(AppConstants::GetAssetPath("Characters/fHero_.png").c_str());

	auto make_player_frame_rect = [](float frame_num) -> Rectangle
		{
			return {
				.x = frame_num * 24.0f,
				.y = 0.0f,
				.width = 24.0f,
				.height = 24.0f };
		};

	animation_map[IDLE] = {
		make_player_frame_rect(0),
		make_player_frame_rect(1),
		make_player_frame_rect(2),
	};

	animation_map[WALK] = {
		make_player_frame_rect(3),
		make_player_frame_rect(4),
		make_player_frame_rect(5),
	};

	animation_map[JUMP_START] = {
		make_player_frame_rect(6),
	};

	animation_map[JUMP_APEX] = {
		make_player_frame_rect(7),
	};

	animation_map[JUMP_FALL] = {
		make_player_frame_rect(8),
	};
}

Player::~Player()
{
	UnloadTexture(this->sprite);
}

void Player::update(float dt)
{
	animation_ticker -= dt;
	if (animation_ticker <= 0)
	{
		animation_ticker = animation_frame_duration;
		current_anim_frame += 1;
	}

	check_if_move();
	check_if_should_respawn();
	update_camera();

	// Handle player weapons
	Sword->update(dt);
	Magic->update(dt);
}

void Player::update_camera()
{
	if (body)
	{
		// Get world position
		auto worldPos = body->GetPosition();

		// Update camera target to follow player in world coordinates
		camera.follow(body->GetPosition());
		camera.update();
	}
}

void Player::draw()
{
	if (!body) return;

	auto spritePosX = (body->GetPosition().x * GameConstants::PhysicsWorldScale) - 12;
	auto spritePosY = (body->GetPosition().y * GameConstants::PhysicsWorldScale) - 13;

	auto current_anim_states = animation_map[anim_state];
	auto current_anim_rect = current_anim_states[current_anim_frame % current_anim_states.size()];

	if (!looking_right)
	{
		current_anim_rect.width *= -1;
	}

	DrawTexturePro(sprite,
		current_anim_rect,
		{ spritePosX, spritePosY, 24, 24 },
		{ 0, 0 },
		0.0f,
		WHITE);
	// Draw player collision box
	if (GameConstants::debugModeCollision)
	{
		// Draw debug collision box
		auto bodyPos = body->GetPosition();
		// Get the box dimensions from the fixture (assuming the first fixture is the box)
		b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
		Vector2 boxSize = {
			polygonShape->m_vertices[2].x * 2 * GameConstants::PhysicsWorldScale, // width
			polygonShape->m_vertices[2].y * 2 * GameConstants::PhysicsWorldScale  // height
		};

		// Draw the physics body rectangle
		DrawRectangleLines(
			(bodyPos.x * GameConstants::PhysicsWorldScale) - (boxSize.x / 2),
			(bodyPos.y * GameConstants::PhysicsWorldScale) - (boxSize.y / 2),
			boxSize.x,
			boxSize.y,
			RED
		);
	}

	// Weapons draw
	Sword->updatePosition(spritePosX, spritePosY, looking_right);
	Sword->draw();

	Magic->updatePosition(spritePosX, spritePosY, looking_right);
	Magic->draw();

}

void Player::init_for_level(const ldtk::Entity* entity, b2World* physicsWorld)
{
	auto pos = entity->getPosition();

	DebugUtils::println("Setting player position to x:{} and y:{}", pos.x, pos.y);

	// Convert LDTK position to physics world position
	level_spawn_position = {
		static_cast<float>(pos.x) / GameConstants::PhysicsWorldScale,
		static_cast<float>(pos.y) / GameConstants::PhysicsWorldScale
	};

	// Adjust player collision box to later me  
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(level_spawn_position.x, level_spawn_position.y);

	this->body = physicsWorld->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.3, 0.4);



	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = .0001f;
	fixtureDef.friction = 0.1f;

	body->CreateFixture(&fixtureDef);

	// Manage weapons

	intializeInventory(physicsWorld);

}

void Player::set_velocity_x(float vx)
{
	body->SetLinearVelocity({
		vx,
		body->GetLinearVelocity().y,
		});
}

void Player::set_velocity_y(float vy)
{
	body->SetLinearVelocity({
		body->GetLinearVelocity().x,
		vy,
		});
}

void Player::set_velocity_xy(float vx, float vy)
{
	body->SetLinearVelocity({ vx, vy });
}

void Player::check_if_move()
{
	const auto effective_speed = 6.0f * GameConstants::scale;

	float vx = 0.0f;
	float vy = 0.0f;

	if (IsKeyDown(KEY_A))
	{
		vx -= effective_speed;
		looking_right = false;
	}

	if (IsKeyDown(KEY_D))
	{
		vx += effective_speed;
		looking_right = true;
	}

	if (IsKeyDown(KEY_W))
	{
		vy -= effective_speed;
	}

	if (IsKeyDown(KEY_S))
	{
		vy += effective_speed;
	}

	set_velocity_xy(vx, vy);
}

void Player::check_if_should_respawn()
{
	// Placeholder for respawn logic
}

Vector2 Player::get_position()
{
	if (body)
	{
		auto pos = body->GetPosition();
		return Vector2{
			pos.x * GameConstants::PhysicsWorldScale,
			pos.y * GameConstants::PhysicsWorldScale
		};
	}
	return Vector2{
		level_spawn_position.x * GameConstants::PhysicsWorldScale,
		level_spawn_position.y * GameConstants::PhysicsWorldScale
	};
}


void Player::intializeInventory(b2World* physicsWorld)
{
	// Load the texture for the sword
	/*   weaponTexture,
		 positonBuffer,
		 activationRotaion,
		 textureGrid,
		 attackSpeed,
		 attackReloadSpeed,
		 attackWaitTime,
		 isMagicWeapon,
		 selectedWeaponFromTileset*/
	Texture2D swordTexture = LoadTexture(AppConstants::GetAssetPath("Weapons/WeaponGem.png").c_str());
	Vector2 swordPosBuffer{ 5.f,22.f };
	Sword = std::make_unique<Weapon>(
		swordTexture,
		swordPosBuffer,
		-140.f,
		4, 
		0.1f,
		2.1f,
		0.2f,
		false,
		1,
		physicsWorld
	);

	// Load the texture for the magic
	Texture2D magicTexture = LoadTexture(AppConstants::GetAssetPath("Weapons/weapons_.png").c_str());
	Vector2 magicPosBuffer{ 28.f,22.f };
	Magic = std::make_unique<Weapon>(magicTexture,
		magicPosBuffer,
		90.f,
		5,
		0.2f,
		0.8f,
		0.1f,
		true,
		18,
		physicsWorld
	);
}

Weapon* Player::getWeapon()
{
	return Sword.get();
}