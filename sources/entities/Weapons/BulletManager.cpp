#include "BulletManager.hpp"
#include "Constants.hpp"
#include <algorithm>

BulletManager::BulletManager()
{
	bulletSprite = LoadTexture(AppConstants::GetAssetPath("Weapons/Projectile.png").c_str());
}
BulletManager::~BulletManager()
{
	UnloadTexture(bulletSprite);
}
void BulletManager::fireBullet(const Vector2& position,b2World* currentWorld)
{

	bullets.emplace_back(std::make_unique<Bullet>(position, currentWorld,&bulletSprite));

	// Fire the bullet in the positive X direction (right)
	bullets.back()->fire(Vector2{ 1.0f, 0.0f });
}

void BulletManager::update(float dt)
{
	for (auto& bullet : bullets)
	{
		bullet->update(dt);
	}

	// Remove bullets that are no longer active
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
		[](const std::unique_ptr<Bullet>& bullet) { return !bullet->isFired(); }),
		bullets.end());
}

void BulletManager::draw()
{
	for (const auto& bullet : bullets)
	{
		bullet->draw();
	}
}

std::vector<const Rectangle*> BulletManager::getBulletHitboxes() const
{
	std::vector<const Rectangle*> hitboxes;
	hitboxes.reserve(bullets.size());
	for (const std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->getHitbox();
		hitboxes.push_back(&bullet->hitbox);
	}
	return hitboxes;
}