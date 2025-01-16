#pragma once
#include "Enemy.hpp"
#include <vector>
#include <memory>
class EnemySpawner
{
public:
	EnemySpawner(int spawnRate,
		int spawnAmount,
		int spawnType);
	~EnemySpawner();

	void update(float deltaTime);

	void DrawEnemies();

	void spawnEnemies();

private:
	 int spawnRate{};
	 int spawnAmount{};
	 int spawnType{};
	 float timeSinceLastSpawn{};
	 std::vector<std::unique_ptr<Enemy>> enemies;


};