#include <raylib.h>
#include "player.h"
#include "alien.h"
#include "MysteryShip.h"
#include "Laser.h"
#include "structure.h"
#include <vector>
#include <stdio.h>

std::vector<Alien> createAliens()
{

    std::vector<Alien> aliens;

    int positionX;
    int positionY = 100;
    int alienPoints = 8;

    for (int row = 0; row < 5; row++)
    {
        positionX = 54;

        for (int columns = 0; columns < 11; columns++)
        {
            aliens.push_back(Alien(positionX, positionY, alienPoints, row));
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

    float timer;

    std::vector<Structure> structures;

    structures.push_back(Structure(screenWidth / 2 - 300, 550));
    structures.push_back(Structure(screenWidth / 2 - 125, 550));
    structures.push_back(Structure(screenWidth / 2 + 50, 550));
    structures.push_back(Structure(screenWidth / 2 + 250, 550));

    MysteryShip mysteryShip = MysteryShip(screenWidth / 2, screenHeight - 200, 4);

    std::vector<Alien> aliens = createAliens();

    std::vector<Laser> lasers;

    Player player = Player(screenWidth / 2, screenHeight - 44);

    InitAudioDevice();

    Sound bulletHitSound = LoadSound("assets/sounds/laser.ogg");
    Sound explosionSound = LoadSound("assets/sounds/explosion.ogg");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        player.Update(deltaTime);

        if (IsKeyDown(KEY_SPACE))
        {
            timer += deltaTime;

            if (timer > 0.8f)
            {
                lasers.push_back(Laser(player.bounds.x +20, player.bounds.y - player.bounds.height));
                PlaySound(bulletHitSound);

                timer = 0;
            }
        }

        // eliminate lasers that are outside of the screen
        for (unsigned int i = 0; i < lasers.size(); i++)
        {
            lasers[i].Update(deltaTime);
        }

        // collision structure - laser its failling. 
        for (unsigned int i = 0; i < structures.size(); i++)
        {
            for (unsigned int i = 0; i < lasers.size(); i++)
            {
                if (!structures[i].isDestroyed && CheckCollisionRecs(structures[i].bounds, lasers[i].bounds))
                {
                    structures[i].lives--;

                    printf("enter here \n");

                    if (structures[i].lives == 0)
                    {
                        structures[i].isDestroyed = true;
                    }

                    PlaySound(explosionSound);

                    lasers.pop_back();
                }
            }
        }

        // for (unsigned int i = 0; i < aliens.size(); i++)
        // {
        //     if (!aliens[i].isDestroyed && CheckCollisionRecs(aliens[i].bounds, bulletBounds))
        //     {
        //         aliens[i].isDestroyed = true;

        //         player.score += aliens[i].points;

        //         PlaySound(bulletHitSound);
        //     }
        // }

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

    UnloadSound(bulletHitSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}