#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Vector2D.h"
#include "enemy.h"


typedef enum GameScreen
{
    TITLE,
    SOLO_GAMEPLAY,
    VS_GAMEPLAY,
    HELP,
    PAUSE,
    ENDING,
    GAMEOVER,
} GameScreen;

typedef struct GameAssets
{
    Texture2D background;
    Texture2D interface;
    Texture2D ship;
	Texture2D bulletTexture;
    Texture2D basicEnemyTexture;
    Texture2D shooterEnemyTexture;
	Texture2D followerEnemyTexture;
	Texture2D followerShooterEnemyTexture;
    Font font;
} GameAssets;

typedef struct Ship {
    Vector2D position;
    Vector2D size;
    Vector2D velocity;
    float angle;                        // en radians
    Color color;
} Ship;

typedef struct Enemy Enemy; // Déclaration anticipée pour éviter boucle d'inclusion


void InitAssets(GameAssets* assets);
void UnloadAssets(GameAssets* assets);
void UpdateGame(GameAssets* assets, Enemy* enemy);
void UpdateTitleScreen(GameAssets* assets);
void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy);
void UpdateHelpGameplay(GameAssets* assets);
void UpdatePauseMenu(GameAssets* assets);
void UpdateGameOver(GameAssets* assets);
void RestartGame(GameAssets* assets, Enemy* enemy);

void InitGame(void);
void UpdateControlGame(void);
Vector2D CheckInput(void);

#endif  // GAME_H	