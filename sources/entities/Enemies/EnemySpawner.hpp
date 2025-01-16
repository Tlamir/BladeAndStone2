#pragma once
class EnemySpawner
{
public:
	EnemySpawner(int spawnRate,
		int spawnAmount,
		int spawnType);
	~EnemySpawner();

	void update(float deltaTime);

	void spawnEnemies();

private:
	 int spawnRate{};
	 int spawnAmount{};
	 int spawnType{};
	 float timeSinceLastSpawn{};

};