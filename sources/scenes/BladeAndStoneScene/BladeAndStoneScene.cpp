#include <memory>
#include <raylib.h>
#include <box2d/box2d.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>
#include <fmt/core.h>
#include <iostream>

#include <Constants.hpp>
#include <utils/DebugUtils.hpp>

#include "BladeAndStoneScene.hpp"
#include "../Scenes.hpp"

#include "./entities/BaseEntity.hpp"
#include <raymath.h>

// Character attributes
Vector2 characterPosition = { AppConstants::ScreenWidth / 2, AppConstants::ScreenHeight / 2 }; // Initial position
float characterSpeed = 200.0f;            // Movement speed
std::unique_ptr<PlayerCharacter> BladeAndStoneScene::player = nullptr;

// Camera system
Camera2D camera = { 0 };

// Map size (to limit movement)
float mapWidth = 0.0f;
float mapHeight = 0.0f;

BladeAndStoneScene::BladeAndStoneScene()
{
    loadLevel();

    // Initialize camera
    camera.target = characterPosition;   // Camera focuses on the character
    camera.offset = { 128, 128 };
    camera.zoom = 1.0f;                  // Default zoom level

    // Set map size (retrieved from LDtk level)
    mapWidth = currentLdtkLevel->size.x;
    mapHeight = currentLdtkLevel->size.y;
}

BladeAndStoneScene::~BladeAndStoneScene()
{
}

void BladeAndStoneScene::draw()
{
    ClearBackground(RAYWHITE);

    BeginMode2D(camera); // Start drawing with the camera applied

    // Fetch layers
    for (auto&& layer : currentLdtkLevel->allLayers())
    {   
        // Draw layers with tileset
        if (layer.hasTileset())
        {
            currentTilesetTexture = LoadTexture(AppConstants::GetAssetPath("BladeAndStoneAssets/" + layer.getTileset().path).c_str());
            // If it's a tile layer, draw every tile to the frame buffer
            for (auto&& tile : layer.allTiles())
            {
                auto source_pos = tile.getTextureRect();
                auto tile_size = float(layer.getTileset().tile_size);

                Rectangle source_rect = {
                    .x = float(source_pos.x),
                    .y = float(source_pos.y),
                    .width = tile.flipX ? -tile_size : tile_size,
                    .height = tile.flipY ? -tile_size : tile_size,
                };

                Vector2 target_pos = {
                    (float)tile.getPosition().x,
                    (float)tile.getPosition().y,
                };

                DrawTextureRec(currentTilesetTexture, source_rect, target_pos, WHITE);
            }
        }

    }
    // get entity Actor positions
    DebugUtils::println("Entities in level:");
    for (auto&& entity : currentLdtkLevel->getLayer("Actors").allEntities())
    {
        DebugUtils::println("  - {}", entity.getName());
        if (entity.getName() == "Player")
        {
            //player->level_spawn_position.x= entity.getPosition().x;
            //player->level_spawn_position.y= entity.getPosition().y;
        }

       // Implement wall colision layer here
    }
    // get entity Wall positions
    DebugUtils::println("Entities in level:");
    for (auto&& entity : currentLdtkLevel->getLayer("Collision").allEntities())
    {
        DebugUtils::println("  - {}", entity.getName());
        if (entity.getName() == "Wall")
        {
            //player->level_spawn_position.x= entity.getPosition().x;
            //player->level_spawn_position.y= entity.getPosition().y;
        }

        // Implement wall colision layer here
    }
    //Draw Character
    player->draw();

    // Draw character (fixed at the center of the screen)
    //DrawCircleV(characterPosition, 10, RED); // Character as a red circle
    EndMode2D(); // End drawing with the camera applied
}

Scenes BladeAndStoneScene::update(float dt)
{
    // Handle input for character movement
    if (IsKeyDown(KEY_RIGHT)) characterPosition.x += characterSpeed * dt;
    if (IsKeyDown(KEY_LEFT)) characterPosition.x -= characterSpeed * dt;
    if (IsKeyDown(KEY_UP)) characterPosition.y -= characterSpeed * dt;
    if (IsKeyDown(KEY_DOWN)) characterPosition.y += characterSpeed * dt;

    // Limit movement to the map boundaries
    //characterPosition.x = Clamp(characterPosition.x, 0.0f, mapWidth);
    //characterPosition.y = Clamp(characterPosition.y, 0.0f, mapHeight);

    // Update camera to follow the character
    camera.target = characterPosition;

    return Scenes::NONE;
}

void BladeAndStoneScene::loadLevel()
{
    ldtkProject = std::make_unique<ldtk::Project>();
    // Get LDtk map
    ldtkProject->loadFromFile(AppConstants::GetAssetPath("BladeAndStoneAssets/BladeAndStoneMap.ldtk"));
    // Get world
    ldtkWorld = &ldtkProject->getWorld();
    // Get player
    player = std::make_unique<PlayerCharacter>();
    // Get LDtk level
    currentLdtkLevel = &ldtkWorld->getLevel("Level_0");
    // Get ground layer
    //auto bg_layer = currentLdtkLevel->getLayer("Ground").getTileset();
}
