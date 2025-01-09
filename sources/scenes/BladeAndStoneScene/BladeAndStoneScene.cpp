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
    loadLevel();
}

BladeAndStoneScene::~BladeAndStoneScene()
{
}

void BladeAndStoneScene::draw()
{
	ClearBackground(RAYWHITE);

	//DrawRectangle(20, 20, 20, 20, RED);

	for (auto&& layer : currentLdtkLevel->allLayers())
	{
		if (layer.hasTileset())
		{
			cout << "Drawing Test" << AppConstants::GetAssetPath("BladeAndStoneAssets/" + layer.getTileset().path) << std::endl;


			currentTilesetTexture = LoadTexture(AppConstants::GetAssetPath("BladeAndStoneAssets/" + layer.getTileset().path).c_str());
			// if it is a tile layer then draw every tile to the frame buffer
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
    
}
Scenes BladeAndStoneScene::update(float dt)
{
	return Scenes::NONE;

}
void BladeAndStoneScene::loadLevel()
{
    ldtkProject = std::make_unique<ldtk::Project>();
    // Get ldtk map
    ldtkProject->loadFromFile(AppConstants::GetAssetPath("BladeAndStoneAssets/BladeAndStoneMap.ldtk"));
    // Get world
    ldtkWorld = &ldtkProject->getWorld();
    // Get ldtk level
    currentLdtkLevel = &ldtkWorld->getLevel("Level_0");
    // Get ground layer
    auto bg_layer = currentLdtkLevel->getLayer("Ground").getTileset();
    
   

	// Draw all tileset layers
	for (auto&& layer : currentLdtkLevel->allLayers())
	{
		if (layer.hasTileset())
		{
			cout << "Drawing Test" << AppConstants::GetAssetPath("BladeAndStoneAssets/" + layer.getTileset().path) << std::endl;
		

			currentTilesetTexture = LoadTexture(AppConstants::GetAssetPath("BladeAndStoneAssets/" + layer.getTileset().path).c_str());
			// if it is a tile layer then draw every tile to the frame buffer
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
}
