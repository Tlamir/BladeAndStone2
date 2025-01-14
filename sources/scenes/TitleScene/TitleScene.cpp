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

    // Draw background image texture (Centered)
    const int textureX = (GameConstants::WorldWidth - texture.width) / 2;
    const int textureY = (GameConstants::WorldHeight - texture.height) / 2;
    DrawTexture(texture, textureX, textureY, WHITE);

    // Draw title text (Centered)
    const string titleText = "BladeAndStone2";
    const int titleFontSize = 20;
    const int titleTextWidth = MeasureText(titleText.c_str(), titleFontSize);
    const int titleTextX = (GameConstants::WorldWidth - titleTextWidth) / 2;
    const int titleTextY = GameConstants::WorldHeight / 10;
    DrawText(titleText.c_str(), titleTextX, titleTextY, titleFontSize, BLACK);

    // Helper lambda for drawing text with a backdrop
    auto draw_with_backdrop = [](const string& text, int x, int y, int fontSize, Color textColor, Color backdropColor) {
        DrawText(text.c_str(), x + 1, y + 1, fontSize, backdropColor);
        DrawText(text.c_str(), x, y, fontSize, textColor);
        };

    // Draw start text (Centered)
    const string startText = "Press 'SPACE' to play!";
    const int startFontSize = 20;
    const int startTextWidth = MeasureText(startText.c_str(), startFontSize);
    const int startTextX = (GameConstants::WorldWidth - startTextWidth) / 2;
    const int startTextY = GameConstants::WorldHeight / 1.3f;

    draw_with_backdrop(startText, startTextX, startTextY, startFontSize, GOLD, BLACK);
}
