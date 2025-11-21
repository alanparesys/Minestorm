#include <raylib.h>

#include "math.h"
#include "game.h"
#include "enemy.h"
#include "vector2D.h"
#include "stdio.h"
#include "stdlib.h"
#include "bullet.h"
#include "sphere2D.h"
#include "explosion.h"
#include "polygone2D.h"
#include "collision2D.h"
#include "aabb2D.h"
#include "bonus.h"

GameScreen currentScreen = TITLE;
bool title = true;
bool help = false;
bool controls = false;
bool solo = false;
bool titlePause = false;
bool pause = false;

bool allowMove = false;
bool gameOver = false;
bool levelSpawned = false;

bool youLosePlayed = false;

// Countdown to launch
static float countdownTimer = 3.0f; // 3 second countdown
static bool countdownActive = false;

Ship* player = NULL;
Collision* collision;


float positionX = (625 / 2) - (50 / 2);                // player's x-position
float positionY = (1070 / 2) - (50 / 2) - (2 - 40);    // player's y-position

// physique du jeu
const float ROTATION_SPEED = 0.08f;                    // the speed of rotation in one frame
const float PUSH = 1.5f;                            // thrust value (press Z or W once = 0.2f)
const float FRICTION = 0.93f;                        // friction (slowdown) = 0.99f

float fireCooldown = 0.25; // 0.25 seconds of cooldown
float timeSinceLastShot = 0.0f;
float rapidFireCountdown = 3.0f;

int score = 0;
int actualLevel = 1;
int lifeNumber = 3;

int bestScore = 0;


// InitGame: initializes the basic values for physics and player position
Enemy* basicEnemy;
Enemy bigBasicEnemies[3];

Color shipColor = { 186, 18, 186, 255 };

static bool collisionDebugEnabled = false;

#define SHIP_BULLET_COOLDOWN_NORMAL 0.35f // 0.35 second between each shot


#define SHIP_POLYGON_COUNT 2
#define RENDER_SCALE_BIG 3.5f
#define RENDER_SCALE_MID 2.0f
#define RENDER_SCALE_SMALL 1.5f
#define COLLISION_DEBUG_KEY KEY_C  // Changed from KEY_H to KEY_F3 because H is used for menus

static const Vector2D SHIP_BODY_POINTS[] = {
    { 25.0f, 0.0f },
    { -10.0f, -18.0f },
    { -30.0f, 0.0f },
    { -10.0f, 18.0f }
};
static const int SHIP_BODY_POINT_COUNT = 4;

static const Vector2D SHIP_THRUSTER_POINTS[] = {
    { -30.0f, -10.0f },
    { -42.0f, -10.0f },
    { -42.0f, 10.0f },
    { -30.0f, 10.0f }
};
static const int SHIP_THRUSTER_POINT_COUNT = 4;

static const Color BROAD_PHASE_COLOR = { 0, 228, 48, 120 };
static const Color BROAD_PHASE_HIT_COLOR = { 255, 161, 0, 200 };
static const Color NARROW_PHASE_COLOR = { 0, 121, 241, 200 };
static const Color NARROW_PHASE_HIT_COLOR = { 230, 41, 55, 230 };
static void BuildShipCollisionPolygons(Polygone2D* outPolys, int* outCount);
static Polygone2D BuildShipPolygonFromLocal(const Vector2D* localPoints, int pointCount, float scaleX, float scaleY);
static void FreeShipCollisionPolygons(Polygone2D* polys, int count);
static void HandleEnemyGroupCollision(Enemy* enemies, int count, float renderScale, Sphere2D playerSphere, AABB2D playerAABB, Polygone2D* shipPolys, int shipPolyCount, bool* shipPolyHits, bool* playerSphereBroadHit, bool* playerAABBBroadHit);
static void ProcessEnemyCollision(Enemy* enemy, float renderScale, Sphere2D playerSphere, AABB2D playerAABB, Polygone2D* shipPolys, int shipPolyCount, bool* shipPolyHits, bool* playerSphereBroadHit, bool* playerAABBBroadHit);
static void DrawPolygonOutline(Polygone2D poly, Color color);
static void DrawAABBOutline(AABB2D box, Color color);
static void DrawSphereOutline(Sphere2D sphere, Color color);
static void PlayerTakeDamage(void);
static void DrawHitboxGroup(Enemy* enemies, int count, Sphere2D(*hitboxFunc)(int), Color color);
static void DrawBulletHitboxes(void);
static void DrawShipVectors(void);
static void DrawArrow(Vector2D start, Vector2D direction, float length, Color color);

void CheckLifeOfPlayer()
{
    if (lifeNumber <= 0) {
        currentScreen = GAMEOVER;
        gameOver = true;
    }
}

void InitAssets(GameAssets* assets)
{
    assets->background = LoadTexture("Assets/Background1080_1300.png");
    assets->interface = LoadTexture("Assets/interface1.png");
    assets->minestorm = LoadTexture("Assets/minestorm.png");
    assets->titleText = LoadTexture("Assets/title_text.png");
    assets->ship = LoadTexture("Assets/Kenney/ship_sidesA.png");
    assets->bulletTexture = LoadTexture("Assets/Kenney/meteor_large.png");
    assets->basicEnemyTexture = LoadTexture("Assets/basic_enemy1.png");
    assets->shooterEnemyTexture = LoadTexture("Assets/shooter_enemy.png");
    assets->followerEnemyTexture = LoadTexture("Assets/follower_enemy.png");
    assets->followerShooterEnemyTexture = LoadTexture("Assets/follower_shooter_enemy.png");
    assets->motherShipTexture = LoadTexture("Assets/mother_ship.png");
    assets->pixelFont = LoadFont("Assets/pixel_police.ttf");
    assets->magnetoFont = LoadFont("Assets/Magneto.ttf");
    assets->explosionTexture = LoadTexture("Assets/explosion3.png");
    assets->redStarTexture = LoadTexture("Assets/red_star.png");
    assets->yellowStarTexture = LoadTexture("Assets/yellow_star.png");
    assets->greenStarTexture = LoadTexture("Assets/green_star.png");
    assets->blueStarTexture = LoadTexture("Assets/blue_star.png");

    assets->explosionSound = LoadSound("Assets/explosion.mp3");
    assets->youLose = LoadSound("Assets/Sound/gameOver.wav");
    assets->die = LoadSound("Assets/Sound/explosion.wav");
    assets->laser = LoadSound("Assets/Sound/Laser.wav");
    assets->laser2 = LoadSound("Assets/Sound/laser2.wav");
    assets->selected = LoadSound("Assets/Sound/selected.wav");
    assets->damage = LoadSound("Assets/Sound/damage.wav");
    assets->levelUp = LoadSound("Assets/Sound/levelup.wav");
}

