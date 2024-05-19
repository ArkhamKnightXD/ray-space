#include <raylib.h>
#include "player.h"
#include "alien.h"
#include <vector>
#include <stdio.h>
#include <stdio.h>

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

    Player player = Player(screenWidth / 2, screenHeight - 44);

    std::vector<Alien> aliens = createAliens();

    InitAudioDevice();

    Sound bulletHitSound = LoadSound("assets/sounds/laser.ogg");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        player.Update(deltaTime);

        // for (unsigned int i = 0; i < bricks.size(); i++)
        // {
        //     if (!bricks[i].isDestroyed)
        //     {
        //         bricks[i].isDestroyed = true;

        //         player.score += bricks[i].brickPoints; 

        //         PlaySound(brickHitSound);
        //     }
        // }


        BeginDrawing();

            ClearBackground(Color{0,0,0,0});

            DrawText(TextFormat("Score: %i", player.score), 150, 10, 20, WHITE);
            DrawText(TextFormat("Lives %i", player.lives), screenWidth - 250, 10, 20, WHITE);

            for (Alien alien : aliens)
            {
                alien.Draw();
            }

            player.Draw();

        EndDrawing();
    }

    UnloadSound(bulletHitSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}