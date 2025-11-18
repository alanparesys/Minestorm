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

typedef struct MotherShip
{
    Vector2D position;
    Vector2D size;
    Vector2D velocity;
    float angle;
    float rotationSpeed;
    float speed;
    int health;
} MotherShip;

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
    bool midBasicEnemiesBulletCollision[6];
    bool midShooterEnemiesBulletCollision[4];
    bool midFollowerEnemiesBulletCollision[4];
    bool midFollowerShooterEnemiesBulletCollision[2];
    bool smallBasicEnemiesBulletCollision[12];
    bool smallShooterEnemiesBulletCollision[8];
    bool smallFollowerEnemiesBulletCollision[8];
    bool smallFollowerShooterEnemiesBulletCollision[4];
} Collision;

// Structure GameAssets
typedef struct GameAssets
{
    Texture2D background;
    Texture2D interface;
    Texture2D minestorm;
    Texture2D titleText;
    Texture2D ship;
    Texture2D bulletTexture;
    Texture2D basicEnemyTexture;
    Texture2D shooterEnemyTexture;
    Texture2D followerEnemyTexture;
    Texture2D followerShooterEnemyTexture;
    Texture2D motherShipTexture;
    Texture2D explosionTexture;
    Font pixelFont;
    Font magnetoFont;
    Sound explosionSound;
    Sound youLose;
    Sound die;
    Sound laser;
    Sound laser2;
    Sound selected;
    Sound damage;
    Sound levelUp;
} GameAssets;

#endif
