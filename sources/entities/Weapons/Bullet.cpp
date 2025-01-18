#include "Bullet.hpp"
#include "Constants.hpp"
#include <box2d/box2d.h>
#include <physics/PhysicsTypes.hpp>
#include <raylib.h>

Bullet::Bullet(Vector2 startPos, b2World* world, Texture2D* bulletSprite) : 
    position(startPos), 
    physicsWorld(world), 
    sprite(bulletSprite ? *bulletSprite : Texture2D{})
{
    
    if (physicsWorld)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.bullet = false;
        

        bulletBody = physicsWorld->CreateBody(&bodyDef);

        b2PolygonShape bulletShape;
        bulletShape.SetAsBox(0.5f, 0.5f);
        

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &bulletShape;
        fixtureDef.isSensor = true;
        fixtureDef.filter.categoryBits = PhysicsTypes::Categories::WEAPON;
        fixtureDef.filter.maskBits = PhysicsTypes::Categories::ENEMY;

        bulletBody->CreateFixture(&fixtureDef);
    }
}

Bullet::~Bullet()
{
    
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
        DrawTextureEx(sprite, {position.x, position.y },0.f,2.f, WHITE);

       
        if (bulletBody)
        {
            b2PolygonShape* bulletShape = dynamic_cast<b2PolygonShape*>(bulletBody->GetFixtureList()->GetShape());
            if (GameConstants::debugModeCollision)
            {
                if (bulletShape)
                {
                    
                    DrawRectangleLinesEx(getHitbox(), 2, RED);
                }
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
    // Update the cached hitbox based on current position
    hitbox = Rectangle{
        position.x - 0.5f,
        position.y - 0.5f,
        (float)sprite.width * 2,
        (float)sprite.height * 2
    };
    return hitbox;
}