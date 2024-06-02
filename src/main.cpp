#include <raylib.h>
#include "player.h"
#include "alien.h"
#include "MysteryShip.h"
#include "Laser.h"
#include "structure.h"
#include <vector>

std::vector<Alien> createAliens() {

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

    std::vector<Structure> structures;

    structures.push_back(Structure(screenWidth / 2 -300, 550));
    structures.push_back(Structure(screenWidth / 2 -125, 550));
    structures.push_back(Structure(screenWidth / 2 +50, 550));
    structures.push_back(Structure(screenWidth / 2 +250, 550));

    MysteryShip mysteryShip = MysteryShip(screenWidth / 2, screenHeight - 200, 4);

    Laser laser = Laser(screenWidth / 2, screenHeight - 200);

    Player player = Player(screenWidth / 2, screenHeight - 44);

    std::vector<Alien> aliens = createAliens();

    InitAudioDevice();

    Sound bulletHitSound = LoadSound("assets/sounds/laser.ogg");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        player.Update(deltaTime);

        laser.Update(deltaTime);

//checking collisions
        // for (unsigned int i = 0; i < structures.size(); i++)
        // {
        //     if (!structures[i].isDestroyed && CheckCollisionRecs(structures[i].bounds, bulletBounds))
        //     {
        //         structures[i].isDestroyed = true;

        //         structures[i].lives --; 

        //         PlaySound(bulletHitSound);
        //     }
        // }

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

            ClearBackground(Color{29, 29, 27 , 255});

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
            laser.Draw();

            player.Draw();

        EndDrawing();
    }

    UnloadSound(bulletHitSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}