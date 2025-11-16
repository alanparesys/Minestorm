#define M_PI 3.14159265f

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "enemy.h"
#include "game.h"


const int screenWidth = 1080;
const int screenHeight = 1300;
extern Ship* player;

// --- Variables globales ---
// =========================== Basic Enemies ===========================
int maxBigBasicEnemies = 3;
Enemy bigBasicEnemies[3];
bool bigBasicEnemiesSpawn[3] = { false };
EnemySize currentBasicEnemySize[3] = { BIG, BIG, BIG };

int maxMidBasicEnemies = 6;
Enemy midBasicEnemies[6];
bool midBasicEnemiesSpawn[6] = { false };
EnemySize currentMidBasicEnemySize[6] = { MID, MID, MID, MID, MID, MID };

int maxSmallBasicEnemies = 12;
Enemy smallBasicEnemies[12];
bool smallBasicEnemiesSpawn[12] = { false };
EnemySize currentSmallBasicEnemySize[12] = { SMALL, SMALL, SMALL, SMALL, SMALL, SMALL,
                                               SMALL, SMALL, SMALL, SMALL, SMALL, SMALL };

// =========================== Shooter Enemies ===========================
int maxBigShooterEnemy = 2;
Enemy bigShooterEnemies[2];
bool bigShooterEnemiesSpawn[2] = { false };
EnemySize currentShooterEnemySize[2] = { BIG, BIG };

int maxMidShooterEnemy = 4;
Enemy midShooterEnemies[4];
bool midShooterEnemiesSpawn[4] = { false };
EnemySize currentMidShooterEnemySize[4] = { MID, MID, MID, MID };

int maxSmallShooterEnemy = 8;
Enemy smallShooterEnemies[8];
bool smallShooterEnemiesSpawn[8] = { false };
EnemySize currentSmallShooterEnemySize[8] = { SMALL, SMALL, SMALL, SMALL,
                                                SMALL, SMALL, SMALL, SMALL };

// =========================== Follower Enemies ===========================
int maxBigFollowerEnemy = 2;
Enemy bigFollowerEnemies[2];
bool bigFollowerEnemiesSpawn[2] = { false };
EnemySize currentFollowerEnemySize[2] = { BIG, BIG };

int maxMidFollowerEnemy = 4;
Enemy midFollowerEnemies[4];
bool midFollowerEnemiesSpawn[4] = { false };
EnemySize currentMidFollowerEnemySize[4] = { MID, MID, MID, MID };


int maxSmallFollowerEnemy = 8;
Enemy smallFollowerEnemies[8];
bool smallFollowerEnemiesSpawn[8] = { false };
EnemySize currentSmallFollowerEnemySize[8] = { SMALL, SMALL, SMALL, SMALL,
                                                    SMALL, SMALL, SMALL, SMALL };

// =========================== Follower-Shooter Enemies ===========================

int maxBigFollowerShooterEnemy = 1;
Enemy bigFollowerShooterEnemies[1];
bool bigFollowerShooterEnemiesSpawn[1] = { false };
EnemySize currentFollowerShooterEnemySize[1] = { BIG };

int maxMidFollowerShooterEnemy = 2;
Enemy midFollowerShooterEnemies[2];
bool midFollowerShooterEnemiesSpawn[2] = { false };
EnemySize currentMidFollowerShooterEnemySize[2] = { MID, MID };

int maxSmallFollowerShooterEnemy = 4;
Enemy smallFollowerShooterEnemies[4];
bool smallFollowerShooterEnemiesSpawn[4] = { false };
EnemySize currentSmallFollowerShooterEnemySize[4] = { SMALL, SMALL, SMALL, SMALL };


// ---------------------------------- BasicEnemy ----------------------------------



void UpdateBigBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Spawn initial si nécessaire
    if (!bigBasicEnemiesSpawn[i])
    {
        BigBasicEnemySpawn(i);
        bigBasicEnemiesSpawn[i] = true;
    }

    // Ne rien afficher si mort
    if (bigBasicEnemies[i].size.x <= 0.0f) return;

    BigBasicEnemyMovement(i);

    // Affichage
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { bigBasicEnemies[i].position.x, bigBasicEnemies[i].position.y,
                          bigBasicEnemies[i].size.x * 3.5f, bigBasicEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        bigBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

    // Gestion collision
    if (collision->bigBasicEnemiesBulletCollision[i])
    {
        float x = bigBasicEnemies[i].position.x;
        float y = bigBasicEnemies[i].position.y;

        bigBasicEnemies[i].size.x = 0.0f;
        bigBasicEnemies[i].size.y = 0.0f;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        MidBasicEnemySpawn(i * 2, x, y);
        MidBasicEnemySpawn(i * 2 + 1, x, y);

        collision->bigBasicEnemiesBulletCollision[i] = false;
    }
}

void BigBasicEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigBasicEnemies[i].position.x = -50.0f; bigBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigBasicEnemies[i].position.x = (float)screenWidth + 50.0f; bigBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigBasicEnemies[i].position.y = -50.0f; bigBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigBasicEnemies[i].position.y = (float)screenHeight + 50.0f; bigBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - bigBasicEnemies[i].position.x;
    float dirY = targetY - bigBasicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.5f - 0.5f);
    bigBasicEnemies[i].velocity.x = dirX / dist * speed;
    bigBasicEnemies[i].velocity.y = dirY / dist * speed;

    bigBasicEnemies[i].size.x = 50.0f;
    bigBasicEnemies[i].size.y = 50.0f;

    bigBasicEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigBasicEnemies[i].rotationSpeed *= -1;
}

void BigBasicEnemyMovement(int i)
{
    bigBasicEnemies[i].position.x += bigBasicEnemies[i].velocity.x;
    bigBasicEnemies[i].position.y += bigBasicEnemies[i].velocity.y;

    bigBasicEnemies[i].angle += bigBasicEnemies[i].rotationSpeed;
    if (bigBasicEnemies[i].angle > 2 * M_PI) bigBasicEnemies[i].angle -= 2 * M_PI;
    if (bigBasicEnemies[i].angle < 0) bigBasicEnemies[i].angle += 2 * M_PI;

    int spawnMargin = 150;
    if (bigBasicEnemies[i].position.x < -spawnMargin || bigBasicEnemies[i].position.x > screenWidth + spawnMargin ||
        bigBasicEnemies[i].position.y < -spawnMargin || bigBasicEnemies[i].position.y > screenHeight + spawnMargin)
    {
        BigBasicEnemySpawn(i);
        bigBasicEnemiesSpawn[i] = false;
    }
}

// === Mid Enemy ===

void UpdateMidBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    if (midBasicEnemies[i].size.x <= 0.0f) return;

    MidBasicEnemyMovement(i);

    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 2.0f;  // Moitié de 3.5f
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { midBasicEnemies[i].position.x, midBasicEnemies[i].position.y,
                          midBasicEnemies[i].size.x * scale, midBasicEnemies[i].size.y * scale };
    Vector2 origin = { (midBasicEnemies[i].size.x * scale) / 2.0f, (midBasicEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        midBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

	// Gestion collision
    if (collision->midBasicEnemiesBulletCollision[i])
    {
        float x = midBasicEnemies[i].position.x;
        float y = midBasicEnemies[i].position.y;

        midBasicEnemies[i].size.x = 0.0f;
        midBasicEnemies[i].size.y = 0.0f;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        SmallBasicEnemySpawn(i * 2, x, y);
        SmallBasicEnemySpawn(i * 2 + 1, x, y);

        collision->midBasicEnemiesBulletCollision[i] = false;
    }
}

void MidBasicEnemySpawn(int i, float parentX, float parentY)
{
    midBasicEnemiesSpawn[i] = true;

    // Position de départ = position du parent (là où le big est mort)
    midBasicEnemies[i].position.x = parentX;
    midBasicEnemies[i].position.y = parentY;
    
    // Taille réduite (50% du parent)
    midBasicEnemies[i].size.x = 60.0f;
    midBasicEnemies[i].size.y = 60.0f;

    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;

    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midBasicEnemies[i].velocity.x = cosf(Angle) * speed;
    midBasicEnemies[i].velocity.y = sinf(Angle) * speed;

    

    // Rotation
    midBasicEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midBasicEnemies[i].rotationSpeed *= -1;
}

void MidBasicEnemyMovement(int i)
{
    midBasicEnemies[i].position.x += midBasicEnemies[i].velocity.x;
    midBasicEnemies[i].position.y += midBasicEnemies[i].velocity.y;

    midBasicEnemies[i].angle += midBasicEnemies[i].rotationSpeed;
    if (midBasicEnemies[i].angle > 2 * M_PI) midBasicEnemies[i].angle -= 2 * M_PI;
    if (midBasicEnemies[i].angle < 0) midBasicEnemies[i].angle += 2 * M_PI;

    int spawnMargin = 150;
    if (midBasicEnemies[i].position.x < -spawnMargin || midBasicEnemies[i].position.x > screenWidth + spawnMargin ||
        midBasicEnemies[i].position.y < -spawnMargin || midBasicEnemies[i].position.y > screenHeight + spawnMargin)
    {
        MidBasicEnemyRespawn(i);
        midBasicEnemiesSpawn[i] = false;
    }
}

void MidBasicEnemyRespawn(i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { midBasicEnemies[i].position.x = -50.0f; midBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { midBasicEnemies[i].position.x = (float)screenWidth + 50.0f; midBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { midBasicEnemies[i].position.y = -50.0f; midBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { midBasicEnemies[i].position.y = (float)screenHeight + 50.0f; midBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    
    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - midBasicEnemies[i].position.x;
    float dirY = targetY - midBasicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    
    midBasicEnemies[i].velocity.x = dirX / dist * speed;
	midBasicEnemies[i].velocity.y = dirY / dist * speed;
}

// === Small Enemy ===

void UpdateSmallBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    if (smallBasicEnemies[i].size.x <= 0.0f) return;
    SmallBasicEnemyMovement(i);
    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 1.5f;  // Moitié de 2.0f
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { smallBasicEnemies[i].position.x, smallBasicEnemies[i].position.y,
                          smallBasicEnemies[i].size.x * scale, smallBasicEnemies[i].size.y * scale };
    Vector2 origin = { (smallBasicEnemies[i].size.x * scale) / 2.0f, (smallBasicEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
		smallBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

    // Gestion collision
    if (collision->smallBasicEnemiesBulletCollision[i])
    {
        smallBasicEnemies[i].size.x = 0.0f;
        smallBasicEnemies[i].size.y = 0.0f;

        collision->smallBasicEnemiesBulletCollision[i] = false;
    }
}

void SmallBasicEnemySpawn(int i, float parentX, float parentY)
{
    smallBasicEnemiesSpawn[i] = true;
    // Position de départ = position du parent (là où le mid est mort)
    smallBasicEnemies[i].position.x = parentX;
    smallBasicEnemies[i].position.y = parentY;
    
    // Taille réduite (50% du parent)
    smallBasicEnemies[i].size.x = 50.0f;
    smallBasicEnemies[i].size.y = 50.0f;
    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 2.0f);
    smallBasicEnemies[i].velocity.x = cosf(Angle) * speed;
    smallBasicEnemies[i].velocity.y = sinf(Angle) * speed;
    
    // Rotation
    smallBasicEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallBasicEnemies[i].angle = 0.0f;
	if (GetRandomValue(0, 1) == 0) smallBasicEnemies[i].rotationSpeed *= -1;
}

void SmallBasicEnemyMovement(int i)
{
    smallBasicEnemies[i].position.x += smallBasicEnemies[i].velocity.x;
    smallBasicEnemies[i].position.y += smallBasicEnemies[i].velocity.y;
    smallBasicEnemies[i].angle += smallBasicEnemies[i].rotationSpeed;
    if (smallBasicEnemies[i].angle > 2 * M_PI) smallBasicEnemies[i].angle -= 2 * M_PI;
    if (smallBasicEnemies[i].angle < 0) smallBasicEnemies[i].angle += 2 * M_PI;
    int spawnMargin = 150;
    if (smallBasicEnemies[i].position.x < -spawnMargin || smallBasicEnemies[i].position.x > screenWidth + spawnMargin ||
        smallBasicEnemies[i].position.y < -spawnMargin || smallBasicEnemies[i].position.y > screenHeight + spawnMargin)
    {
        MidBasicEnemyRespawn(i);
        smallBasicEnemiesSpawn[i] = false;
	}
}

void SmallBasicEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { smallBasicEnemies[i].position.x = -50.0f; smallBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { smallBasicEnemies[i].position.x = (float)screenWidth + 50.0f; smallBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { smallBasicEnemies[i].position.y = -50.0f; smallBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { smallBasicEnemies[i].position.y = (float)screenHeight + 50.0f; smallBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - smallBasicEnemies[i].position.x;
    float dirY = targetY - smallBasicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);

    smallBasicEnemies[i].velocity.x = dirX / dist * speed;
    smallBasicEnemies[i].velocity.y = dirY / dist * speed;
}


//////////////////////////////////      ShooterEnemy      ////////////////////////////////////////////////////////////////

// --- ShooterEnemy ---
void UpdateShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    switch (currentShooterEnemySize[i])
    {
    case BIG:
        UpdateBigShooterEnemy(i, assets, collision);
        break;
    case MID:
        break;
    case SMALL:
        break;
    }
}

void UpdateBigShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigShooterEnemiesSpawn[i])
    {
        BigShooterEnemySpawn(i);
        bigShooterEnemiesSpawn[i] = true;
    }

    BigShooterEnemyMovement(i);

    Rectangle sourceRec = { 0, 0, assets->shooterEnemyTexture.width, assets->shooterEnemyTexture.height };
    Rectangle destRec = { bigShooterEnemies[i].position.x, bigShooterEnemies[i].position.y,
                          bigShooterEnemies[i].size.x * 3.5f, bigShooterEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

    DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin,
        bigShooterEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->bigShooterEnemiesBulletCollision[i])
    {

        currentShooterEnemySize[i] = MID;
        collision->bigShooterEnemiesBulletCollision[i] = false;
    }
}

void BigShooterEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigShooterEnemies[i].position.x = -50.0f; bigShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigShooterEnemies[i].position.x = (float)screenWidth + 50.0f; bigShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigShooterEnemies[i].position.y = -50.0f; bigShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigShooterEnemies[i].position.y = (float)screenHeight + 50.0f; bigShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - bigShooterEnemies[i].position.x;
    float dirY = targetY - bigShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.5f - 0.5f);
    bigShooterEnemies[i].velocity.x = dirX / dist * speed;
    bigShooterEnemies[i].velocity.y = dirY / dist * speed;

    bigShooterEnemies[i].size.x = 50.0f;
    bigShooterEnemies[i].size.y = 50.0f;

    bigShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigShooterEnemies[i].rotationSpeed *= -1;
}

void BigShooterEnemyMovement(int i)
{
    bigShooterEnemies[i].position.x += bigShooterEnemies[i].velocity.x;
    bigShooterEnemies[i].position.y += bigShooterEnemies[i].velocity.y;

    bigShooterEnemies[i].angle += bigShooterEnemies[i].rotationSpeed;
    if (bigShooterEnemies[i].angle > 2 * M_PI) bigShooterEnemies[i].angle -= 2 * M_PI;
    if (bigShooterEnemies[i].angle < 0) bigShooterEnemies[i].angle += 2 * M_PI;

    int spawnMargin = 150;
    if (bigShooterEnemies[i].position.x < -spawnMargin || bigShooterEnemies[i].position.x > screenWidth + spawnMargin ||
        bigShooterEnemies[i].position.y < -spawnMargin || bigShooterEnemies[i].position.y > screenHeight + spawnMargin)
    {
        BigShooterEnemySpawn(i);
        bigShooterEnemiesSpawn[i] = false;
    }
}


//////////////////////////////////      FollowerEnemy      ////////////////////////////////////////////////////////////////
// --- FollowerEnemy ---
void UpdateFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    switch (currentFollowerEnemySize[i])
    {
    case BIG:
        UpdateBigFollowerEnemy(i, assets, collision);
        break;
    case MID:
        break;
    case SMALL:
        break;
    }
}

void UpdateBigFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigFollowerEnemiesSpawn[i])
    {
        BigFollowerEnemySpawn(i);
        bigFollowerEnemiesSpawn[i] = true;
    }

    BigFollowerEnemyMovement(i);

    Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
    Rectangle destRec = { bigFollowerEnemies[i].position.x, bigFollowerEnemies[i].position.y,
                          bigFollowerEnemies[i].size.x * 3.5f, bigFollowerEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
        bigFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->bigFollowerEnemiesBulletCollision[i])
    {
        currentFollowerEnemySize[i] = MID;
        collision->bigFollowerEnemiesBulletCollision[i] = false;
    }
}

void BigFollowerEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigFollowerEnemies[i].position.x = -50.0f; bigFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigFollowerEnemies[i].position.x = (float)screenWidth + 50.0f; bigFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigFollowerEnemies[i].position.y = -50.0f; bigFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigFollowerEnemies[i].position.y = (float)screenHeight + 50.0f; bigFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    bigFollowerEnemies[i].speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.0f - 1.0f);
    bigFollowerEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigFollowerEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigFollowerEnemies[i].rotationSpeed *= -1;

    bigFollowerEnemies[i].size.x = 50.0f;
    bigFollowerEnemies[i].size.y = 50.0f;
}

void BigFollowerEnemyMovement(int i)
{
    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - bigFollowerEnemies[i].position.x;
    float dirY = targetY - bigFollowerEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    bigFollowerEnemies[i].velocity.x = dirX / dist * bigFollowerEnemies[i].speed;
    bigFollowerEnemies[i].velocity.y = dirY / dist * bigFollowerEnemies[i].speed;

    bigFollowerEnemies[i].position.x += bigFollowerEnemies[i].velocity.x;
    bigFollowerEnemies[i].position.y += bigFollowerEnemies[i].velocity.y;

    bigFollowerEnemies[i].angle += bigFollowerEnemies[i].rotationSpeed;
    if (bigFollowerEnemies[i].angle > 2 * M_PI) bigFollowerEnemies[i].angle -= 2 * M_PI;
    if (bigFollowerEnemies[i].angle < 0) bigFollowerEnemies[i].angle += 2 * M_PI;

    int spawnMargin = 150;
    if (bigFollowerEnemies[i].position.x < -spawnMargin || bigFollowerEnemies[i].position.x > screenWidth + spawnMargin ||
        bigFollowerEnemies[i].position.y < -spawnMargin || bigFollowerEnemies[i].position.y > screenHeight + spawnMargin)
    {
        BigFollowerEnemySpawn(i);
        bigFollowerEnemiesSpawn[i] = false;
    }
}


