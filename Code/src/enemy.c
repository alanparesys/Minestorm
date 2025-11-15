#define M_PI 3.14159265f

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "enemy.h"
#include "game.h"

// Dimensions de l'écran
const int screenWidth = 1080;
const int screenHeight = 1300;

Ship* player;


//////////////////////////////////      BasicEnemy      ////////////////////////////////////////////////////////////////

int maxBigBasicEnemies = 3;

// Tableau des ennemis
Enemy bigBasicEnemies[3];
bool bigBasicEnemiesSpawn[3] = { false };
bool bigBasicEnemiesCollision[3] = { false };


// Taille actuelle des ennemis
EnemySize currentBasicEnemySize[3] = { BIG, BIG, BIG };

// Mise à jour de tous les ennemis
void UpdateBasicEnemy(Enemy* enemy, GameAssets* assets)
{
    for (int i = 0; i < maxBigBasicEnemies; i++)
    {
        switch (currentBasicEnemySize[i])
        {
        case BIG:
            UpdateBigBasicEnemy(assets);
            break;
        case MID:
            UpdateMidBasicEnemy(enemy, assets);
            break;
        case SMALL:
            // UpdateSmallBasicEnemy(enemy, assets);
            break;
        default:
            break;
        }
    }
}

// BIG
// 
// Mise à jour des ennemis BIG
void UpdateBigBasicEnemy(GameAssets* assets)
{
    for (int i = 0; i < maxBigBasicEnemies; i++)
    {
        if (!bigBasicEnemiesSpawn[i])
        {
            BigBasicEnemySpawn(i);
            bigBasicEnemiesSpawn[i] = true;
        }
    }

    int i;
    for (i = 0; i < maxBigBasicEnemies; i++)
    {
        BigBasicEnemyMovement(i);

        // portion de la texture (toute la texture)
        Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };

        // rectangle de destination (position et taille finale)
        Rectangle destRec = { bigBasicEnemies[i].position.x, bigBasicEnemies[i].position.y, bigBasicEnemies[i].size.x * 3.5f, bigBasicEnemies[i].size.y * 3.5f };

        // centre de rotation = centre du rectangle destination
        Vector2 origin = { (bigBasicEnemies[i].size.x * 3.5f) / 2, (bigBasicEnemies[i].size.y * 3.5f) / 2 };

        // dessiner la texture
        DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin, bigBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

        if (bigBasicEnemiesCollision[i] == true)
        {
            currentBasicEnemySize[i] = MID;
        }
    }
}

// Respawn d’un seul ennemi
void BigBasicEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigBasicEnemies[i].position.x = -50; bigBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigBasicEnemies[i].position.x = screenWidth + 50; bigBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigBasicEnemies[i].position.y = -50; bigBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigBasicEnemies[i].position.y = screenHeight + 50; bigBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - bigBasicEnemies[i].position.x;
    float dirY = targetY - bigBasicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    // vitesse
    float speed = 0.2f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.0f - 0.2f);
    bigBasicEnemies[i].velocity.x = dirX / dist * speed;
    bigBasicEnemies[i].velocity.y = dirY / dist * speed;

    bigBasicEnemies[i].size.x = 50.0f;
    bigBasicEnemies[i].size.y = 50.0f;

    bigBasicEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigBasicEnemies[i].angle = 0;
    if (GetRandomValue(0, 1) == 0)
    {
        bigBasicEnemies[i].rotationSpeed *= -1; 
    }
}

// Déplacement de l’ennemi et respawn si hors écran
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

// MID

void UpdateMidBasicEnemy(Enemy* enemy, GameAssets* assets)
{

}


//////////////////////////////////      ShooterEnemy      ////////////////////////////////////////////////////////////////

Enemy bigShooterEnemies[2];
int maxBigShooterEnemy = 2;
bool bigShooterEnemiesSpawn[2] = { false };
bool bigShooterEnemiesCollision[2] = { false };



EnemySize currentShooterEnemySize[2] = { BIG, BIG };

