#pragma once
#include "../BaseEntity.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <raylib.h>
#include <box2d/box2d.h>
#include <LDtkLoader/Entity.hpp>
#include<entities/Camera/Camera.hpp>
#include<entities/Weapons/Weapon.hpp>

using namespace std;

enum PlayerAnimationState
{
    IDLE,
    WALK,
    JUMP_START,
    JUMP_APEX,
    JUMP_FALL
};

class Player : public BaseEntity
{
private:
    Texture2D sprite;
    b2Body* body{};
    b2Vec2 level_spawn_position;
    bool is_touching_floor = true;
    bool looking_right = true;
    const float animation_frame_duration = 0.2f;
    float animation_ticker = animation_frame_duration;
    size_t current_anim_frame = 0;
    PlayerAnimationState anim_state = PlayerAnimationState::IDLE;
    unordered_map<PlayerAnimationState, vector<Rectangle>> animation_map;
    GameCamera camera;

    void set_velocity_x(float vx);
    void set_velocity_y(float vy);
    void set_velocity_xy(float vx, float vy);
    bool can_move_in_x_direction(bool moving_right);
    void check_if_on_floor();
    void check_if_jump();
    void check_if_move();
    void check_if_should_respawn();
    void update_camera();  // Added camera update method

public:
    Player();
    ~Player();
    void update(float dt) override;
    void draw() override;
    Vector2 get_position();
    void intializeInventory();
    void adjustCollisionBox(const b2Vec2& offset);
    GameCamera& get_camera() { return camera; }
    void init_for_level(const ldtk::Entity* entity, b2World* physicsWorld);

    static std::unique_ptr<Weapon> Sword;
    static std::unique_ptr<Weapon> Magic;
 
};