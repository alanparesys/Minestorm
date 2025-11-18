#include <raylib.h>
#include "game.h"
#include "enemy.h"
#include "explosion.h"

int main(void)
{
    const int screenWidth = 1080;
    const int screenHeight = 1300;

    InitWindow(screenWidth, screenHeight, "MineStorm");
    InitAudioDevice(); // Initialiser le système audio
    SetTargetFPS(60);

    GameAssets assets;
    Enemy basicEnemy = { 0 };
    Collision collision = { 0 };
    InitAssets(&assets);
    InitGame();
    SoundInGame(&assets);
    
    // Configurer le son d'explosion
    SetExplosionSound(assets.explosionSound);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(titleMusic);
        UpdateMusicStream(bgMusic);  // Keep the background music stream updated      
        UpdateGame(&assets, &basicEnemy, &collision);
    }
    UnloadAssets(&assets);
    UnloadMusicStream(titleMusic);
    UnloadMusicStream(bgMusic); // Free background music resources
    CloseAudioDevice(); // Fermer le système audio
    CloseWindow();

    return 0;
}