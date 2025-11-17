#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "raylib.h"
#include "Vector2D.h"

#define MAX_EXPLOSIONS 20
#define EXPLOSION_FRAME_COUNT 25
#define EXPLOSION_FRAME_SPEED 2  // Nombre de frames de jeu par frame d'animation
#define EXPLOSION_SHEET_COLUMNS 5
#define EXPLOSION_SHEET_ROWS 5

typedef struct Explosion {
    Vector2D position;
    int currentFrame;
    int frameCounter;
    float scale;
    bool active;
} Explosion;

extern Explosion explosions[MAX_EXPLOSIONS];

void InitExplosions(void);
void SpawnExplosion(Vector2D position, float scale);
void UpdateExplosions(void);
void DrawExplosions(Texture2D explosionTexture);    

#endif