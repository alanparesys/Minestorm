#include <raylib.h>
#include "game.h"
#include "enemy.h"

int main(void)
{
    const int screenWidth = 1080;
    const int screenHeight = 1300;

    InitWindow(screenWidth, screenHeight, "MineStorm");
    SetTargetFPS(60);

    GameAssets assets;
    Enemy Enemy = { 0 };
    Collision collision = { 0 };
    InitAssets(&assets);
    InitGame();

    while (!WindowShouldClose())
    {
        UpdateGame(&assets, &Enemy, &collision);
    }

    UnloadAssets(&assets);
    CloseWindow();

    return 0;
}
