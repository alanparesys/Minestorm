#include <raylib.h>

#include "game.h"


int main()
{
    InitWindow(640, 800, "MineStorm");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("Assets/minestorm_background.png");

    while (!WindowShouldClose())
    {
        UpdateGame(background/*, currentScreen */);
    }

    UnloadTexture(background);

    CloseWindow();

    return 0;
}