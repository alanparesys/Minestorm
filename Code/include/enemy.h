#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "struct.h"
#include "sphere2D.h"

// Variables globales ennemis
// =========================== Basics Enemies ===========================
extern int maxBigBasicEnemies;
extern Enemy bigBasicEnemies[3];
extern bool bigBasicEnemiesSpawn[3];
//extern EnemySize currentBasicEnemySize[3];

extern int maxMidBasicEnemies;
extern Enemy midBasicEnemies[6];
extern bool midBasicEnemiesSpawn[6];
//extern EnemySize currentMidBasicEnemySize[6];

extern int maxSmallBasicEnemies;
extern Enemy smallBasicEnemies[12];
extern bool smallBasicEnemiesSpawn[12];
//extern EnemySize currentSmallBasicEnemySize[12];

// =========================== Shooters Enemies ===========================

extern int maxBigShooterEnemy;
extern Enemy bigShooterEnemies[2];
extern bool bigShooterEnemiesSpawn[2];
//extern EnemySize currentShooterEnemySize[2];

extern int maxMidShooterEnemy;
extern Enemy midShooterEnemies[4];
extern bool midShooterEnemiesSpawn[4];
//extern EnemySize currentMidShooterEnemySize[4];

extern int maxSmallShooterEnemy;
extern Enemy smallShooterEnemies[8];
extern bool smallShooterEnemiesSpawn[8];
//extern EnemySize currentSmallShooterEnemySize[8];

// =========================== Followers Enemies ===========================

extern int maxBigFollowerEnemy;
extern Enemy bigFollowerEnemies[2];
extern bool bigFollowerEnemiesSpawn[2];
//extern EnemySize currentFollowerEnemySize[2];

extern int maxMidFollowerEnemy;
extern Enemy midFollowerEnemies[4];
extern bool midFollowerEnemiesSpawn[4];
//extern EnemySize currentMidFollowerEnemySize[4];

extern int maxSmallFollowerEnemy;
extern Enemy smallFollowerEnemies[8];
extern bool smallFollowerEnemiesSpawn[8];
//extern EnemySize currentSmallFollowerEnemySize[8];

// =========================== Follower-Shooters Enemies ===========================

extern int maxBigFollowerShooterEnemy;
extern Enemy bigFollowerShooterEnemies[1];
extern bool bigFollowerShooterEnemiesSpawn[1];
//extern EnemySize currentFollowerShooterEnemySize[1];

extern int maxMidFollowerShooterEnemy;
extern Enemy midFollowerShooterEnemies[2];
extern bool midFollowerShooterEnemiesSpawn[2];
//extern EnemySize currentMidFollowerShooterEnemySize[2];

extern int maxSmallFollowerShooterEnemy;
extern Enemy smallFollowerShooterEnemies[4];
extern bool smallFollowerShooterEnemiesSpawn[4];
//extern EnemySize currentSmallFollowerShooterEnemySize[4];

// ============================ Hitboxes ===========================

Sphere2D GetBigBasicEnemyHitbox(int i);
Sphere2D GetMidBasicEnemyHitbox(int i);
Sphere2D GetSmallBasicEnemyHitbox(int i);

Sphere2D GetBigShooterEnemyHitbox(int i);
Sphere2D GetMidShooterEnemyHitbox(int i);
Sphere2D GetSmallShooterEnemyHitbox(int i);

Sphere2D GetBigFollowerEnemyHitbox(int i);
Sphere2D GetMidFollowerEnemyHitbox(int i);
Sphere2D GetSmallFollowerEnemyHitbox(int i);

Sphere2D GetBigFollowerShooterEnemyHitbox(int i);
Sphere2D GetMidFollowerShooterEnemyHitbox(int i);
Sphere2D GetSmallFollowerShooterEnemyHitbox(int i);

// Fonctions

// =========================== BasicEnemy ===========================
void UpdateBasicEnemy(int i, GameAssets* assets, Collision* collision);

void UpdateBigBasicEnemy(int i, GameAssets* assets, Collision* collision);
void BigBasicEnemySpawn(int i);
void BigBasicEnemyMovement(int i);

void UpdateMidBasicEnemy(int i, GameAssets* assets, Collision* collision);
void MidBasicEnemySpawn(int i, float parentX, float parentY);
void MidBasicEnemyMovement(int i);
void MidBasicEnemyRespawn(int i);

void UpdateSmallBasicEnemy(int i, GameAssets* assets, Collision* collision);
void SmallBasicEnemySpawn(int i, float parentX, float parentY);
void SmallBasicEnemyMovement(int i);
void SmallBasicEnemyRespawn(int i);

// =========================== ShooterEnemy ===========================
void UpdateShooterEnemy(int i, GameAssets* assets, Collision* collision);

void UpdateBigShooterEnemy(int i, GameAssets* assets, Collision* collision);
void BigShooterEnemySpawn(int i);
void BigShooterEnemyMovement(int i);


void UpdateMidShooterEnemy(int i, GameAssets* assets, Collision* collision);
void MidShooterEnemySpawn(int i, float parentX, float parentY);
void MidShooterEnemyMovement(int i);
void MidShooterEnemyRespawn(int i);

void UpdateSmallShooterEnemy(int i, GameAssets* assets, Collision* collision);
void SmallShooterEnemySpawn(int i, float parentX, float parentY);
void SmallShooterEnemyMovement(int i);
void SmallShooterEnemyRespawn(int i);

// =========================== FollowerEnemy ===========================
void UpdateFollowerEnemy(int i, GameAssets* assets, Collision* collision);

void UpdateBigFollowerEnemy(int i, GameAssets* assets, Collision* collision);
void BigFollowerEnemySpawn(int i);
void BigFollowerEnemyMovement(int i);


void UpdateMidFollowerEnemy(int i, GameAssets* assets, Collision* collision);
void MidFollowerEnemySpawn(int i, float parentX, float parentY);
void MidFollowerEnemyMovement(int i);
void MidFollowerEnemyRespawn(int i);

void UpdateSmallFollowerEnemy(int i, GameAssets* assets, Collision* collision);
void SmallFollowerEnemySpawn(int i, float parentX, float parentY);
void SmallFollowerEnemyMovement(int i);
void SmallFollowerEnemyRespawn(int i);

// =========================== Follower-ShooterEnemy ===========================
void UpdateFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision);

void UpdateBigFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision);
void BigFollowerShooterEnemySpawn(int i);
void BigFollowerShooterEnemyMovement(int i);


void UpdateMidFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision);
void MidFollowerShooterEnemySpawn(int i, float parentX, float parentY);
void MidFollowerShooterEnemyMovement(int i);
void MidFollowerShooterEnemyRespawn(int i);

void UpdateSmallFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision);
void SmallFollowerShooterEnemySpawn(int i, float parentX, float parentY);
void SmallFollowerShooterEnemyMovement(int i);
void SmallFollowerShooterEnemyRespawn(int i);

#endif