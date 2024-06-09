#include "Alien.h"

//There is a method to load all 
Alien::Alien(float positionX, float positionY, int alienPoints, Texture2D sprite)
{
    this->sprite = sprite;
        
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};

    points = alienPoints;
    isDestroyed = false;
    velocity = 50;
}

Alien::~Alien()
{
    // UnloadTexture(sprite);
}

void Alien::Update(float deltaTime)
{
    bounds.x += velocity * deltaTime;
}

void Alien::Draw()
{
    if (!isDestroyed)
        DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}