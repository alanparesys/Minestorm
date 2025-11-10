#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Vector2D.h"

typedef struct Enemy {
    Vector2D position;
    Vector2D size;
    float angle;                        // en radians
    Color color;
} Enemy;


#endif