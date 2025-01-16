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
    ENEMY_ATTACK
};

class Enemy : public BaseEntity
{
private:
    // Enemy sprite and physical body
    Texture2D sprite;
    b2Body* body{ nullptr };

    // Enemy's spawn position and state
    b2Vec2 spawn_position;
    bool looking_right = true;

    // Animation variables
    const float animation_frame_duration = 0.2f;
    float animation_ticker = animation_frame_duration;
    size_t current_anim_frame = 0;
    EnemyAnimationState anim_state = EnemyAnimationState::ENEMY_IDLE;
    std::unordered_map<EnemyAnimationState, std::vector<Rectangle>> animation_map;

    void set_velocity(float vx, float vy);
    void initializeAnimations();

public:
    Enemy();
    ~Enemy();

    void init_for_level(const b2Vec2& position, b2World* physicsWorld);
    void update(float dt) override;
    void draw() override;
};
