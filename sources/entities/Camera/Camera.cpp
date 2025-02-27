#include "Camera.hpp"
#include <Constants.hpp>

GameCamera::GameCamera()
{
    camera.zoom = GameConstants::cameraZoom;
    camera.offset = {
        GameConstants::WorldWidth/2,
        GameConstants::WorldHeight/ 2
    };
    camera.rotation = 0.0f;
    camera.target = { 200.0f, 200.0f };
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