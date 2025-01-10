#include "PlayerCharacter.hpp"
#include <math.h>
#include <unordered_map>
#include <vector>


#include <raylib.h>
#include <box2d/box2d.h>


#include <Constants.hpp>
#include <utils/DebugUtils.hpp>
PlayerCharacter::PlayerCharacter()
{
	this->sprite = LoadTexture(AppConstants::GetAssetPath("BladeAndStoneAssets/Characters/fHero_.png").c_str());
	level_spawn_position= { AppConstants::ScreenWidth / 2, AppConstants::ScreenHeight / 2 };
}

PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::update(float dt)
{
}

void PlayerCharacter::draw()
{
	DrawTextureEx(sprite, level_spawn_position, 0.f, 1.f, WHITE);

	//DrawTexture(sprite, level_spawn_position.x, level_spawn_position.y ,WHITE);
}
