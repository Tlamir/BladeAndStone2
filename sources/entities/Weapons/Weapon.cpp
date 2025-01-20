#include "Weapon.hpp"
#include "Constants.hpp"
#include "BulletManager.hpp"
#include <physics/PhysicsTypes.hpp>
#include <utils/SoundManager.hpp>

std::unique_ptr<BulletManager> Weapon::bulletManager = nullptr;

Weapon::Weapon(
    Texture2D& weaponTexture,
    Vector2& positionBuffer,
    float activationRotation,
    int textureGrid,
    float attackSpeed,
    float attackReloadSpeed,
    float attackWaitTime,
    bool isMagicWeapon,
    int selectedWeaponFromTileset,
    b2World* world,
    int weaponDamage)
    : sprite(weaponTexture),
    positionBuffer(positionBuffer),
    attackRotation(activationRotation),
    spriteGridSize(textureGrid),
    attackSpeed(attackSpeed),
    attackReloadSpeed(attackReloadSpeed),
    attackWaitingAtPeak(attackWaitTime),
    isMagicWeapon(isMagicWeapon),
    selectedWeaponFromTileset(selectedWeaponFromTileset),
    damage(weaponDamage),
    physicsWorld(world)
{
    bulletManager = std::make_unique<BulletManager>();

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

        isMagicWeapon ? soundName="spell" : soundName="sword";
    }

}

Weapon::~Weapon()
{
    UnloadTexture(sprite);
}

void Weapon::update(float dt)
{
    attack(dt);
    if (isMagicWeapon) bulletManager->update(dt);
}

void Weapon::draw()
{
    Rectangle sourceRec = selectWeaponFromTexture(sprite, selectedWeaponFromTileset, spriteGridSize);
    Vector2 origin = { sourceRec.width, sourceRec.height };
    Rectangle dest = {
        position.x + positionBuffer.x,
        position.y + positionBuffer.y,
        sourceRec.width,
        sourceRec.height
    };

    DrawTexturePro(sprite, sourceRec, dest, origin, rotationActive, WHITE);

    if (isMagicWeapon) bulletManager->draw();

    if (GameConstants::debugModeCollision && !isMagicWeapon)
    {
        drawHitbox();
    }
}

Rectangle Weapon::selectWeaponFromTexture(Texture2D sprite, int index, int spriteGridSize)
{
    int row = index / spriteGridSize;
    int col = index % spriteGridSize;

    float cellWidth = static_cast<float>(sprite.width) / spriteGridSize;
    float cellHeight = static_cast<float>(sprite.height) / spriteGridSize;

    return Rectangle{
        col * cellWidth,
        row * cellHeight,
        cellWidth,
        cellHeight
    };
}

void Weapon::updatePosition(float posX, float posY, bool isLookingRight)
{
    position = { posX, posY };
    this->isLookingRight = isLookingRight;

    if (weaponBody)
    {
        weaponBody->SetTransform(
            b2Vec2(posX / GameConstants::PhysicsWorldScale,
                posY / GameConstants::PhysicsWorldScale),
            weaponBody->GetAngle());
    }
}

void Weapon::attack(float dt)
{
    const float ATTACK_TIME = attackSpeed;
    const float WAIT_TIME = attackWaitingAtPeak;
    const float RETURN_TIME = attackReloadSpeed;
    const float ATTACK_ANGLE = attackRotation;
    // Initialize sound manager and play start sound
    auto soundManager = SoundManager::getInstance();
    

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
                Vector2 bulletStartPos = { position.x + positionBuffer.x, position.y + positionBuffer.y / 2 };
                bulletManager->fireBullet(bulletStartPos, physicsWorld);
                
            }
            soundManager->playSoundEffect(soundName);
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
            isAttacking = true;
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

    float weaponWidth = static_cast<float>(sprite.width) / spriteGridSize;
    float weaponHeight = static_cast<float>(sprite.height) / spriteGridSize;

    float hitboxWidth = weaponWidth;
    float hitboxHeight = weaponHeight * 2;

    float hitboxX = position.x + positionBuffer.x - hitboxWidth;
    float hitboxY = position.y + positionBuffer.y - (hitboxHeight / 2);

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
        DrawCircle(
            static_cast<int>(position.x + positionBuffer.x),
            static_cast<int>(position.y + positionBuffer.y),
            3,
            GREEN
        );
    }
}

BulletManager* Weapon::getBulletManager()
{
    return bulletManager.get();
}
