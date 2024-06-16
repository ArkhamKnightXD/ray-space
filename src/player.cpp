#include "Player.h"

Player::Player(float positionX, float positionY)
{
    sprite = LoadTexture("assets/sprites/spaceship.png");
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};
    
    speed = 600;
    score = 0;
    lives = 3;    
}

Player::~Player()
{
    UnloadTexture(sprite);
}

void Player::Update(float deltaTime)
{
    if (IsKeyDown(KEY_A) && bounds.x >= 0)
        bounds.x -= speed * deltaTime;

    if (IsKeyDown(KEY_D) && bounds.x <= GetScreenWidth() - bounds.width)
        bounds.x += speed * deltaTime;
}

void Player::Draw()
{
    DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}
