#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "struct.h"
#include "sphere2D.h"

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

extern Ship* player;

void InitAssets(GameAssets* assets);
void UnloadAssets(GameAssets* assets);
void UpdateGame(GameAssets* assets, Enemy* enemy, Collision* collision);
void UpdateTitleScreen(GameAssets* assets);
void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy, Collision* collision);
void UpdateHelpGameplay(GameAssets* assets);
void UpdatePauseMenu(GameAssets* assets);
void UpdateGameOver(GameAssets* assets, Collision* collision);
void RestartGame(GameAssets* assets, Enemy* enemy, Collision* collision);
void InitGame(void);
void UpdateControlGame(void);
void UpdateEnemies(GameAssets* assets, Collision* collision);
void CheckInput(void);
void BoundingBoxPlayer(void);
void DrawHitboxes(void);
void PlayerEnemyCollision(void);


#endif