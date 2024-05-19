#pragma once

#include <raylib.h>

class Ball
{
public:
    Ball(float positionX, float positionY, Sound sound);
    void ResetPosition();
    void Update(float deltaTime);
    void Draw();
    bool HasCollide(Rectangle playerBounds);

public:
    Vector2 position;
    Vector2 velocity;
    int radius;
    Sound hitWallSound;
};