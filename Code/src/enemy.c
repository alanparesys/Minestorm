#define M_PI 3.14159265f

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "enemy.h"
#include "game.h"

const int screenWidth = 1080;
const int screenHeight = 1300;

Enemy basicEnemies[3];
int basicEnemyCount = 3;
bool enemiesSpawned = false;
EnemySize currentBasicEnemySize = BIG;

// Mettre à jour tous les ennemis
void UpdateBasicEnemy(Enemy* dummy, GameAssets* assets)
{
    switch (currentBasicEnemySize)
    {
    case BIG:
        UpdateBigBasicEnemy(assets);
        break;
    case MID:
        // UpdateMidBasicEnemy(dummy, assets);
        break;
    case SMALL:
        // UpdateSmallBasicEnemy(dummy, assets);
        break;
    default:
        break;
    }
}

// Mettre à jour les ennemis BIG
void UpdateBigBasicEnemy(GameAssets* assets)
{
    if (!enemiesSpawned)
    {
        BasicEnemySpawn();
        enemiesSpawned = true;
    }

    for (int i = 0; i < basicEnemyCount; i++)
    {
        BasicEnemyMovement(&basicEnemies[i]);
        DrawTextureEx(
            assets->basicEnemyTexture,
            (Vector2) {
            basicEnemies[i].position.x, basicEnemies[i].position.y
        },
            basicEnemies[i].angle * 180.0f / M_PI,
            2.5f, WHITE
        );
    }
}

// Spawn des ennemis
void BasicEnemySpawn(void)
{
    for (int i = 0; i < basicEnemyCount; i++)
    {
        int side = GetRandomValue(1, 4);
        if (side == 1) { basicEnemies[i].position.x = -50; basicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
        if (side == 2) { basicEnemies[i].position.x = screenWidth + 50; basicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
        if (side == 3) { basicEnemies[i].position.y = -50; basicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
        if (side == 4) { basicEnemies[i].position.y = screenHeight + 50; basicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

        basicEnemies[i].target.x = (float)GetRandomValue(50, screenWidth - 50);
        basicEnemies[i].target.y = (float)GetRandomValue(50, screenHeight - 50);

        basicEnemies[i].size.x = 50.0f;
        basicEnemies[i].size.y = 50.0f;

        basicEnemies[i].rotationSpeed = ((float)GetRandomValue(-50, 50)) / 100.0f;
        basicEnemies[i].angle = 0.0f;
    }
}

// Déplacement et rotation de l’ennemi
void BasicEnemyMovement(Enemy* enemy)
{
    float dirX = enemy->target.x - enemy->position.x;
    float dirY = enemy->target.y - enemy->position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    float speed = 2.0f; // ajustable
    if (dist > 1.0f)
    {
        enemy->velocity.x = dirX / dist * speed;
        enemy->velocity.y = dirY / dist * speed;

        enemy->position.x += enemy->velocity.x;
        enemy->position.y += enemy->velocity.y;

        enemy->angle = atan2f(enemy->velocity.y, enemy->velocity.x);
    }
    else
    {
        enemy->target.x = (float)GetRandomValue(50, screenWidth - 50);
        enemy->target.y = (float)GetRandomValue(50, screenHeight - 50);
    }

    enemy->angle += enemy->rotationSpeed;
}

// grossir l'enemi
// angle d'apparition
// faire tourner l'enemi
// moyenne et petite taille
// autre type d'enemis
// hitbox
// respawner les ennemis quand ils sortent de l'ecran avec hitbox de l'ecran
