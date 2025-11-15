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
	float speed;           // vitesse de déplacement
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
void BigBasicEnemySpawn(int i);
void BigBasicEnemyMovement(int i);

void UpdateMidBasicEnemy(Enemy* enemy, GameAssets* assets);


void UpdateShooterEnemy (Enemy* enemy, GameAssets* assets);
void UpdateBigShooterEnemy(GameAssets* assets);
void BigShooterEnemySpawn(int i);
void BigShooterEnemyMovement(int i);


void UpdateFollowerEnemy(Enemy* enemy, GameAssets* assets);
void UpdateBigFollowerEnemy(GameAssets* assets);
void BigFollowerEnemySpawn(int i);
void BigFollowerEnemyMovement(int i);


void UpdateFollowerShooterEnemy(Enemy* enemy, GameAssets* assets);
void UpdateBigFollowerShooterEnemy(GameAssets* assets);
void BigFollowerShooterEnemySpawn(int i);
void BigFollowerShooterEnemyMovement(int i);


#endif