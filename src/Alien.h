#pragma once

#include <raylib.h>

class Alien
{
public:
    Alien(float positionX, float positionY, int points, int row);
    ~Alien();
    void Draw();

private:
    Texture2D sprite;

public:
    Rectangle bounds;
    int points;
    bool isDestroyed;
};