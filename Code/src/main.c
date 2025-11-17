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
    
    // Configurer le son d'explosion
    SetExplosionSound(assets.explosionSound);
    
    // Démarrer la musique de fond en boucle
    if (assets.backgroundMusic.frameCount > 0)
    {
        PlayMusicStream(assets.backgroundMusic);
    }

    while (!WindowShouldClose())
    {
        // Mettre à jour la musique de fond (nécessaire pour le streaming)
        if (assets.backgroundMusic.frameCount > 0)
        {
            UpdateMusicStream(assets.backgroundMusic);
            
            // Si la musique est terminée, la relancer en boucle
            if (!IsMusicStreamPlaying(assets.backgroundMusic))
            {
                SeekMusicStream(assets.backgroundMusic, 0.0f);
                PlayMusicStream(assets.backgroundMusic);
            }
        }
        
        UpdateGame(&assets, &basicEnemy, &collision);
    }

    UnloadAssets(&assets);
    CloseAudioDevice(); // Fermer le système audio
    CloseWindow();

    return 0;
}