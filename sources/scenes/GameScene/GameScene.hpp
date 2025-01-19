#pragma once

#include <memory> 
#include <box2d/box2d.h>
#include <raylib.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>

#include "../BaseScene.hpp"
#include "../Scenes.hpp"

#include "../../entities/Player/Player.hpp"
#include "./entities/BaseEntity.hpp"
#include "./entities/Enemies/EnemySpawner.hpp"

class GameScene : public BaseScene
{
private:
    int current_level;
    int killedEnemies{0};
    bool isPlayerAlive{ true };

    std::unique_ptr<ldtk::Project> ldtkProject;
    const ldtk::World *ldtkWorld{};
    const ldtk::Level *currentLdtkLevel{};

    Texture2D currentTilesetTexture;
    Texture2D renderedLevelTexture;

    std::vector<std::unique_ptr<EnemySpawner>> enemySpawners;


public:
    GameScene();
    ~GameScene();

    static std::unique_ptr<b2World> world;
    static std::unique_ptr<Player> player;

    void draw() override;
    Scenes update(float dt) override;

    void setSelectedLevel(int lvl);

    void createSolidBlock(float targetX, float targetY, float tileSize);

    void CheckCollisions(std::unique_ptr<EnemySpawner>& enemySpawner);

};
