#pragma once

#include <raylib.h>

class Laser
{
public:
    Laser(float positionX, float positionY);
    ~Laser();
    void Update(float deltaTime);
    void Draw();

public:
    Rectangle bounds;
    bool isDestroyed;
};