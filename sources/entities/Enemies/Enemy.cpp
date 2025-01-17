#include "Enemy.hpp"
#include <Constants.hpp>
#include <iostream>

#include"../Player/Player.hpp"
#include <physics/PhysicsTypes.hpp>
Enemy::Enemy(Texture2D sprite)
	:sprite(sprite)
{
	initializeAnimations();
}

Enemy::~Enemy()
{
	// This destroctor handled by enemytextureloader 
	//UnloadTexture(this->sprite);
}

void Enemy::initializeAnimations()
{
	// Lambda function to create a frame rectangle
	auto make_enemy_frame_rect = [](float frame_num) -> Rectangle {
		return {
			.x = frame_num * 24.0f,  // Assuming 24x24 frames on the sprite sheet
			.y = 0.0f,                
			.width = 24.0f,
			.height = 24.0f
		};
		};

	// Idle Animation (Frames 0-3, First 4 frames of the first row)
	animation_map[ENEMY_IDLE] = {
		make_enemy_frame_rect(0),
		make_enemy_frame_rect(1),
		make_enemy_frame_rect(2),
		make_enemy_frame_rect(3)
	};

	// Walk Animation (Frames 8-11, First 4 frames of the second row)
	animation_map[ENEMY_WALK] = {
		make_enemy_frame_rect(8),
		make_enemy_frame_rect(9),
		make_enemy_frame_rect(10),
		make_enemy_frame_rect(11)
	};

	// Attack Animation (Frames 16-19, First 4 frames of the third row)
	animation_map[ENEMY_ATTACK] = {
		make_enemy_frame_rect(16),
		make_enemy_frame_rect(17),
		make_enemy_frame_rect(18),
		make_enemy_frame_rect(19)
	};

	// Jump Animation (Frames 24-27, First 4 frames of the fourth row)
	animation_map[ENEMY_JUMP] = {
		make_enemy_frame_rect(24),
		make_enemy_frame_rect(25),
		make_enemy_frame_rect(26),
		make_enemy_frame_rect(27)
	};

	// Dead Animation (Frames 32-35, First 4 frames of the fifth row)
	animation_map[ENEMY_DEAD] = {
		make_enemy_frame_rect(32),
		make_enemy_frame_rect(33),
		make_enemy_frame_rect(34),
		make_enemy_frame_rect(35)
	};

	// Hurt Animation (Frames 40-43, First 4 frames of the sixth row)
	animation_map[ENEMY_HURT] = {
		make_enemy_frame_rect(40),
		make_enemy_frame_rect(41),
		make_enemy_frame_rect(42),
		make_enemy_frame_rect(43)
	};
}

void Enemy::init_for_level(const b2Vec2& position, b2World* physicsWorld)
{
	spawn_position = position;
	spawn_position = {
	   static_cast<float>(position.x) / GameConstants::PhysicsWorldScale,
	   static_cast<float>(position.y) / GameConstants::PhysicsWorldScale
	};

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(spawn_position.x, spawn_position.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

	this->body = physicsWorld->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.3, 0.4);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;
	fixtureDef.filter.categoryBits = PhysicsTypes::Categories::ENEMY;
	fixtureDef.filter.maskBits = PhysicsTypes::Categories::WEAPON | PhysicsTypes::Categories::SOLID;

	body->CreateFixture(&fixtureDef);
}

void Enemy::set_velocity(float vx, float vy)
{
	body->SetLinearVelocity({ vx, vy });
}

void Enemy::update(float dt)
{
	animation_ticker -= dt;
	updateHitState(dt);
	if (animation_ticker <= 0)
	{
		animation_ticker = animation_frame_duration;
		current_anim_frame = (current_anim_frame + 1) % animation_map[anim_state].size();
	}

	move();

	if (body->GetLinearVelocity().Length() > 0.1f && anim_state != ENEMY_HURT)
	{
		anim_state = ENEMY_WALK;
	}
	else if (body->GetLinearVelocity().Length() <= 0.1f && anim_state != ENEMY_HURT)
	{
		anim_state = ENEMY_IDLE;
	}
}

void Enemy::draw()
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
	
	// Draw enemy collision box
	if (GameConstants::debugModeCollision)
	{

		auto bodyPos = body->GetPosition();
		b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
		Vector2 boxSize = {
			polygonShape->m_vertices[2].x * 2 * GameConstants::PhysicsWorldScale, // width
			polygonShape->m_vertices[2].y * 2 * GameConstants::PhysicsWorldScale  // height
		};

		DrawRectangleLines(
			(bodyPos.x * GameConstants::PhysicsWorldScale) - (boxSize.x / 2),
			(bodyPos.y * GameConstants::PhysicsWorldScale) - (boxSize.y / 2),
			boxSize.x,
			boxSize.y,
			RED
		);
	}

	DrawTexturePro(sprite,
		current_anim_rect,
		{ spritePosX, spritePosY, 24, 24 },
		{ 0, 0 },
		0.0f,
		WHITE);
}

void Enemy::move()
{
	if (!body) return;

	b2Vec2 enemyPosition = body->GetPosition();

	Vector2 enemyPos = { enemyPosition.x, enemyPosition.y };

	Vector2 scaledTargetPos = { targetPos.x / GameConstants::PhysicsWorldScale, targetPos.y / GameConstants::PhysicsWorldScale };

	Vector2 direction = { scaledTargetPos.x - enemyPos.x, scaledTargetPos.y - enemyPos.y };

	// Normalize the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.0f)
	{
		direction.x /= length;
		direction.y /= length;
	}

	b2Vec2 velocity = { direction.x * speed, direction.y * speed };
	body->SetLinearVelocity(velocity);
}

void Enemy::setTargetPos(Vector2 playerPos)
{
	targetPos = playerPos;
}

void Enemy::updateHitState(float dt)
{
	if (isHit)
	{
		currentHitCooldown -= dt;
		if (currentHitCooldown <= 0)
		{
			isHit = false;
			currentHitCooldown = hitCooldown;
			if (health > 0)
			{
				anim_state = ENEMY_IDLE;
			}
		}

	}
}

void Enemy::onHit(int damage)
{
	if (!isHit && health > 0)
	{
		health -= damage;
		isHit = true;
		currentHitCooldown = hitCooldown;
		anim_state = ENEMY_HURT;
		std::cout<<"=========ENEMY_HURT======="<<endl;
		if (health <= 0)
		{
			anim_state = ENEMY_DEAD;
		}


	}
}

Rectangle Enemy::getHitbox() const
{
	if (!body) return Rectangle{ 0, 0, 0, 0 };

	auto bodyPos = body->GetPosition();
	// Get the box dimensions from the fixture
	b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
	Vector2 boxSize = {
		polygonShape->m_vertices[2].x * 2 * GameConstants::PhysicsWorldScale,
		polygonShape->m_vertices[2].y * 2 * GameConstants::PhysicsWorldScale
	};

	return Rectangle{
		(bodyPos.x * GameConstants::PhysicsWorldScale) - (boxSize.x / 2),
		(bodyPos.y * GameConstants::PhysicsWorldScale) - (boxSize.y / 2),
		boxSize.x,
		boxSize.y
	};
}

bool Enemy::checkCollisionWithWeapon(const Rectangle& weaponHitbox) const
{
	return CheckCollisionRecs(getHitbox(), weaponHitbox);
}


