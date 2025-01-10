#include <math.h>
#include <unordered_map>
#include <vector>
#include <math.h>

#include <raylib.h>

#include <LDtkLoader/World.hpp>

#include <Constants.hpp>
#include <utils/DebugUtils.hpp>

#include "Player.hpp"
#include "../../physics/PhysicsTypes.hpp"
#include "../../scenes/GameScene/GameScene.hpp"
#include "../../physics/RaycastUtils.hpp"

using namespace std;
auto spritePosX_LastFrame = 0.f;
auto spritePosY_LastFrame = 0.f;

Player::Player()
{
    this->sprite = LoadTexture(AppConstants::GetAssetPath("BladeAndStoneAssets/Characters/fHero_.png").c_str());

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
}

void Player::draw()
{
    

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
    if (spritePosX_LastFrame != spritePosX)
    {
        isMovingX = true;
    }
    else
    {
        isMovingX = false;
    }
    if (spritePosY_LastFrame != spritePosY)
    {
        isMovingY = true;
    }
    else
    {
        isMovingY = false;
    }
    spritePosX_LastFrame = spritePosX;
    spritePosY_LastFrame = spritePosY;
}



void Player::init_for_level(const ldtk::Entity* entity, b2World* physicsWorld)
{
    auto pos = entity->getPosition();

    DebugUtils::println("Setting player position to x:{} and y:{}", pos.x, pos.y);

    level_spawn_position = { (float)pos.x / GameConstants::PhysicsWorldScale,
                            (float)pos.y / GameConstants::PhysicsWorldScale };

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(level_spawn_position.x, level_spawn_position.y);

    this->body = physicsWorld->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.9, 1);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.0001f;
    fixtureDef.friction = 0.f; // Adjusted for smoother top-down movement

    body->CreateFixture(&fixtureDef);
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
    const auto effective_speed = 5.0f;

    float vx = 0.0f;
    float vy = 0.0f;


    if (IsKeyDown(KEY_LEFT))
    {
        vx -= effective_speed;
        looking_right = false;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        vx += effective_speed;
        looking_right = true;
    }

    if (IsKeyDown(KEY_UP))
    {
        vy -= effective_speed;
    }

    if (IsKeyDown(KEY_DOWN))
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
    return Vector2(level_spawn_position.x, level_spawn_position.y);

}
