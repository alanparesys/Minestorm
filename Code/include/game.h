#ifndef GAME_H
#define GAME_H

typedef enum GameScreen
{
    TITLE,      
    GAMEPLAY,   
    ENDING,     
    PAUSE      
} GameScreen;

int UpdateGame(Texture2D background, GameScreen currentScreen);
void UpdateTitleScreen(Texture2D background);

#endif// GAME_H	