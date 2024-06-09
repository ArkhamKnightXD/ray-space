#pragma once

#include <raylib.h>

class Alien
{
public:
    Alien(float positionX, float positionY, int points, Texture2D sprite);
    ~Alien();
    void Update(float deltaTime);
    void Draw();

private:
    Texture2D sprite;

public:
    Rectangle bounds;
    int points;
    int velocity;
    bool isDestroyed;
};