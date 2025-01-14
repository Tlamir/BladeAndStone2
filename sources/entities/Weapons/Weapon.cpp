#include "Weapon.hpp"
#include "Constants.hpp"

Weapon::Weapon(Texture2D& weaponTexture, Vector2& positonBuffer, float activationRotaion, int textureGrid, float attackSpeed,
	float attackReloadSpeed, float attackWaitTime)
{
	this->sprite = weaponTexture;
	this->positonBuffer = positonBuffer;
	attackRotation = activationRotaion;
	spriteGridSize = textureGrid;
	this->attackSpeed = attackSpeed;
	this->attackReloadSpeed = attackReloadSpeed;
	this->attackWaitingAtPeak = attackWaitTime;

}

Weapon::~Weapon()
{
	UnloadTexture(this->sprite);
}

void Weapon::update(float dt)
{
	// Attack
	Attack(dt);
}

void Weapon::draw()
{
	Rectangle sourceRec{ 0,0 };
	sourceRec.width = sprite.width / spriteGridSize;
	sourceRec.height = sprite.height / spriteGridSize;
	Vector2 origin = { sourceRec.width, sourceRec.height };
	Rectangle dest = {
		position.x + positonBuffer.x,
		position.y + positonBuffer.y,
		sourceRec.width,
		sourceRec.height };
	DrawTexturePro(sprite, sourceRec, dest, origin, rotationActive, WHITE);
}

void Weapon::updatePosition(float posX, float posY, bool isLookingRight)
{
	position = { posX,posY };
	this->isLookingRight = isLookingRight;
}

void Weapon::Attack(float dt)
{
	const float ATTACK_TIME = attackSpeed;       // Time to reach full attack rotation
	const float WAIT_TIME = attackWaitingAtPeak; // Time to wait at the peak of the attack
	const float RETURN_TIME = attackReloadSpeed; // Time to return to the original position
	const float ATTACK_ANGLE = attackRotation;   // Full rotation angle for attack

	elapsedTime += dt;

	if (isAttacking)
	{
		// Attack Phase: Rotate to ATTACK_ANGLE in ATTACK_TIME
		if (elapsedTime >= ATTACK_TIME)
		{
			rotationActive = ATTACK_ANGLE; // Fully rotated
			elapsedTime = 0.0f;
			isAttacking = false;
			isWaitingAtPeak = true; // Move to wait phase after attack
		}
		else
		{
			// Interpolate smoothly from 0 to ATTACK_ANGLE
			float t = elapsedTime / ATTACK_TIME;
			t = t * t * (3.0f - 2.0f * t);
			rotationActive = ATTACK_ANGLE * t;
		}
	}
	else if (isWaitingAtPeak)
	{
		// Wait Phase: Hold at the peak rotation (ATTACK_ANGLE) for WAIT_TIME
		if (elapsedTime >= WAIT_TIME)
		{
			elapsedTime = 0.0f;
			isWaitingAtPeak = false;
			isReturning = true; // Start returning after the wait phase
		}
	}
	else if (isReturning)
	{
		// Return Phase: Smoothly return to 0 over RETURN_TIME
		if (elapsedTime >= RETURN_TIME)
		{
			rotationActive = 0.0f; // Reset to original position
			elapsedTime = 0.0f;
			isReturning = false;
			isAttacking = true; // Start the attack phase again
		}
		else
		{
			// Interpolate smoothly from ATTACK_ANGLE back to 0
			float t = elapsedTime / RETURN_TIME;
			t = t * t * (3.0f - 2.0f * t);
			rotationActive = ATTACK_ANGLE * (1.0f - t);
		}
	}
}
