#ifndef BULLET_H
#define BULLET_H
#include "raylib.h"
#include "struct.h"
#include "Vector2D.h"
#include "sphere2D.h"

// Bullet limits and gameplay constants
#define MAX_BULLETS 100
#define MAX_ENEMY_BULLETS 50
#define BULLET_RADIUS 5
#define BULLET_SPEED 10.0f
#define ENEMY_BULLET_SPEED 6.0f

// Represents a player-fired bullet
typedef struct ShipBullet {
    Vector2 position;   // Current bullet position
    Vector2 velocity;   // Movement direction and speed
    float radius;       // Collision radius
    bool active;        // Indicates whether the bullet is in use
} ShipBullet;

// Represents an enemy-fired bullet
typedef struct EnemyBullet {
    Vector2 position;   // Current bullet position
    Vector2 velocity;   // Movement direction and speed
    float radius;       // Collision radius
    bool active;        // Indicates whether the bullet is active
} EnemyBullet;

// Global gameplay variables
extern int score;           // Player score
extern int actualLevel;     // Current game level
extern int lifeNumber;      // Remaining player lives

extern ShipBullet shipBullets[MAX_BULLETS];  // Pool of player bullets
extern EnemyBullet enemyBullets[MAX_ENEMY_BULLETS]; // Pool of enemy bullets

// Player bullet firing cooldown values
#define SHIP_BULLET_COOLDOWN_NORMAL 0.35f
#define SHIP_BULLET_COOLDOWN_RAPID  0.10f

extern float shipBulletCooldown;   // Current cooldown duration
extern float timeSinceLastShot;    // Timer since the last bullet was fired

// Player bullet functions
void InitBullets(void);                     // Initialize player bullet pool
void FireBullet(Vector2D startPos, float angle, GameAssets* assets); // Spawn a player bullet
void UpdateBullets(GameAssets* assets, Collision* collision);         // Update movement and collisions

// Enemy bullet functions
void InitEnemyBullets(void);                              // Initialize enemy bullet pool
void FireEnemyBullet(Vector2D startPos, Vector2D targetPos, GameAssets* assets); // Spawn an enemy bullet
void PlayEnemyShootSound(GameAssets* assets);             // Play sound when enemies shoot
void UpdateEnemyBullets(GameAssets* assets, Ship* player); // Update enemy bullet movement and collisions

// Bullet collision checks
void CheckBulletBulletCollisions(void); // Detect collisions between player and enemy bullets

// Collision utility
bool CheckCollisionShipEnemy(Rectangle2D shipBox, Sphere2D enemySphere); // Ship vs enemy collision test

#endif