//////////////////////////////////      FollowerShooterEnemy      ////////////////////////////////////////////////////////////////

// --- FollowerShooterEnemy ---
void UpdateFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    switch (currentFollowerShooterEnemySize[i])
    {
    case BIG:
        UpdateBigFollowerShooterEnemy(i, assets, collision);
        break;
    case MID:
        break;
    case SMALL:
        break;
    }
}

void UpdateBigFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigFollowerShooterEnemiesSpawn[i])
    {
        BigFollowerShooterEnemySpawn(i);
        bigFollowerShooterEnemiesSpawn[i] = true;
    }

    BigFollowerShooterEnemyMovement(i);

    Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
    Rectangle destRec = { bigFollowerShooterEnemies[i].position.x, bigFollowerShooterEnemies[i].position.y,
                          bigFollowerShooterEnemies[i].size.x * 3.5f, bigFollowerShooterEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

    DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin,
        bigFollowerShooterEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->bigFollowerShooterEnemiesBulletCollision[i])
    {
        currentFollowerShooterEnemySize[i] = MID;
        collision->bigFollowerShooterEnemiesBulletCollision[i] = false;
    }
}

void BigFollowerShooterEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigFollowerShooterEnemies[i].position.x = -50.0f; bigFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigFollowerShooterEnemies[i].position.x = (float)screenWidth + 50.0f; bigFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigFollowerShooterEnemies[i].position.y = -50.0f; bigFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigFollowerShooterEnemies[i].position.y = (float)screenHeight + 50.0f; bigFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    bigFollowerShooterEnemies[i].speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.5f - 0.5f);
    bigFollowerShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigFollowerShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigFollowerShooterEnemies[i].rotationSpeed *= -1;
    bigFollowerShooterEnemies[i].size.x = 50.0f;
    bigFollowerShooterEnemies[i].size.y = 50.0f;
}

void BigFollowerShooterEnemyMovement(int i)
{
    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - bigFollowerShooterEnemies[i].position.x;
    float dirY = targetY - bigFollowerShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    bigFollowerShooterEnemies[i].velocity.x = dirX / dist * bigFollowerShooterEnemies[i].speed;
    bigFollowerShooterEnemies[i].velocity.y = dirY / dist * bigFollowerShooterEnemies[i].speed;

    bigFollowerShooterEnemies[i].position.x += bigFollowerShooterEnemies[i].velocity.x;
    bigFollowerShooterEnemies[i].position.y += bigFollowerShooterEnemies[i].velocity.y;

    bigFollowerShooterEnemies[i].angle += bigFollowerShooterEnemies[i].rotationSpeed;
    if (bigFollowerShooterEnemies[i].angle > 2 * M_PI) bigFollowerShooterEnemies[i].angle -= 2 * M_PI;
    if (bigFollowerShooterEnemies[i].angle < 0) bigFollowerShooterEnemies[i].angle += 2 * M_PI;

    int spawnMargin = 150;
    if (bigFollowerShooterEnemies[i].position.x < -spawnMargin || bigFollowerShooterEnemies[i].position.x > screenWidth + spawnMargin ||
        bigFollowerShooterEnemies[i].position.y < -spawnMargin || bigFollowerShooterEnemies[i].position.y > screenHeight + spawnMargin)
    {
        BigFollowerShooterEnemySpawn(i);
        bigFollowerShooterEnemiesSpawn[i] = false;
    }
}
