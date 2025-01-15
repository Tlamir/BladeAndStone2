#pragma once
#include <vector>
#include <memory>
#include "Bullet.hpp"
#include "../BaseEntity.hpp"

class BulletManager : public BaseEntity
{
public:
	void fireBullet(const Vector2& position);
	void update(float dt);
	void draw();
	void checkCollisions();

private:
	std::vector<std::unique_ptr<Bullet>> bullets;
};
