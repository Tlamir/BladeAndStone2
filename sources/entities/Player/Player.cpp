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
    // Load player sprite (a sprite sheet)
    this->sprite = LoadTexture(AppConstants::GetAssetPath("Characters/fHero_.png").c_str());

    // Define frame width and height for sprite sheet
    const float frame_width = 24.0f;
    const float frame_height = 24.0f;

    // Function to calculate the frame rectangle in the sprite sheet
    auto make_frame_rect = [frame_width, frame_height](int frame_x, int frame_y) -> Rectangle
        {
            return {
                .x = frame_x * frame_width,
                .y = frame_y * frame_height,
                .width = frame_width,
                .height = frame_height
            };
        };

    // Define animations using frame grid positions
    animation_map[IDLE] = {
        make_frame_rect(0, 0), // Frame 1 (IDLE)
        make_frame_rect(1, 0), // Frame 2 (IDLE)
        make_frame_rect(2, 0)  // Frame 3 (IDLE)
    };

    animation_map[WALK] = {
        make_frame_rect(0, 1), // Frame 1 (WALK)
        make_frame_rect(1, 1), // Frame 2 (WALK)
        make_frame_rect(2, 1)  // Frame 3 (WALK)
    };

    animation_map[DEAD] = {
        make_frame_rect(0, 2)  // Frame 1 (DEAD)
    };

    animation_map[HURT] = {
        make_frame_rect(1, 2)  // Frame 1 (HURT)
    };
}

Player::~Player()
{
    UnloadTexture(this->sprite);
}

void Player::update(float dt)
{
    // Update animation based on time
    animation_ticker -= dt;
    if (animation_ticker <= 0)
    {
        animation_ticker = animation_frame_duration;
        current_anim_frame += 1;
    }

    // Update player movement and behaviors
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

    // Draw the player sprite based on the current animation state
    DrawTexturePro(sprite,
        current_anim_rect,
        { spritePosX, spritePosY, 24, 24 },
        { 0, 0 },
        0.0f,
        WHITE);

    // Draw player collision box for debug mode
    if (GameConstants::debugModeCollision)
    {
        auto bodyPos = body->GetPosition();
        b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
        Vector2 boxSize = {
            polygonShape->m_vertices[2].x * 2 * GameConstants::PhysicsWorldScale,
            polygonShape->m_vertices[2].y * 2 * GameConstants::PhysicsWorldScale
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

    // Initialize player physics body
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

    // Update animation state based on movement
    if (vx != 0 || vy != 0)
    {
        anim_state = WALK;
    }
    else
    {
        anim_state = IDLE;
    }
}

void Player::check_if_should_respawn()
{
    // Placeholder for respawn logic (could be extended for a respawn mechanic)
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
        physicsWorld,
        50
    );

    // Load the texture for the magic weapon
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
        physicsWorld,
        1
    );
}

Weapon* Player::getWeapon()
{
    return Sword.get();
}
