#include <raylib.h>

#include "game.h"

GameScreen currentScreen = TITLE;
bool title = true;
bool help = false;
bool solo = false;
bool pause = false;

void InitAssets(GameAssets* assets)
{
    assets->background = LoadTexture("Assets/minestorm_background.png");
    assets->ship = LoadTexture("Assets/Kenney/ship_K.png");
    assets->basicEnemy = LoadTexture("Assets/Kenney/meteor_detailedLarge.png");
}

void UnloadAssets(GameAssets* assets)
{
    UnloadTexture(assets->background);
    UnloadTexture(assets->ship);
    UnloadTexture(assets->basicEnemy);
}


void UpdateGame(GameAssets* assets)
{
    switch (currentScreen)
    {
    case TITLE:
        UpdateTitleScreen(assets);
        break;
    case HELP:
        UpdateHelpGameplay(assets);
        break;
    case SOLO_GAMEPLAY:
        UpdateSoloGameplay(assets);
        break;
    case PAUSE:
        UpdatePauseMenu(assets);
        break;
    default:
        break;
    }
}


void UpdateTitleScreen(GameAssets* assets)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);

    //Text
    DrawText("(F) : Launch Solo Game", 160, 350, 20, WHITE);
    DrawText("(K) : Launch Duo Game", 160, 400, 20, WHITE);
    DrawText("(H) : Help", 160, 450, 20, WHITE);
    DrawText("(P) : Pause Menu", 160, 500, 20, WHITE);
    DrawText("(Esc) : Quit Game", 160, 550, 20, WHITE);

    EndDrawing();

    if (IsKeyPressed(KEY_H))
    {
        currentScreen = HELP;
        title = false;
        help = true;
    }
    if (IsKeyPressed(KEY_F))
    {
        currentScreen = SOLO_GAMEPLAY;
        title = false;
        solo = true;
    }
}

void UpdateHelpGameplay(GameAssets* assets)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    DrawText("Pause Menu Screen", 160, 300, 20, WHITE);
    DrawText("(H) to return", 160, 350, 20, WHITE);
    EndDrawing();
    if (IsKeyPressed(KEY_H))
    {
        currentScreen = TITLE;
        help = false;
        title = true;
    }
}

void UpdateSoloGameplay(GameAssets* assets)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    DrawTexture(assets->ship, 625 / 2 - 50 / 2, 1070 / 2 - 50 / 2 - 40, WHITE);
    DrawTexture(assets->basicEnemy, 550 / 2 - 50 / 2, 1070 / 2 - 50 / 2 - 40, WHITE);
    DrawText("Solo Gameplay Screen", 160, 300, 20, WHITE);
    EndDrawing();

    if (IsKeyPressed(KEY_P))
    {
        currentScreen = PAUSE;
        solo = false;
        pause = true;
    }
}

void UpdatePauseMenu(GameAssets* assets)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    DrawText("Pause Menu Screen", 160, 300, 20, WHITE);
    EndDrawing();
    if (IsKeyPressed(KEY_P))
    {
        currentScreen = SOLO_GAMEPLAY;
        pause = false;
        solo = true;
    }
}


