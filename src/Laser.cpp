#include "Laser.h"

Laser::Laser(float positionX, float positionY, bool isAlienLaser)
{
    bounds = Rectangle{positionX, positionY, 4, 16};
    this->isAlienLaser = isAlienLaser;
}

void Laser::CheckIfShouldBeDestroy()
{
    if (bounds.y < 0 || bounds.y > GetScreenHeight())
        isDestroyed = true;    
}

void Laser::Draw()
{
    if (!isDestroyed)
        DrawRectangleRec(bounds, {243, 216, 63, 255});
}
