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

void EnemySpawner::update(float deltaTime)
{
	timeSinceLastSpawn += deltaTime; // Accumulate time

	// Check if enough time has passed to spawn
	if (timeSinceLastSpawn >= spawnRate)
	{
		spawnEnemies();
		timeSinceLastSpawn = 0.0f; // Reset the timer
	}

	for (auto& enemy : enemies)
	{
		enemy->draw();
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
		float spawnX = posX; // Random offset around the spawner
		float spawnY = posY;

		// Create a b2Vec2 object for the position
		b2Vec2 spawnPosition(spawnX, spawnY);
		// Initialize the enemy
		enemy->init_for_level(spawnPosition, physicsWorld);

		// Add the enemy to the list
		enemies.emplace_back(std::move(enemy));
	}
}