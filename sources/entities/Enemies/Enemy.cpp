#include "Enemy.hpp"
#include "raylib.h"
#include "Constants.hpp"
#include<iostream>

Enemy::Enemy()
{
	//sprite = LoadTexture(AppConstants::GetAssetPath("./Characters/demon_.png").c_str());
}

Enemy::~Enemy()
{
}

void Enemy::update(float dt)
{
	
}

void Enemy::draw()
{
	DrawRectangle(100, 100, 200, 200, RED);

	std::cout << "drawing----------" << endl;
}
