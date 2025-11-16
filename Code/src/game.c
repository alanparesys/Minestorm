#include <raylib.h>

#include "math.h"
#include "game.h"
#include "enemy.h"
#include "vector2D.h"
#include "stdio.h"
#include "stdlib.h"
#include "bullet.h"
#include "sphere2D.h"
#include "ship_player.h"

GameScreen currentScreen = TITLE;
bool title = true;
bool help = false;
bool solo = false;
bool pause = false;

bool allowMove = false;
bool gameOver = false;

Ship* player = NULL;
Collision* collision;


float positionX = (625 / 2) - (50 / 2);                // player's x-position
float positionY = (1070 / 2) - (50 / 2) - (2 - 40);    // player's y-position

// physique du jeu
const float ROTATION_SPEED = 0.05f;                    // the speed of rotation in one frame
const float PUSH = 1.5f;                            // thrust value (press Z or W once = 0.2f)
const float FRICTION = 0.93f;                        // friction (slowdown) = 0.99f

float fireCooldown = 0.25; // 0.25 seconds of cooldown
float timeSinceLastShot = 0.0f;

char buffer[64];  // stock some text (lifes)
int lifeNumber = 3;                          // number of lives

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
    assets->basicEnemyTexture = LoadTexture("Assets/basic_enemy1.png");
    assets->shooterEnemyTexture = LoadTexture("Assets/shooter_enemy.png");
    assets->followerEnemyTexture = LoadTexture("Assets/follower_enemy.png");
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

void UpdateGame(GameAssets* assets, Enemy* enemy, Collision* collision)
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
        UpdateSoloGameplay(assets, enemy, collision);
        break;
    case PAUSE:
        UpdatePauseMenu(assets);
        break;
    case GAMEOVER:
        UpdateGameOver(assets, collision);
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

void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy, Collision* collision)
{

    ClearBackground(BLACK);
    player->bbox = Rectangle2D_SetFromCenterLengthWidthAngle(
        player->position,
        player->size.x,   // longueur
        player->size.y,   // largeur
        player->angle     // rotation
    );

    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    if (currentScreen == SOLO_GAMEPLAY)
    {
        PlayerEnemyCollision();
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
        UpdateEnemies(assets, collision);


        Vector2 interfacePos = { 0, 0 };
        
       

        // --- Updating and drawing bullets ---
        UpdateBullets(assets, collision);

        DrawHitboxes();


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

        for (int i = 0; i < 4; i++)
        {
            DrawTextPro(assets->font, lines[i],
                (Vector2) {
                controlPos.x, controlPos.y + i * (fontSize + 5)
            },
                controlPosrot, rotation,
                fontSize, spacing, WHITE);
        }
        DrawTextureEx(assets->interface, interfacePos, 0, 1, WHITE);
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

void UpdateGameOver(GameAssets* assets, Collision* collision)
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
        RestartGame(assets, basicEnemy, collision);
    }
}

void RestartGame(GameAssets* assets, Enemy* enemy, Collision* collision)
{
    UpdateGame(assets, enemy, collision);
    gameOver = false;
}

