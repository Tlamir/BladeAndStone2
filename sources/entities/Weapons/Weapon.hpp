#pragma once
#include "../BaseEntity.hpp"
#include "raylib.h"
#include <memory>
#include"BulletManager.hpp"
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
        bool isMagicWeapon
    );

    ~Weapon();
    void update(float dt) override;
    void draw() override;
    void updatePosition(float posX, float posY, bool isLookingRight);
    void Attack(float dt);
    

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

    // Instance-specific state variables (no static)
    float elapsedTime{ 0.0f };
    bool isAttacking{ true };
    bool isWaitingAtPeak{ false };
    bool isReturning{ false };

    static std::unique_ptr<BulletManager> bulletManager;
    bool isMagicWeapon{ false };
};
