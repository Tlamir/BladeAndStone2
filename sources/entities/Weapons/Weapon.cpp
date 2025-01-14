#include "Weapon.hpp"
#include "Constants.hpp"

Weapon::Weapon(Texture2D& weaponTexture, Vector2& positonBuffer, float activationRotaion, int textureGrid)
{
    this->sprite = weaponTexture;
    this->positonBuffer = positonBuffer;
    rotation = activationRotaion;
    tileAmout = textureGrid;   
}

Weapon::~Weapon()
{
    UnloadTexture(this->sprite);
}

void Weapon::update(float dt)
{
    float rotationSpeed = 5.0f; // Set a fixed rotation increment value

    if (IsKeyDown(KEY_SPACE))
    {
        rotation += rotationSpeed; // Increment rotation by a fixed amount
    }
    else
    {
        rotation -= rotationSpeed; // Decrement rotation by a fixed amount
    }
}

void Weapon::draw()
{
    // Fix later weapon pos
    Rectangle sourceRec{0,0};
    sourceRec.width = sprite.width / tileAmout; // Width of one tile
    sourceRec.height = sprite.height / tileAmout; // Height of one tile
    Vector2 origin = { sourceRec.width, sourceRec.height };
    Rectangle dest = { Position.x + positonBuffer.x, Position.y+positonBuffer.y, sourceRec.width, sourceRec.height };
    DrawTexturePro(sprite, sourceRec, dest, origin, rotation, WHITE);


}

void Weapon::updatePosition(float posX, float posY,bool isLookingRight)
{
	Position = { posX,posY };
    this->isLookingRight = isLookingRight;
}
