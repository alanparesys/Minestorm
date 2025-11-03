#ifndef GAME_H
#define GAME_H

typedef enum GameScreen
{
    TITLE,
    SOLO_GAMEPLAY,
    VS_GAMEPLAY,
	PAUSE,
    ENDING,
} GameScreen;

int UpdateGame(Texture2D background, Texture2D ship/*, GameScreen currentScreen*/);
void UpdateTitleScreen(Texture2D background /*GameScreen currentScreen*/);
void UpdateSoloGameplay(Texture2D background, Texture2D ship/*, GameScreen currentScreen*/);
void UpdatePauseMenu(Texture2D background/*, GameScreen currentScreen*/);

#endif  // GAME_H	