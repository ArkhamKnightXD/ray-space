#include "Structure.h"

Structure::Structure(float positionX, float positionY, Texture2D textureSprite)
{
    sprite = textureSprite;
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};

    lives = 5;
}

void Structure::Draw()
{
    if (!isDestroyed)
        DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}
