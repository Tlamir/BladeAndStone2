#pragma once

#include <string>

using namespace std;

const int ScreenScale = 3;
namespace GameConstants
{
    const int WorldWidth = 256;
    const int WorldHeight = 256;
    const int debugModeCollision = false;

    const int CellSize = 16;
    const float PhysicsWorldScale = 16.0f / ScreenScale; // everything is this times bigger than what physics world says
    const int  scale = ScreenScale;
}

namespace AppConstants
{
    const string WindowTitle = "BladeAndStone2";

    const int ScreenWidth = GameConstants::WorldWidth * ScreenScale;
    const int ScreenHeight = GameConstants::WorldHeight * ScreenScale;

    inline string GetAssetPath(string assetName)
    {
        return ASSETS_PATH "" + assetName;
    }
}
