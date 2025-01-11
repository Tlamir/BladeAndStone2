#include "Camera.hpp"
#include <Constants.hpp>

GameCamera::GameCamera()
{
    camera.zoom = 1.0f;
   /* camera.offset = {
        AppConstants::ScreenWidth / 2.0f,
        AppConstants::ScreenHeight / 2.0f
    };*/
    camera.offset = {
       128.f,
       128.f
    };
    camera.rotation = 0.0f;
    camera.target = { 100.0f, 100.0f };
}

void GameCamera::follow(const b2Vec2& target)
{
    target_position = {
        target.x * GameConstants::PhysicsWorldScale,
        target.y * GameConstants::PhysicsWorldScale
    };
}

void GameCamera::update()
{
    // Smooth camera movement
    camera.target.x += (target_position.x - camera.target.x) * smooth_speed;
    camera.target.y += (target_position.y - camera.target.y) * smooth_speed;
}

void GameCamera::set_zoom(float zoom)
{
    camera.zoom = zoom;
}

void GameCamera::set_offset(Vector2 offset)
{
    camera.offset = offset;
}

void GameCamera::set_rotation(float rotation)
{
    camera.rotation = rotation;
}