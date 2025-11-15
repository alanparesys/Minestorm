#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include "struct.h"
#define MAX_BULLETS 100
#define BULLET_RADIUS 5
#define BULLET_SPEED 10.0f

typedef struct ShipBullet {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
} ShipBullet;

extern ShipBullet shipBullets[MAX_BULLETS];

void InitBullets(void);
void FireBullet(Vector2D startPos, float angle);
void UpdateBullets(GameAssets* assets, Collision* collision);

#endif