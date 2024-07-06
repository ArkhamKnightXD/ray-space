#include <raylib.h>
#include <vector>
#include <stdlib.h>
#include "Player.h"
#include "Alien.h"
#include "MysteryShip.h"
#include "Structure.h"

const int screenWidth = 750;
const int screenHeight = 700;

bool isGamePaused;

bool shouldChangeVelocity;

float lastTimePlayerShoot;
float lastTimeAlienShoot;
float lastTimeMysteryShipSpawn;

Sound shootSound;
Sound explosionSound;
Sound pauseSound;

// To initialize objects outside of the main it's necessary to define the objects as pointers.
MysteryShip *mysteryShip = nullptr;
Player *player = nullptr;

std::vector<Alien> aliens;
std::vector<Structure> structures;

typedef struct
{
    Rectangle bounds;
    bool isDestroyed;
} Laser;

std::vector<Laser> playerLasers;
std::vector<Laser> alienLasers;

Texture2D structureSprite;
Texture2D alienSprite1;
Texture2D alienSprite2;
Texture2D alienSprite3;

void QuitGame()
{
    // If I define objects as a pointer I need to later fr
    delete player;    // Deletes the allocated memory for player
    player = nullptr; // Resets the pointer to nullptr

    delete mysteryShip;
    mysteryShip = nullptr;

    UnloadTexture(structureSprite);
    UnloadTexture(alienSprite1);
    UnloadTexture(alienSprite2);
    UnloadTexture(alienSprite3);

    // Unload all sprites.
    UnloadSound(shootSound);
    UnloadSound(explosionSound);
    UnloadSound(pauseSound);
    CloseAudioDevice();

    CloseWindow();
}

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
        alien.Update(deltaTime);

        float alienPosition = alien.bounds.x + alien.bounds.width;

        if ((!shouldChangeVelocity && alienPosition > screenWidth) || alienPosition < alien.bounds.width)
        {
            shouldChangeVelocity = true;
            break;
        }
    }

    if (shouldChangeVelocity)
    {
        for (Alien &alien : aliens)
        {
            alien.velocity *= -1;
            alien.bounds.y += 10;
        }

        shouldChangeVelocity = false;
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

            break;
        }
    }
}

void RemoveDestroyedElements()
{
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
}

void Update()
{
    float deltaTime = GetFrameTime();

    player->Update(deltaTime);

    if (!mysteryShip->shouldMove)
    {
        lastTimeMysteryShipSpawn += deltaTime;

        if (lastTimeMysteryShipSpawn >= 10)
        {
            lastTimeMysteryShipSpawn = 0;

            mysteryShip->shouldMove = true;
        }
    }

    mysteryShip->Update(deltaTime);

    if (IsKeyDown(KEY_SPACE))
    {
        // shoot one laser every 350 ms
        if (GetTime() - lastTimePlayerShoot >= 0.5)
        {
            Rectangle laserBounds = {player->bounds.x + 20, player->bounds.y - player->bounds.height, 4, 16};
            playerLasers.push_back({laserBounds, false});

            lastTimePlayerShoot = GetTime();

            PlaySound(shootSound);
        }
    }

    // Alien lasers instantiation
    if (!aliens.empty() && GetTime() - lastTimeAlienShoot >= 0.6)
    {
        int randomAlienIndex = GetRandomValue(0, aliens.size() - 1);

        Alien alien = aliens[randomAlienIndex];

        Rectangle laserBounds = {alien.bounds.x + 20, alien.bounds.y + alien.bounds.height, 4, 16};

        alienLasers.push_back({laserBounds, false});
        lastTimeAlienShoot = GetTime();

        PlaySound(shootSound);
    }

    for (Laser &laser : alienLasers)
    {
        laser.bounds.y += 400 * deltaTime;

        if (laser.bounds.y > GetScreenHeight())
            laser.isDestroyed = true;

        if (laser.bounds.y < 0 || laser.bounds.y > GetScreenHeight())
            laser.isDestroyed = true;

        if (player->lives > 0 && CheckCollisionRecs(player->bounds, laser.bounds))
        {
            laser.isDestroyed = true;

            player->lives--;

            PlaySound(explosionSound);

            break;
        }

        CheckCollisionBetweenStructureAndLaser(laser);
    }

    // When I have to change state of the object with a ranged based loop I need to use & if not the state of the object won't change
    for (Laser &laser : playerLasers)
    {
        laser.bounds.y -= 400 * deltaTime;

        if (laser.bounds.y < 0)
            laser.isDestroyed = true;

        if (!mysteryShip->isDestroyed && CheckCollisionRecs(mysteryShip->bounds, laser.bounds))
        {
            laser.isDestroyed = true;

            player->score += mysteryShip->points;

            mysteryShip->isDestroyed = true;

            PlaySound(explosionSound);

            break;
        }

        for (Alien &alien : aliens)
        {
            if (!alien.isDestroyed && CheckCollisionRecs(alien.bounds, laser.bounds))
            {
                alien.isDestroyed = true;
                laser.isDestroyed = true;

                player->score += alien.points;

                PlaySound(explosionSound);

                break;
            }
        }

        CheckCollisionBetweenStructureAndLaser(laser);
    }

    AliensMovement(deltaTime);

    RemoveDestroyedElements();
}

void Draw()
{
    BeginDrawing();

    ClearBackground(Color{29, 29, 27, 255});

    DrawText(TextFormat("Score: %i", player->score), 150, 10, 20, WHITE);
    DrawText(TextFormat("Lives %i", player->lives), screenWidth - 250, 10, 20, WHITE);

    if (isGamePaused)
    {
        DrawText("Game Paused", screenWidth / 2 - 100, 40, 32, WHITE);
    }

    mysteryShip->Draw();

    for (Alien alien : aliens)
    {
        alien.Draw();
    }

    for (Laser laser : alienLasers)
    {
        if (!laser.isDestroyed)
        {
            DrawRectangleRec(laser.bounds, {243, 216, 63, 255});
        }
        
    }

    for (Structure structure : structures)
    {
        structure.Draw();
    }

    for (Laser laser : playerLasers)
    {
        if (!laser.isDestroyed)
        {
            DrawRectangleRec(laser.bounds, {243, 216, 63, 255});
        }
    }

    player->Draw();

    EndDrawing();
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Space!");
    SetTargetFPS(144);

    // This is how I need to instantiate objects that are pointers this with allocate a new mysteryShip in the heap
    mysteryShip = new MysteryShip(screenWidth, 40);

    aliens = CreateAliens();

    structureSprite = LoadTexture("assets/sprites/structure.png");

    structures.push_back(Structure(screenWidth / 2 - 300, 550, structureSprite));
    structures.push_back(Structure(screenWidth / 2 - 125, 550, structureSprite));
    structures.push_back(Structure(screenWidth / 2 + 50, 550, structureSprite));
    structures.push_back(Structure(screenWidth / 2 + 250, 550, structureSprite));

    player = new Player(screenWidth / 2, screenHeight - 44);

    InitAudioDevice();

    shootSound = LoadSound("assets/sounds/laser.ogg");
    explosionSound = LoadSound("assets/sounds/explosion.ogg");
    pauseSound = LoadSound("assets/sounds/magic.wav");

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            break;
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            isGamePaused = !isGamePaused;
            PlaySound(pauseSound);
        }

        if (!isGamePaused)
        {
            Update();
        }

        Draw();
    }

    QuitGame();
}