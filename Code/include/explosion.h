#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "raylib.h"
#include "Vector2D.h"

// Explosion animation configuration
#define MAX_EXPLOSIONS 20              // Maximum simultaneous explosions
#define EXPLOSION_FRAME_COUNT 25       // Total animation frames
#define EXPLOSION_FRAME_SPEED 2        // Game frames per animation frame
#define EXPLOSION_SHEET_COLUMNS 5      // Sprite sheet column count
#define EXPLOSION_SHEET_ROWS 5         // Sprite sheet row count

// Represents an animated explosion effect
typedef struct Explosion {
    Vector2D position;     // World position of the explosion
    int currentFrame;      // Current animation frame
    int frameCounter;      // Counter used to control frame progression
    float scale;           // Render scale factor
    bool active;           // Indicates if the explosion is active
} Explosion;

extern Explosion explosions[MAX_EXPLOSIONS]; // Pool of reusable explosions

// Initialize explosion pool
void InitExplosions(void);

// Assign a sound to be played when explosions spawn
void SetExplosionSound(Sound sound);

// Activate a new explosion at a given position
void SpawnExplosion(Vector2D position, float scale);

// Update animation states for all explosions
void UpdateExplosions(void);

// Render all active explosions using the sprite sheet
void DrawExplosions(Texture2D explosionTexture);

#endif