#include "player.h"
#include <raylib.h>

Player::Player(float positionX, float positionY)
{
    bounds = Rectangle{positionX, positionY, 64, 16};
    speed = 600;
    score = 0;
    lives = 2;
}

void Player::Update(float deltaTime)
{
    if (IsKeyDown(KEY_A) && bounds.x >= 0)
        bounds.x -= speed * deltaTime;

    if (IsKeyDown(KEY_D) && bounds.x <= GetScreenWidth() - bounds.width)
        bounds.x += speed * deltaTime;
}

void Player::Draw()
{
    DrawRectangleRounded(bounds, 0.8, 0, WHITE);
}