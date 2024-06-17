#pragma once

#include <raylib.h>

class MysteryShip
{
public:
    MysteryShip(float positionX, float positionY);
    ~MysteryShip();
    void Update(float deltaTime);
    void Draw();

private:
    Texture2D sprite;
    float velocityX;

public:
    Rectangle bounds;
    int points;
    bool isDestroyed;
    bool shouldMove;
};