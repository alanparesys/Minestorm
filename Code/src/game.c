#include <raylib.h>

#include "game.h"

GameScreen currentScreen = TITLE;
bool title = true;
bool pause = false;
bool solo = false;

void UpdateTitleScreen(Texture2D background/*, GameScreen currentScreen*/)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw
    DrawTexture(background, 0, 0, WHITE);

    //Text
    DrawText("(F) : Launch Solo Game", 160, 350, 20, WHITE);
    DrawText("(K) : Launch Duo Game", 160, 400, 20, WHITE);
    DrawText("(P) : Pause Menu", 160, 500, 20, WHITE);
    DrawText("(Esc) : Quit Game", 160, 450, 20, WHITE);

    EndDrawing();

    if (IsKeyPressed(KEY_F))
    {
        currentScreen = SOLO_GAMEPLAY;
        bool title = false;
        bool solo = true;
    }
    /*
    else if (IsKeyPressed(KEY_K))
    {
        currentScreen = VS_GAMEPLAY;
    }
    */
}


void UpdateSoloGameplay(Texture2D background, Texture2D ship/*, GameScreen currentScreen*/)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(background, 0, 0, WHITE);
    DrawTexture(ship, 625 / 2 - 50 / 2, 1070 / 2 - 50 / 2 - 40, WHITE);
    DrawText("Solo Gameplay Screen", 160, 300, 20, WHITE);
    EndDrawing();

    if (IsKeyPressed(KEY_P))
    {
        currentScreen = PAUSE;
        bool solo = false;
        bool pause = true;

    }
}


void UpdatePauseMenu(Texture2D background/*, GameScreen currentScreen*/)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(background, 0, 0, WHITE);
    DrawText("Pause Menu Screen", 160, 300, 20, WHITE);
    EndDrawing();
    if (IsKeyPressed(KEY_P))
    {
        currentScreen = SOLO_GAMEPLAY;
        bool pause = false;
        bool solo = true;

    }
}


int UpdateGame(Texture2D background, Texture2D ship/*, GameScreen currentScreen*/)
{
    switch (currentScreen)
    {
    case TITLE:
        UpdateTitleScreen(background/*, currentScreen*/);
        break;
    case SOLO_GAMEPLAY:
        UpdateSoloGameplay(background, ship/*, GameScreen currentScreen*/);
        break;
    case PAUSE:
        UpdatePauseMenu(background/*, currentScreen*/);
        break;
    default:
        break;
    }
}

