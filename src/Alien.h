#pragma once

#include <raylib.h>

class Alien
{
public:
    Alien(float positionX, float positionY, int points, int row);
    ~Alien();
    void Draw();

public:
    Texture2D sprite;
    Rectangle bounds;
    int points;
    bool isDestroyed;
};