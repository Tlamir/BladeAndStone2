#pragma once
#include "Enemy.hpp"
#include <memory>
#include <vector>
class EnemySpawner
{
public:
	EnemySpawner(int spawnRate,
		int spawnAmount,
		int spawnType,
		float x,
		float y,
		b2World* physicsWorld
	);
	~EnemySpawner();

	void update(float deltaTime,Vector2 playerPos);

	void DrawEnemies();

	void spawnEnemies();
	std::vector<std::unique_ptr<Enemy>>& getEnemies();

private:
	 int spawnRate{};
	 int spawnAmount{};
	 int spawnType{};
	 float timeSinceLastSpawn{};
	 std::vector<std::unique_ptr<Enemy>> enemies;
	 float posX;
	 float posY;
	 b2World* physicsWorld{ nullptr };



};