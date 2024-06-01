#pragma once

#include <raylib.h>

class Player
{
public:
    Player(float positionX, float positionY);
    ~Player();
    void Update(float deltaTime);
    void Draw();
    void ShootLaser();

private:
    Texture2D sprite;

public:
    Rectangle bounds;
    int speed;
    int score;
    int lives;
};