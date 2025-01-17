#pragma once

#include <raylib.h>
#include <string>
#include <stdexcept>

class EnemyTextureLoader
{
public:
    enum EnemyTextures
    {
        DEMON,
        DRAGON,
        GHOST,
        GOBLIN,
        HOBGOBLIN,
        ORC,
        SKELETON,
        SLIMEBALL
    };

    static EnemyTextureLoader& getInstance();

    static Texture2D getEnemyTexture(EnemyTextures enemyType);

private:
    EnemyTextureLoader();
    ~EnemyTextureLoader();

    EnemyTextureLoader(const EnemyTextureLoader&) = delete;
    EnemyTextureLoader& operator=(const EnemyTextureLoader&) = delete;

    void loadTextures();

    Texture2D enemyTextures[8]{};
};
