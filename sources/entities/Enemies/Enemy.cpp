#include "Enemy.hpp"
#include <Constants.hpp>
#include <iostream>

Enemy::Enemy()
{
    // Load enemy sprite
    this->sprite = LoadTexture(AppConstants::GetAssetPath("Characters/demon_.png").c_str());

    initializeAnimations();
}

Enemy::~Enemy()
{
    UnloadTexture(this->sprite);
}

void Enemy::initializeAnimations()
{
    auto make_enemy_frame_rect = [](float frame_num) -> Rectangle {
        return {
            .x = frame_num * 24.0f,
            .y = 0.0f,
            .width = 24.0f,
            .height = 24.0f
        };
        };

    animation_map[ENEMY_IDLE] = {
        make_enemy_frame_rect(0),
        make_enemy_frame_rect(1),
    };

    animation_map[ENEMY_WALK] = {
        make_enemy_frame_rect(2),
        make_enemy_frame_rect(3),
    };

    animation_map[ENEMY_ATTACK] = {
        make_enemy_frame_rect(4),
        make_enemy_frame_rect(5),
    };
}

void Enemy::init_for_level(const b2Vec2& position, b2World* physicsWorld)
{
    spawn_position = position;
    // Convert ldtk position to physics world position
    spawn_position = {
       static_cast<float>(position.x) / GameConstants::PhysicsWorldScale,
       static_cast<float>(position.y) / GameConstants::PhysicsWorldScale
    };

    // Create enemy body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(spawn_position.x, spawn_position.y);;
    // Here its setting body pos 256 400 like that to body
    // Convert LDTK position to physics world position
   
    this->body = physicsWorld->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.6, 0.8);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.f;
    fixtureDef.friction = 0.f;

    body->CreateFixture(&fixtureDef);
}

void Enemy::set_velocity(float vx, float vy)
{
    body->SetLinearVelocity({ vx, vy });
}

void Enemy::update(float dt)
{
    animation_ticker -= dt;
    if (animation_ticker <= 0)
    {
        animation_ticker = animation_frame_duration;
        current_anim_frame = (current_anim_frame + 1) % animation_map[anim_state].size();
    }

    // Placeholder for movement or AI logic
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
    // Debug message
        std::cout << "Enemy position: (" << spritePosX << ", " << spritePosY << ")" << std::endl;
    DrawTexturePro(sprite,
        current_anim_rect,
        { spritePosX, spritePosY, 24, 24 },
        { 0, 0 },
        0.0f,
        WHITE);
}
