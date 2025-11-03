#include <raylib.h>

#include "game.h"


int main()
{
    InitWindow(640, 800, "MineStorm");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("Assets/minestorm_background.png");
    Texture2D ship = LoadTexture("Assets/Kenney/ship_K.png");


    while (!WindowShouldClose())
    {
        UpdateGame(background, ship/*, currentScreen */);
    }

    UnloadTexture(background);
    UnloadTexture(ship);

    CloseWindow();

    return 0;
}