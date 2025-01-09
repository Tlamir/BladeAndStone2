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
}
Scenes BladeAndStoneScene::update(float dt)
{
	return Scenes::NONE;

}