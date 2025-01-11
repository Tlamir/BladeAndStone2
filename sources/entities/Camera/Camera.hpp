#pragma once
#include<raylib.h>
#include<box2d/box2d.h>
class GameCamera
{
public:
    GameCamera();
    ~GameCamera() = default;

    void follow(const b2Vec2& target);
    void update();
    Camera2D& get_camera() { return camera; }

    // Optional: you can add these features later
    void set_zoom(float zoom);
    void set_offset(Vector2 offset);
    void set_rotation(float rotation);

private:
    Camera2D camera;
    float smooth_speed = 0.1f;
    Vector2 target_position{ 0.0f, 0.0f };
};