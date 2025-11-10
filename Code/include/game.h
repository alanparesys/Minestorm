#ifndef GAME_H
#define GAME_H

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
    Texture2D basicEnemy;
} GameAssets;


void InitAssets(GameAssets* assets);
void UnloadAssets(GameAssets* assets);
void UpdateGame(GameAssets* assets);
void UpdateTitleScreen(GameAssets* assets);
void UpdateSoloGameplay(GameAssets* assets);
void UpdateHelpGameplay(GameAssets* assets);
void UpdatePauseMenu(GameAssets* assets);

#endif  // GAME_H	