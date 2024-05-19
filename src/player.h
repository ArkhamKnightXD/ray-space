#pragma once

#include <raylib.h>

class Player
{
public:
    Player(float positionX, float positionY);
    ~Player();
    void Update(float deltaTime);
    void Draw();

public:
    Texture2D sprite;
    Rectangle bounds;
    int speed;
    int score;
    int lives;
};