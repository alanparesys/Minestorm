#define M_PI 3.14159265f

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "enemy.h"
#include "game.h"

// Dimensions de l'écran
const int screenWidth = 1080;
const int screenHeight = 1300;

// Tableau des ennemis
Enemy basicEnemies[3];
int maxBasicEnemy = 3;
bool bigBasicEnemiesSpawn = false;
bool bigBasicEnemiesColision = false;

// Taille actuelle des ennemis
EnemySize currentBasicEnemySize = BIG;

// Mise à jour de tous les ennemis
void UpdateBasicEnemy(Enemy* enemy, GameAssets* assets)
{
    switch (currentBasicEnemySize)
    {
    case BIG:
        UpdateBigBasicEnemy(assets);
        break;
    case MID:
        // UpdateMidBasicEnemy(enemy, assets);
        break;
    case SMALL:
        // UpdateSmallBasicEnemy(enemy, assets);
        break;
    default:
        break;
    }
}

// Mise à jour des ennemis BIG
void UpdateBigBasicEnemy(GameAssets* assets)
{
    // Spawn une seule fois
    if (!bigBasicEnemiesSpawn)
    {
        int i;
        for (i = 0; i < maxBasicEnemy; i++)
            BasicEnemySpawn(i);
        bigBasicEnemiesSpawn = true;
    }

    int i;
    for (i = 0; i < maxBasicEnemy; i++)
    {
        BasicEnemyMovement(i);

        // portion de la texture (toute la texture)
        Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };

        // rectangle de destination (position et taille finale)
        Rectangle destRec = { basicEnemies[i].position.x, basicEnemies[i].position.y, basicEnemies[i].size.x * 3.5f, basicEnemies[i].size.y * 3.5f };

        // centre de rotation = centre du rectangle destination
        Vector2 origin = { (basicEnemies[i].size.x * 3.5f) / 2, (basicEnemies[i].size.y * 3.5f) / 2 };

        // dessiner la texture
        DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin, basicEnemies[i].angle * 180.0f / M_PI, WHITE);
        /*
        if (bigBasicEnemiesColision = true)
        {
            bigBasicEnemiesSpawn = false;
        }
        */
    }
}

// Respawn d’un seul ennemi
void BasicEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { basicEnemies[i].position.x = -50; basicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { basicEnemies[i].position.x = screenWidth + 50; basicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { basicEnemies[i].position.y = -50; basicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { basicEnemies[i].position.y = screenHeight + 50; basicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - basicEnemies[i].position.x;
    float dirY = targetY - basicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 1.0f);
    basicEnemies[i].velocity.x = dirX / dist * speed;
    basicEnemies[i].velocity.y = dirY / dist * speed;

    basicEnemies[i].size.x = 50.0f;
    basicEnemies[i].size.y = 50.0f;

    basicEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    if (GetRandomValue(0, 1) == 0)
        basicEnemies[i].rotationSpeed *= -1;
    basicEnemies[i].angle = 0;
}

// Déplacement de l’ennemi et respawn si hors écran
void BasicEnemyMovement(int i)
{
    basicEnemies[i].position.x += basicEnemies[i].velocity.x;
    basicEnemies[i].position.y += basicEnemies[i].velocity.y;

    basicEnemies[i].angle += basicEnemies[i].rotationSpeed;
    if (basicEnemies[i].angle > 2 * M_PI) basicEnemies[i].angle -= 2 * M_PI;
    if (basicEnemies[i].angle < 0) basicEnemies[i].angle += 2 * M_PI;

    int spawnMargin = 150;
    if (basicEnemies[i].position.x < -spawnMargin || basicEnemies[i].position.x > screenWidth + spawnMargin ||
        basicEnemies[i].position.y < -spawnMargin || basicEnemies[i].position.y > screenHeight + spawnMargin)
    {
        BasicEnemySpawn(i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Enemy shooterEnemies[3];
int maxShooterEnemy = 2;
bool bigShooterEnemiesSpawn = false;
bool bigShooterEnemiesColision = false;

EnemySize currentShooterEnemySize = BIG;

void UpdateShooterEnemy(Enemy* enemy, GameAssets* assets)
{
    switch (currentShooterEnemySize)
    {
    case BIG:
        UpdateBigShooterEnemy(assets);
        break;
    case MID:
        // UpdateMidBasicEnemy(enemy, assets);
        break;
    case SMALL:
        // UpdateSmallBasicEnemy(enemy, assets);
        break;
    default:
        break;
    }
}


UpdateBigShooterEnemy(GameAssets* assets)
*/