void InitGame(void)
{
    player = (Ship*)malloc(sizeof(Ship));
    if (player == NULL) {
        printf("Allocation unsuccessful");
        return;
    }
    player->position = Vector2D_SetFromComponents(1300.0f / 2, 1080.0f / 2);
    player->size = Vector2D_SetFromComponents(50, 50);
    player->velocity = Vector2D_SetFromComponents(0, 0);
    player->angle = PI;
    player->color = shipColor;

    collision = (Collision*)malloc(sizeof(Collision));
    if (collision == NULL) {
        printf("Allocation unsuccessful for collision\n");
        return;
    }

    // Initialiser tous les tableaux de collision à false
    for (int i = 0; i < maxBigBasicEnemies; i++) {
        collision->bigBasicEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxMidBasicEnemies; i++) {
        collision->midBasicEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxSmallBasicEnemies; i++) {
        collision->smallBasicEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxBigShooterEnemy; i++) {
        collision->bigShooterEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxMidShooterEnemy; i++) {
        collision->midShooterEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxSmallShooterEnemy; i++) {
        collision->smallShooterEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxBigFollowerEnemy; i++) {
        collision->bigFollowerEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxMidFollowerEnemy; i++) {
        collision->midFollowerEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxSmallFollowerEnemy; i++) {
        collision->smallFollowerEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxBigFollowerShooterEnemy; i++) {
        collision->bigFollowerShooterEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxMidFollowerShooterEnemy; i++) {
        collision->midFollowerShooterEnemiesBulletCollision[i] = false;
    }
    for (int i = 0; i < maxSmallFollowerShooterEnemy; i++) {
        collision->smallFollowerShooterEnemiesBulletCollision[i] = false;
    }

    // =========================== Initialiser BASIC Enemies ===========================
    for (int i = 0; i < maxBigBasicEnemies; i++)
    {
        bigBasicEnemiesSpawn[i] = false;
        bigBasicEnemies[i].size.x = 0.0f;
        bigBasicEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxMidBasicEnemies; i++)
    {
        midBasicEnemiesSpawn[i] = false;
        midBasicEnemies[i].size.x = 0.0f;
        midBasicEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxSmallBasicEnemies; i++)
    {
        smallBasicEnemiesSpawn[i] = false;
        smallBasicEnemies[i].size.x = 0.0f;
        smallBasicEnemies[i].size.y = 0.0f;
    }

    // =========================== Initialiser SHOOTER Enemies ===========================
    for (int i = 0; i < maxBigShooterEnemy; i++)
    {
        bigShooterEnemiesSpawn[i] = false;
        bigShooterEnemies[i].size.x = 0.0f;
        bigShooterEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxMidShooterEnemy; i++)
    {
        midShooterEnemiesSpawn[i] = false;
        midShooterEnemies[i].size.x = 0.0f;
        midShooterEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxSmallShooterEnemy; i++)
    {
        smallShooterEnemiesSpawn[i] = false;
        smallShooterEnemies[i].size.x = 0.0f;
        smallShooterEnemies[i].size.y = 0.0f;
    }

    // =========================== Initialiser FOLLOWER Enemies ===========================
    for (int i = 0; i < maxBigFollowerEnemy; i++)
    {
        bigFollowerEnemiesSpawn[i] = false;
        bigFollowerEnemies[i].size.x = 0.0f;
        bigFollowerEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxMidFollowerEnemy; i++)
    {
        midFollowerEnemiesSpawn[i] = false;
        midFollowerEnemies[i].size.x = 0.0f;
        midFollowerEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxSmallFollowerEnemy; i++)
    {
        smallFollowerEnemiesSpawn[i] = false;
        smallFollowerEnemies[i].size.x = 0.0f;
        smallFollowerEnemies[i].size.y = 0.0f;
    }

    // =========================== Initialiser FOLLOWER-SHOOTER Enemies ===========================
    for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
    {
        bigFollowerShooterEnemiesSpawn[i] = false;
        bigFollowerShooterEnemies[i].size.x = 0.0f;
        bigFollowerShooterEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxMidFollowerShooterEnemy; i++)
    {
        midFollowerShooterEnemiesSpawn[i] = false;
        midFollowerShooterEnemies[i].size.x = 0.0f;
        midFollowerShooterEnemies[i].size.y = 0.0f;
    }

    for (int i = 0; i < maxSmallFollowerShooterEnemy; i++)
    {
        smallFollowerShooterEnemiesSpawn[i] = false;
        smallFollowerShooterEnemies[i].size.x = 0.0f;
        smallFollowerShooterEnemies[i].size.y = 0.0f;
    }

    printf("InitGame: Big enemies initialized\n");
}

