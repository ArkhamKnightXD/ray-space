#pragma once

#include <raylib.h>

class Laser
{
public:
    Laser(float positionX, float positionY, bool isAlienLaser);
    void Update(float deltaTime);
    void Draw();

private:
    bool isAlienLaser;

public:
    Rectangle bounds;
    bool isDestroyed;
};