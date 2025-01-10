#pragma once
#include <raylib.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>

#include "../BaseScene.hpp"
#include "../Scenes.hpp"

#include "../../entities/Player/PlayerCharacter.hpp"

class BladeAndStoneScene:public BaseScene
{
public:
	BladeAndStoneScene();
	~BladeAndStoneScene();
	
	std::unique_ptr<ldtk::Project> ldtkProject;
	static std::unique_ptr<PlayerCharacter> player;

	const ldtk::World* ldtkWorld{};
	const ldtk::Level* currentLdtkLevel{};

	Texture2D currentTilesetTexture;
	Texture2D renderedLevelTexture;

	void draw() override;
	Scenes update(float dt) override;
	


private:

	void loadLevel();

};