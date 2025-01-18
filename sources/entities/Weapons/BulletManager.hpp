#pragma once
#include <vector>
#include <memory>
#include "Bullet.hpp"
#include "../BaseEntity.hpp"
#include"box2d/box2d.h"

class BulletManager : public BaseEntity
{
public:
	BulletManager();
	~BulletManager();
	void fireBullet(const Vector2& position, b2World* currentWorld);
	void update(float dt);
	void draw();

	std::vector<const Rectangle*> getBulletHitboxes() const;



private:
	std::vector<std::unique_ptr<Bullet>> bullets;
	Texture2D bulletSprite;
	Rectangle hitbox;
};
