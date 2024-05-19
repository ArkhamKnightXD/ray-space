#pragma once

#include "ball.h"
#include <raylib.h>

class Brick
{
public:
    Brick(float positionX, float positionY, int brickPoints);
    void Draw();

public:
    Rectangle bounds;
    int brickPoints;
    bool isDestroyed;
};