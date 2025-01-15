#pragma once
#include "raylib.h"
#include "../BaseEntity.hpp"

class Bullet : public BaseEntity
{
public:
	Bullet(Vector2 startPos);
	~Bullet();
	void fire(Vector2 direction);
	void update(float dt) override;
	void draw() override;
	bool isFired() const;
	void deactivate();

private:
	Texture2D sprite;
	Vector2 position;
	Vector2 direction;
	float speed{ 400.0f };
	bool active{ false };
	float lifetime{ 2.0f };
	float age{ 0.0f };
};
