#include "Bullet.hpp"
#include "Constants.hpp"
#include <physics/PhysicsTypes.hpp>
#include <box2d/box2d.h>
#include <raylib.h>
#include <physics/PhysicsTypes.hpp>

Bullet::Bullet(Vector2 startPos, b2World* world) : position(startPos), physicsWorld(world)
{
    // Move this to bullet manager
    sprite = LoadTexture(AppConstants::GetAssetPath("Weapons/Projectile.png").c_str());
    if (physicsWorld)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.bullet = false;
        

        bulletBody = physicsWorld->CreateBody(&bodyDef);

        b2PolygonShape bulletShape;
        bulletShape.SetAsBox(0.5f, 0.5f); // Set weapon shape as a box
        

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &bulletShape;
        fixtureDef.isSensor = true; // Make it a sensor for collision detection
        fixtureDef.filter.categoryBits = PhysicsTypes::Categories::WEAPON;
        fixtureDef.filter.maskBits = PhysicsTypes::Categories::ENEMY;

        bulletBody->CreateFixture(&fixtureDef); // Attach fixture to the weapon body
    }
}

Bullet::~Bullet()
{
    UnloadTexture(sprite);
}

void Bullet::fire(Vector2 direction)
{
    this->direction = direction;
    active = true;
    age = 0.0f;
}

void Bullet::update(float dt)
{
    if (!active) return;

    // Update bullet position based on direction and speed
    position.x += direction.x * speed * dt;
    position.y += direction.y * speed * dt;

    // Increase bullet age and deactivate if lifetime exceeded
    age += dt;
    if (age > lifetime) deactivate();
}

void Bullet::draw()
{
    if (active)
    {
        //DrawTexture(sprite, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
        DrawTextureEx(sprite, {position.x, position.y },0.f,2.f, WHITE);

        // Get the size of the bullet shape from the physics body
        if (bulletBody)
        {
            b2PolygonShape* bulletShape = dynamic_cast<b2PolygonShape*>(bulletBody->GetFixtureList()->GetShape());
            if (bulletShape)
            {
                // Draw the rectangle in red with line thickness of 2
                DrawRectangleLinesEx(getHitbox(), 2, RED);
            }
        }
    
    }
}

bool Bullet::isFired() const
{
    return active;
}

void Bullet::deactivate()
{
    active = false;
}
Vector2 Bullet::getPosition() const
{
    return position;
}

Rectangle Bullet::getHitbox() const
{
    return Rectangle{ position.x - 0.5f, position.y - 0.5f, (float)sprite.width*2, (float)sprite.height*2};
}