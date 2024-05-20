#pragma once

#include <raylib.h>

class Structure
{
public:
    Structure(float positionX, float positionY);
    ~Structure();
    void Draw();

public:
    Texture2D sprite;
    Rectangle bounds;
    int lives;
    bool isDestroyed;
};