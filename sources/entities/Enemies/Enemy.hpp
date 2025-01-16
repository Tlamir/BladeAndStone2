#pragma once
#include "raylib.h"
#include"../BaseEntity.hpp"
class Enemy :BaseEntity
{
public:
	Enemy();
	~Enemy();
	void update(float dt);
	void draw();

private:
	Texture2D sprite{};
};