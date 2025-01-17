#include "BulletManager.hpp"

void BulletManager::fireBullet(const Vector2& position,b2World* currentWorld)
{
	bullets.emplace_back(std::make_unique<Bullet>(position, currentWorld));

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

void BulletManager::checkCollisions()
{
	// Placeholder for collision 
}
std::vector<Rectangle> BulletManager::getBulletHitboxes() const
{
	std::vector<Rectangle> hitboxes;
	for (const auto& bullet : bullets)
	{
		hitboxes.push_back(bullet->getHitbox());
	}
	return hitboxes;
}
