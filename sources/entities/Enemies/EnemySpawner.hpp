#pragma once
#include "Enemy.hpp"
#include <vector>
#include <memory>
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

private:
	 int spawnRate{};
	 int spawnAmount{};
	 int spawnType{};
	 float timeSinceLastSpawn{};
	 std::vector<std::unique_ptr<Enemy>> enemies;
	 float posX; // X position of the spawner
	 float posY; // Y position of the spawner
	 b2World* physicsWorld{ nullptr }; // Replace with your actual physics world reference



};