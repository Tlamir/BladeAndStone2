#pragma once

#include <string>

using namespace std;

const int ScreenScale = 1;
namespace GameConstants
{
    // resolution
    const int WorldWidth = 1280;
    const int WorldHeight = 720;
    const int debugModeCollision = false;

    const int CellSize = 16;
    const float PhysicsWorldScale = 16.0f / ScreenScale; // everything is this times bigger than what physics world says
    const int  scale = ScreenScale;
    const float cameraZoom = 3;

    // Player variables
    const int playerHealth = 100;
    const float playerSpeed = 6.0f;

    // Enemy variables
    const int enemyHealth = 100;
    const float enemySpeed = 1.5f;
    const int enemyDamage = 1;

    /* Weapon variables
     Each weapon has 3 variables for attacking animation
     speed,wait,reload times 
     we can modify each weapon how it feels
     */
    const int swordDamage = 50;                     
    const float swordSpeed = 0.1f;
    const float swordWaitingAtPeak = 0.3f;
    const float swordReloadSpeed = 2.1f;
    const float swordActivationAngle = -150.f;

    // Each weapon texture is in square grid like 4*4 
    const int swordWeaponTextureGrid = 4;
    const int magicWeaponTextureGrid = 5;

    // Select which weapon from grid
    const int swordSelectWeaponFromGrid = 8; 
    const int magicSelectWeaponFromGrid = 18; 

    const int magicDamage = 25;
    const float magicSpeed = 0.1f;
    const float magicWaitingAtPeakTime = 0.1;
    const float magicReloadTime = 0.8f;
    const float magicActivationAngle = 90.f;


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
