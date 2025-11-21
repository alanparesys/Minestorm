#include <raylib.h>
#include <stdlib.h>
#include "game.h"
#include "enemy.h"
#include "explosion.h"
#include "struct.h"

int main(void)
{
    const int screenWidth = 1080;
    const int screenHeight = 1300;

    InitWindow(screenWidth, screenHeight, "MineStorm");
    InitAudioDevice(); // Initialize audio system
    SetTargetFPS(60);

    GameAssets assets;
    Enemy enemy = { 0 };
    Collision collision = { 0 };
    InitAssets(&assets);
    InitGame();
    SoundInGame(&assets);

    // Configure explosion sound
    SetExplosionSound(assets.explosionSound);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(titleMusic);
        UpdateMusicStream(bgMusic);  // Keep background music stream updated      
        UpdateGame(&assets, &enemy, &collision);
    }

    free(&collision);
    free(player);
    UnloadAssets(&assets);
    UnloadMusicStream(titleMusic);
    UnloadMusicStream(bgMusic);
    CloseAudioDevice(); // Close audio system
    CloseWindow();

    return 0;
}
