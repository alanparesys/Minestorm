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
} GameScreen;

typedef struct GameAssets
{
    Texture2D background;
    Texture2D ship;
    Texture2D basicEnemyTexture;
} GameAssets;

typedef struct Enemy Enemy; // Déclaration anticipée pour éviter boucle d'inclusion


void InitAssets(GameAssets* assets);
void UnloadAssets(GameAssets* assets);
void UpdateGame(GameAssets* assets, Enemy* enemy);
void UpdateTitleScreen(GameAssets* assets);
void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy);
void UpdateHelpGameplay(GameAssets* assets);
void UpdatePauseMenu(GameAssets* assets);

#endif  // GAME_H	