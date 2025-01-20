#include "GameSceneUI.hpp"
#include "raylib.h"
#include "Constants.hpp"
#include <string>

GameSceneUI::GameSceneUI() {}

GameSceneUI::~GameSceneUI() {}

void GameSceneUI::drawWithBackdrop(const std::string& text, int x, int y, int fontSize, Color textColor, Color backdropColor)
{
    DrawText(text.c_str(), x + 1, y + 1, fontSize, backdropColor); // Backdrop (offset text)
    DrawText(text.c_str(), x, y, fontSize, textColor);            // Foreground text
}

void GameSceneUI::drawGameUI(int playerHealth, int enemyKilled)
{
    displayHealth(playerHealth);
    displayKilledEnemy(enemyKilled);

}

void GameSceneUI::displayHealth(int playerHealth)
{
    const int upperLeftFontSize = 20 * GameConstants::cameraZoom;
    const int upperLeftX = 10;
    const int upperLeftY = 10;

    Color healthColor = GOLD;
    if (previousHealth != -1 && playerHealth < previousHealth)
    {
        healthColor = RED;
    }
    previousHealth = playerHealth;


    const std::string label = "Health: ";
    const std::string healthValue = std::to_string(playerHealth);

    drawWithBackdrop(label, upperLeftX, upperLeftY, upperLeftFontSize, GOLD, BLACK);

    int valueX = upperLeftX + MeasureText(label.c_str(), upperLeftFontSize);
    drawWithBackdrop(healthValue, valueX, upperLeftY, upperLeftFontSize, healthColor, BLACK);
}

void GameSceneUI::displayKilledEnemy(int enemyKilled)
{
    // Upper-right corner text
    const std::string upperRightText = "Defeated Pals: " + std::to_string(enemyKilled);
    const int upperRightFontSize = 20 * GameConstants::cameraZoom;
    const int upperRightTextWidth = MeasureText(upperRightText.c_str(), upperRightFontSize);
    const int upperRightX = GameConstants::WorldWidth - upperRightTextWidth - 10;
    const int upperRightY = 10;
    drawWithBackdrop(upperRightText, upperRightX, upperRightY, upperRightFontSize, GOLD, BLACK);
}

void GameSceneUI::drawGameEndUI()
{
    // Centered text
    const std::string centerText = "Game Over";
    const int centerFontSize = 20 * GameConstants::cameraZoom;
    const int centerTextWidth = MeasureText(centerText.c_str(), centerFontSize);
    const int centerX = (GameConstants::WorldWidth - centerTextWidth) / 2;
    const int centerY = GameConstants::WorldHeight / 2;
    drawWithBackdrop(centerText, centerX, centerY, centerFontSize, RED, BLACK);

    // Below-center text
    const std::string belowCenterTextLine1 = "Thank You For Playing";
    const std::string belowCenterTextLine2 = "Press 'SPACE' to Restart!";
    const int belowCenterFontSize = 15 * GameConstants::cameraZoom;

    const int belowCenterTextWidthLine1 = MeasureText(belowCenterTextLine1.c_str(), belowCenterFontSize);
    const int belowCenterTextWidthLine2 = MeasureText(belowCenterTextLine2.c_str(), belowCenterFontSize);

    const int belowCenterXLine1 = (GameConstants::WorldWidth - belowCenterTextWidthLine1) / 2;
    const int belowCenterXLine2 = (GameConstants::WorldWidth - belowCenterTextWidthLine2) / 2;

    const float verticalOffsetFraction = 0.3f;
    const int belowCenterYLine1 = GameConstants::WorldHeight / 2 + static_cast<int>(GameConstants::WorldHeight * verticalOffsetFraction);
    const int belowCenterYLine2 = belowCenterYLine1 + belowCenterFontSize; // Offset by the height of the font size for the second line

    drawWithBackdrop(belowCenterTextLine1, belowCenterXLine1, belowCenterYLine1, belowCenterFontSize, WHITE, BLACK);
    drawWithBackdrop(belowCenterTextLine2, belowCenterXLine2, belowCenterYLine2, belowCenterFontSize, GREEN, BLACK);
}
