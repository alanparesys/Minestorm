#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include "Vector2D.h"
#include "rectangle2D.h"

// Represents the player's ship
typedef struct Ship {
    Vector2D position;             // Ship's world position
    Vector2D size;                 // Ship dimensions
    Vector2D velocity;             // Movement velocity
    float angle;                   // Rotation angle in radians
    Color color;                   // Ship color (unused or for debug)
    Rectangle2D bbox;              // Ship's bounding box
    float invincibilityFrames;     // Timer for damage invincibility
} Ship;

// Basic enemy structure
typedef struct Enemy
{
    Vector2D position;             // Enemy world position
    Vector2D size;                 // Enemy size
    Vector2D velocity;             // Current movement vector
    Vector2D target;               // Target position (player or destination)
    float angle;                   // Visual or movement rotation
    float rotationSpeed;           // Rotation speed
    float speed;                   // Base movement speed
} Enemy;

// Stronger enemy variant with health and similar movement properties
typedef struct MotherShip
{
    Vector2D position;             // Mothership position
    Vector2D size;                 // Mothership size
    Vector2D velocity;             // Movement vector
    float angle;                   // Current rotation
    float rotationSpeed;           // Rotation speed
    float speed;                   // Movement speed
    int health;                    // Total hit points
} MotherShip;

// Enemy size categories
typedef enum EnemySize
{
    BIG,
    MID,
    SMALL,
} EnemySize;

// Collectible bonus with effects depending on its type
typedef struct Bonus
{
    Vector2D position;             // Bonus world position
    Vector2D size;                 // Bonus dimensions
    Vector2D velocity;             // Movement vector
    float angle;                   // Rotation angle
    float rotationSpeed;           // Speed of rotation animation
    float speed;                   // Movement speed
    int type;                      // Bonus effect ID
    bool active;                   // Is currently visible/in play
    bool collected;                // Whether player picked it up
} Bonus;

// Bonus type enumeration for texture or display purposes
typedef enum BonusType
{
    RED_STAR,
    YELLOW_STAR,
    BLUE_STAR,
    GREEN_STAR,
} BonusType;

// Tracks bullet collision results for all enemy categories
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

// Stores every texture, font, and sound used in the game
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
    Texture2D yellowStarTexture;
    Texture2D redStarTexture;
    Texture2D blueStarTexture;
    Texture2D greenStarTexture;
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