#pragma once
#include <box2d/box2d.h>
#include <raylib.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>

#include "../BaseScene.hpp"
#include "../Scenes.hpp"

class BladeAndStoneScene:public BaseScene
{
public:
	BladeAndStoneScene();
	~BladeAndStoneScene();

	void draw() override;
	Scenes update(float dt) override;

private:

	

};