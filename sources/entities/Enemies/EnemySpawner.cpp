#include "EnemySpawner.hpp"
#include <iostream>
#include "Enemy.hpp"


EnemySpawner::EnemySpawner(int spawnRate, int spawnAmount, int spawnType)
{
	this->spawnRate = spawnRate;
	this->spawnAmount = spawnAmount;
	this->spawnType = spawnType;
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

    for (auto &enemy : enemies)
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
    std::cout << "------------------------------------------\n";
    enemies.emplace_back(std::make_unique<Enemy>());



}