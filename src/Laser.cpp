#include "Laser.h"

//Going to use my own code for the lasers.
Laser::Laser(float positionX, float positionY)
{
    bounds = Rectangle{positionX, positionY, 4, 16};
}

Laser::~Laser()
{
}

void Laser::Update(float deltaTime)
{
    bounds.y -= 400 * deltaTime;

    if (bounds.y < 0 || bounds.y > GetScreenHeight())
        isDestroyed = true;    
    else
        isDestroyed = false;
}

void Laser::Draw()
{
    if (!isDestroyed)
        DrawRectangleRec(bounds, {243, 216, 63, 255});
}
