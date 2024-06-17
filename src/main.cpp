#include <raylib.h>
#include "Player.h"
#include "Alien.h"
#include "MysteryShip.h"
#include "Laser.h"
#include "Structure.h"
#include <vector>

const int screenWidth = 750;
const int screenHeight = 700;

bool shouldAliensGoLeft;
bool shouldAliensGoRight = true;
bool shouldAliensGoDown = false;

Sound shootSound;
Sound explosionSound;

// MysteryShip *mysteryShip = NULL;
// Player *player = NULL;

std::vector<Alien> aliens;
std::vector<Structure> structures;
std::vector<Laser> playerLasers;
std::vector<Laser> alienLasers;

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

void AliensMovement(float deltaTime)
{
    for (Alien &alien : aliens)
    {
        float alienPosition = alien.bounds.x + alien.bounds.width;

        if (!shouldAliensGoLeft && alienPosition > screenWidth)
        {
            shouldAliensGoLeft = true;
            shouldAliensGoRight = false;
            shouldAliensGoDown = true;
            break;
        }

        if (!shouldAliensGoRight && alienPosition < alien.bounds.width)
        {
            shouldAliensGoRight = true;
            shouldAliensGoLeft = false;
            shouldAliensGoDown = true;
            break;
        }
    }

    if (shouldAliensGoLeft)
    {
        for (Alien &alien : aliens)
        {
            alien.velocity = -50;
        }
    }

    if (shouldAliensGoRight)
    {
        for (Alien &alien : aliens)
        {
            alien.velocity = 50;
        }
    }

    if (shouldAliensGoDown)
    {
        for (Alien &alien : aliens)
        {
            alien.bounds.y += 10;
        }

        shouldAliensGoDown = false;
    }

    for (Alien &alien : aliens)
    {
        alien.Update(deltaTime);
    }
}

// The rule of the reference also applies to function parameters if I don't put the & I will be sending a copy of the object.
void CheckCollisionBetweenStructureAndLaser(Laser &laser)
{
    for (Structure &structure : structures)
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

int main()
{
    InitWindow(screenWidth, screenHeight, "Space!");
    SetTargetFPS(144);

    MysteryShip mysteryShip = MysteryShip(screenWidth, 40);

    aliens = CreateAliens();

    structures.push_back(Structure(screenWidth / 2 - 300, 550));
    structures.push_back(Structure(screenWidth / 2 - 125, 550));
    structures.push_back(Structure(screenWidth / 2 + 50, 550));
    structures.push_back(Structure(screenWidth / 2 + 250, 550));

    Player player = Player(screenWidth / 2, screenHeight - 44);

    float lastTimePlayerShoot;
    float lastTimeAlienShoot;
    float lastTimeMysteryShipSpawn;

    InitAudioDevice();

    shootSound = LoadSound("assets/sounds/laser.ogg");
    explosionSound = LoadSound("assets/sounds/explosion.ogg");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        player.Update(deltaTime);

        if (!mysteryShip.shouldMove)
        {
            lastTimeMysteryShipSpawn += deltaTime;

            if (lastTimeMysteryShipSpawn >= 10)
            {
                lastTimeMysteryShipSpawn = 0;

                mysteryShip.shouldMove = true;
            }
        }

        mysteryShip.Update(deltaTime);

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

        // When I have to change state of the object with a ranged based loop I need to use & if not the state of the object won't change
        for (Laser &laser : playerLasers)
        {
            laser.Update(deltaTime);
        }

        // Alien lasers
        if (!aliens.empty() && GetTime() - lastTimeAlienShoot >= 0.6)
        {
            int randomAlienIndex = GetRandomValue(0, aliens.size() - 1);

            Alien alien = aliens[randomAlienIndex];

            alienLasers.push_back(Laser(alien.bounds.x + 20, alien.bounds.y + alien.bounds.height, true));
            lastTimeAlienShoot = GetTime();

            PlaySound(shootSound);
        }

        for (Laser &laser : alienLasers)
        {
            laser.Update(deltaTime);
        }

        for (Laser &laser : playerLasers)
        {
            if (!mysteryShip.isDestroyed && CheckCollisionRecs(mysteryShip.bounds, laser.bounds))
            {
                laser.isDestroyed = true;

                player.score += mysteryShip.points;

                mysteryShip.isDestroyed = true;

                PlaySound(explosionSound);
            }

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

            CheckCollisionBetweenStructureAndLaser(laser);
        }

        for (Laser &laser : alienLasers)
        {
            if (player.lives > 0 && CheckCollisionRecs(player.bounds, laser.bounds))
            {
                laser.isDestroyed = true;

                player.lives--;

                PlaySound(explosionSound);
            }

            CheckCollisionBetweenStructureAndLaser(laser);
        }

        for (auto iterator = aliens.begin(); iterator != aliens.end();)
        {
            if (iterator->isDestroyed)
            {
                aliens.erase(iterator);
            }
            else
            {
                iterator++;
            }
        }

        // method for removing lasers after all the collision checks removing
        //  Accessing the laseres elements using iterators like in java
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

        AliensMovement(deltaTime);

        BeginDrawing();

        ClearBackground(Color{29, 29, 27, 255});

        DrawText(TextFormat("Score: %i", player.score), 150, 10, 20, WHITE);
        DrawText(TextFormat("Lives %i", player.lives), screenWidth - 250, 10, 20, WHITE);

        mysteryShip.Draw();

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

//Unload all sprites.
    UnloadSound(shootSound);
    UnloadSound(explosionSound);
    CloseAudioDevice();
    CloseWindow();
}