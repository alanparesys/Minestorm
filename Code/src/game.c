#include <raylib.h>

#include "game.h"

GameScreen currentScreen = TITLE;

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
    }
    /*
        else if (IsKeyPressed(KEY_K))
        {
            currentScreen = VS_GAMEPLAY;
        }*/
}


void UpdateSoloGameplay(Texture2D background/*, GameScreen currentScreen*/)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(background, 0, 0, WHITE);
    DrawText("Solo Gameplay Screen", 160, 300, 20, WHITE);
    EndDrawing();

    if (IsKeyPressed(KEY_P))
    {
        currentScreen = TITLE;
    }
}


int UpdateGame(Texture2D background/*, GameScreen currentScreen*/)
{
    switch (currentScreen)
    {
    case TITLE:
        UpdateTitleScreen(background/*, currentScreen*/);
        break;
    case SOLO_GAMEPLAY:
        UpdateSoloGameplay(background/*, currentScreen*/);
        break;
    default:
        break;
    }
}