void UnloadAssets(GameAssets* assets)
{
    UnloadTexture(assets->background);
    UnloadTexture(assets->interface);
    UnloadTexture(assets->minestorm);
    UnloadTexture(assets->titleText);
    UnloadTexture(assets->ship);
    UnloadTexture(assets->bulletTexture);
    UnloadTexture(assets->basicEnemyTexture);
    UnloadTexture(assets->shooterEnemyTexture);
    UnloadTexture(assets->followerEnemyTexture);
    UnloadTexture(assets->followerShooterEnemyTexture);
    UnloadTexture(assets->motherShipTexture);
    UnloadTexture(assets->explosionTexture);
    UnloadTexture(assets->blueStarTexture);
    UnloadTexture(assets->yellowStarTexture);
    UnloadTexture(assets->greenStarTexture);
    UnloadTexture(assets->redStarTexture);
    UnloadFont(assets->pixelFont);
    UnloadFont(assets->magnetoFont);
    UnloadSound(assets->explosionSound);
    UnloadSound(assets->die);
    UnloadSound(assets->laser);
    UnloadSound(assets->laser2);
    UnloadSound(assets->selected);
    UnloadSound(assets->damage);
    UnloadSound(assets->levelUp);
}

void UpdateGame(GameAssets* assets, Enemy* enemy, Collision* collision)
{
    switch (currentScreen)
    {
    case TITLE:
        UpdateTitleScreen(assets);
        break;
    case SOLO_GAMEPLAY:
        UpdateSoloGameplay(assets, enemy, collision);
        break;
    case CONTROLS:
        UpdateControlsGameplay(assets);
        break;
    case HELP:
        UpdateHelpGameplay(assets);
        break;
    case TITLE_PAUSE:
        UpdateTitlePause(assets);
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
    StopMusicStream(bgMusic);
    SetMusicVolume(titleMusic, 1.0f);
    PlayMusicStream(titleMusic);

    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);

    DrawTextureEx(assets->interface, (Vector2) { 0, 0 }, 0, 1, WHITE);
    DrawTextureEx(assets->minestorm, (Vector2) { 230, -25 }, 0, 0.7f, WHITE);
    DrawTextureEx(assets->titleText, (Vector2) { 0, -150 }, 0, 1, WHITE);
    EndDrawing();

    if (IsKeyPressed(KEY_H))
    {
        PlaySound(assets->selected);
        currentScreen = HELP;
        title = false;
        help = true;
    }

    if (IsKeyPressed(KEY_C))
    {
        PlaySound(assets->selected);
        currentScreen = CONTROLS;
        title = false;
        controls = true;
    }

    if (IsKeyPressed(KEY_P))
    {
        PlaySound(assets->selected);
        currentScreen = TITLE_PAUSE;
        title = false;
        titlePause = true;
    }

    if (IsKeyPressed(KEY_F))
    {
        PlaySound(assets->selected);
        currentScreen = SOLO_GAMEPLAY;
        title = false;
        solo = true;
        // Start the countdown
        countdownTimer = 3.0f;
        countdownActive = true;
        allowMove = false; // Disable inputs during the countdown
    }
}

void UpdateControlsGameplay(GameAssets* assets)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    DrawTextureEx(assets->interface, (Vector2) { 0, 0 }, 0, 1, WHITE);
    DrawTextureEx(assets->minestorm, (Vector2) { 230, -25 }, 0, 0.7f, WHITE);
    DrawTextPro(assets->magnetoFont, "[ Z or W ] To move forward\n\n\n\n\n\n[ A or Q ] To turn left\n\n\n\n\n\n[ D ] To turn right\n\n\n\n\n\n[ Left Ctrl ] To teleport", (Vector2) { 150, 300 }, (Vector2) { 0, 0 }, 0.0f, 50, 2, WHITE);
    DrawTextPro(assets->magnetoFont, "[ H ] To return to title screen", (Vector2) { 250, 1000 }, (Vector2) { 0, 0 }, 0.0f, 30, 2, WHITE);

    EndDrawing();

    if (IsKeyPressed(KEY_C))
    {
        PlaySound(assets->selected);
        currentScreen = TITLE;
        controls = false;
        title = true;
    }
}

