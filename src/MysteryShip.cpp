#include "MysteryShip.h"

MysteryShip::MysteryShip(float positionX, float positionY)
{
    sprite = LoadTexture("assets/sprites/mystery.png");

    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};

    points = 50;
    velocityX = -100;
    isDestroyed = false;
    shouldMove = false;
}

MysteryShip::~MysteryShip()
{
    UnloadTexture(sprite);
}

void MysteryShip::Update(float deltaTime)
{
    if (shouldMove)
    {
        if (bounds.x > GetScreenWidth() + bounds.width || bounds.x < -80)
        {
            velocityX *= -1;
            shouldMove = false;
        }

        bounds.x += velocityX * deltaTime;
    }
}

void MysteryShip::Draw()
{
    if (!isDestroyed)
        DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}