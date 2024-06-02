#pragma once

#include <raylib.h>

class MysteryShip
{
public:
    MysteryShip(float positionX, float positionY, int points);
    ~MysteryShip();
    void Draw();

private:
    Texture2D sprite;

public:
    Rectangle bounds;
    int points;
    bool isDestroyed;
};