void UpdateShooterEnemy(Enemy* enemy, GameAssets* assets)
{
    for (int i = 0; i < maxBigShooterEnemy; i++)
    {
        switch (currentShooterEnemySize[i])
        {
        case BIG:
            UpdateBigShooterEnemy(assets);
            break;
        case MID:
            // UpdateMidShooterEnemy(enemy, assets);
            break;
        case SMALL:
            // UpdateSmallShooterEnemy(enemy, assets);
            break;
        default:
            break;
        }
    }
}


// BIG

// Mise à jour des ennemis BIG
void UpdateBigShooterEnemy(GameAssets* assets)
{

    for (int i = 0; i < maxBigShooterEnemy; i++)
    {
        if (!bigShooterEnemiesSpawn[i])
        {
            BigShooterEnemySpawn(i);
            bigShooterEnemiesSpawn[i] = true;
        }
        BigShooterEnemyMovement(i);

        // portion de la texture (toute la texture)
        Rectangle sourceRec = { 0, 0, assets->shooterEnemyTexture.width, assets->shooterEnemyTexture.height };

        // rectangle de destination (position et taille finale)
        Rectangle destRec = { bigShooterEnemies[i].position.x, bigShooterEnemies[i].position.y, bigShooterEnemies[i].size.x * 3.5f, bigShooterEnemies[i].size.y * 3.5f };

        // centre de rotation = centre du rectangle destination
        Vector2 origin = { (bigShooterEnemies[i].size.x * 3.5f) / 2, (bigShooterEnemies[i].size.y * 3.5f) / 2 };

        // dessiner la texture
        DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin, bigShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
        if (bigShooterEnemiesCollision[i] == true)
        {
            currentShooterEnemySize[i] = MID;
        }
    }
}

// Respawn d’un seul ennemi
void BigShooterEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1)
    {
        bigShooterEnemies[i].position.x = -50;
        bigShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight);
    }
    if (side == 2)
    {
        bigShooterEnemies[i].position.x = screenWidth + 50;
        bigShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight);
    }
    if (side == 3)
    {
        bigShooterEnemies[i].position.y = -50;
        bigShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth);
    }
    if (side == 4)
    {
        bigShooterEnemies[i].position.y = screenHeight + 50;
        bigShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth);
    }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - bigShooterEnemies[i].position.x;
    float dirY = targetY - bigShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    float speed = 0.2f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.0f - 0.2f);
    bigShooterEnemies[i].velocity.x = dirX / dist * speed;
    bigShooterEnemies[i].velocity.y = dirY / dist * speed;

    bigShooterEnemies[i].size.x = 50.0f;
    bigShooterEnemies[i].size.y = 50.0f;

    bigShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);

    bigShooterEnemies[i].angle = 0;
    if (GetRandomValue(0, 1) == 0)
    {
        bigShooterEnemies[i].rotationSpeed *= -1;
    }
}

// Déplacement de l’ennemi et respawn si hors écran
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

Enemy bigFollowerEnemies[2];
int maxBigFollowerEnemy = 2;
bool bigFollowerEnemiesSpawn[2] = { false };
bool bigFollowerEnemiesCollision[2] = { false };

EnemySize currentFollowerEnemySize[2] = { BIG, BIG };


void UpdateFollowerEnemy(Enemy* enemy, GameAssets* assets)
{
    for (int i = 0; i < maxBigFollowerEnemy; i++)
    {
        switch (currentFollowerEnemySize[i])
        {
        case BIG:
            UpdateBigFollowerEnemy(assets);
            break;
        case MID:
            // UpdateMidFollowerEnemy(enemy, assets);
            break;
        case SMALL:
            // UpdateSmallFollowerEnemy(enemy, assets);
            break;
        default:
            break;
        }
    }
}

// BIG

