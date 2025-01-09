#include <memory> 
#include <raylib.h>
#include <box2d/box2d.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>
#include <fmt/core.h>

#include <Constants.hpp>
#include <utils/DebugUtils.hpp>

#include "BladeAndStoneScene.hpp"
#include "../../physics/PhysicsTypes.hpp"
#include "../Scenes.hpp"

#include "./entities/BaseEntity.hpp"

BladeAndStoneScene::BladeAndStoneScene()
{
}

BladeAndStoneScene::~BladeAndStoneScene()
{
}

void BladeAndStoneScene::draw()
{
	ClearBackground(RAYWHITE);

	DrawRectangle(20, 20, 20, 20, RED);

    ldtkProject = std::make_unique<ldtk::Project>();

    ldtkProject->loadFromFile(AppConstants::GetAssetPath("BladeAndStoneAssets/BladeAndStoneMap.ldtk"));

    ldtkWorld = &ldtkProject->getWorld();
    const auto& level1 = ldtkWorld->getLevel("Level_0");
   
    const auto& bg_layer = level1.getLayer("GroundTile");
    Texture2D currentTilesetTexture = LoadTexture(AppConstants::GetAssetPath("BladeAndStoneAssets/"+bg_layer.getTileset().path).c_str());

    for (const auto& tile : bg_layer.allTiles())
    {
        std::cout << tile.getGridPosition() << " ";
        auto source_pos = tile.getTextureRect();
        auto tile_size = float(bg_layer.getTileset().tile_size);

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
    std :: cout << endl;
    
   

     
   
    //// load the file
    //ldtk::Project ldtk_project;
    //ldtk_project.loadFromFile(AppConstants::GetAssetPath("BladeAndStoneAssets/BladeAndStoneMap.ldtk"));

    //// get a world
    //const auto& world = ldtk_project.getWorld("WORLD");



    // get a level
    //const auto& level1 = world.getLevel("Level_0");

    // get a layer
    //const auto& bg_layer = level1.getLayer("GroundTile");

    // iterate on the tiles of the layer
    //for (const auto& tile : bg_layer.allTiles())
    //{
    //    // do something with the tile (storing, rendering ...)
    //}

  
}
Scenes BladeAndStoneScene::update(float dt)
{
	return Scenes::NONE;

}
