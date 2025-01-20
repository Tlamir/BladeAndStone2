#pragma once

#include "../BaseEntity.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <raylib.h>
#include <box2d/box2d.h>
#include <LDtkLoader/Entity.hpp>
#include <entities/Camera/Camera.hpp>
#include <entities/Weapons/Weapon.hpp>

// Animation states for the player
enum PlayerAnimationState
{
    IDLE,
    WALK,
    HURT,
    DEAD
};

class Player : public BaseEntity
{
private:
    // Sprite and physical body
    Texture2D sprite;
    b2Body* body{ nullptr };

    // Spawn position and movement states
    b2Vec2 level_spawn_position;
    bool is_touching_floor = true;
    bool looking_right = true;

    // Animation properties
    const float animation_frame_duration = 0.2f;
    float animation_ticker = animation_frame_duration;
    size_t current_anim_frame = 0;
    PlayerAnimationState anim_state = PlayerAnimationState::IDLE;
    std::unordered_map<PlayerAnimationState, std::vector<Rectangle>> animation_map;

    // Camera
    GameCamera camera;

    // Weapons
    static std::unique_ptr<Weapon> Sword;
    static std::unique_ptr<Weapon> Magic;

    // Player health
    int health = 10000;

    // Movement and physics utilities
    void setVelocityX(float vx);
    void setVelocityY(float vy);
    void setVelocityXY(float vx, float vy);
    void checkIfMove();
    void checkIfRespawn();

    // Camera and weapon updates
    void updateCamera();

public:
    // Constructor and destructor
    Player();
    ~Player();

    // Core functionality
    void update(float dt) override;
    void draw() override;

    // Position and camera accessor
    Vector2 getPosition();
    GameCamera& get_camera() { return camera; }

    // Initialization and inventory setup
    void initForLevel(const ldtk::Entity* entity, b2World* physicsWorld);
    void intializeInventory(b2World* physicsWorld);

    // Weapons and collision handling
    Weapon* getWeapon();
    BulletManager* getBulletManager();
    bool checkCollisionWithEnemy(const Rectangle& enemyHitbox);
    Rectangle getHitbox();

    // Damage handling
    void getDamage(int damage);
    bool isAlive();
    int getHealth();

    // Player variables
    float playerSpeed;
};
