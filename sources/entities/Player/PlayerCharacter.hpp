#pragma once

#include "../BaseEntity.hpp"
#include <vector>
#include <raylib.h>
#include <LDtkLoader/Entity.hpp>
class PlayerCharacter : public BaseEntity
{
public:
	PlayerCharacter();
	~PlayerCharacter();
	int maxFrames{ 6 };
	int speed{ 10 };
	Vector2 level_spawn_position{0.f,0.f};
	Texture2D sprite;

	void update(float dt) override;
	void draw() override;


private:
	


};