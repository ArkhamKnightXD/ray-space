#include "alien.h"

Alien::Alien(float positionX, float positionY, int alienPoints, int row)
{
    switch (row)  {

        case 0:
            sprite = LoadTexture("assets/sprites/alien_3.png");
            break;

        case 1:
        case 2:
            sprite = LoadTexture("assets/sprites/alien_2.png");
            break;

        default:
            sprite = LoadTexture("assets/sprites/alien_1.png");
    }
        
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};

    points = alienPoints;
    isDestroyed = false;
}

Alien::~Alien()
{
    // UnloadTexture(sprite);
}

void Alien::Update()
{
}

void Alien::Draw()
{
    if (!isDestroyed)
        DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}