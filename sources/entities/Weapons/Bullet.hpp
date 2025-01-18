#pragma once
#include "../BaseEntity.hpp"
#include"box2d/box2d.h"
#include "raylib.h"

class Bullet : public BaseEntity
{
public:
	Bullet(Vector2 startPos, b2World* world,Texture2D* bulletSprite);
	~Bullet();
	void fire(Vector2 direction);
	void update(float dt) override;
	void draw() override;
	bool isFired() const;
	void deactivate();
	Vector2 getPosition() const;
	Rectangle getHitbox() const;

	// Physics
	b2Body* bulletBody = nullptr;
	b2World* physicsWorld = nullptr;
	mutable Rectangle hitbox{};

private:
	Texture2D sprite{};
	Vector2 position{};
	Vector2 direction{};
	float speed{ 400.0f };
	bool active{ false };
	float lifetime{ 2.0f };
	float age{ 0.0f };
};
