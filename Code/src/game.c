
#include <raylib.h>

#include "game.h"
#include "enemy.h"

GameScreen currentScreen = TITLE;
bool title = true;
bool help = false;
bool solo = false;
bool pause = false;

Enemy* basicEnemy;

void InitAssets(GameAssets* assets)
{
    assets->background = LoadTexture("Assets/Background1080_1300.png");
    assets->ship = LoadTexture("Assets/Kenney/ship_K.png");
    assets->basicEnemyTexture = LoadTexture("Assets/Kenney/meteor_detailedLarge.png");
}

void UnloadAssets(GameAssets* assets)
{
    UnloadTexture(assets->background);
    UnloadTexture(assets->ship);
    UnloadTexture(assets->basicEnemyTexture);
}


void UpdateGame(GameAssets* assets, Enemy* enemy)
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
        UpdateSoloGameplay(assets, enemy);
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
    DrawText("(F) : Launch Solo Game", 375, 450, 20, WHITE);
    DrawText("(K) : Launch Duo Game", 375, 500, 20, WHITE);
    DrawText("(H) : Help", 375, 550, 20, WHITE);
    DrawText("(P) : Pause Menu", 375, 600, 20, WHITE);
    DrawText("(Esc) : Quit Game", 375, 650, 20, WHITE);

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

void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    DrawTexture(assets->ship, 625 / 2 - 50 / 2, 1070 / 2 - 50 / 2 - 40, WHITE);
    //DrawTexture(assets->basicEnemy, 550 / 2 - 50 / 2, 1070 / 2 - 50 / 2 - 40, WHITE);
    DrawText("Solo Gameplay Screen", 160, 300, 20, WHITE);
    // <-- passe assets au gestionnaire d'ennemis
    UpdateBasicEnemy(enemy, assets);
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