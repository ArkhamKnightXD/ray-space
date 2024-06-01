#include "MysteryShip.h"

MysteryShip::MysteryShip(float positionX, float positionY, int alienPoints)
{
    sprite = LoadTexture("assets/sprites/mystery.png");
        
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};

    points = alienPoints;
    isDestroyed = false;
}

MysteryShip::~MysteryShip()
{
    // UnloadTexture(sprite);
}

void MysteryShip::Draw()
{
    if (!isDestroyed)
        DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}