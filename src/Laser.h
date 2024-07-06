#pragma once

#include <raylib.h>

class Laser
{
public:
    Laser(float positionX, float positionY, bool isAlienLaser);
    void CheckIfShouldBeDestroy();
    void Draw();

private:
    bool isAlienLaser;

public:
    Rectangle bounds;
    bool isDestroyed;
};