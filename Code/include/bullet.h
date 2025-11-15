#ifndef BULLET_H
#define BULLET_H

#include "Vector2D.h"
#include "sphere2D.h"
#include "game.h"
#include <stdbool.h>

#define MAX_BULLETS 50
#define BULLET_SPEED 12.0f
#define BULLET_RADIUS 6.0f

typedef struct ShipBullet {
    Vector2D position;
    Vector2D velocity;
    float radius;
    bool active;
} ShipBullet;

extern ShipBullet shipBullets[MAX_BULLETS];

void InitBullets(void);
void FireBullet(Vector2D startPos, float angle);
void UpdateBullets(GameAssets* assets);

#endif // BULLET_H