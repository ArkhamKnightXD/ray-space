#include "Structure.h"

Structure::Structure(float positionX, float positionY)
{
    sprite = LoadTexture("assets/sprites/structure.png");
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};

    lives = 5;
}

void Structure::Draw()
{
    if (!isDestroyed)
        DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}
