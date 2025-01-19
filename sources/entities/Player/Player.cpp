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

    animation_map[HURT] = {
      make_frame_rect(0, 4),  // Frame 1 (HURT)
      make_frame_rect(1, 4),  // Frame 1 (HURT)
      make_frame_rect(2, 4)  // Frame 1 (HURT)
    };

    animation_map[DEAD] = {
        make_frame_rect(3, 5),  // Frame 1 (DEAD)
    };

}

Player::~Player()
{
    UnloadTexture(this->sprite);
}

void Player::update(float dt)
{
    if (health <= 0)
    {
        if (anim_state != DEAD)
        {
            anim_state = DEAD;
        }
        return;
    }

    animation_ticker -= dt;
    if (animation_ticker <= 0)
    {
        animation_ticker = animation_frame_duration;
        current_anim_frame += 1;
    }

    checkIfMove();
    checkIfRespawn();
    updateCamera();

    Sword->update(dt);
    Magic->update(dt);
}

void Player::updateCamera()
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

    // Draw player collision box for debug mode
    if (GameConstants::debugModeCollision)
    {
        auto bodyPos = body->GetPosition();
        b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();

        // Calculate the box size directly from the polygon shape's half-width and half-height
        float halfWidth = polygonShape->m_vertices[1].x - polygonShape->m_vertices[0].x;  // Half-width
        float halfHeight = polygonShape->m_vertices[2].y - polygonShape->m_vertices[0].y; // Half-height
        Vector2 boxSize = {
            halfWidth * 2 * GameConstants::PhysicsWorldScale,
            halfHeight * 2 * GameConstants::PhysicsWorldScale
        };

        // Account for the shape's center offset when drawing
        b2Vec2 shapeOffset = polygonShape->m_centroid;
        Vector2 drawPosition = {
            (bodyPos.x + shapeOffset.x) * GameConstants::PhysicsWorldScale - (boxSize.x / 2),
            (bodyPos.y + shapeOffset.y) * GameConstants::PhysicsWorldScale - (boxSize.y / 2)
        };

        // Draw the physics body rectangle
        DrawRectangleLines(
            drawPosition.x,
            drawPosition.y,
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

void Player::initForLevel(const ldtk::Entity* entity, b2World* physicsWorld)
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
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    // Add linear damping to prevent sliding
    bodyDef.linearDamping = 5.0f;

    this->body = physicsWorld->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    // Adjust player collison box
    dynamicBox.SetAsBox(0.25f, 0.15f,b2Vec2(0.f,0.4f),0.f);  // Reduced from 0.3, 0.4

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = .0001f;
    // Lower friction for smoother movement
    fixtureDef.friction = 0.05f;
    // Add slight restitution to prevent ghost collision
    fixtureDef.restitution = 0.01f;
    fixtureDef.filter.categoryBits = PhysicsTypes::Categories::PLAYER;
    fixtureDef.filter.maskBits = PhysicsTypes::Categories::SOLID;

    body->CreateFixture(&fixtureDef);

    intializeInventory(physicsWorld);
}

void Player::getDamage(int damage)
{
    anim_state = HURT;
    health -= damage;
}

void Player::setVelocityX(float vx)
{
    body->SetLinearVelocity({
        vx,
        body->GetLinearVelocity().y,
        });
}

void Player::setVelocityY(float vy)
{
    body->SetLinearVelocity({
        body->GetLinearVelocity().x,
        vy,
        });
}

void Player::setVelocityXY(float vx, float vy)
{
    body->SetLinearVelocity({ vx, vy });
}

void Player::checkIfMove()
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

    setVelocityXY(vx, vy);

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

void Player::checkIfRespawn()
{
    // Placeholder for respawn
}

Vector2 Player::getPosition()
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
        8,
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

BulletManager* Player::getBulletManager()
{
    return Magic->getBulletManager();
}

bool Player::checkCollisionWithEnemy(const Rectangle& enemyHitbox)
{
    return CheckCollisionRecs(getHitbox(), enemyHitbox);
}

Rectangle Player::getHitbox()
{

    if (!body) return Rectangle{ 0, 0, 0, 0 };

    auto bodyPos = body->GetPosition();
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

bool Player::isAlive()
{
    return health >= 0;
}

int Player::getHealth()
{
    return health >= 0 ? health : 0;
}