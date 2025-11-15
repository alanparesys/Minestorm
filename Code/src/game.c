#include <raylib.h>

#include "math.h"
#include "game.h"
#include "enemy.h"
#include "vector2D.h"
#include "stdio.h"
#include "stdlib.h"
#include "bullet.h"

GameScreen currentScreen = TITLE;
bool title = true;
bool help = false;
bool solo = false;
bool pause = false;

bool allowMove = false;
bool gameOver = false;

Ship* player = NULL;

float positionX = (625 / 2) - (50 / 2);                // player's x-position
float positionY = (1070 / 2) - (50 / 2) - (2 - 40);    // player's y-position

// physique du jeu
const float ROTATION_SPEED = 0.05f;                    // the speed of rotation in one frame
const float PUSH = 1.5f;                            // thrust value (press Z or W once = 0.2f)
const float FRICTION = 0.93f;                        // friction (slowdown) = 0.99f

// InitGame: initializes the basic values for physics and player position
Enemy* basicEnemy;
Enemy bigBasicEnemies[3];

Color shipColor = { 186, 18, 186, 255 };

void InitAssets(GameAssets* assets)
{
    assets->background = LoadTexture("Assets/Background1080_1300.png");
    assets->interface = LoadTexture("Assets/interface1.png");
    assets->ship = LoadTexture("Assets/Kenney/ship_sidesA.png"); // ship_K.png  // Kenney/ship_sidesA.png
    assets->bulletTexture = LoadTexture("Assets/Kenney/meteor_large.png");
    assets->basicEnemyTexture = LoadTexture("Assets/Kenney/meteor_detailedLarge.png");
    assets->shooterEnemyTexture = LoadTexture("Assets/shooter_enemy.png");
    assets->followerEnemyTexture = LoadTexture("Assets/Kenney/icon_crossLarge.png");
	assets->followerShooterEnemyTexture = LoadTexture("Assets/follower_shooter_enemy.png");
    assets->font = LoadFont("Assets/pixel_police.ttf");
}

void UnloadAssets(GameAssets* assets)
{
    UnloadTexture(assets->background);
    UnloadTexture(assets->interface);
    UnloadTexture(assets->ship);
	UnloadTexture(assets->bulletTexture);
    UnloadTexture(assets->basicEnemyTexture);
    UnloadTexture(assets->shooterEnemyTexture);
	UnloadTexture(assets->followerEnemyTexture);
	UnloadTexture(assets->followerShooterEnemyTexture);
    UnloadFont(assets->font);
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

void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy)
{

    ClearBackground(BLACK);
    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    if (currentScreen == SOLO_GAMEPLAY)
    {
        BeginDrawing();


        // Ship display
        DrawTexturePro(assets->ship, (Rectangle) { 0, 0, assets->ship.width, assets->ship.height },
            (Rectangle) {
            player->position.x, player->position.y, player->size.x, player->size.y
        },
            (Vector2) {
            player->size.x / 2, player->size.y / 2
        }, (player->angle * (180.0f / PI)) + 90, player->color);

        //DrawText("Solo Gameplay Screen", 160, 300, 20, WHITE);
        // <-- passes assets to the enemy manager
        UpdateControlGame();
        UpdateBasicEnemy(enemy, assets);
		UpdateShooterEnemy(enemy, assets);
        UpdateFollowerEnemy(enemy, assets);
		UpdateFollowerShooterEnemy(enemy, assets);
        Vector2 interfacePos = { 0, 0 };
        DrawTextureEx(assets->interface, interfacePos, 0, 1, WHITE);
        const float scale = 3.5f; // same scale as in enemy.c
        for (int i = 0; i < 3 /*maxBigBasicEnemy*/; i++)
        {
            if (bigBasicEnemies[i].size.x <= 0.0f || bigBasicEnemies[i].size.y <= 0.0f) continue;

            float destW = bigBasicEnemies[i].size.x * scale;
            float destH = bigBasicEnemies[i].size.y * scale;

            // exact center of the displayed sprite
            float centerX = bigBasicEnemies[i].position.x + destW / 2.0f;
            float centerY = bigBasicEnemies[i].position.y + destH / 2.0f;

            float radius = fmaxf(destW, destH) / 2.0f;

            // collision circle
            DrawCircleLines((int)centerX, (int)centerY, radius, RED);
        }

        // --- Updating and drawing bullets ---
        UpdateBullets(assets);


        // Help Text
        Vector2 controlPos = { 15, 61 };
        float fontSize = 15;
        float spacing = 2;
        Vector2 controlPosrot = { 0, 0 };
        float rotation = -6.0f;
        // Texte séparé en lignes
        const char* lines[] = {
            "Use Z or W to move",
            "A or Q to turn left",
            "D to turn right",
            "Ctrl to teleport"
        };

        for (int i = 0; i < 4; i++) {
            DrawTextPro(assets->font, lines[i],
                (Vector2) {
                controlPos.x, controlPos.y + i * (fontSize + 5)
            },
                controlPosrot, rotation,
                fontSize, spacing, WHITE);
        }
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
        // I just wanted to center the text.
        DrawText("YOU LOSE !\n        PRESS [ESC] TO RETURN IN THE MENU",
            GetScreenWidth() / 2 - MeasureText("YOU LOSE !", 20) / 2,
            GetScreenHeight() / 2 - 50, 20, RED);
    }
    EndDrawing();
    if (IsKeyPressed(KEY_ENTER))
    {
        RestartGame(assets, basicEnemy);
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
        printf("Allocation unsuccessful");
        return;
    }
    player->position = Vector2D_SetFromComponents(1300 / 2, 1080 / 2);
    player->size = Vector2D_SetFromComponents(50, 50);                        // The player's base size is “size.”
    player->velocity = Vector2D_SetFromComponents(0, 0);
    player->angle = PI;
    player->color = shipColor;
}

// Implementation and verification of game controls
Vector2D CheckInput(void)
{
    // Turn right
    if (IsKeyDown('D'))
    {
        player->angle = player->angle + ROTATION_SPEED;
    }

    // Turn left
    if (IsKeyDown('A') | IsKeyDown('Q'))
    {
        player->angle = player->angle - ROTATION_SPEED;
    }

    // accelerated toward the front of the ship
    if (IsKeyDown('Z') | IsKeyDown('W'))
    {
        Vector2D pushVector = Vector2D_SetFromComponents(
            cos(player->angle) * PUSH, sin(player->angle) * PUSH);            // calculates the thrust vector based on the angle of the spacecraft
        Vector2D_Print(pushVector);
        printf("\n");
        player->velocity = Vector2D_Add(player->velocity, pushVector);
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        // tir depuis le centre du vaisseau
        Vector2D firePos = Vector2D_SetFromComponents(player->position.x + player->size.x * 0.5f,
            player->position.y + player->size.y * 0.5f);
        FireBullet(firePos, player->angle);
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

        // inertia is applied: position = position + velocity
        player->position = Vector2D_Add(player->position, player->velocity);

        // friction is applied: velocity = velocity x friction
        player->velocity = Vector2D_Scale(player->velocity, FRICTION, Vector2D_SetFromComponents(0, 0));
    }
}