#include "EnemySpawner.hpp"
#include <iostream>
#include "Enemy.hpp"

EnemySpawner::EnemySpawner(int spawnRate, int spawnAmount, int spawnType, float x, float y, b2World* physicsWorld) :
	spawnRate(spawnRate),
	spawnAmount(spawnAmount),
	spawnType(spawnType),
	posX(x),
	posY(y),
	physicsWorld(physicsWorld)
{
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::update(float dt)
{
	timeSinceLastSpawn += dt; // Accumulate time

	// Check if enough time has passed to spawn
	if (timeSinceLastSpawn >= spawnRate)
	{
		spawnEnemies();
		timeSinceLastSpawn = 0.0f; // Reset the timer
	}

	for (auto& enemy : enemies)
	{
		enemy->draw();
		enemy->update(dt);
	}

}

void EnemySpawner::DrawEnemies()
{
	for (auto& enemy : enemies)
	{
		enemy->draw();
	}
}

void EnemySpawner::spawnEnemies()
{
	std::cout << "-------------- ENEMY SPAWNED --------------\n";
	std::cout << "Type: " << spawnType << "\n";
	std::cout << "Amount: " << spawnAmount << "\n";
	std::cout << "Spawner Position: (" << posX << ", " << posY << ")\n";
	std::cout << "------------------------------------------\n";

	for (int i = 0; i < spawnAmount; i++)
	{
		// Create a new Enemy
		auto enemy = std::make_unique<Enemy>();

		// Generate enemy spawn position near the spawner
		float spawnX = posX + (rand() % 21 - 10); // Random offset in range [-10, 10]
		float spawnY = posY + (rand() % 21 - 10); // Random offset in range [-10, 10

		// Create a b2Vec2 object for the position
		b2Vec2 spawnPosition(spawnX, spawnY);
		// Initialize the enemy
		enemy->init_for_level(spawnPosition, physicsWorld);

		// Add the enemy to the list
		enemies.emplace_back(std::move(enemy));
	}
}