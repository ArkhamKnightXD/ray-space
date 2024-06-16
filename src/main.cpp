#include <raylib.h>
#include "Player.h"
#include "Alien.h"
#include "MysteryShip.h"
#include "Laser.h"
#include "Structure.h"
#include <vector>
#include <stdio.h>

std::vector<Alien> CreateAliens()
{
    // Since in my old method I was loading almost 50 texture, with this method I'm only loading 3 textures
    Texture2D alienSprite1 = LoadTexture("assets/sprites/alien_1.png");
    Texture2D alienSprite2 = LoadTexture("assets/sprites/alien_2.png");
    Texture2D alienSprite3 = LoadTexture("assets/sprites/alien_3.png");

    std::vector<Alien> aliens;

    int positionX;
    int positionY = 100;
    int alienPoints = 8;

    Texture2D actualSprite;

    for (int row = 0; row < 5; row++)
    {
        positionX = 54;

        switch (row)
        {

        case 0:
            actualSprite = alienSprite3;
            break;

        case 1:
        case 2:
            actualSprite = alienSprite2;
            break;

        default:
            actualSprite = alienSprite1;
        }

        for (int columns = 0; columns < 11; columns++)
        {
            aliens.push_back(Alien(positionX, positionY, alienPoints, actualSprite));
            positionX += 60;
        }

        alienPoints--;
        positionY += 50;
    }

    return aliens;
}

int main()
{
    const int screenWidth = 750;
    const int screenHeight = 700;

    bool shouldGoLeft;
    bool shouldGoRight = true;
    bool shouldGoDown = false;

    InitWindow(screenWidth, screenHeight, "Breakout!");
    SetTargetFPS(144);

    std::vector<Structure> structures;

    structures.push_back(Structure(screenWidth / 2 - 300, 550));
    structures.push_back(Structure(screenWidth / 2 - 125, 550));
    structures.push_back(Structure(screenWidth / 2 + 50, 550));
    structures.push_back(Structure(screenWidth / 2 + 250, 550));

    MysteryShip mysteryShip = MysteryShip(screenWidth / 2, screenHeight - 200);

    std::vector<Alien> aliens = CreateAliens();

    std::vector<Laser> playerLasers;
    std::vector<Laser> alienLasers;

    float lastTimePlayerShoot;
    float lastTimeAlienShoot;

    Player player = Player(screenWidth / 2, screenHeight - 44);

    InitAudioDevice();

    Sound shootSound = LoadSound("assets/sounds/laser.ogg");
    Sound explosionSound = LoadSound("assets/sounds/explosion.ogg");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        player.Update(deltaTime);

        if (IsKeyDown(KEY_SPACE))
        {
            // shoot one laser every 350 ms
            if (GetTime() - lastTimePlayerShoot >= 0.5)
            {
                playerLasers.push_back(Laser(player.bounds.x + 20, player.bounds.y - player.bounds.height, false));
                lastTimePlayerShoot = GetTime();

                PlaySound(shootSound);
            }
        }

        // Accessing the laseres elements using iterators like in java
        for (auto iterator = playerLasers.begin(); iterator != playerLasers.end();)
        {
            // If the element is not active I remove this element
            if (iterator->isDestroyed)
            {
                playerLasers.erase(iterator);
            }
            // If the element is not active I need to increase the iterator to check the next element.
            else
            {
                iterator++;
            }
        }

        // When I have to change state of the object with a ranged based loop I need to use & if not the state of the object won't change
        for (Laser &laser : playerLasers)
        {
            laser.Update(deltaTime);
        }

        // Alien lasers
        if (GetTime() - lastTimeAlienShoot >= 1)
        {
            int randomAlienIndex = GetRandomValue(0, aliens.size() - 1);

            Alien alien = aliens[randomAlienIndex];

            alienLasers.push_back(Laser(alien.bounds.x + 20, alien.bounds.y + alien.bounds.height, true));
            lastTimeAlienShoot = GetTime();

            PlaySound(shootSound);
        }

        for (auto iterator = alienLasers.begin(); iterator != alienLasers.end();)
        {
            if (iterator->isDestroyed)
            {
                alienLasers.erase(iterator);
            }
            else
            {
                iterator++;
            }
        }

        for (Laser &laser : alienLasers)
        {
            laser.Update(deltaTime);
        }

        for (Structure &structure : structures)
        {
            for (Laser &laser : playerLasers)
            {
                if (!structure.isDestroyed && CheckCollisionRecs(structure.bounds, laser.bounds))
                {
                    laser.isDestroyed = true;

                    structure.lives--;

                    if (structure.lives == 0)
                    {
                        structure.isDestroyed = true;
                    }

                    PlaySound(explosionSound);
                }
            }
        }

        for (Structure &structure : structures)
        {
            for (Laser &laser : alienLasers)
            {
                if (!structure.isDestroyed && CheckCollisionRecs(structure.bounds, laser.bounds))
                {
                    laser.isDestroyed = true;

                    structure.lives--;

                    if (structure.lives == 0)
                    {
                        structure.isDestroyed = true;
                    }

                    PlaySound(explosionSound);
                }
            }
        }

        for (Laser &laser : playerLasers)
        {
            for (Alien &alien : aliens)
            {
                if (!alien.isDestroyed && CheckCollisionRecs(alien.bounds, laser.bounds))
                {
                    alien.isDestroyed = true;
                    laser.isDestroyed = true;

                    player.score += alien.points;

                    PlaySound(explosionSound);
                }
            }
        }

        for (Laser &laser : alienLasers)
        {
            if (player.lives > 0 && CheckCollisionRecs(player.bounds, laser.bounds))
            {
                laser.isDestroyed = true;

                player.lives--;

                PlaySound(explosionSound);
            }
        }

        for (Alien &alien : aliens)
        {
            float alienPosition = alien.bounds.x + alien.bounds.width;

            if (!shouldGoLeft && alienPosition > screenWidth)
            {
                shouldGoLeft = true;
                shouldGoRight = false;
                shouldGoDown = true;
                break;
            }

            if (!shouldGoRight && alienPosition < alien.bounds.width)
            {
                shouldGoRight = true;
                shouldGoLeft = false;
                shouldGoDown = true;
                break;
            }
        }

        if (shouldGoLeft)
        {
            for (Alien &alien : aliens)
            {
                alien.velocity = -50;
            }
        }

        if (shouldGoRight)
        {
            for (Alien &alien : aliens)
            {
                alien.velocity = 50;
            }
        }

        if (shouldGoDown)
        {
            for (Alien &alien : aliens)
            {
                alien.bounds.y += 10;
            }

            shouldGoDown = false;
        }

        for (Alien &alien : aliens)
        {
            alien.Update(deltaTime);
        }

        BeginDrawing();

        ClearBackground(Color{29, 29, 27, 255});

        DrawText(TextFormat("Score: %i", player.score), 150, 10, 20, WHITE);
        DrawText(TextFormat("Lives %i", player.lives), screenWidth - 250, 10, 20, WHITE);

        // mysteryShip.Draw();

        for (Alien alien : aliens)
        {
            alien.Draw();
        }

        for (Laser laser : alienLasers)
        {
            laser.Draw();
        }

        for (Structure structure : structures)
        {
            structure.Draw();
        }

        for (Laser laser : playerLasers)
        {
            laser.Draw();
        }

        player.Draw();

        EndDrawing();
    }

    UnloadSound(shootSound);
    UnloadSound(explosionSound);
    CloseAudioDevice();

    CloseWindow();
}