#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Vector2D.h"
#include "game.h"

typedef struct Enemy
{
    Vector2D position;
    Vector2D size;
    Vector2D velocity;
    Vector2D target;       // point cible aléatoire
    float angle;           // orientation du sprite
    float rotationSpeed;   // rotation sur lui-même
} Enemy;

typedef enum EnemySize
{
    BIG,
    MID,
    SMALL,
} EnemySize;

typedef struct GameAssets GameAssets; // Déclaration anticipée

// Fonctions
void UpdateBasicEnemy(Enemy* enemy, GameAssets* assets);
void UpdateBigBasicEnemy(GameAssets* assets);
void BasicEnemySpawn(int i);
void BasicEnemyMovement(int i);

#endif