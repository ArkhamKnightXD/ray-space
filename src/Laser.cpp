#include "Laser.h"

//Going to use my own code for the lasers.
Laser::Laser(float positionX, float positionY)
{
    bounds = Rectangle{positionX, positionY, 4, 16};
    isActive = true;
}

Laser::~Laser()
{
}

void Laser::Update(float deltaTime)
{
    bounds.y -= 400 * deltaTime;

    if (bounds.y < 0 || bounds.y > GetScreenHeight())
        isActive = false;    
    else
        isActive = true;
}

void Laser::Draw()
{
    if (isActive)
        DrawRectangleRec(bounds, {243, 216, 63, 255});
}