void UpdateHelpGameplay(GameAssets* assets)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    DrawTextureEx(assets->interface, (Vector2) { 0, 0 }, 0, 1, WHITE);
    DrawTextureEx(assets->minestorm, (Vector2) { 230, -25 }, 0, 0.7f, WHITE);
    DrawTextureEx(assets->basicEnemyTexture, (Vector2) { 225, 200 }, 0, 0.75, WHITE);
    DrawTextureEx(assets->shooterEnemyTexture, (Vector2) { 650, 205 }, 0, 1.45, WHITE);
    DrawTextureEx(assets->followerEnemyTexture, (Vector2) { 225, 550 }, 0, 0.8, WHITE);
    DrawTextureEx(assets->followerShooterEnemyTexture, (Vector2) { 665, 575 }, 0, 2.50, WHITE);
    DrawTextPro(assets->magnetoFont, "   Basic Enemy\n\n\nHe always moves\n\n  straight ahead", (Vector2) { 185, 375 }, (Vector2) { 0, 0 }, 0.0f, 25, 2, WHITE);
    DrawTextPro(assets->magnetoFont, "Shooter Enemy\n\n\n   He moves\n\nstraight ahead\n\nand shoots you", (Vector2) { 625, 375 }, (Vector2) { 0, 0 }, 0.0f, 25, 2, WHITE);
    DrawTextPro(assets->magnetoFont, "Follower Enemy\n\n\n   He follows\n\nyou everywhere", (Vector2) { 185, 725 }, (Vector2) { 0, 0 }, 0.0f, 25, 2, WHITE);
    DrawTextPro(assets->magnetoFont, "Follower Shooter\n\n      Enemy\n\n\n   He follows\n\nyou everywhere\n\n and shoots you", (Vector2) { 625, 725 }, (Vector2) { 0, 0 }, 0.0f, 25, 2, WHITE);
    DrawTextPro(assets->magnetoFont, "[ H ] To return to title screen", (Vector2) { 250, 1000 }, (Vector2) { 0, 0 }, 0.0f, 30, 2, WHITE);

    EndDrawing();

    if (IsKeyPressed(KEY_H))
    {
        PlaySound(assets->selected);
        currentScreen = TITLE;
        help = false;
        title = true;
    }
}

void UpdateTitlePause(GameAssets* assets)
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw
    DrawTexture(assets->background, 0, 0, WHITE);
    DrawTextureEx(assets->interface, (Vector2) { 0, 0 }, 0, 1, WHITE);
    DrawTextureEx(assets->minestorm, (Vector2) { 230, -25 }, 0, 0.7f, WHITE);
    DrawTextPro(assets->magnetoFont, "You need to click on [ P ] in game\n\n\n\n\n\n\n            not in the title  :(", (Vector2) { 70, 450 }, (Vector2) { 0, 0 }, 0.0f, 50, 2, WHITE);

    EndDrawing();

    if (IsKeyPressed(KEY_P))
    {
        PlaySound(assets->selected);
        currentScreen = TITLE;
        titlePause = false;
        solo = true;
    }
}

void UpdateSoloGameplay(GameAssets* assets, Enemy* enemy, Collision* collision)
{
    StopMusicStream(titleMusic);
    SetMusicVolume(bgMusic, 1.0f);  // Set the music volume to 20%
    PlayMusicStream(bgMusic);  // Start playing the music stream

    // Manage the countdown
    if (countdownActive)
    {
        countdownTimer -= GetFrameTime();
        if (countdownTimer <= 0.0f)
        {
            countdownTimer = 0.0f;
            countdownActive = false;
            allowMove = true; // Activate inputs after the countdown
        }
    }

    if (IsKeyPressed(COLLISION_DEBUG_KEY))
    {
        PlaySound(assets->selected);
        collisionDebugEnabled = !collisionDebugEnabled;
        printf("Debug mode: %s (F3 key pressed)\n", collisionDebugEnabled ? "ON" : "OFF");
    }

    player->bbox = Rectangle2D_SetFromCenterLengthWidthAngle(
        player->position,
        player->size.x,   // longueur
        player->size.y,   // largeur
        player->angle     // rotation
    );

    LevelProgress(assets);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(assets->background, 0, 0, WHITE);


    if (currentScreen == SOLO_GAMEPLAY)
    {
        PlayerEnemyCollision();
        UpdateControlGame(assets);

        DrawSpawnedPoints();

        UpdateBullets(assets, collision);
        UpdateEnemyBullets(assets, player);
        CheckBulletBulletCollisions();

        // Ship display
        DrawTexturePro(assets->ship, (Rectangle) { 0, 0, assets->ship.width, assets->ship.height },
            (Rectangle) {
            player->position.x, player->position.y, player->size.x, player->size.y
        },
            (Vector2) {
            player->size.x / 2, player->size.y / 2
        }, (player->angle * (180.0f / PI)) + 90, player->color);

        MotherShipUpdate(assets, collision);
        UpdateEnemies(assets, collision);

        UpdateBonus(assets);

        UpdateShield();

        CheckBonusPlayerCollision(assets);
        DrawBonus(assets);
        DrawShield();
        UpdateExplosions();
        DrawExplosions(assets->explosionTexture);


        DrawTextureEx(assets->interface, (Vector2) { 0, 0 }, 0, 1, WHITE);
        DrawTextureEx(assets->minestorm, (Vector2) { 230, -25 }, 0, 0.7f, WHITE);

        if (collisionDebugEnabled)
        {
            DrawShipVectors();
            DrawHitboxes();
            DrawBulletHitboxes();
            DrawBonusHitboxes();

        }

        if (score > bestScore)
        {
            bestScore = score;
        }

        DrawTextPro(assets->pixelFont, TextFormat("Lives:%d", lifeNumber), (Vector2) { 75, 55 }, (Vector2) { 0, 0 }, -6.0, 20, 2, WHITE);
        DrawTextPro(assets->pixelFont, TextFormat("Level:%d", actualLevel), (Vector2) { 85, 100 }, (Vector2) { 0, 0 }, -6.0, 20, 2, WHITE);


        DrawTextPro(assets->pixelFont, TextFormat("Score:%d", score), (Vector2) { 850, 40 }, (Vector2) { 0, 0 }, 5.5f, 20, 2, WHITE);
        DrawTextPro(assets->pixelFont, TextFormat("Best Score:%d", bestScore), (Vector2) { 850, 85 }, (Vector2) { 0, 0 }, 5.5f, 20, 2, WHITE);

        EndDrawing();
        shieldTimer += 1.0f / 60.0f;

    }

    if (IsKeyPressed(KEY_P))
    {
        PlaySound(assets->selected);
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
    DrawTextureEx(assets->interface, (Vector2) { 0, 0 }, 0, 1, WHITE);
    DrawTextureEx(assets->minestorm, (Vector2) { 230, -25 }, 0, 0.7f, WHITE);
    DrawTextPro(assets->magnetoFont, "[ Z or W ] To move forward\n\n\n\n\n\n[ A or Q ] To turn left\n\n\n\n\n\n[ D ] To turn right\n\n\n\n\n\n[ Left Ctrl ] To teleport", (Vector2) { 150, 300 }, (Vector2) { 0, 0 }, 0.0f, 50, 2, WHITE);
    DrawTextPro(assets->magnetoFont, "[ P ] To return to resume", (Vector2) { 250, 1000 }, (Vector2) { 0, 0 }, 0.0f, 30, 2, WHITE);

    EndDrawing();
    if (IsKeyPressed(KEY_P))
    {
        PlaySound(assets->selected);
        currentScreen = SOLO_GAMEPLAY;
        pause = false;
        solo = true;
    }
    if (IsKeyPressed('R')) {
        currentScreen = TITLE;
    }
}

void UpdateGameOver(GameAssets* assets, Collision* collision)
{
    if (currentScreen == GAMEOVER)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // Draw
        DrawTexture(assets->background, 0, 0, WHITE);
        DrawTextureEx(assets->interface, (Vector2) { 0, 0 }, 0, 1, WHITE);
        DrawTextureEx(assets->minestorm, (Vector2) { 230, -25 }, 0, 0.7f, WHITE);
        DrawTextPro(assets->magnetoFont, "GAME OVER", (Vector2) { 225, 500 }, (Vector2) { 0, 0 }, 0.0f, 80, 2, RED);
        DrawTextPro(assets->magnetoFont, TextFormat("Your Score : %d", score), (Vector2) { 275, 650 }, (Vector2) { 0, 0 }, 0.0f, 50, 2, WHITE);
        DrawTextPro(assets->magnetoFont, TextFormat("Best Score : %d", bestScore), (Vector2) { 275, 700 }, (Vector2) { 0, 0 }, 0.0f, 50, 2, WHITE);
        DrawTextPro(assets->magnetoFont, "Press [ ENTER ] to restart", (Vector2) { 220, 800 }, (Vector2) { 0, 0 }, 0.0f, 40, 2, WHITE);
        EndDrawing();

        StopMusicStream(bgMusic);
        if (!youLosePlayed) {
            printf("PlaySound called\n");
            SetSoundVolume(assets->youLose, 1.0f);
            PlaySound(assets->youLose);
            youLosePlayed = true;
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            PlaySound(assets->selected);
            RestartGame(assets, basicEnemy, collision);
        }
        if (IsKeyPressed('R')) {
            currentScreen = TITLE;
        }
    }
}


