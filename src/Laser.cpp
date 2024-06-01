#include "Laser.h"

Laser::Laser(float positionX, float positionY)
{
    bounds = Rectangle{positionX, positionY, 4, 16};
}

Laser::~Laser()
{
}

void Laser::Draw()
{
    DrawRectangleRec(bounds, {243, 216, 63, 255});
}
