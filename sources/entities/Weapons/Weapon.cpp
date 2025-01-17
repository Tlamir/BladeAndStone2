#include "Weapon.hpp"
#include "Constants.hpp"
#include "BulletManager.hpp"
#include <physics/PhysicsTypes.hpp>

std::unique_ptr<BulletManager> Weapon::bulletManager = nullptr;



Weapon::Weapon(Texture2D& weaponTexture, Vector2& positonBuffer, float activationRotaion,
	int textureGrid, float attackSpeed, float attackReloadSpeed, float attackWaitTime,
	bool isMagicWeapon, int selectedWeaponFromTileset, b2World* world)
{
	this->sprite = weaponTexture;
	this->positonBuffer = positonBuffer;
	this->physicsWorld = world;
	attackRotation = activationRotaion;
	spriteGridSize = textureGrid;
	this->attackSpeed = attackSpeed;
	this->attackReloadSpeed = attackReloadSpeed;
	this->attackWaitingAtPeak = attackWaitTime;
	this->isMagicWeapon = isMagicWeapon;
	this->selectedWeaponFromTileset = selectedWeaponFromTileset;

	bulletManager = std::make_unique<BulletManager>();

	// Create weapon physics body
	if (physicsWorld)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.bullet = true;

		weaponBody = physicsWorld->CreateBody(&bodyDef);

		b2PolygonShape weaponShape;
		weaponShape.SetAsBox(0.5f, 0.1f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &weaponShape;
		fixtureDef.isSensor = true;
		fixtureDef.filter.categoryBits = PhysicsTypes::Categories::WEAPON;
		fixtureDef.filter.maskBits = PhysicsTypes::Categories::ENEMY;

		weaponBody->CreateFixture(&fixtureDef);
	}
}

Weapon::~Weapon()
{
	UnloadTexture(this->sprite);
}

void Weapon::update(float dt)
{
	// Attack
	Attack(dt);
	if (isMagicWeapon) bulletManager->update(dt);

}

void Weapon::draw()
{
	Rectangle sourceRec{ 0,0 };
	sourceRec = selectWeaponFromTexture(sprite, selectedWeaponFromTileset, spriteGridSize);
	Vector2 origin = { sourceRec.width, sourceRec.height };
	Rectangle dest = {
		position.x + positonBuffer.x,
		position.y + positonBuffer.y,
		sourceRec.width,
		sourceRec.height };
	DrawTexturePro(sprite, sourceRec, dest, origin, rotationActive, WHITE);

	if (isMagicWeapon) bulletManager->draw();
	if (GameConstants::debugModeCollision && !isMagicWeapon)
	{
		drawHitbox();

	}
}

Rectangle Weapon::selectWeaponFromTexture(Texture2D sprite, int index, int spriteGridSize)
{
	int row = index / spriteGridSize;  // Calculate the row (y-axis) in the grid
	int col = index % spriteGridSize;  // Calculate the column (x-axis) in the grid

	// Define the width and height of each cell in the grid
	float cellWidth = static_cast<float>(sprite.width) / spriteGridSize;
	float cellHeight = static_cast<float>(sprite.height) / spriteGridSize;

	// Define the source rectangle for the selected part of the texture
	Rectangle sourceRec = {
		col * cellWidth,
		row * cellHeight,
		cellWidth,
		cellHeight
	};

	return sourceRec;
}

void Weapon::updatePosition(float posX, float posY, bool isLookingRight)
{
	position = { posX, posY };
	this->isLookingRight = isLookingRight;

	// Update physics body position
	if (weaponBody)
	{
		weaponBody->SetTransform(
			b2Vec2(posX / GameConstants::PhysicsWorldScale,
				posY / GameConstants::PhysicsWorldScale),
			weaponBody->GetAngle());
	}
}


void Weapon::Attack(float dt)
{
	const float ATTACK_TIME = attackSpeed;
	const float WAIT_TIME = attackWaitingAtPeak;
	const float RETURN_TIME = attackReloadSpeed;
	const float ATTACK_ANGLE = attackRotation;

	elapsedTime += dt;

	if (isAttacking)
	{
		if (elapsedTime >= ATTACK_TIME)
		{
			rotationActive = ATTACK_ANGLE;
			elapsedTime = 0.0f;
			isAttacking = false;
			isWaitingAtPeak = true;
			if (isMagicWeapon)
			{
				Vector2 bulletStartPos = { position.x + positonBuffer.x, position.y + positonBuffer.y };
				bulletManager->fireBullet(bulletStartPos);
			}
		}
		else
		{
			float t = elapsedTime / ATTACK_TIME;
			t = t * t * (3.0f - 2.0f * t);
			rotationActive = ATTACK_ANGLE * t;
		}
	}
	else if (isWaitingAtPeak)
	{
		if (elapsedTime >= WAIT_TIME)
		{
			elapsedTime = 0.0f;
			isWaitingAtPeak = false;
			isReturning = true;
		}
	}
	else if (isReturning)
	{
		if (elapsedTime >= RETURN_TIME)
		{
			rotationActive = 0.0f;
			elapsedTime = 0.0f;
			isReturning = false;
			isAttacking = true; // Reset the attack cycle
		}
		else
		{
			float t = elapsedTime / RETURN_TIME;
			t = t * t * (3.0f - 2.0f * t);
			rotationActive = ATTACK_ANGLE * (1.0f - t);
		}
	}
}

Rectangle Weapon::getHitbox() const
{
	if (!isAttacking && !isWaitingAtPeak)
	{
		return Rectangle{ 0, 0, 0, 0 };
	}

	// Calculate the size of the weapon sprite in the grid
	float weaponWidth = static_cast<float>(sprite.width) / spriteGridSize;
	float weaponHeight = static_cast<float>(sprite.height) / spriteGridSize;

	// Calculate the hitbox dimensions
	float hitboxWidth = weaponWidth * 1;  
	float hitboxHeight = weaponHeight * 2; 

	// Center the hitbox around the weapon's position
	float hitboxX = position.x + positonBuffer.x - (hitboxWidth);
	float hitboxY = position.y + positonBuffer.y - (hitboxHeight / 2);

	return Rectangle{ hitboxX, hitboxY, hitboxWidth, hitboxHeight };
}

void Weapon::drawHitbox() const
{
	Rectangle hitbox = getHitbox();

	if (hitbox.width > 0 && hitbox.height > 0)
	{
		DrawRectangleLines(
			static_cast<int>(hitbox.x),
			static_cast<int>(hitbox.y),
			static_cast<int>(hitbox.width),
			static_cast<int>(hitbox.height),
			RED
		);
		DrawCircle(static_cast<int>(position.x + positonBuffer.x),
			static_cast<int>(position.y + positonBuffer.y),
			3,
			GREEN);
	}
}