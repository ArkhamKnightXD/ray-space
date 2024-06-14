#include "MysteryShip.h"

MysteryShip::MysteryShip(float positionX, float positionY)
{
    sprite = LoadTexture("assets/sprites/mystery.png");
        
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};

    points = 50;
    isDestroyed = false;
}

MysteryShip::~MysteryShip()
{
    UnloadTexture(sprite);
}

void MysteryShip::Draw()
{
    if (!isDestroyed)
        DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}