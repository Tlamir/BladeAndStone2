#pragma once
#include "../BaseEntity.hpp"
#include "raylib.h"

class Bullet : public BaseEntity
{
public:
	Bullet();
	~Bullet();

private:
	void update(float dt) override;
	void draw() override;
	Texture2D sprite;

};