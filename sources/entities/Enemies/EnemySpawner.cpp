#include "EnemySpawner.hpp"
#include <iostream>
#include "Enemy.hpp"
#include "EnemyTextureLoader.hpp"

EnemySpawner::EnemySpawner(int spawnRate, int spawnAmount, int spawnType, float x, float y, b2World* physicsWorld, Weapon* weapon, BulletManager* bulletManager) :
spawnRate(spawnRate),
spawnAmount(spawnAmount),
spawnType(spawnType),
posX(x),
posY(y),
physicsWorld(physicsWorld),
weapon(weapon),
myrefBulletManager(bulletManager)
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

        // Update before checking collision
        enemy->update(dt);
        enemy->setTargetPos(playerPos);

        // Check collision with weapon
        if (enemy->checkCollisionWithWeapon(weapon->getHitbox()))
        {
            enemy->onHit(50);
            if (!enemy->isAlive())
            {
                it = enemies.erase(it);
                continue; 
            }
        }

        // Check collision with bullets
        for (const auto& bulletHitbox : myrefBulletManager->getBulletHitboxes())
        {
            if (enemy->checkCollisionWithWeapon(bulletHitbox))
            {
                enemy->onHit(25);
                if (!enemy->isAlive())
                {
                    it = enemies.erase(it);
                    goto next_enemy;
                }
            }
        }

        ++it;
    next_enemy:;
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