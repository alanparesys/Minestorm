#include <raylib.h>

#include "game.h"


int UpdateGame(Texture2D background, GameScreen currentScreen)
{
    switch (currentScreen)
    {
    case TITLE:
		UpdateTitleScreen(background);
        break;
    default:
        break;
    }
}

void UpdateTitleScreen(Texture2D background)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw
    DrawTexture(background, 0, 0, WHITE);

    //Text
    DrawText("(F) : Launch Solo Game", 160, 350, 20, WHITE);
    DrawText("(K) : Launch Duo Game", 160, 400, 20, WHITE);
    DrawText("(Esc) : Quit Game", 160, 450, 20, WHITE);

    EndDrawing();
}
