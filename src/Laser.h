#pragma once

#include <raylib.h>

class Laser
{
public:
    Laser(float positionX, float positionY);
    ~Laser();
    void Draw();

public:
    Rectangle bounds;
};