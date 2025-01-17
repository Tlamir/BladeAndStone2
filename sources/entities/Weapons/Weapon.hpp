#pragma once
#include "../BaseEntity.hpp"
#include "raylib.h"
#include <memory>
#include "BulletManager.hpp"
#include <box2d/box2d.h>

class Weapon : public BaseEntity
{
public:
    Weapon(
        Texture2D& weaponTexture,
        Vector2& positonBuffer,
        float activationRotaion,
        int textureGrid,
        float attackSpeed,
        float attackReloadSpeed,
        float attackWaitTime,
        bool isMagicWeapon,
        int selectedWeaponFromTileset,
        b2World* world,
        int damage
    );
    ~Weapon();
    void update(float dt) override;
    void draw() override;
    Rectangle selectWeaponFromTexture(Texture2D sprite, int index, int spriteGridSize);
    void updatePosition(float posX, float posY, bool isLookingRight);
    void Attack(float dt);
    Rectangle getHitbox() const;
    BulletManager* getBulletManager();


    void drawHitbox() const;
    int damage{ 50 };
private:
    Texture2D sprite;
    // Player pos + pos buffer = weapon pos
    Vector2 positonBuffer{};
    Vector2 origin = {};
    // Player pos
    Vector2 position{ 0.f,0.f };
    Rectangle destRec = {};
    Rectangle sourceRec{};
    int spriteGridSize{ 4 };
    float attackRotation{ 0.f };
    float rotationActive{ 0.f };
    float attackSpeed{ 1.f };
    float attackWaitingAtPeak{ 0.f };
    float attackReloadSpeed{ 1.f };
    bool isLookingRight{ false };
    int selectedWeaponFromTileset{ 0 };

    float elapsedTime{ 0.0f };
    bool isAttacking{ true };
    bool isWaitingAtPeak{ false };
    bool isReturning{ false };
    static std::unique_ptr<BulletManager> bulletManager;
    bool isMagicWeapon{ false };

    // Physics
    b2Body* weaponBody = nullptr;
    b2World* physicsWorld = nullptr;
};