void UpdateEnemies(GameAssets* assets, Collision* collision)
{
    for (int i = 0; i < maxBigBasicEnemies; i++)
    {
        UpdateBigBasicEnemy(i, assets, collision);
    }

    // Update les 6 MID basic enemies
    for (int i = 0; i < maxMidBasicEnemies; i++)
    {
        UpdateMidBasicEnemy(i, assets, collision);
    }

    // Update 12 SMALL basic enemies
    for (int i = 0; i < maxSmallBasicEnemies; i++)
    {
        UpdateSmallBasicEnemy(i, assets, collision);
    }

    // Update Big Shooter Enemies
    for (int i = 0; i < maxBigShooterEnemy; i++)
    {
        UpdateBigShooterEnemy(i, assets, collision);
    }

    // Update Mid Shooter Enemies
    for (int i = 0; i < maxMidShooterEnemy; i++)
    {
        UpdateMidShooterEnemy(i, assets, collision);
    }

    // Update Small Shooter Enemies
    for (int i = 0; i < maxSmallShooterEnemy; i++)
    {
        UpdateSmallShooterEnemy(i, assets, collision);
    }

    // Update Big Follower Enemies
    for (int i = 0; i < maxBigFollowerEnemy; i++)
    {
        UpdateBigFollowerEnemy(i, assets, collision);
    }

    // Update Mid Follower Enemies
    for (int i = 0; i < maxMidFollowerEnemy; i++)
    {
        UpdateMidFollowerEnemy(i, assets, collision);
    }

    //Update Small Follower Enemies
    for (int i = 0; i < maxSmallFollowerEnemy; i++)
    {
        UpdateSmallFollowerEnemy(i, assets, collision);
    }

    // Update Big Follower-Shooter Enemies
    for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
    {
        UpdateBigFollowerShooterEnemy(i, assets, collision);
    }

    // Update Mid Follower-Shooter Enemies
    for (int i = 0; i < maxMidFollowerShooterEnemy; i++)
    {
        UpdateMidFollowerShooterEnemy(i, assets, collision);
    }

    // Update Small Follower-Shooter Enemies
    for (int i = 0; i < maxSmallFollowerShooterEnemy; i++)
    {
        UpdateSmallFollowerShooterEnemy(i, assets, collision);
    }
}

// Implementation and verification of game controls
void CheckInput(void)
{
    // Turn right
    if (IsKeyDown('D'))
    {
        player->angle = player->angle + ROTATION_SPEED;
    }

    // Turn left
    if (IsKeyDown('A') || IsKeyDown('Q'))
    {
        player->angle = player->angle - ROTATION_SPEED;
    }

    // accelerated toward the front of the ship
    if (IsKeyDown('Z') || IsKeyDown('W'))
    {
        Vector2D pushVector = Vector2D_SetFromComponents(
            cos(player->angle) * PUSH, sin(player->angle) * PUSH);            // calculates the thrust vector based on the angle of the spacecraft
        // Vector2D_Print(pushVector);
        // printf("\n");
        player->velocity = Vector2D_Add(player->velocity, pushVector);
    }

    

    timeSinceLastShot = timeSinceLastShot + GetFrameTime();

    if (IsKeyDown(KEY_SPACE) && timeSinceLastShot >= fireCooldown)
    {

        // tir depuis le centre du vaisseau
        Vector2D firePos = Vector2D_SetFromComponents(player->position.x + player->size.x * 0.5f,
            player->position.y + player->size.y * 0.5f);
        FireBullet(firePos, player->angle);

        timeSinceLastShot = 0.0f; // reset of the timer
    }

    if (IsKeyPressed(KEY_LEFT_CONTROL))
    {
		float randX = (float)(GetRandomValue(50, GetScreenWidth() - 50 - (int)player->size.x));
		float randY = (float)(GetRandomValue(100, GetScreenHeight() - 50 - (int)player->size.y));
		float randAngle = (float)(GetRandomValue(0, 360)) * (PI / 180.0f); // random angle in radians
		player->angle = randAngle;
        player->position = Vector2D_SetFromComponents(randX, randY);
    }

    if (IsKeyDown('H'))
    {
        BoundingBoxPlayer();
    }
}

