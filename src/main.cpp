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
    std::vector<Texture2D> alienSprites;

    alienSprites.push_back(LoadTexture("assets/sprites/alien_3.png"));
    alienSprites.push_back(LoadTexture("assets/sprites/alien_2.png"));
    alienSprites.push_back(LoadTexture("assets/sprites/alien_1.png"));

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
            actualSprite = alienSprites[0];
            break;

        case 1:
        case 2:
            actualSprite = alienSprites[1];
            break;

        default:
            actualSprite = alienSprites[2];
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

    InitWindow(screenWidth, screenHeight, "Breakout!");
    SetTargetFPS(144);

    std::vector<Structure> structures;

    structures.push_back(Structure(screenWidth / 2 - 300, 550));
    structures.push_back(Structure(screenWidth / 2 - 125, 550));
    structures.push_back(Structure(screenWidth / 2 + 50, 550));
    structures.push_back(Structure(screenWidth / 2 + 250, 550));

    MysteryShip mysteryShip = MysteryShip(screenWidth / 2, screenHeight - 200, 4);

    std::vector<Alien> aliens = CreateAliens();

    std::vector<Laser> lasers;

    float lastShootLaserTime;

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
            if (GetTime() - lastShootLaserTime >= 0.35)
            {
                lasers.push_back(Laser(player.bounds.x + 20, player.bounds.y - player.bounds.height));
                lastShootLaserTime = GetTime();

                PlaySound(shootSound);
            }
        }

        // Accessing the laseres elements using iterators like in java
        for (auto iterator = lasers.begin(); iterator != lasers.end();)
        {
            // If the element is not active I remove this element
            if (iterator->isDestroyed)
            {
                lasers.erase(iterator);
            }
            // If the element is not active I need to increase the iterator to check the next element.
            else
            {
                iterator++;
            }
        }

        // When I have to change state of the object with a ranged based loop I need to use & if not the state of the object won't change
        for (Laser &laser : lasers)
        {
            laser.Update(deltaTime);
        }

        for (Structure &structure : structures)
        {
            for (Laser &laser : lasers)
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

        // Alien movement its failling
        // for (Alien &alien : aliens)
        // {
        //     float alienPosition = alien.bounds.x + alien.bounds.width;

        //     if (alienPosition > screenWidth || alienPosition < 0)
        //     {
        //         alien.velocity *= -1;
        //     }

        //     alien.Update(deltaTime);
        // }

        for (Laser &laser : lasers)
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

        BeginDrawing();

            ClearBackground(Color{29, 29, 27, 255});

            DrawText(TextFormat("Score: %i", player.score), 150, 10, 20, WHITE);
            DrawText(TextFormat("Lives %i", player.lives), screenWidth - 250, 10, 20, WHITE);

            for (Alien alien : aliens)
            {
                alien.Draw();
            }

            for (Structure structure : structures)
            {
                structure.Draw();
            }

            // mysteryShip.Draw();
            for (Laser laser : lasers)
            {
                laser.Draw();
            }

            player.Draw();

        EndDrawing();
    }

    UnloadSound(shootSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}