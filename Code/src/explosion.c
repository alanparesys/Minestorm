#include "explosion.h"

Explosion explosions[MAX_EXPLOSIONS];
static Sound explosionSound = { 0 }; // Son d'explosion (initialisé à 0)

void InitExplosions(void)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        explosions[i].active = false;
        explosions[i].currentFrame = 0;
        explosions[i].frameCounter = 0;
        explosions[i].scale = 1.0f;
        explosions[i].position.x = 0.0f;
        explosions[i].position.y = 0.0f;
    }
}

void SetExplosionSound(Sound sound)
{
    explosionSound = sound;
}

void SpawnExplosion(Vector2D position, float scale)
{
    // Trouver un slot libre
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].active)
        {
            explosions[i].active = true;
            explosions[i].position = position;
            explosions[i].scale = scale;
            explosions[i].currentFrame = 0;
            explosions[i].frameCounter = 0;
            
            // Jouer le son d'explosion si disponible
            if (explosionSound.frameCount > 0)
            {
                PlaySound(explosionSound);
            }
            
            return;
        }
    }
}

void UpdateExplosions(void)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].active) continue;

        explosions[i].frameCounter++;

        // Changer de frame tous les EXPLOSION_FRAME_SPEED frames
        if (explosions[i].frameCounter >= EXPLOSION_FRAME_SPEED)
        {
            explosions[i].frameCounter = 0;
            explosions[i].currentFrame++;

            // Si l'animation est termin�e, d�sactiver l'explosion
            if (explosions[i].currentFrame >= EXPLOSION_FRAME_COUNT)
            {
                explosions[i].active = false;
            }
        }
    }
}

void DrawExplosions(Texture2D explosionTexture)
{
    if (explosionTexture.id == 0) return;

    const int frameWidth = explosionTexture.width / EXPLOSION_SHEET_COLUMNS;
    const int frameHeight = explosionTexture.height / EXPLOSION_SHEET_ROWS;

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].active) continue;

        // Calculer la position dans le sprite sheet (5x5 grid)
        int frameX = explosions[i].currentFrame % EXPLOSION_SHEET_COLUMNS;
        int frameY = explosions[i].currentFrame / EXPLOSION_SHEET_COLUMNS;

        // Rectangle source (dans le sprite sheet)
        Rectangle sourceRec = {
            (float)(frameX * frameWidth),
            (float)(frameY * frameHeight),
            (float)frameWidth,
            (float)frameHeight
        };

        // Rectangle destination (� l'�cran)
        Rectangle destRec = {
            explosions[i].position.x,
            explosions[i].position.y,
            frameWidth * explosions[i].scale,
            frameHeight * explosions[i].scale
        };

        // Origin au centre pour centrer l'explosion sur la position
        Vector2 origin = {
            (frameWidth * explosions[i].scale) / 2.0f,
            (frameHeight * explosions[i].scale) / 2.0f
        };

        // Dessiner la frame actuelle
        DrawTexturePro(explosionTexture, sourceRec, destRec, origin, 0.0f, WHITE);
    }
}