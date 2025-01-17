#pragma once
#include <string>
using namespace std;

namespace PhysicsTypes
{
    // Existing collision types as strings
    const string SolidBlock = "SOLID_BLOCK";

    // Add new collision categories for Box2D filtering
    namespace Categories
    {
        static const uint16 DEFAULT = 0x0001;
        static const uint16 PLAYER = 0x0002;
        static const uint16 ENEMY = 0x0004;
        static const uint16 WEAPON = 0x0008;
        static const uint16 SOLID = 0x0010;
    }
}