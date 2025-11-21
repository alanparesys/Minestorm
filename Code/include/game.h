#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "struct.h"
#include "sphere2D.h"

// All possible game screens/states
typedef enum GameScreen
{
    TITLE,              // Main title screen
    SOLO_GAMEPLAY,      // Single player gameplay
    VS_GAMEPLAY,        // Versus mode gameplay
    CONTROLS,           // Controls information screen
    HELP,               // Help/instructions screen
    TITLE_PAUSE,        // Pause menu on title screen
    PAUSE,              // In-game pause
    ENDING,             // Ending sequence
    GAMEOVER,           // Game over screen
} GameScreen;

extern bool motherShipSpawned;   // Tracks if a mothership has appeared

extern Ship* player;             // Pointer to the main player ship

// Background music assets
Music bgMusic;
Music titleMusic;

// Load all required game assets
void InitAssets(GameAssets* assets);

// Free all previously loaded assets
void UnloadAssets(GameAssets* assets);

// Update the global game logic (generic entry point)
void UpdateGame(GameAssets* assets, Enemy* enemy, Collision* collision);

// Screen-specific update functions
void UpdateTitleScreen(GameAssets* assets);                        // Title screen
void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy, Collision* collision); // Solo gameplay 
void UpdateControlsGameplay(GameAssets* assets);                   // Controls screen
void UpdateHelpGameplay(GameAssets* assets);                       // Help screen
void UpdateTitlePause(assets);                                     // Pause overlay for title screen
void UpdatePauseMenu(GameAssets* assets);                          // General pause menu
void UpdateGameOver(GameAssets* assets, Collision* collision);     // Game over logic

// Reset game state to start a new run
void RestartGame(GameAssets* assets, Enemy* enemy, Collision* collision);

// Initialize game
void InitGame(void);

// Control input
void UpdateControlGame(GameAssets* assets);

// Process player inputs
void CheckInput(GameAssets* assets);

// Player hitbox and bounding box handling
void BoundingBoxPlayer(void);

// Debug drawing for hitboxes
void DrawHitboxes(void);
void DrawBonusHitboxes(void);

// Collision helpers
void BorderPlayerCollision(Ship* player);   // Player vs map borders
void PlayerEnemyCollision(void);            // Player vs enemies

// Gameplay progression and feedback
void CheckLifeOfPlayer();                   // Handle player life updates
void LevelProgress(GameAssets* assets);     // Manage level progression
void SoundInGame(GameAssets* assets);       // Centralize gameplay sound logic

#endif