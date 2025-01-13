#pragma once
#include"../BaseEntity.hpp"
#include "raylib.h"
class Weapon :public BaseEntity
{
public:
	Weapon();

	~Weapon();
	void update(float dt) override;
	void draw() override;
	void updatePosition(float posX, float posY, bool isLookingRight);

private:
	Texture2D sprite;
	Vector2 Position{0.f,0.f};
	float rotation{0.f};
	int tileAmout{ 4 };
	bool isLookingRight{ false };
	Vector2 positonBuffer{};


};