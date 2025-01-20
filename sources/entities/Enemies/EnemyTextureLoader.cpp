#include "EnemyTextureLoader.hpp"
#include "Constants.hpp"

EnemyTextureLoader& EnemyTextureLoader::getInstance()
{
	static EnemyTextureLoader instance; // Single instance
	return instance;
}

EnemyTextureLoader::EnemyTextureLoader()
{
	loadTextures();
}

EnemyTextureLoader::~EnemyTextureLoader()
{
	for (Texture2D& texture : enemyTextures)
	{
		UnloadTexture(texture);
	}
}

void EnemyTextureLoader::loadTextures()
{
	const std::string basePath = AppConstants::GetAssetPath("Characters/");
	const std::string textureFiles[] = {
		"demon_.png",
		"ghost_.png",
		"goblin_.png",
		"hobgoblin_.png",
		"orc_.png",
		"skeleton_.png",
		"slimeball_.png",
		"dragon_.png"
	};

	for (size_t i = 0; i < 8; ++i)
	{
		enemyTextures[i] = LoadTexture((basePath + textureFiles[i]).c_str());
	}
}

Texture2D EnemyTextureLoader::getEnemyTexture(EnemyTextures enemyType)
{
	auto& instance = getInstance();

	if (enemyType < DEMON || enemyType > DRAGON)
	{
		throw std::out_of_range("Invalid enemy type requested");
	}

	return instance.enemyTextures[enemyType];
}
