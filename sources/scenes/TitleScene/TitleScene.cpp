#include <string>
#include <iostream>

#include <raylib.h>

#include <Constants.hpp>
#include "TitleScene.hpp"
#include "../Scenes.hpp"

using namespace std;

TitleScene::TitleScene()
{
	texture = LoadTexture(AppConstants::GetAssetPath("TitleBackground.png").c_str());
}

TitleScene::~TitleScene()
{
	UnloadTexture(texture);
}


Scenes TitleScene::update(float dt)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		return Scenes::GAME;
	}

	return Scenes::NONE;
}

void TitleScene::draw()
{
	ClearBackground(RAYWHITE);

	// Draw background image texture 

	const int texture_x = GameConstants::WorldWidth / 2 - texture.width / 2;
	const int texture_y = GameConstants::WorldHeight / 2 - texture.height / 2;
	DrawTexture(texture, texture_x, texture_y, WHITE);


	// Draw title text

	const string text = "BladeAndStone2";
	const int text_width = MeasureText(text.c_str(), 20);
	DrawText(text.c_str(), (GameConstants::WorldWidth-text_width)/2, GameConstants::WorldHeight / 10, 20, BLACK);

	// Draw start text

	auto draw_with_backdrop = [](const string& text, int x, int y, int fontSize, Color color, Color backdropColor) {
		DrawText(text.c_str(), x + 1, y + 1, fontSize, backdropColor);
		DrawText(text.c_str(), x, y, fontSize, color);
	};

	draw_with_backdrop("Press 'SPACE' to play!", GameConstants::WorldWidth/4, GameConstants::WorldHeight / 1.3, 10, GOLD, BLACK);

}

