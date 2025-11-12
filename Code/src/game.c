
#include <raylib.h>

#include "math.h"
#include "game.h"
#include "enemy.h"
#include "vector2D.h"
#include "stdio.h"
#include "stdlib.h"

GameScreen currentScreen = TITLE;
bool title = true;
bool help = false;
bool solo = false;
bool pause = false;

bool allowMove = false;
bool gameOver = false;

Ship* player = NULL;

float positionX = (625 / 2) - (50 / 2);                // position en x du joueur
float positionY = (1070 / 2) - (50 / 2) - (2 - 40);    // position en y du joueur


// physique du jeu
const float ROTATION_SPEED = 0.05f;                    // la vitesse de la rotation en une frame
const float PUSH = 1.5f;                            // valeur de la poussée (appuyer une fois sur Z ou W = 0.2f)
const float FRICTION = 0.93f;                        // une friction (ralantissement) = 0.99f

// InitGame : initialise les valeurs de base de la physique et de la position du joueur

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
    case GAMEOVER:
        UpdateGameOver(assets);
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



//---------------------------------------------------------------

void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy)
{

    ClearBackground(BLACK);
    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    if (currentScreen == SOLO_GAMEPLAY)
    {
        BeginDrawing();


        // Affichage du vaisseau
        DrawTexturePro(assets->ship, (Rectangle) {0, 0, assets->ship.width, assets->ship.height}, (Rectangle) {player->position.x, player->position.y, player->size.x, player->size.y}, (Vector2) {player->size.x / 2, player->size.y / 2},(player->angle * (180.0f / PI)) + 90, player->color);

        //DrawTexture(assets->basicEnemy, 550 / 2 - 50 / 2, 1070 / 2 - 50 / 2 - 40, WHITE);
        DrawText("Solo Gameplay Screen", 160, 300, 20, WHITE);
        // <-- passe assets au gestionnaire d'ennemis
        UpdateBasicEnemy(enemy, assets);

        // Texte d’aide
        DrawText("Utilise Z ou W pour bouger\n A ou Q pour tourner a gauche\n D pour tourner a droite\n Ctrl pour se teleporter", 10, 10, 20, WHITE);
        EndDrawing();
    }


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

void UpdateGameOver(GameAssets* assets)
{
    if (currentScreen == GAMEOVER)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // j'voulais juste centrer le texte
        DrawText("YOU LOSE !\n        PRESS [ESC] TO RETURN IN THE MENU",
            GetScreenWidth() / 2 - MeasureText("You LOSE !", 20) / 2,
            GetScreenHeight() / 2 - 50, 20, RED);
    }
    EndDrawing();
    if (IsKeyPressed(KEY_ENTER))
    {
        RestartGame(assets);
    }
}

void RestartGame(GameAssets* assets, Enemy* enemy)
{
    UpdateGame(assets, enemy);
    gameOver = false;
}

void InitGame(void)
{
    player = (Ship*)malloc(sizeof(Ship));
    if (player == NULL) {
        printf("Allocation non reussi");
        return;
    }
    player->position = Vector2D_SetFromComponents(1300 / 2, 1080 / 2);
    player->size = Vector2D_SetFromComponents(50, 50);                        // la taille de base du joueur est "size"
    player->velocity = Vector2D_SetFromComponents(0, 0);
    player->angle = PI;
    player->color = WHITE;
}

// Implémentation et vérification des controles du jeu
Vector2D CheckInput(void) {
    //tourner à droite

    if (IsKeyDown('D')) {
        player->angle = player->angle + ROTATION_SPEED;
    }

    // tourner à gauche
    if (IsKeyDown('A') | IsKeyDown('Q')) {
        player->angle = player->angle - ROTATION_SPEED;
    }

    // accléléré vers l'avant du vaisseau
    if (IsKeyDown('Z') | IsKeyDown('W')) {
        Vector2D pushVector = Vector2D_SetFromComponents(
            cos(player->angle) * PUSH, sin(player->angle) * PUSH);            // calcule du vecteur de poussée en fonction de l'angle du vaisseau
        Vector2D_Print(pushVector);
        printf("\n");
        player->velocity = Vector2D_Add(player->velocity, pushVector);
    }

    if (IsKeyDown(KEY_SPACE)) {
        // les tirs ici (Alan)
    }

    if (IsKeyPressed(KEY_LEFT_CONTROL)) {
        float randX = (float)(rand() % (GetScreenWidth() - (int)player->size.x));
        float randY = (float)(rand() % (GetScreenHeight() - (int)player->size.y));
        player->position = Vector2D_SetFromComponents(randX, randY);
    }
}

void UpdateControlGame(void) {
    if (allowMove == true || gameOver == false) {
        CheckInput();

        // on applique l'inertie : la position = la position + la vélocité
        player->position = Vector2D_Add(player->position, player->velocity);

        // on applique la friction : la vélocity = la vélocité x la friction
        player->velocity = Vector2D_Scale(player->velocity, FRICTION, Vector2D_SetFromComponents(0, 0));

    }
}