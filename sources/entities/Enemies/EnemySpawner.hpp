#pragma once
#include "Enemy.hpp"
#include <vector>
#include <memory>
#include<./entities/Weapons/Weapon.hpp>
class EnemySpawner
{
public:
	EnemySpawner(int spawnRate,
		int spawnAmount,
		int spawnType,
		float x,
		float y,
		b2World* physicsWorld,
		Weapon* weapon,
		BulletManager* bulletManager
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
	 float posX;
	 float posY;
	 b2World* physicsWorld{ nullptr };
	 Weapon* weapon;
	 BulletManager* myrefBulletManager;



};