void UpdateBigFollowerEnemy(GameAssets* assets)
{
    for (int i = 0; i < maxBigFollowerEnemy; i++)
    {
        if (!bigFollowerEnemiesSpawn[i])
        {
            BigFollowerEnemySpawn(i);
            bigFollowerEnemiesSpawn[i] = true;
        }
        BigFollowerEnemyMovement(i);

        Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
        Rectangle destRec = {
            bigFollowerEnemies[i].position.x,
            bigFollowerEnemies[i].position.y,
            bigFollowerEnemies[i].size.x * 3.5f,
            bigFollowerEnemies[i].size.y * 3.5f
        };
        Vector2 origin = {
            (bigFollowerEnemies[i].size.x * 3.5f) / 2,
            (bigFollowerEnemies[i].size.y * 3.5f) / 2
        };

        DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
            bigFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);
        if (bigFollowerEnemiesCollision[i] == true)
        {
            currentFollowerEnemySize[i] = MID;
        }
    }
}

void BigFollowerEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigFollowerEnemies[i].position.x = -50; bigFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    else if (side == 2) { bigFollowerEnemies[i].position.x = screenWidth + 50; bigFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    else if (side == 3) { bigFollowerEnemies[i].position.y = -50; bigFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    else if (side == 4) { bigFollowerEnemies[i].position.y = screenHeight + 50; bigFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    bigFollowerEnemies[i].speed = 0.8f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.8f - 0.1f);
    bigFollowerEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.06f - 0.03f);
    bigFollowerEnemies[i].angle = 0;
    if (GetRandomValue(0, 1) == 0) bigFollowerEnemies[i].rotationSpeed *= -1;

    // taille de l’ennemi
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

Enemy bigFollowerShooterEnemies[1];
int maxBigFollowerShooterEnemy = 1;
bool bigFollowerShooterEnemiesSpawn[1] = { false };
bool bigFollowerShooterEnemiesCollision[1] = { false };

EnemySize currentFollowerShooterEnemySize[1] = { BIG };



void UpdateFollowerShooterEnemy(Enemy* enemy, GameAssets* assets)
{
    for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
    {
        switch (currentFollowerShooterEnemySize[i])
        {
        case BIG:
            UpdateBigFollowerShooterEnemy(assets);
            break;
        case MID:
            // UpdateMidFollowerShooterEnemy(enemy, assets);
            break;
        case SMALL:
            // UpdateSmallFollowerShooterEnemy(enemy, assets);
            break;
        default:
            break;
        }
    }
}

// BIG

void UpdateBigFollowerShooterEnemy(GameAssets* assets)
{
    for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
    {
        if (!bigFollowerShooterEnemiesSpawn[i])
        {
            BigFollowerShooterEnemySpawn(i);
            bigFollowerShooterEnemiesSpawn[i] = true;
        }

        BigFollowerShooterEnemyMovement(i);

        Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
        Rectangle destRec = {
            bigFollowerShooterEnemies[i].position.x,
            bigFollowerShooterEnemies[i].position.y,
            bigFollowerShooterEnemies[i].size.x * 3.5f,
            bigFollowerShooterEnemies[i].size.y * 3.5f
        };
        Vector2 origin = {
            (bigFollowerShooterEnemies[i].size.x * 3.5f) / 2,
            (bigFollowerShooterEnemies[i].size.y * 3.5f) / 2
        };
        DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin,
            bigFollowerShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
        if (bigFollowerShooterEnemiesCollision[i] == true)
        {
            currentFollowerShooterEnemySize[i] = MID;
        }
    }
}

void BigFollowerShooterEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigFollowerShooterEnemies[i].position.x = -50; bigFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    else if (side == 2) { bigFollowerShooterEnemies[i].position.x = screenWidth + 50; bigFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    else if (side == 3) { bigFollowerShooterEnemies[i].position.y = -50; bigFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    else if (side == 4) { bigFollowerShooterEnemies[i].position.y = screenHeight + 50; bigFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    bigFollowerShooterEnemies[i].speed = 0.8f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.8f - 0.1f);
    bigFollowerShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    

    bigFollowerShooterEnemies[i].angle = 0;

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