void RestartGame(GameAssets* assets, Enemy* enemy, Collision* collision)
{
    InitGame();
    InitBullets();
    motherShipActive = false;
    motherShipSpawned = false;
    enemiesDropped = false;
    currentSpawnIndex = 0;
    spawnTimer = 0.0f;
    shipBulletCooldown = SHIP_BULLET_COOLDOWN_NORMAL;

    for (int i = 0; i < maxMotherShip; i++) {
        motherShip[i].size.x = 0.0f;
        motherShip[i].size.y = 0.0f;
    }

    InitEnemyBullets();
    InitBonus();

    UpdateGame(assets, enemy, collision);
    gameOver = false;
    levelSpawned = false;
    currentScreen = SOLO_GAMEPLAY;
    score = 0;
    lifeNumber = 3;
    actualLevel = actualLevel;
    shieldTimer = 0.0f;


    countdownTimer = 3.0f;
    countdownActive = true;
    allowMove = false;
    youLosePlayed = false;
}

void InitGame(void)
{
    InitBullets();
    InitEnemyBullets();
    InitExplosions();
    InitBonus();

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
    // Réinitialiser les compteurs
    usedSpawnPoints = 0;

    printf("InitGame: All systems initialized\n");
}


void CheckInput(GameAssets* assets)
{

    if (IsKeyDown('D'))
    {
        player->angle = player->angle + ROTATION_SPEED;
    }

    if (IsKeyDown('A') || IsKeyDown('Q'))
    {
        player->angle = player->angle - ROTATION_SPEED;
    }


    // Accélérer vers l'avant
    if (IsKeyDown('Z') || IsKeyDown('W'))
    {
        Vector2D pushVector = Vector2D_SetFromComponents(
            cos(player->angle) * PUSH,
            sin(player->angle) * PUSH
        );
        player->velocity = Vector2D_Add(player->velocity, pushVector);
    }


    float deltaTime = GetFrameTime();

    // Incrémenter le timer de tir
    timeSinceLastShot += deltaTime;

    if (rapidFireActive)
    {
        rapidFireCountDown -= deltaTime;

        // Vérifier si le bonus est terminé
        if (rapidFireCountDown <= 0.0f)
        {
            rapidFireActive = false;
            shipBulletCooldown = SHIP_BULLET_COOLDOWN_NORMAL;
            printf("Rapid fire ended.\n");
        }
    }

    if (IsKeyDown(KEY_SPACE) && timeSinceLastShot >= shipBulletCooldown)
    {
        if (motherShipSpawned)
        {
            return;
        }

        Vector2D center = player->bbox.center;
        float halfLength = player->size.x * 0.5f;
        Vector2D firePos = Vector2D_SetFromComponents(
            center.x + cosf(player->angle) * halfLength,
            center.y + sinf(player->angle) * halfLength
        );

        // Tirer une balle
        FireBullet(firePos, player->angle, assets);
        PlaySound(assets->laser);

        // Réinitialiser le timer de tir
        timeSinceLastShot = 0.0f;
    }

    if (IsKeyPressed(KEY_LEFT_CONTROL))
    {
        PlaySound(assets->selected);

        // Position aléatoire sur l'écran
        float randX = (float)(GetRandomValue(50, GetScreenWidth() - 50 - (int)player->size.x));
        float randY = (float)(GetRandomValue(100, GetScreenHeight() - 50 - (int)player->size.y));
        float randAngle = (float)(GetRandomValue(0, 360)) * (PI / 180.0f);

        player->angle = randAngle;
        player->position = Vector2D_SetFromComponents(randX, randY);
    }


    if (IsKeyDown('C'))
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

void UpdateControlGame(GameAssets* assets) {
    BorderEnemyCollision(player);

    if (allowMove && !motherShipSpawned && !countdownActive) {
        CheckInput(assets);

        // inertia is applied: position = position + velocity
        player->position = Vector2D_Add(player->position, player->velocity);

        // friction is applied: velocity = velocity x friction
        player->velocity = Vector2D_Scale(player->velocity, FRICTION, Vector2D_SetFromComponents(0, 0));
    }
    else if (motherShipSpawned || countdownActive)
    {
        player->velocity = Vector2D_Scale(player->velocity, FRICTION, Vector2D_SetFromComponents(0, 0));
        if (player->velocity.x * player->velocity.x + player->velocity.y * player->velocity.y > 0.01f)
        {
            player->position = Vector2D_Add(player->position, player->velocity);
        }
    }
}

void BorderPlayerCollision(Ship* player)
{
    if (player->position.x < -50)
        player->position.x = 1080 + 50;

    if (player->position.x > 1080 + 50)
        player->position.x = -50;

    if (player->position.y < 0)
        player->position.y = 1300 + 50;

    if (player->position.y > 1300 + 50)
        player->position.y = 0;
}

void PlayerEnemyCollision(void)
{

    if (player == NULL)
    {
        return;
    }

    if (motherShipSpawned)
    {
        return;
    }

    if (!enemiesActive)
    {
        return;
    }

    if (shieldActive)
    {
        return;
    }

    if (player->invincibilityFrames > 0)
    {
        player->invincibilityFrames--;
    }

    player->bbox = Rectangle2D_SetFromCenterLengthWidthAngle(
        player->position,
        player->size.x,
        player->size.y,
        player->angle
    );

    Sphere2D playerSphere = Sphere2D_SetFromCenterRadius(
        player->position,
        fmaxf(player->size.x, player->size.y) * 0.6f
    );

    AABB2D playerAABB = AABB2D_FromRectangle(player->bbox);

    Polygone2D shipPolys[SHIP_POLYGON_COUNT];
    bool shipPolyHits[SHIP_POLYGON_COUNT] = { false };
    int shipPolyCount = 0;
    BuildShipCollisionPolygons(shipPolys, &shipPolyCount);

    bool playerSphereBroadHit = false;
    bool playerAABBBroadHit = false;

    HandleEnemyGroupCollision(bigBasicEnemies, maxBigBasicEnemies, RENDER_SCALE_BIG,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(midBasicEnemies, maxMidBasicEnemies, RENDER_SCALE_MID,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(smallBasicEnemies, maxSmallBasicEnemies, RENDER_SCALE_SMALL,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(bigShooterEnemies, maxBigShooterEnemy, RENDER_SCALE_BIG,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(midShooterEnemies, maxMidShooterEnemy, RENDER_SCALE_MID,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(smallShooterEnemies, maxSmallShooterEnemy, RENDER_SCALE_SMALL,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(bigFollowerEnemies, maxBigFollowerEnemy, RENDER_SCALE_BIG,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(midFollowerEnemies, maxMidFollowerEnemy, RENDER_SCALE_MID,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(smallFollowerEnemies, maxSmallFollowerEnemy, RENDER_SCALE_SMALL,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(bigFollowerShooterEnemies, maxBigFollowerShooterEnemy, RENDER_SCALE_BIG,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(midFollowerShooterEnemies, maxMidFollowerShooterEnemy, RENDER_SCALE_MID,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    HandleEnemyGroupCollision(smallFollowerShooterEnemies, maxSmallFollowerShooterEnemy, RENDER_SCALE_SMALL,
        playerSphere, playerAABB, shipPolys, shipPolyCount,
        shipPolyHits, &playerSphereBroadHit, &playerAABBBroadHit);

    if (collisionDebugEnabled)
    {
        DrawSphereOutline(playerSphere, playerSphereBroadHit ? BROAD_PHASE_HIT_COLOR : BROAD_PHASE_COLOR);
        DrawAABBOutline(playerAABB, playerAABBBroadHit ? BROAD_PHASE_HIT_COLOR : BROAD_PHASE_COLOR);

        for (int i = 0; i < shipPolyCount; ++i)
        {
            Color color = shipPolyHits[i] ? NARROW_PHASE_HIT_COLOR : NARROW_PHASE_COLOR;
            DrawPolygonOutline(shipPolys[i], color);
        }

    }

    FreeShipCollisionPolygons(shipPolys, shipPolyCount);
}


void DrawHitboxes(void)
{
    if (motherShipSpawned) return;
    DrawHitboxGroup(bigBasicEnemies, maxBigBasicEnemies, GetBigBasicEnemyHitbox, RED);
    DrawHitboxGroup(midBasicEnemies, maxMidBasicEnemies, GetMidBasicEnemyHitbox, ORANGE);
    DrawHitboxGroup(smallBasicEnemies, maxSmallBasicEnemies, GetSmallBasicEnemyHitbox, PURPLE);

    DrawHitboxGroup(bigShooterEnemies, maxBigShooterEnemy, GetBigShooterEnemyHitbox, GREEN);
    DrawHitboxGroup(midShooterEnemies, maxMidShooterEnemy, GetMidShooterEnemyHitbox, SKYBLUE);
    DrawHitboxGroup(smallShooterEnemies, maxSmallShooterEnemy, GetSmallShooterEnemyHitbox, DARKGREEN);

    DrawHitboxGroup(bigFollowerEnemies, maxBigFollowerEnemy, GetBigFollowerEnemyHitbox, BLUE);
    DrawHitboxGroup(midFollowerEnemies, maxMidFollowerEnemy, GetMidFollowerEnemyHitbox, DARKBLUE);
    DrawHitboxGroup(smallFollowerEnemies, maxSmallFollowerEnemy, GetSmallFollowerEnemyHitbox, SKYBLUE);

    DrawHitboxGroup(bigFollowerShooterEnemies, maxBigFollowerShooterEnemy, GetBigFollowerShooterEnemyHitbox, YELLOW);
    DrawHitboxGroup(midFollowerShooterEnemies, maxMidFollowerShooterEnemy, GetMidFollowerShooterEnemyHitbox, GOLD);
    DrawHitboxGroup(smallFollowerShooterEnemies, maxSmallFollowerShooterEnemy, GetSmallFollowerShooterEnemyHitbox, ORANGE);
}

void LevelProgress(GameAssets* assets)
{
    CheckLifeOfPlayer();

    if (!levelSpawned)
    {
        // Niveau 1 : spawn normal
        if (actualLevel == 1)
        {
            for (int i = 0; i < maxBigBasicEnemies; i++) {
                BigBasicEnemySpawn(i);
                bigBasicEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 21;
        }
        // Niveau 2 : spawn deux fois plus d’ennemis
        else if (actualLevel == 2)
        {
            lifeNumber = 3;
            for (int i = 0; i < maxBigBasicEnemies; i++) {
                BigBasicEnemySpawn(i);
                bigBasicEnemiesSpawn[i] = true;
            }
            maxBigFollowerEnemy = 1;
            for (int i = 0; i < maxBigFollowerEnemy; i++) {
                BigFollowerEnemySpawn(i);
                bigFollowerEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 28;
        }

        // Niveau 3 et plus : spawn tout le monde
        else if (actualLevel == 3)
        {
            lifeNumber = 3;
            for (int i = 0; i < maxBigBasicEnemies; i++) {
                BigBasicEnemySpawn(i);
                bigBasicEnemiesSpawn[i] = true;
            }
            maxBigFollowerEnemy = 2;
            for (int i = 0; i < maxBigFollowerEnemy; i++) {
                BigFollowerEnemySpawn(i);
                bigFollowerEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 35;
        }
        else if (actualLevel == 4) {
            lifeNumber = 3;
            // Exemple : spawn des ennemis qui suivent
            maxBigFollowerEnemy = 1;
            for (int i = 0; i < maxBigFollowerEnemy; i++) {
                BigFollowerEnemySpawn(i);
                bigFollowerEnemiesSpawn[i] = true;
            }
            maxBigBasicEnemies = 1;
            for (int i = 0; i < maxBigBasicEnemies; i++) {
                BigBasicEnemySpawn(i);
                bigBasicEnemiesSpawn[i] = true;
            }
            maxBigShooterEnemy = 1;
            for (int i = 0; i < 1; i++) {
                BigShooterEnemySpawn(i);
                bigShooterEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 18;
        }
        else if (actualLevel == 5) {
            lifeNumber = 3;
            // Exemple : spawn des ennemis qui suivent
            for (int i = 0; i < maxBigFollowerEnemy; i++) {
                BigFollowerEnemySpawn(i);
                bigFollowerEnemiesSpawn[i] = true;
            }
            maxBigBasicEnemies = 0;
            for (int i = 0; i < 1; i++) {
                BigShooterEnemySpawn(i);
                bigShooterEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 21;
        }
        else if (actualLevel == 6) {
            lifeNumber = 3;
            maxBigShooterEnemy = 0;
            // Exemple : spawn des ennemis qui suivent
            maxBigFollowerEnemy = 1;
            for (int i = 0; i < maxBigFollowerEnemy; i++) {
                BigFollowerEnemySpawn(i);
                bigFollowerEnemiesSpawn[i] = true;
            }
            for (int i = 0; i < maxBigFollowerShooterEnemy; i++) {
                BigFollowerShooterEnemySpawn(i);
                bigFollowerShooterEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 14;
        }
        else if (actualLevel == 7) {
            lifeNumber = 3;
            // Exemple : spawn des ennemis qui suivent
            maxBigFollowerEnemy = 2;
            for (int i = 0; i < maxBigFollowerEnemy; i++) {
                BigFollowerEnemySpawn(i);
                bigFollowerEnemiesSpawn[i] = true;
            }
            for (int i = 0; i < maxBigBasicEnemies; i++) {
                BigBasicEnemySpawn(i);
                bigBasicEnemiesSpawn[i] = true;
            }
            for (int i = 0; i < maxBigFollowerShooterEnemy; i++) {
                BigFollowerShooterEnemySpawn(i);
                bigFollowerShooterEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 42;
        }
        else if (actualLevel == 8) {
            lifeNumber = 3;
            // Exemple : spawn des ennemis qui suivent
            maxBigShooterEnemy = 1;
            maxBigBasicEnemies = 1;
            maxBigFollowerEnemy = 1;

            for (int i = 0; i < maxBigBasicEnemies; i++) {
                BigBasicEnemySpawn(i);
                bigBasicEnemiesSpawn[i] = true;
            }
            for (int i = 0; i < maxBigFollowerEnemy; i++) {
                BigFollowerEnemySpawn(i);
                bigFollowerEnemiesSpawn[i] = true;
            }
            for (int i = 0; i < 1; i++) {
                BigShooterEnemySpawn(i);
                bigShooterEnemiesSpawn[i] = true;
            }
            for (int i = 0; i < maxBigShooterEnemy; i++) {
                BigShooterEnemySpawn(i);
                bigShooterEnemiesSpawn[i] = true;
            }
            for (int i = 0; i < maxBigFollowerShooterEnemy; i++) {
                BigFollowerShooterEnemySpawn(i);
                bigFollowerShooterEnemiesSpawn[i] = true;
            }
            maxSpawnPoint = 28;
        }
        else if (actualLevel == 9) {
            lifeNumber = 3;
            // Boss ! Niveau Final.
        }
        levelSpawned = true;
        return;
    }

    // Vérifie si tous les ennemis spawnés sont morts
    if (AllEnemiesDead())
    {
        PlaySound(assets->levelUp);
        actualLevel++;
        printf("Tous les ennemis détruits ! Passage au niveau %d\n", actualLevel);
        levelSpawned = false; // déclenche le spawn du prochain niveau
        ResetMotherShip(); // Réinitialise le vaisseau mère pour rejouer l'animation
    }

}

static void BuildShipCollisionPolygons(Polygone2D* outPolys, int* outCount)
{
    if (outPolys == NULL || outCount == NULL || player == NULL)
    {
        if (outCount) *outCount = 0;
        return;
    }

    float scaleX = player->size.x / 50.0f;
    float scaleY = player->size.y / 50.0f;

    outPolys[0] = BuildShipPolygonFromLocal(SHIP_BODY_POINTS, SHIP_BODY_POINT_COUNT, scaleX, scaleY);
    outPolys[1] = BuildShipPolygonFromLocal(SHIP_THRUSTER_POINTS, SHIP_THRUSTER_POINT_COUNT, scaleX, scaleY);

    *outCount = SHIP_POLYGON_COUNT;
}

static Polygone2D BuildShipPolygonFromLocal(const Vector2D* localPoints, int pointCount, float scaleX, float scaleY)
{
    Points* first = NULL;
    Points* prev = NULL;

    for (int i = 0; i < pointCount; ++i)
    {
        Points* node = malloc(sizeof(Points));
        if (node == NULL)
        {
            Points_Free(first);
            Polygone2D empty = { 0 };
            empty.point = NULL;
            return empty;
        }

        Vector2D scaled = Vector2D_SetFromComponents(localPoints[i].x * scaleX, localPoints[i].y * scaleY);
        Vector2D rotated = Vector2D_Rotate(scaled, player->angle, Vector2D_SetFromComponents(0.0f, 0.0f));
        node->coordinate = Vector2D_Add(rotated, player->position);
        node->next = NULL;
        node->prev = prev;

        if (first == NULL)
        {
            first = node;
        }
        else
        {
            prev->next = node;
        }

        prev = node;
    }

    return Polygone2D_SetFromPoints(-1, first);
}

static void FreeShipCollisionPolygons(Polygone2D* polys, int count)
{
    if (polys == NULL) return;
    for (int i = 0; i < count; ++i)
    {
        if (polys[i].point != NULL)
        {
            Points_Free(polys[i].point);
            polys[i].point = NULL;
        }
    }
}

static void HandleEnemyGroupCollision(Enemy* enemies, int count, float renderScale,
    Sphere2D playerSphere, AABB2D playerAABB,
    Polygone2D* shipPolys, int shipPolyCount,
    bool* shipPolyHits, bool* playerSphereBroadHit, bool* playerAABBBroadHit)
{
    for (int i = 0; i < count; ++i)
    {
        ProcessEnemyCollision(&enemies[i], renderScale, playerSphere, playerAABB,
            shipPolys, shipPolyCount, shipPolyHits, playerSphereBroadHit, playerAABBBroadHit);
    }
}

static void ProcessEnemyCollision(Enemy* enemy, float renderScale,
    Sphere2D playerSphere, AABB2D playerAABB,
    Polygone2D* shipPolys, int shipPolyCount,
    bool* shipPolyHits, bool* playerSphereBroadHit, bool* playerAABBBroadHit)
{
    if (enemy->size.x <= 0.0f || enemy->size.y <= 0.0f)
    {
        return;
    }

    float halfWidth = (enemy->size.x * renderScale) * 0.5f;
    float halfHeight = (enemy->size.y * renderScale) * 0.5f;

    Sphere2D enemySphere = Sphere2D_SetFromCenterRadius(
        enemy->position,
        fmaxf(halfWidth, halfHeight)
    );

    Vector2D halfSize = Vector2D_SetFromComponents(halfWidth, halfHeight);
    AABB2D enemyAABB = AABB2D_FromCenterHalfSize(enemy->position, halfSize);

    bool sphereOverlap = Collision_CircleCircle(playerSphere, enemySphere);
    bool aabbOverlap = Collision_AABBAABB(playerAABB, enemyAABB);

    if (sphereOverlap && playerSphereBroadHit != NULL)
    {
        *playerSphereBroadHit = true;
    }
    if (aabbOverlap && playerAABBBroadHit != NULL)
    {
        *playerAABBBroadHit = true;
    }

    if (collisionDebugEnabled)
    {
        DrawSphereOutline(enemySphere, sphereOverlap ? BROAD_PHASE_HIT_COLOR : BROAD_PHASE_COLOR);
        DrawAABBOutline(enemyAABB, aabbOverlap ? BROAD_PHASE_HIT_COLOR : BROAD_PHASE_COLOR);
    }

    if (!(sphereOverlap && aabbOverlap))
    {
        return;
    }

    bool enemyTouched = false;
    for (int i = 0; i < shipPolyCount; ++i)
    {
        if (shipPolys[i].point == NULL) continue;
        bool hit = Collision_CirclePolygon(enemySphere, shipPolys[i]);
        if (hit)
        {
            enemyTouched = true;
            if (shipPolyHits != NULL)
            {
                shipPolyHits[i] = true;
            }
        }
    }

    if (enemyTouched && player->invincibilityFrames <= 0)
    {
        player->invincibilityFrames = 60;
        PlayerTakeDamage();
    }
}

static void DrawPolygonOutline(Polygone2D poly, Color color)
{
    if (poly.point == NULL) return;

    Points* current = poly.point;
    Points* first = poly.point;

    while (current->next != NULL)
    {
        DrawLine((int)current->coordinate.x, (int)current->coordinate.y,
            (int)current->next->coordinate.x, (int)current->next->coordinate.y, color);
        current = current->next;
    }

    DrawLine((int)current->coordinate.x, (int)current->coordinate.y,
        (int)first->coordinate.x, (int)first->coordinate.y, color);
}

static void DrawAABBOutline(AABB2D box, Color color)
{
    DrawRectangleLines(
        (int)box.min.x,
        (int)box.min.y,
        (int)(box.max.x - box.min.x),
        (int)(box.max.y - box.min.y),
        color);
}

static void DrawSphereOutline(Sphere2D sphere, Color color)
{
    DrawCircleLines((int)sphere.center.x, (int)sphere.center.y, (int)sphere.radius, color);
}

static void PlayerTakeDamage(void)
{
    if (lifeNumber > 0)
    {
        lifeNumber--;
        printf("COLLISION DETECTEE ! Vie restante : %d\n", lifeNumber);
    }
    CheckLifeOfPlayer();
}

static void DrawHitboxGroup(Enemy* enemies, int count, Sphere2D(*hitboxFunc)(int), Color color)
{
    if (enemies == NULL || hitboxFunc == NULL) return;

    for (int i = 0; i < count; ++i)
    {
        if (enemies[i].size.x <= 0.0f || enemies[i].size.y <= 0.0f) continue;
        Sphere2D hitbox = hitboxFunc(i);
        DrawCircleLines((int)hitbox.center.x, (int)hitbox.center.y, (int)hitbox.radius, color);
    }
}

static void DrawBulletHitboxes(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (shipBullets[i].active)
        {
            DrawCircle((int)shipBullets[i].position.x, (int)shipBullets[i].position.y,
                (int)shipBullets[i].radius, (Color) { 255, 255, 0, 100 });
            // Contour du cercle
            DrawCircleLines((int)shipBullets[i].position.x, (int)shipBullets[i].position.y,
                (int)shipBullets[i].radius, YELLOW);
        }
    }

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (enemyBullets[i].active)
        {
            DrawCircle((int)enemyBullets[i].position.x, (int)enemyBullets[i].position.y,
                (int)enemyBullets[i].radius, (Color) { 255, 0, 0, 100 });
            DrawCircleLines((int)enemyBullets[i].position.x, (int)enemyBullets[i].position.y,
                (int)enemyBullets[i].radius, RED);
        }
    }
}

void DrawBonusHitboxes(void)
{
    for (int i = 0; i < MAX_BONUS; i++)
    {
        if (bonus[i].active && bonus[i].size.x > 0.0f)
        {
            Sphere2D bonusHitbox = GetBonusHitbox(i);
            if (bonusHitbox.radius > 0.0f)
            {
                DrawCircle((int)bonusHitbox.center.x, (int)bonusHitbox.center.y,
                    (int)bonusHitbox.radius, (Color) { 255, 215, 0, 100 });
                DrawCircleLines((int)bonusHitbox.center.x, (int)bonusHitbox.center.y,
                    (int)bonusHitbox.radius, GOLD);
            }
        }
    }
}


static void DrawArrow(Vector2D start, Vector2D direction, float length, Color color)
{
    float dirNorm = Vector2D_Norm(direction);
    if (dirNorm < 0.0001f) return;

    float nx = direction.x / dirNorm;
    float ny = direction.y / dirNorm;

    float endX = start.x + nx * length;
    float endY = start.y + ny * length;

    DrawLineEx((Vector2) { start.x, start.y }, (Vector2) { endX, endY }, 5.0f, color);
    DrawCircle((int)endX, (int)endY, 8, color);
    DrawCircleLines((int)endX, (int)endY, 8, BLACK);
}

static void DrawShipVectors(void)
{
    if (player == NULL) return;

    float centerX = player->bbox.center.x;
    float centerY = player->bbox.center.y;

    DrawCircle((int)centerX, (int)centerY, 8, WHITE);
    DrawCircleLines((int)centerX, (int)centerY, 8, BLACK);

    float velNorm = Vector2D_Norm(player->velocity);
    if (velNorm > 0.001f)
    {
        float velLength = velNorm * 40.0f;
        if (velLength < 20.0f) velLength = 20.0f;

        float velX = player->velocity.x / velNorm;
        float velY = player->velocity.y / velNorm;

        float endX = centerX + velX * velLength;
        float endY = centerY + velY * velLength;

        DrawLineEx((Vector2) { centerX, centerY }, (Vector2) { endX, endY }, 6.0f, GREEN);
        DrawCircle((int)endX, (int)endY, 10, GREEN);
        DrawCircleLines((int)endX, (int)endY, 10, BLACK);
    }

    float dirX = cosf(player->angle);
    float dirY = sinf(player->angle);
    float dirLength = 80.0f;

    float dirEndX = centerX + dirX * dirLength;
    float dirEndY = centerY + dirY * dirLength;

    DrawLineEx((Vector2) { centerX, centerY }, (Vector2) { dirEndX, dirEndY }, 6.0f, MAGENTA);
    DrawCircle((int)dirEndX, (int)dirEndY, 10, MAGENTA);
    DrawCircleLines((int)dirEndX, (int)dirEndY, 10, BLACK);

    float angleDegrees = player->angle * (180.0f / PI);
    char velText[128];
    char dirText[128];

    snprintf(velText, sizeof(velText), "Vitesse: (%.2f, %.2f) | Norme: %.2f",
        player->velocity.x, player->velocity.y, velNorm);
    DrawText(velText, 10, 130, 18, GREEN);

    // Orientation
    snprintf(dirText, sizeof(dirText), "Orientation: %.2f deg | Direction: (%.2f, %.2f)",
        angleDegrees, dirX, dirY);
    DrawText(dirText, 10, 155, 18, MAGENTA);
}

void SoundInGame(GameAssets* assets) {
    bgMusic = LoadMusicStream("Assets/Sound/SmartSystems.wav");
    titleMusic = LoadMusicStream("Assets/Sound/Title.wav");
    SetSoundVolume(assets->youLose, 0.5f);
    SetSoundVolume(assets->die, 0.4f);
    SetSoundVolume(assets->laser, 0.2f);
    SetSoundVolume(assets->laser2, 1.0f);
    SetSoundVolume(assets->selected, 1.0f);
    SetSoundVolume(assets->damage, 0.4f);
}