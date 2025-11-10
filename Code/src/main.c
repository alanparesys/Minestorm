#include <raylib.h>

#include "game.h"

int main()
{
    int screenHeight = 1300;
    int screenWigth = 1080;

    InitWindow(screenWigth, screenHeight, "MineStorm");
    SetTargetFPS(60);
    GameAssets assets;
    InitAssets(&assets); // charge les textures et initialise le module game

    while (!WindowShouldClose())
    {
        UpdateGame(&assets);
    }

    UnloadAssets(&assets); // décharge les textures
    CloseWindow();

    return 0;
}