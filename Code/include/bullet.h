#ifndef BULLET_H
#define BULLET_H
#include "raylib.h"
#include "struct.h"
#include "Vector2D.h"
#include "sphere2D.h"

#define MAX_BULLETS 100
#define MAX_ENEMY_BULLETS 50
#define BULLET_RADIUS 5
#define BULLET_SPEED 10.0f
#define ENEMY_BULLET_SPEED 6.0f

typedef struct ShipBullet {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
} ShipBullet;

typedef struct EnemyBullet {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
} EnemyBullet;

// Variables globales
extern int score;
extern int actualLevel;
extern int lifeNumber;

extern ShipBullet shipBullets[MAX_BULLETS];
extern EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];

// Fonctions pour les bullets du joueur
void InitBullets(void);
void FireBullet(Vector2D startPos, float angle);
void UpdateBullets(GameAssets* assets, Collision* collision);

// Fonctions pour les bullets des ennemis
void InitEnemyBullets(void);
void FireEnemyBullet(Vector2D startPos, Vector2D targetPos);
void UpdateEnemyBullets(GameAssets* assets, Ship* player);

// Fonctions utilitaires
bool CheckCollisionShipEnemy(Rectangle2D shipBox, Sphere2D enemySphere);

#endif