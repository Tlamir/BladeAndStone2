#include "Enemy.hpp"
#include "EnemySpawner.hpp"
#include "EnemyTextureLoader.hpp"
#include <iostream>

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

void EnemySpawner::update(float dt, Vector2 playerPos)
{
    timeSinceLastSpawn += dt;
    if (timeSinceLastSpawn >= spawnRate)
    {
        spawnEnemies();
        timeSinceLastSpawn = 0.0f;
    }

    for (auto& enemy :enemies)
    {
        enemy->update(dt);
        enemy->setTargetPos(playerPos);
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
	if (spawnType < EnemyTextureLoader::DEMON || spawnType > EnemyTextureLoader::DRAGON)
		throw std::out_of_range("Invalid spawnType provided.");

	auto enemyType = static_cast<EnemyTextureLoader::EnemyTextures>(spawnType);
	auto enemyTexture = EnemyTextureLoader::getEnemyTexture(enemyType);

	// Create random are for enemy pos
	for (int i = 0; i < spawnAmount; i++)
	{
		float spawnX = posX + (rand() % 21 - 10);
		float spawnY = posY + (rand() % 10 - 1);
		b2Vec2 spawnPosition(spawnX, spawnY);

		auto enemy = std::make_unique<Enemy>(enemyTexture, spawnType);
		enemy->initForLevel(spawnPosition, physicsWorld);

		enemies.emplace_back(std::move(enemy));
	}
}

std::vector<std::unique_ptr<Enemy>>& EnemySpawner::getEnemies()
{
    return enemies;
}