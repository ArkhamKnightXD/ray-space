#pragma once

#include <raylib.h>

class Player
{
public:
    Player(float positionX, float positionY);
    ~Player();
    void Update(float deltaTime);
    void Draw();

private:
    Texture2D sprite;
    int speed;

public:
    Rectangle bounds;
    int score;
    int lives;
};