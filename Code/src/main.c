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
<<<<<<< HEAD
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
=======
    
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
>>>>>>> a1d5b559b11f4fbdf00f17918fa5242aa9a63ff9
    CloseAudioDevice(); // Fermer le système audio
    CloseWindow();

    return 0;
}