#pragma once

#include <raylib.h>

class Structure
{
public:
    Structure(float positionX, float positionY, Texture2D textureSprite);
    void Draw();

private:
    Texture2D sprite;
    
public:
    Rectangle bounds;
    int lives;
    bool isDestroyed;
};