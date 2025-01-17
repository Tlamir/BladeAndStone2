#include "EnemySpawner.hpp"
#include <iostream>
#include "Enemy.hpp"
#include "EnemyTextureLoader.hpp"



EnemySpawner::EnemySpawner(int spawnRate, int spawnAmount, int spawnType, float x, float y, b2World* physicsWorld, Weapon* weapon) : spawnRate(spawnRate),
spawnAmount(spawnAmount),
spawnType(spawnType),
posX(x),
posY(y),
physicsWorld(physicsWorld),
weapon(weapon)
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

	for (auto it = enemies.begin(); it != enemies.end();)
	{
		auto& enemy = *it;
		enemy->draw();
		enemy->update(dt);
		// If the enemy hits the weapon's hitbox
		if (enemy->checkCollisionWithWeapon(weapon->getHitbox()))
		{
			enemy->onHit(50);
			if ((!enemy->isAlive()))
			{
				it = enemies.erase(it);

			}
			return;
		}
		else
		{
			++it;
		}
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
	std::cout << "-------------- ENEMY SPAWNED --------------\n";
	std::cout << "Type: " << spawnType << "\n";
	std::cout << "Amount: " << spawnAmount << "\n";
	std::cout << "Spawner Position: (" << posX << ", " << posY << ")\n";
	std::cout << "------------------------------------------\n";

	if (spawnType < EnemyTextureLoader::DEMON || spawnType > EnemyTextureLoader::SLIMEBALL)
		throw std::out_of_range("Invalid spawnType provided.");

	auto enemyType = static_cast<EnemyTextureLoader::EnemyTextures>(spawnType);
	auto enemyTexture = EnemyTextureLoader::getEnemyTexture(enemyType);

	for (int i = 0; i < spawnAmount; i++)
	{
		float spawnX = posX + (rand() % 21 - 10);
		float spawnY = posY + (rand() % 21 - 10);
		b2Vec2 spawnPosition(spawnX, spawnY);

		auto enemy = std::make_unique<Enemy>(enemyTexture);
		enemy->init_for_level(spawnPosition, physicsWorld);

		enemies.emplace_back(std::move(enemy));
	}
}