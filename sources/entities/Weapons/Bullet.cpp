#include "Bullet.hpp"
#include "Constants.hpp"

Bullet::Bullet(Vector2 startPos) : position(startPos)
{
    sprite = LoadTexture(AppConstants::GetAssetPath("Weapons/Projectile.png").c_str());
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
