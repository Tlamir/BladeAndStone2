#pragma once
#include <raylib.h>
#include <string>
class GameSceneUI
{
public:
	GameSceneUI();
	~GameSceneUI();
	void drawGameUI(int playerHealth, int enemyKilled);
	void drawGameEndUI();

private:
	void displayHealth(int playerHealth);
	void displayKilledEnemy(int enemyKilled);
	void drawWithBackdrop(const std::string& text, int x, int y, int fontSize, Color textColor, Color backdropColor);
	int previousHealth = -1;
};