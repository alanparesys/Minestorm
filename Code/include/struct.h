#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include "Vector2D.h"
#include "rectangle2D.h"

// Structure Ship
typedef struct Ship {
    Vector2D position;
    Vector2D size;
    Vector2D velocity;
    float angle;    // radiant
    Color color;
    Rectangle2D bbox;        // bounding box of the ship
	float invincibilityFrames; // duration of invincibility after being hit
} Ship;

// Structure Enemy
typedef struct Enemy
{
    Vector2D position;
    Vector2D size;
    Vector2D velocity;
    Vector2D target;
    float angle;
    float rotationSpeed;
    float speed;
} Enemy;

// Enum EnemySize
typedef enum EnemySize
{
    BIG,
    MID,
    SMALL,
} EnemySize;

// Structure Collision (UNE SEULE FOIS)
typedef struct Collision
{
    bool bigBasicEnemiesBulletCollision[3];
    bool bigShooterEnemiesBulletCollision[2];
    bool bigFollowerEnemiesBulletCollision[2];
    bool bigFollowerShooterEnemiesBulletCollision[1];
    bool midBasicEnemiesBulletCollision[3];
    bool midShooterEnemiesBulletCollision[2];
    bool midFollowerEnemiesBulletCollision[2];
    bool midFollowerShooterEnemiesBulletCollision[1];
    bool smallBasicEnemiesBulletCollision[3];
    bool smallShooterEnemiesBulletCollision[2];
    bool smallFollowerEnemiesBulletCollision[2];
    bool smallFollowerShooterEnemiesBulletCollision[1];
} Collision;

// Structure GameAssets
typedef struct GameAssets
{
    Texture2D background;
    Texture2D interface;
    Texture2D ship;
    Texture2D bulletTexture;
    Texture2D basicEnemyTexture;
    Texture2D shooterEnemyTexture;
    Texture2D followerEnemyTexture;
    Texture2D followerShooterEnemyTexture;
    Font font;
} GameAssets;

#endif