#pragma once
#include "../BaseEntity.hpp"
#include <memory>
#include <raylib.h>
#include <box2d/box2d.h>
#include <unordered_map>
#include <vector>

enum EnemyAnimationState
{
    ENEMY_IDLE,
    ENEMY_WALK,
    ENEMY_ATTACK,
    ENEMY_HURT,
    ENEMY_DEAD
};

class Enemy : public BaseEntity
{
private:

    Texture2D sprite;
    b2Body* body{ nullptr };
    b2World* currentWorld = nullptr;

    b2Vec2 spawn_position;
    bool looking_right = true;

    // Animation variables
    const float animation_frame_duration = 0.2f;
    float animation_ticker = animation_frame_duration;
    size_t current_anim_frame = 0;
    EnemyAnimationState anim_state = EnemyAnimationState::ENEMY_IDLE;
    std::unordered_map<EnemyAnimationState, std::vector<Rectangle>> animation_map;
    Vector2 targetPos{};
    float speed{ 1.5f };

    // Combat variables
    bool isHit = false;
    int health = 99;
    float hitCooldown = 0.5f;
    float currentHitCooldown = 0.0f;

    void setVelocity(float vx, float vy);
    void initializeAnimations();
    void updateHitState(float dt);

    // Enemy types
    const int enemyType{};

public:
    Enemy(Texture2D sprite, int enemyType);
    ~Enemy();
    void initForLevel(const b2Vec2& position, b2World* physicsWorld);
    void update(float dt) override;
    void draw() override;
    void move();
    void setTargetPos(Vector2 playerPos);
    Rectangle getHitbox() const;
    bool checkCollisionWithWeapon(const Rectangle& weaponHitbox) const;

    // Combat methods
    void onHit(int damage);
    bool shouldDestroy() const { return health <= 0; }
    bool isAlive() const { return health > 0; }
};