void BoundingBoxPlayer(void) {
    DrawLine(player->bbox.p1.x, player->bbox.p1.y, player->bbox.p2.x, player->bbox.p2.y, RED);
    DrawLine(player->bbox.p2.x, player->bbox.p2.y, player->bbox.p3.x, player->bbox.p3.y, RED);
    DrawLine(player->bbox.p3.x, player->bbox.p3.y, player->bbox.p4.x, player->bbox.p4.y, RED);
    DrawLine(player->bbox.p4.x, player->bbox.p4.y, player->bbox.p1.x, player->bbox.p1.y, RED);
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
/*

// a faire and une fonction avec la touche h
void DrawHitboxes(void)
{
    const float scale = 3.5f; // same scale as in enemy.c
    // BigBasicEnemies 
    for (int i = 0; i < maxBigBasicEnemies; i++)
    {
        if (bigBasicEnemies[i].size.x <= 0.0f || bigBasicEnemies[i].size.y <= 0.0f) continue;
        float destW = bigBasicEnemies[i].size.x * scale;
        float destH = bigBasicEnemies[i].size.y * scale;
        float centerX = bigBasicEnemies[i].position.x + destW / 2.0f;
        float centerY = bigBasicEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, RED);
    }

	// MidBasicEnemies
    for (int i = 0; i < maxMidBasicEnemies; i++)
    {
        if (midBasicEnemies[i].size.x <= 0.0f || midBasicEnemies[i].size.y <= 0.0f) continue;
        float destW = midBasicEnemies[i].size.x * (scale * 2.0f / 3.5f); // scale for mid enemies
        float destH = midBasicEnemies[i].size.y * (scale * 2.0f / 3.5f);
        float centerX = midBasicEnemies[i].position.x + destW / 2.0f;
        float centerY = midBasicEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, ORANGE);
	}

	// SmallBasicEnemies
    for (int i = 0; i < maxSmallBasicEnemies; i++)
    {
        if (smallBasicEnemies[i].size.x <= 0.0f || smallBasicEnemies[i].size.y <= 0.0f) continue;
        float destW = smallBasicEnemies[i].size.x * (scale * 1.0f / 3.5f); // scale for small enemies
        float destH = smallBasicEnemies[i].size.y * (scale * 1.0f / 3.5f);
        float centerX = smallBasicEnemies[i].position.x + destW / 2.0f;
        float centerY = smallBasicEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, PURPLE);
	}

	// BigShooterEnemies
    for (int i = 0; i < maxBigShooterEnemy; i++)
    {
        if (bigShooterEnemies[i].size.x <= 0.0f || bigShooterEnemies[i].size.y <= 0.0f) continue;
        float destW = bigShooterEnemies[i].size.x * scale;
        float destH = bigShooterEnemies[i].size.y * scale;
        float centerX = bigShooterEnemies[i].position.x + destW / 2.0f;
        float centerY = bigShooterEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, GREEN);
    }

	// MidShooterEnemies
    for (int i = 0; i < maxMidShooterEnemy; i++)
    {
        if (midShooterEnemies[i].size.x <= 0.0f || midShooterEnemies[i].size.y <= 0.0f) continue;
        float destW = midShooterEnemies[i].size.x * (scale * 2.0f / 3.5f); // scale for mid enemies
        float destH = midShooterEnemies[i].size.y * (scale * 2.0f / 3.5f);
        float centerX = midShooterEnemies[i].position.x + destW / 2.0f;
        float centerY = midShooterEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, SKYBLUE);
	}

	// smallShooterEnemies
    for (int i = 0; i < maxSmallShooterEnemy; i++)
    {
        if (smallShooterEnemies[i].size.x <= 0.0f || smallShooterEnemies[i].size.y <= 0.0f) continue;
        float destW = smallShooterEnemies[i].size.x * (scale * 1.0f / 3.5f); // scale for small enemies
        float destH = smallShooterEnemies[i].size.y * (scale * 1.0f / 3.5f);
        float centerX = smallShooterEnemies[i].position.x + destW / 2.0f;
        float centerY = smallShooterEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, DARKGREEN);
	}

	// BigFollowerEnemies
    for (int i = 0; i < maxBigFollowerEnemy; i++)
    {
        if (bigFollowerEnemies[i].size.x <= 0.0f || bigFollowerEnemies[i].size.y <= 0.0f) continue;
        float destW = bigFollowerEnemies[i].size.x * scale;
        float destH = bigFollowerEnemies[i].size.y * scale;
        float centerX = bigFollowerEnemies[i].position.x + destW / 2.0f;
        float centerY = bigFollowerEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, BLUE);
    }

	// MidFollowerEnemies
    for (int i = 0; i < maxMidFollowerEnemy; i++)
    {
        if (midFollowerEnemies[i].size.x <= 0.0f || midFollowerEnemies[i].size.y <= 0.0f) continue;
        float destW = midFollowerEnemies[i].size.x * (scale * 2.0f / 3.5f); // scale for mid enemies
        float destH = midFollowerEnemies[i].size.y * (scale * 2.0f / 3.5f);
        float centerX = midFollowerEnemies[i].position.x + destW / 2.0f;
        float centerY = midFollowerEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, DARKBLUE);
    }

	// SmallFollowerEnemies
    for (int i = 0; i < maxSmallFollowerEnemy; i++)
    {
        if (smallFollowerEnemies[i].size.x <= 0.0f || smallFollowerEnemies[i].size.y <= 0.0f) continue;
        float destW = smallFollowerEnemies[i].size.x * (scale * 1.0f / 3.5f); // scale for small enemies
        float destH = smallFollowerEnemies[i].size.y * (scale * 1.0f / 3.5f);
        float centerX = smallFollowerEnemies[i].position.x + destW / 2.0f;
        float centerY = smallFollowerEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, SKYBLUE);
	}
    
	// BigFollowerShooterEnemies
    for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
    {
        if (bigFollowerShooterEnemies[i].size.x <= 0.0f || bigFollowerShooterEnemies[i].size.y <= 0.0f) continue;
        float destW = bigFollowerShooterEnemies[i].size.x * scale;
        float destH = bigFollowerShooterEnemies[i].size.y * scale;
        float centerX = bigFollowerShooterEnemies[i].position.x + destW / 2.0f;
        float centerY = bigFollowerShooterEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, YELLOW);
    }

	// MidFollowerShooterEnemies
    for (int i = 0; i < maxMidFollowerShooterEnemy; i++)
    {
        if (midFollowerShooterEnemies[i].size.x <= 0.0f || midFollowerShooterEnemies[i].size.y <= 0.0f) continue;
        float destW = midFollowerShooterEnemies[i].size.x * (scale * 2.0f / 3.5f); // scale for mid enemies
        float destH = midFollowerShooterEnemies[i].size.y * (scale * 2.0f / 3.5f);
        float centerX = midFollowerShooterEnemies[i].position.x + destW / 2.0f;
        float centerY = midFollowerShooterEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, GOLD);
	}

	// SmallFollowerShooterEnemies
    for (int i = 0; i < maxSmallFollowerShooterEnemy; i++)
    {
        if (smallFollowerShooterEnemies[i].size.x <= 0.0f || smallFollowerShooterEnemies[i].size.y <= 0.0f) continue;
        float destW = smallFollowerShooterEnemies[i].size.x * (scale * 1.0f / 3.5f); // scale for small enemies
        float destH = smallFollowerShooterEnemies[i].size.y * (scale * 1.0f / 3.5f);
        float centerX = smallFollowerShooterEnemies[i].position.x + destW / 2.0f;
        float centerY = smallFollowerShooterEnemies[i].position.y + destH / 2.0f;
        float radius = fmaxf(destW, destH) / 2.0f;
        DrawCircleLines((int)centerX, (int)centerY, radius, ORANGE);
	}

}
*/

void PlayerEnemyCollision(void)
{
    for (int i = 0; i < maxBigBasicEnemies; i++) {
        if (player->invincibilityFrames > 0) {
            player->invincibilityFrames--;
        }
        player->bbox = Rectangle2D_SetFromCenterLengthWidthAngle(
            player->position,
            player->size.x,   // longueur
            player->size.y,   // largeur
            player->angle     // rotation
        );

        for (int j = 0; j < maxBigBasicEnemies; j++) {
            if (bigBasicEnemies[j].size.x <= 0 || bigBasicEnemies[j].size.y <= 0) continue;

            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                bigBasicEnemies[j].position,
                bigBasicEnemies[j].size.x * 2.5f,
                bigBasicEnemies[j].size.y * 2.5f,
                0.0f
            );

            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);

            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);

                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
        }

        for (int j = 0; j < maxMidBasicEnemies; j++) {
            if (midBasicEnemies[j].size.x <= 0 || midBasicEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                midBasicEnemies[j].position,
                midBasicEnemies[j].size.x * 2.5f,
                midBasicEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
		}

        for (int j = 0; j < maxSmallBasicEnemies; j++) {
            if (smallBasicEnemies[j].size.x <= 0 || smallBasicEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                smallBasicEnemies[j].position,
                smallBasicEnemies[j].size.x * 2.5f,
                smallBasicEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
		}

        for (int j = 0; j < maxBigShooterEnemy; j++) {
            if (bigShooterEnemies[j].size.x <= 0 || bigShooterEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                bigShooterEnemies[j].position,
                bigShooterEnemies[j].size.x * 2.5f,
                bigShooterEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
        }

        for (int j = 0; j < maxMidShooterEnemy; j++) {
            if (midShooterEnemies[j].size.x <= 0 || midShooterEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                midShooterEnemies[j].position,
                midShooterEnemies[j].size.x * 2.5f,
                midShooterEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
		}

        for (int j = 0; j < maxSmallShooterEnemy; j++) {
            if (smallShooterEnemies[j].size.x <= 0 || smallShooterEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                smallShooterEnemies[j].position,
                smallShooterEnemies[j].size.x * 2.5f,
                smallShooterEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
        }

        for (int j = 0; j < maxBigFollowerEnemy; j++) {
            if (bigFollowerEnemies[j].size.x <= 0 || bigFollowerEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                bigFollowerEnemies[j].position,
                bigFollowerEnemies[j].size.x * 2.5f,
                bigFollowerEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
        }

        for (int j = 0; j < maxMidFollowerEnemy; j++) {
            if (midFollowerEnemies[j].size.x <= 0 || midFollowerEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                midFollowerEnemies[j].position,
                midFollowerEnemies[j].size.x * 2.5f,
                midFollowerEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
        }

        for (int j = 0; j < maxSmallFollowerEnemy; j++) {
            if (smallFollowerEnemies[j].size.x <= 0 || smallFollowerEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                smallFollowerEnemies[j].position,
                smallFollowerEnemies[j].size.x * 2.5f,
                smallFollowerEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
		}

        for (int j = 0; j < maxBigFollowerShooterEnemy; j++) {
            if (bigFollowerShooterEnemies[j].size.x <= 0 || bigFollowerShooterEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                bigFollowerShooterEnemies[j].position,
                bigFollowerShooterEnemies[j].size.x * 2.5f,
                bigFollowerShooterEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
		}

        for (int j = 0; j < maxMidFollowerShooterEnemy; j++) {
            if (midFollowerShooterEnemies[j].size.x <= 0 || midFollowerShooterEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                midFollowerShooterEnemies[j].position,
                midFollowerShooterEnemies[j].size.x * 2.5f,
                midFollowerShooterEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
        }

        for (int j = 0; j < maxSmallFollowerShooterEnemy; j++) {
            if (smallFollowerShooterEnemies[j].size.x <= 0 || smallFollowerShooterEnemies[j].size.y <= 0) continue;
            // Rectangle de l’ennemi avec ton module
            Rectangle2D enemyBox = Rectangle2D_SetFromCenterLengthWidthAngle(
                smallFollowerShooterEnemies[j].position,
                smallFollowerShooterEnemies[j].size.x * 2.5f,
                smallFollowerShooterEnemies[j].size.y * 2.5f,
                0.0f
            );
            Vector2D enemyCenter = enemyBox.center;
            float enemyRadius = fmax(enemyBox.length, enemyBox.width) / 2.0f;
            Sphere2D hitbox = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);
            if (player->invincibilityFrames <= 0 && CheckCollisionShipEnemy(player->bbox, hitbox)) {
                player->invincibilityFrames = 60;
                lifeNumber = lifeNumber--;
                printf("COLLISION DETECTÉE ! Vie restante : %d\n", lifeNumber);
                if (lifeNumber <= 0) {
                    currentScreen = GAMEOVER;
                    gameOver = true;
                }
            }
		}
    }
}


void DrawHitboxes(void)
{
    // BigBasicEnemies 
    for (int i = 0; i < maxBigBasicEnemies; i++)
    {
        if (bigBasicEnemies[i].size.x <= 0.0f) continue;
        Sphere2D hitbox = GetBigBasicEnemyHitbox(i);
        DrawCircleLines((int)hitbox.center.x, (int)hitbox.center.y, (int)hitbox.radius, RED);
    }

    // MidBasicEnemies
    for (int i = 0; i < maxMidBasicEnemies; i++)
    {
        if (midBasicEnemies[i].size.x <= 0.0f) continue;
        Sphere2D hitbox = GetMidBasicEnemyHitbox(i);
        DrawCircleLines((int)hitbox.center.x, (int)hitbox.center.y, (int)hitbox.radius, ORANGE);
    }

    // SmallBasicEnemies
    for (int i = 0; i < maxSmallBasicEnemies; i++)
    {
        if (smallBasicEnemies[i].size.x <= 0.0f) continue;
        Sphere2D hitbox = GetSmallBasicEnemyHitbox(i);
        DrawCircleLines((int)hitbox.center.x, (int)hitbox.center.y, (int)hitbox.radius, PURPLE);
    }

    // BigShooterEnemies
    for (int i = 0; i < maxBigShooterEnemy; i++)
    {
        if (bigShooterEnemies[i].size.x <= 0.0f) continue;
        Sphere2D hitbox = GetBigShooterEnemyHitbox(i);
        DrawCircleLines((int)hitbox.center.x, (int)hitbox.center.y, (int)hitbox.radius, GREEN);
    }

    // BigFollowerEnemies
    for (int i = 0; i < maxBigFollowerEnemy; i++)
    {
        if (bigFollowerEnemies[i].size.x <= 0.0f) continue;
        Sphere2D hitbox = GetBigFollowerEnemyHitbox(i);
        DrawCircleLines((int)hitbox.center.x, (int)hitbox.center.y, (int)hitbox.radius, BLUE);
    }

    // BigFollowerShooterEnemies
    for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
    {
        if (bigFollowerShooterEnemies[i].size.x <= 0.0f) continue;
        Sphere2D hitbox = GetBigFollowerShooterEnemyHitbox(i);
        DrawCircleLines((int)hitbox.center.x, (int)hitbox.center.y, (int)hitbox.radius, YELLOW);
    }
}
