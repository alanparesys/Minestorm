#define M_PI 3.14159265f

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "enemy.h"
#include "game.h"
#include "bullet.h"


const int screenWidth = 1080;
const int screenHeight = 1300;
extern Ship* player;

// Scales pour le rendu
#define BIG_SCALE 3.5f
#define MID_SCALE 2.0f
#define SMALL_SCALE 1.5f

// --- Variables globales --- 

// Cooldowns pour les tirs
static float bigShooterCooldowns[2] = { 0.0f, 0.0f };
static float midShooterCooldowns[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
static float smallShooterCooldowns[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
static float bigFollowerShooterCooldowns[1] = { 0.0f };
static float midFollowerShooterCooldowns[2] = { 0.0f, 0.0f };
static float smallFollowerShooterCooldowns[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

#define BIG_SHOOTER_FIRE_RATE 120.0f
#define MID_SHOOTER_FIRE_RATE 90.0f
#define SMALL_SHOOTER_FIRE_RATE 60.0f

// =========================== Basic Enemies ===========================
int maxBigBasicEnemies = 3;
Enemy bigBasicEnemies[3];
bool bigBasicEnemiesSpawn[3] = { false };
//EnemySize currentBasicEnemySize[3] = { BIG };

int maxMidBasicEnemies = 6;
Enemy midBasicEnemies[6];
bool midBasicEnemiesSpawn[6] = { false };
//EnemySize currentMidBasicEnemySize[6] = { MID };

int maxSmallBasicEnemies = 12;
Enemy smallBasicEnemies[12];
bool smallBasicEnemiesSpawn[12] = { false };
//EnemySize currentSmallBasicEnemySize[12] = { SMALL, SMALL, SMALL, SMALL, SMALL, SMALL,
//                                               SMALL, SMALL, SMALL, SMALL, SMALL, SMALL };

// =========================== Shooter Enemies ===========================
int maxBigShooterEnemy = 2;
Enemy bigShooterEnemies[2];
bool bigShooterEnemiesSpawn[2] = { false };
//EnemySize currentShooterEnemySize[2] = { BIG, BIG };

int maxMidShooterEnemy = 4;
Enemy midShooterEnemies[4];
bool midShooterEnemiesSpawn[4] = { false };
//EnemySize currentMidShooterEnemySize[4] = { MID, MID, MID, MID };

int maxSmallShooterEnemy = 8;
Enemy smallShooterEnemies[8];
bool smallShooterEnemiesSpawn[8] = { false };
//EnemySize currentSmallShooterEnemySize[8] = { SMALL, SMALL, SMALL, SMALL,
//                                                SMALL, SMALL, SMALL, SMALL };

// =========================== Follower Enemies ===========================
int maxBigFollowerEnemy = 2;
Enemy bigFollowerEnemies[2];
bool bigFollowerEnemiesSpawn[2] = { false };
//EnemySize currentFollowerEnemySize[2] = { BIG, BIG };

int maxMidFollowerEnemy = 4;
Enemy midFollowerEnemies[4];
bool midFollowerEnemiesSpawn[4] = { false };
//EnemySize currentMidFollowerEnemySize[4] = { MID, MID, MID, MID };


int maxSmallFollowerEnemy = 8;
Enemy smallFollowerEnemies[8];
bool smallFollowerEnemiesSpawn[8] = { false };
//EnemySize currentSmallFollowerEnemySize[8] = { SMALL, SMALL, SMALL, SMALL,
//                                                    SMALL, SMALL, SMALL, SMALL };

// =========================== Follower-Shooter Enemies ===========================

int maxBigFollowerShooterEnemy = 1;
Enemy bigFollowerShooterEnemies[1];
bool bigFollowerShooterEnemiesSpawn[1] = { false };
//EnemySize currentFollowerShooterEnemySize[1] = { BIG };

int maxMidFollowerShooterEnemy = 2;
Enemy midFollowerShooterEnemies[2];
bool midFollowerShooterEnemiesSpawn[2] = { false };
//EnemySize currentMidFollowerShooterEnemySize[2] = { MID, MID };

int maxSmallFollowerShooterEnemy = 4;
Enemy smallFollowerShooterEnemies[4];
bool smallFollowerShooterEnemiesSpawn[4] = { false };
//EnemySize currentSmallFollowerShooterEnemySize[4] = { SMALL, SMALL, SMALL, SMALL };


// ===================== FONCTIONS DE HITBOX UTILISANT SPHERE2D =====================
Sphere2D GetBigBasicEnemyHitbox(int i)
{
    float renderedSize = bigBasicEnemies[i].size.x * BIG_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        bigBasicEnemies[i].position.x,  // PAS D'OFFSET
        bigBasicEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetMidBasicEnemyHitbox(int i)
{
    float renderedSize = midBasicEnemies[i].size.x * MID_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        midBasicEnemies[i].position.x,  // PAS D'OFFSET
        midBasicEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetSmallBasicEnemyHitbox(int i)
{
    float renderedSize = smallBasicEnemies[i].size.x * SMALL_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        smallBasicEnemies[i].position.x,  // PAS D'OFFSET
        smallBasicEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetBigShooterEnemyHitbox(int i)
{
    float renderedSize = bigShooterEnemies[i].size.x * BIG_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        bigShooterEnemies[i].position.x,  // PAS D'OFFSET
        bigShooterEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetMidShooterEnemyHitbox(int i)
{
    float renderedSize = midShooterEnemies[i].size.x * MID_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        midShooterEnemies[i].position.x,  // PAS D'OFFSET
        midShooterEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetSmallShooterEnemyHitbox(int i)
{
    float renderedSize = smallShooterEnemies[i].size.x * SMALL_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        smallShooterEnemies[i].position.x,  // PAS D'OFFSET
        smallShooterEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}


Sphere2D GetBigFollowerEnemyHitbox(int i)
{
    float renderedSize = bigFollowerEnemies[i].size.x * BIG_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        bigFollowerEnemies[i].position.x,  // PAS D'OFFSET
        bigFollowerEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetMidFollowerEnemyHitbox(int i)
{
    float renderedSize = midFollowerEnemies[i].size.x * MID_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        midFollowerEnemies[i].position.x,  // PAS D'OFFSET
        midFollowerEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetSmallFollowerEnemyHitbox(int i)
{
    float renderedSize = smallFollowerEnemies[i].size.x * SMALL_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        smallFollowerEnemies[i].position.x,  // PAS D'OFFSET
        smallFollowerEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetBigFollowerShooterEnemyHitbox(int i)
{
    float renderedSize = bigFollowerShooterEnemies[i].size.x * BIG_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        bigFollowerShooterEnemies[i].position.x,  // PAS D'OFFSET
        bigFollowerShooterEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetMidFollowerShooterEnemyHitbox(int i)
{
    float renderedSize = midFollowerShooterEnemies[i].size.x * MID_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        midFollowerShooterEnemies[i].position.x,  // PAS D'OFFSET
        midFollowerShooterEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetSmallFollowerShooterEnemyHitbox(int i)
{
    float renderedSize = smallFollowerShooterEnemies[i].size.x * SMALL_SCALE;
    Vector2D center = Vector2D_SetFromComponents(
        smallFollowerShooterEnemies[i].position.x,  // PAS D'OFFSET
        smallFollowerShooterEnemies[i].position.y   // PAS D'OFFSET
    );
    float radius = renderedSize / 2.0f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

// ================================== Border Colision ==================================

void BorderEnemyCollision(Enemy* enemy)
{
    // Si sort par la gauche, réapparaît à droite
    if (enemy->position.x < -50)
        enemy->position.x = screenWidth + 50;

    // Si sort par la droite, réapparaît à gauche
    if (enemy->position.x > screenWidth + 50)
        enemy->position.x = -50;

    // Si sort par le haut, réapparaît en bas
    if (enemy->position.y < 0)
        enemy->position.y = screenHeight + 50;

    // Si sort par le bas, réapparaît en haut
    if (enemy->position.y > screenHeight + 50)
        enemy->position.y = 0;
}

// mothership variables
Vector2D spawnPoints[56];
bool spawnPointsInitialized = false;
bool motherShipActive = false;
bool motherShipSpawned = false;
int maxMotherShip = 1;
Enemy motherShip[1];
bool enemiesDropped = false;

int currentSpawnIndex = 0;  // Pour suivre quel point spawner
float spawnTimer = 0.0f;    // Timer pour spawner progressivement
float spawnInterval = 0.1f; // Intervalle entre chaque spawn (en secondes)

void MotherShipUpdate(GameAssets* assets, Collision* collision)
{
    if (!motherShipActive)
    {
        motherShipActive = true;
        MotherShipSpawn(assets);
    }

    // Déplacer le mothership dans une logique séparée
    if (motherShipSpawned)
    {
        MotherShipMovement(assets, collision);
    }
}

void MotherShipSpawn(GameAssets* assets)
{
    if (!motherShipSpawned)
    {
        motherShipSpawned = true;
        enemiesDropped = false;
        currentSpawnIndex = 0;  // Réinitialiser le compteur
        spawnTimer = 0.0f;

        motherShip[0].position.x = 140;
        motherShip[0].position.y = screenHeight;
        motherShip[0].size.x = 64.0f;
        motherShip[0].size.y = 64.0f;
        motherShip[0].speed = 3.0f;
        motherShip[0].angle = 0.0f;
    }
}


void MotherShipMovement(GameAssets* assets, Collision* collision)
{
    motherShip[0].speed = 3.0f;

    if (motherShip[0].position.y > -300)
    {
        motherShip[0].position.y -= motherShip[0].speed;
        DrawTextureEx(assets->motherShipTexture,
            (Vector2) {
            motherShip[0].position.x, motherShip[0].position.y
        },
            0, 1.0f, WHITE);

        // Spawner progressivement les points rouges et les ennemis
        if (currentSpawnIndex < 56)
        {
            spawnTimer += GetFrameTime();

            if (spawnTimer >= spawnInterval)
            {
                spawnTimer = 0.0f;
                SpawnPointAndEnemy(currentSpawnIndex);
                currentSpawnIndex++;
            }
        }

        // Dessiner tous les points rouges déjà spawnés
        DrawSpawnedPoints();
    }
    else
    {
        motherShip[0].size.x = 0.0f;
        motherShip[0].size.y = 0.0f;
        motherShipSpawned = false;
    }
}


// Fonction pour spawner un point rouge ET l'ennemi correspondant
void SpawnPointAndEnemy(int index)
{
    // Créer le point rouge à une position aléatoire
    spawnPoints[index].x = GetRandomValue(50, screenWidth - 50);
    spawnPoints[index].y = GetRandomValue(50, screenHeight - 50);

    // Déterminer quel type d'ennemi spawner en fonction de l'index
    // Total: 3 + 6 + 12 + 2 + 4 + 8 + 2 + 4 + 8 + 1 + 2 + 4 = 56

    if (index < 3) // Big Basic (0-2)
    {
        int i = index;
        bigBasicEnemies[i].position.x = spawnPoints[index].x;
        bigBasicEnemies[i].position.y = spawnPoints[index].y;
        bigBasicEnemies[i].size.x = 50.0f;
        bigBasicEnemies[i].size.y = 50.0f;

        // Direction aléatoire
        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;
        bigBasicEnemies[i].velocity.x = cosf(angle) * speed;
        bigBasicEnemies[i].velocity.y = sinf(angle) * speed;

        bigBasicEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
        bigBasicEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) bigBasicEnemies[i].rotationSpeed *= -1;

        bigBasicEnemiesSpawn[i] = true;
    }
    else if (index < 9) // Mid Basic (3-8)
    {
        int i = index - 3;
        midBasicEnemies[i].position.x = spawnPoints[index].x;
        midBasicEnemies[i].position.y = spawnPoints[index].y;
        midBasicEnemies[i].size.x = 60.0f;
        midBasicEnemies[i].size.y = 60.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f);
        midBasicEnemies[i].velocity.x = cosf(angle) * speed;
        midBasicEnemies[i].velocity.y = sinf(angle) * speed;

        midBasicEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.05f;
        midBasicEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) midBasicEnemies[i].rotationSpeed *= -1;

        midBasicEnemiesSpawn[i] = true;
    }
    else if (index < 21) // Small Basic (9-20)
    {
        int i = index - 9;
        smallBasicEnemies[i].position.x = spawnPoints[index].x;
        smallBasicEnemies[i].position.y = spawnPoints[index].y;
        smallBasicEnemies[i].size.x = 50.0f;
        smallBasicEnemies[i].size.y = 50.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f);
        smallBasicEnemies[i].velocity.x = cosf(angle) * speed;
        smallBasicEnemies[i].velocity.y = sinf(angle) * speed;

        smallBasicEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.06f;
        smallBasicEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) smallBasicEnemies[i].rotationSpeed *= -1;

        smallBasicEnemiesSpawn[i] = true;
    }
    else if (index < 23) // Big Shooter (21-22)
    {
        int i = index - 21;
        bigShooterEnemies[i].position.x = spawnPoints[index].x;
        bigShooterEnemies[i].position.y = spawnPoints[index].y;
        bigShooterEnemies[i].size.x = 50.0f;
        bigShooterEnemies[i].size.y = 50.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;
        bigShooterEnemies[i].velocity.x = cosf(angle) * speed;
        bigShooterEnemies[i].velocity.y = sinf(angle) * speed;

        bigShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
        bigShooterEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) bigShooterEnemies[i].rotationSpeed *= -1;

        bigShooterEnemiesSpawn[i] = true;
    }
    else if (index < 27) // Mid Shooter (23-26)
    {
        int i = index - 23;
        midShooterEnemies[i].position.x = spawnPoints[index].x;
        midShooterEnemies[i].position.y = spawnPoints[index].y;
        midShooterEnemies[i].size.x = 60.0f;
        midShooterEnemies[i].size.y = 60.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f);
        midShooterEnemies[i].velocity.x = cosf(angle) * speed;
        midShooterEnemies[i].velocity.y = sinf(angle) * speed;

        midShooterEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.05f;
        midShooterEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) midShooterEnemies[i].rotationSpeed *= -1;

        midShooterEnemiesSpawn[i] = true;
    }
    else if (index < 35) // Small Shooter (27-34)
    {
        int i = index - 27;
        smallShooterEnemies[i].position.x = spawnPoints[index].x;
        smallShooterEnemies[i].position.y = spawnPoints[index].y;
        smallShooterEnemies[i].size.x = 50.0f;
        smallShooterEnemies[i].size.y = 50.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f);
        smallShooterEnemies[i].velocity.x = cosf(angle) * speed;
        smallShooterEnemies[i].velocity.y = sinf(angle) * speed;

        smallShooterEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.06f;
        smallShooterEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) smallShooterEnemies[i].rotationSpeed *= -1;

        smallShooterEnemiesSpawn[i] = true;
    }
    else if (index < 37) // Big Follower (35-36)
    {
        int i = index - 35;
        bigFollowerEnemies[i].position.x = spawnPoints[index].x;
        bigFollowerEnemies[i].position.y = spawnPoints[index].y;
        bigFollowerEnemies[i].size.x = 50.0f;
        bigFollowerEnemies[i].size.y = 50.0f;
        bigFollowerEnemies[i].speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

        bigFollowerEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
        bigFollowerEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) bigFollowerEnemies[i].rotationSpeed *= -1;

        bigFollowerEnemiesSpawn[i] = true;
    }
    else if (index < 41) // Mid Follower (37-40)
    {
        int i = index - 37;
        midFollowerEnemies[i].position.x = spawnPoints[index].x;
        midFollowerEnemies[i].position.y = spawnPoints[index].y;
        midFollowerEnemies[i].size.x = 60.0f;
        midFollowerEnemies[i].size.y = 60.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f);
        midFollowerEnemies[i].velocity.x = cosf(angle) * speed;
        midFollowerEnemies[i].velocity.y = sinf(angle) * speed;

        midFollowerEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.05f;
        midFollowerEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) midFollowerEnemies[i].rotationSpeed *= -1;

        midFollowerEnemiesSpawn[i] = true;
    }
    else if (index < 49) // Small Follower (41-48)
    {
        int i = index - 41;
        smallFollowerEnemies[i].position.x = spawnPoints[index].x;
        smallFollowerEnemies[i].position.y = spawnPoints[index].y;
        smallFollowerEnemies[i].size.x = 50.0f;
        smallFollowerEnemies[i].size.y = 50.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f);
        smallFollowerEnemies[i].velocity.x = cosf(angle) * speed;
        smallFollowerEnemies[i].velocity.y = sinf(angle) * speed;

        smallFollowerEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.06f;
        smallFollowerEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) smallFollowerEnemies[i].rotationSpeed *= -1;

        smallFollowerEnemiesSpawn[i] = true;
    }
    else if (index < 50) // Big Follower-Shooter (49)
    {
        int i = index - 49;
        bigFollowerShooterEnemies[i].position.x = spawnPoints[index].x;
        bigFollowerShooterEnemies[i].position.y = spawnPoints[index].y;
        bigFollowerShooterEnemies[i].size.x = 50.0f;
        bigFollowerShooterEnemies[i].size.y = 50.0f;
        bigFollowerShooterEnemies[i].speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

        bigFollowerShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
        bigFollowerShooterEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) bigFollowerShooterEnemies[i].rotationSpeed *= -1;

        bigFollowerShooterEnemiesSpawn[i] = true;
    }
    else if (index < 52) // Mid Follower-Shooter (50-51)
    {
        int i = index - 50;
        midFollowerShooterEnemies[i].position.x = spawnPoints[index].x;
        midFollowerShooterEnemies[i].position.y = spawnPoints[index].y;
        midFollowerShooterEnemies[i].size.x = 60.0f;
        midFollowerShooterEnemies[i].size.y = 60.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f);
        midFollowerShooterEnemies[i].velocity.x = cosf(angle) * speed;
        midFollowerShooterEnemies[i].velocity.y = sinf(angle) * speed;

        midFollowerShooterEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.05f;
        midFollowerShooterEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) midFollowerShooterEnemies[i].rotationSpeed *= -1;

        midFollowerShooterEnemiesSpawn[i] = true;
    }
    else if (index < 56) // Small Follower-Shooter (52-55)
    {
        int i = index - 52;
        smallFollowerShooterEnemies[i].position.x = spawnPoints[index].x;
        smallFollowerShooterEnemies[i].position.y = spawnPoints[index].y;
        smallFollowerShooterEnemies[i].size.x = 50.0f;
        smallFollowerShooterEnemies[i].size.y = 50.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f);
        smallFollowerShooterEnemies[i].velocity.x = cosf(angle) * speed;
        smallFollowerShooterEnemies[i].velocity.y = sinf(angle) * speed;

        smallFollowerShooterEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.06f;
        smallFollowerShooterEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) smallFollowerShooterEnemies[i].rotationSpeed *= -1;

        smallFollowerShooterEnemiesSpawn[i] = true;
    }

    //printf("Spawned point %d and enemy at (%.1f, %.1f)\n", index, spawnPoints[index].x, spawnPoints[index].y);
}

// Fonction pour dessiner tous les points rouges déjà spawnés
void DrawSpawnedPoints()
{
    for (int i = 0; i < currentSpawnIndex; i++)
    {
        DrawCircle(spawnPoints[i].x, spawnPoints[i].y, 5, RED);
    }
}
/*
// Fonction pour initialiser les points UNE SEULE FOIS
void InitSpawnPoints()
{
    for (int i = 0; i < 56; i++)
    {
        spawnPoints[i].x = GetRandomValue(50, screenWidth - 50);
        spawnPoints[i].y = GetRandomValue(50, screenHeight - 50);
    }
    spawnPointsInitialized = true;
    

}

// Nouvelle version de SpawnPoints qui DESSINE les positions sauvegardées
void SpawnPoints()
{
    // Initialiser seulement au premier appel
    if (!spawnPointsInitialized)
    {
        InitSpawnPoints();
    }

    // Dessiner les points sauvegardés
    for (int i = 0; i < 56; i++)
    {
        DrawCircle(spawnPoints[i].x, spawnPoints[i].y, 3, RED);
    }
}

*/

void UpdateEnemies(GameAssets* assets, Collision* collision)
{
    if (!motherShipSpawned)
    {

        for (int i = 0; i < maxBigBasicEnemies; i++)
        {
            UpdateBigBasicEnemy(i, assets, collision);
        }

        // Update les 6 MID basic enemies
        for (int i = 0; i < maxMidBasicEnemies; i++)
        {
            UpdateMidBasicEnemy(i, assets, collision);
        }

        // Update 12 SMALL basic enemies
        for (int i = 0; i < maxSmallBasicEnemies; i++)
        {
            UpdateSmallBasicEnemy(i, assets, collision);
        }

        // Update Big Shooter Enemies
        for (int i = 0; i < maxBigShooterEnemy; i++)
        {
            UpdateBigShooterEnemy(i, assets, collision);
        }

        // Update Mid Shooter Enemies
        for (int i = 0; i < maxMidShooterEnemy; i++)
        {
            UpdateMidShooterEnemy(i, assets, collision);
        }

        // Update Small Shooter Enemies
        for (int i = 0; i < maxSmallShooterEnemy; i++)
        {
            UpdateSmallShooterEnemy(i, assets, collision);
        }

        // Update Big Follower Enemies
        for (int i = 0; i < maxBigFollowerEnemy; i++)
        {
            UpdateBigFollowerEnemy(i, assets, collision);
        }

        // Update Mid Follower Enemies
        for (int i = 0; i < maxMidFollowerEnemy; i++)
        {
            UpdateMidFollowerEnemy(i, assets, collision);
        }

        //Update Small Follower Enemies
        for (int i = 0; i < maxSmallFollowerEnemy; i++)
        {
            UpdateSmallFollowerEnemy(i, assets, collision);
        }

        // Update Big Follower-Shooter Enemies
        for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
        {
            UpdateBigFollowerShooterEnemy(i, assets, collision);
        }

        // Update Mid Follower-Shooter Enemies
        for (int i = 0; i < maxMidFollowerShooterEnemy; i++)
        {
            UpdateMidFollowerShooterEnemy(i, assets, collision);
        }

        // Update Small Follower-Shooter Enemies
        for (int i = 0; i < maxSmallFollowerShooterEnemy; i++)
        {
            UpdateSmallFollowerShooterEnemy(i, assets, collision);
        }
    }
}

// ================================== BasicEnemy ==================================

void UpdateBigBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigBasicEnemiesSpawn[i])
    {
        BigBasicEnemySpawn(i);
        bigBasicEnemiesSpawn[i] = true;
    }
    //if (!bigBasicEnemiesSpawn[i]) return;
    if (bigBasicEnemies[i].size.x <= 0.0f) return;

    BigBasicEnemyMovement(i);

    // Affichage
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { bigBasicEnemies[i].position.x, bigBasicEnemies[i].position.y,
                          bigBasicEnemies[i].size.x * 3.5f, bigBasicEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        bigBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

   
    // Gestion collision
    if (collision->bigBasicEnemiesBulletCollision[i])
    {
        printf(">>> ENTERING COLLISION HANDLER for Big Basic Enemy %d\n", i);

        float x = bigBasicEnemies[i].position.x;
        float y = bigBasicEnemies[i].position.y;

        bigBasicEnemies[i].size.x = 0.0f;
        bigBasicEnemies[i].size.y = 0.0f;
        // bigBasicEnemiesSpawn[i] = false;

        // Spawn 2 mid enemies
        printf(">>> Calling MidBasicEnemySpawn for indices %d and %d\n", i * 2, i * 2 + 1);
        MidBasicEnemySpawn(i * 2, x, y);
        MidBasicEnemySpawn(i * 2 + 1, x, y);

        collision->bigBasicEnemiesBulletCollision[i] = false;
        printf("Big Basic Enemy %d destroyed, spawning mids %d and %d\n", i, i * 2, i * 2 + 1);
    }
}

void BigBasicEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigBasicEnemies[i].position.x = -50.0f; bigBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigBasicEnemies[i].position.x = (float)screenWidth + 50.0f; bigBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigBasicEnemies[i].position.y = -50.0f; bigBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigBasicEnemies[i].position.y = (float)screenHeight + 50.0f; bigBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - bigBasicEnemies[i].position.x;
    float dirY = targetY - bigBasicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.5f - 0.5f);
    bigBasicEnemies[i].velocity.x = dirX / dist * speed;
    bigBasicEnemies[i].velocity.y = dirY / dist * speed;

    bigBasicEnemies[i].size.x = 50.0f;
    bigBasicEnemies[i].size.y = 50.0f;

    bigBasicEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigBasicEnemies[i].rotationSpeed *= -1;
}

void BigBasicEnemyMovement(int i)
{
    bigBasicEnemies[i].position.x += bigBasicEnemies[i].velocity.x;
    bigBasicEnemies[i].position.y += bigBasicEnemies[i].velocity.y;

    bigBasicEnemies[i].angle += bigBasicEnemies[i].rotationSpeed;
    if (bigBasicEnemies[i].angle > 2 * M_PI) bigBasicEnemies[i].angle -= 2 * M_PI;
    if (bigBasicEnemies[i].angle < 0) bigBasicEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&bigBasicEnemies[i]);
}

// === Mid Enemy ===

void UpdateMidBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Vérifier si cet ennemi doit être actif
    //if (!midBasicEnemiesSpawn[i]) return;

    // Ne rien afficher si mort
    if (midBasicEnemies[i].size.x <= 0.0f) return;

    MidBasicEnemyMovement(i);

    // Affichage
    const float scale = 2.0f;
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { midBasicEnemies[i].position.x, midBasicEnemies[i].position.y,
                          midBasicEnemies[i].size.x * scale, midBasicEnemies[i].size.y * scale };
    Vector2 origin = { (midBasicEnemies[i].size.x * scale) / 2.0f, (midBasicEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        midBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

    // Gestion collision
    if (collision->midBasicEnemiesBulletCollision[i])
    {
        float x = midBasicEnemies[i].position.x;
        float y = midBasicEnemies[i].position.y;

        midBasicEnemies[i].size.x = 0.0f;
        midBasicEnemies[i].size.y = 0.0f;
        //midBasicEnemiesSpawn[i] = false;

        // Spawn 2 small enemies
        SmallBasicEnemySpawn(i * 2, x, y);
        SmallBasicEnemySpawn(i * 2 + 1, x, y);

        collision->midBasicEnemiesBulletCollision[i] = false;
        printf("Mid Basic Enemy %d destroyed, spawning smalls %d and %d\n", i, i * 2, i * 2 + 1);
    }
}

void MidBasicEnemySpawn(int i, float parentX, float parentY)
{
    printf(">>> MidBasicEnemySpawn CALLED for index %d at position (%.1f, %.1f)\n", i, parentX, parentY);

    midBasicEnemiesSpawn[i] = true;
    midBasicEnemies[i].position.x = parentX;
    midBasicEnemies[i].position.y = parentY;
    midBasicEnemies[i].size.x = 60.0f;
    midBasicEnemies[i].size.y = 60.0f;

    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midBasicEnemies[i].velocity.x = cosf(Angle) * speed;
    midBasicEnemies[i].velocity.y = sinf(Angle) * speed;

    midBasicEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midBasicEnemies[i].rotationSpeed *= -1;

    printf("Mid Basic Enemy %d spawned at (%.1f, %.1f), spawn flag = %d, size = %.1f\n",
        i, parentX, parentY, midBasicEnemiesSpawn[i], midBasicEnemies[i].size.x);
}

void MidBasicEnemyMovement(int i)
{
    midBasicEnemies[i].position.x += midBasicEnemies[i].velocity.x;
    midBasicEnemies[i].position.y += midBasicEnemies[i].velocity.y;

    midBasicEnemies[i].angle += midBasicEnemies[i].rotationSpeed;
    if (midBasicEnemies[i].angle > 2 * M_PI) midBasicEnemies[i].angle -= 2 * M_PI;
    if (midBasicEnemies[i].angle < 0) midBasicEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&midBasicEnemies[i]);
}

void MidBasicEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { midBasicEnemies[i].position.x = -50.0f; midBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { midBasicEnemies[i].position.x = (float)screenWidth + 50.0f; midBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { midBasicEnemies[i].position.y = -50.0f; midBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { midBasicEnemies[i].position.y = (float)screenHeight + 50.0f; midBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - midBasicEnemies[i].position.x;
    float dirY = targetY - midBasicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);

    midBasicEnemies[i].velocity.x = dirX / dist * speed;
    midBasicEnemies[i].velocity.y = dirY / dist * speed;
}

// === Small Enemy ===

void UpdateSmallBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Vérifier si cet ennemi doit être actif
    //if (!smallBasicEnemiesSpawn[i]) return;

    // Ne rien afficher si mort
    if (smallBasicEnemies[i].size.x <= 0.0f) return;

    SmallBasicEnemyMovement(i);

    // Affichage
    const float scale = 1.5f;
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { smallBasicEnemies[i].position.x, smallBasicEnemies[i].position.y,
                          smallBasicEnemies[i].size.x * scale, smallBasicEnemies[i].size.y * scale };
    Vector2 origin = { (smallBasicEnemies[i].size.x * scale) / 2.0f, (smallBasicEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        smallBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

    // Gestion collision
    if (collision->smallBasicEnemiesBulletCollision[i])
    {
        smallBasicEnemies[i].size.x = 0.0f;
        smallBasicEnemies[i].size.y = 0.0f;
        //smallBasicEnemiesSpawn[i] = false;

        collision->smallBasicEnemiesBulletCollision[i] = false;
        printf("Small Basic Enemy %d destroyed\n", i);
    }
}

void SmallBasicEnemySpawn(int i, float parentX, float parentY)
{
    smallBasicEnemiesSpawn[i] = true;
    smallBasicEnemies[i].position.x = parentX;
    smallBasicEnemies[i].position.y = parentY;
    smallBasicEnemies[i].size.x = 50.0f;
    smallBasicEnemies[i].size.y = 50.0f;

    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 2.0f);
    smallBasicEnemies[i].velocity.x = cosf(Angle) * speed;
    smallBasicEnemies[i].velocity.y = sinf(Angle) * speed;

    smallBasicEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallBasicEnemies[i].rotationSpeed *= -1;

    printf("Small Basic Enemy %d spawned at (%.1f, %.1f)\n", i, parentX, parentY);
}

void SmallBasicEnemyMovement(int i)
{
    smallBasicEnemies[i].position.x += smallBasicEnemies[i].velocity.x;
    smallBasicEnemies[i].position.y += smallBasicEnemies[i].velocity.y;
    smallBasicEnemies[i].angle += smallBasicEnemies[i].rotationSpeed;
    if (smallBasicEnemies[i].angle > 2 * M_PI) smallBasicEnemies[i].angle -= 2 * M_PI;
    if (smallBasicEnemies[i].angle < 0) smallBasicEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&smallBasicEnemies[i]);
}

void SmallBasicEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { smallBasicEnemies[i].position.x = -50.0f; smallBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { smallBasicEnemies[i].position.x = (float)screenWidth + 50.0f; smallBasicEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { smallBasicEnemies[i].position.y = -50.0f; smallBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { smallBasicEnemies[i].position.y = (float)screenHeight + 50.0f; smallBasicEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - smallBasicEnemies[i].position.x;
    float dirY = targetY - smallBasicEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);

    smallBasicEnemies[i].velocity.x = dirX / dist * speed;
    smallBasicEnemies[i].velocity.y = dirY / dist * speed;
}

//////////////////////////////////      ShooterEnemy      ////////////////////////////////////////////////////////////////

void UpdateBigShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Spawn initial si nécessaire
    if (!bigShooterEnemiesSpawn[i])
    {
        BigShooterEnemySpawn(i);
        bigShooterEnemiesSpawn[i] = true;
    }
    // Ne rien afficher si mort
    //if (!bigShooterEnemiesSpawn[i]) return;
    if (bigShooterEnemies[i].size.x <= 0.0f) return;
    BigShooterEnemyMovement(i);
    
    // Logique de tir vers le joueur
    if (player != NULL && bigShooterCooldowns[i] <= 0.0f)
    {
        Vector2D enemyPos = { bigShooterEnemies[i].position.x, bigShooterEnemies[i].position.y };
        Vector2D playerPos = { player->position.x, player->position.y };

        FireEnemyBullet(enemyPos, playerPos);
        bigShooterCooldowns[i] = BIG_SHOOTER_FIRE_RATE;
    }

    if (bigShooterCooldowns[i] > 0.0f)
    {
        bigShooterCooldowns[i] -= 1.0f;
    }
    Rectangle sourceRec = { 0, 0, assets->shooterEnemyTexture.width, assets->shooterEnemyTexture.height };
    Rectangle destRec = { bigShooterEnemies[i].position.x, bigShooterEnemies[i].position.y,
                          bigShooterEnemies[i].size.x * 3.5f, bigShooterEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin,
        bigShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->bigShooterEnemiesBulletCollision[i])
    {
        float x = bigShooterEnemies[i].position.x;
        float y = bigShooterEnemies[i].position.y;

        bigShooterEnemies[i].size.x = 0.0f;
        bigShooterEnemies[i].size.y = 0.0f;
        //bigShooterEnemiesSpawn[i] = false;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        MidShooterEnemySpawn(i * 2, x, y);
        MidShooterEnemySpawn(i * 2 + 1, x, y);
        collision->bigShooterEnemiesBulletCollision[i] = false;
        bigShooterCooldowns[i] = 0.0f;
    }
}

void BigShooterEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigShooterEnemies[i].position.x = -50.0f; bigShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigShooterEnemies[i].position.x = (float)screenWidth + 50.0f; bigShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigShooterEnemies[i].position.y = -50.0f; bigShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigShooterEnemies[i].position.y = (float)screenHeight + 50.0f; bigShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - bigShooterEnemies[i].position.x;
    float dirY = targetY - bigShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);

    float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.5f - 0.5f);
    bigShooterEnemies[i].velocity.x = dirX / dist * speed;
    bigShooterEnemies[i].velocity.y = dirY / dist * speed;

    bigShooterEnemies[i].size.x = 50.0f;
    bigShooterEnemies[i].size.y = 50.0f;

    bigShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigShooterEnemies[i].rotationSpeed *= -1;
}

void BigShooterEnemyMovement(int i)
{
    bigShooterEnemies[i].position.x += bigShooterEnemies[i].velocity.x;
    bigShooterEnemies[i].position.y += bigShooterEnemies[i].velocity.y;

    bigShooterEnemies[i].angle += bigShooterEnemies[i].rotationSpeed;
    if (bigShooterEnemies[i].angle > 2 * M_PI) bigShooterEnemies[i].angle -= 2 * M_PI;
    if (bigShooterEnemies[i].angle < 0) bigShooterEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&bigShooterEnemies[i]);
}

// === Mid Enemy ===

void UpdateMidShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    //if (!midShooterEnemiesSpawn[i]) return;
    if (midShooterEnemies[i].size.x <= 0.0f) return;
    MidShooterEnemyMovement(i);


    // Logique de tir vers le joueur
    if (player != NULL && midShooterCooldowns[i] <= 0.0f)
    {
        Vector2D enemyPos = { midShooterEnemies[i].position.x, midShooterEnemies[i].position.y };
        Vector2D playerPos = { player->position.x, player->position.y };

        FireEnemyBullet(enemyPos, playerPos);
        midShooterCooldowns[i] = MID_SHOOTER_FIRE_RATE;
    }

    if (midShooterCooldowns[i] > 0.0f)
    {
        midShooterCooldowns[i] -= 1.0f;
    }

    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 2.0f;  // Moitié de 3.5f
    Rectangle sourceRec = { 0, 0, assets->shooterEnemyTexture.width, assets->shooterEnemyTexture.height };
    Rectangle destRec = { midShooterEnemies[i].position.x, midShooterEnemies[i].position.y,
                          midShooterEnemies[i].size.x * scale, midShooterEnemies[i].size.y * scale };
    Vector2 origin = { (midShooterEnemies[i].size.x * scale) / 2.0f, (midShooterEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin,
        midShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->midShooterEnemiesBulletCollision[i])
    {
        float x = midShooterEnemies[i].position.x;
        float y = midShooterEnemies[i].position.y;
        midShooterEnemies[i].size.x = 0.0f;
        midShooterEnemies[i].size.y = 0.0f;
		//midShooterEnemiesSpawn[i] = false;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        SmallShooterEnemySpawn(i * 2, x, y);
        SmallShooterEnemySpawn(i * 2 + 1, x, y);
        collision->midShooterEnemiesBulletCollision[i] = false;
        midShooterCooldowns[i] = 0.0f;

    }
}

void MidShooterEnemySpawn(int i, float parentX, float parentY)
{
    midShooterEnemiesSpawn[i] = true;
    // Position de départ = position du parent (là où le big est mort)
    midShooterEnemies[i].position.x = parentX;
    midShooterEnemies[i].position.y = parentY;
    // Taille réduite (50% du parent)
    midShooterEnemies[i].size.x = 60.0f;
    midShooterEnemies[i].size.y = 60.0f;
    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midShooterEnemies[i].velocity.x = cosf(Angle) * speed;
    midShooterEnemies[i].velocity.y = sinf(Angle) * speed;
    // Rotation
    midShooterEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midShooterEnemies[i].rotationSpeed *= -1;
}

void MidShooterEnemyMovement(int i)
{
    midShooterEnemies[i].position.x += midShooterEnemies[i].velocity.x;
    midShooterEnemies[i].position.y += midShooterEnemies[i].velocity.y;
    midShooterEnemies[i].angle += midShooterEnemies[i].rotationSpeed;
    if (midShooterEnemies[i].angle > 2 * M_PI) midShooterEnemies[i].angle -= 2 * M_PI;
    if (midShooterEnemies[i].angle < 0) midShooterEnemies[i].angle += 2 * M_PI;
    
    BorderEnemyCollision(&midShooterEnemies[i]);

}

void MidShooterEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { midShooterEnemies[i].position.x = -50.0f; midShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { midShooterEnemies[i].position.x = (float)screenWidth + 50.0f; midShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { midShooterEnemies[i].position.y = -50.0f; midShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { midShooterEnemies[i].position.y = (float)screenHeight + 50.0f; midShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - midShooterEnemies[i].position.x;
    float dirY = targetY - midShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midShooterEnemies[i].velocity.x = dirX / dist * speed;
    midShooterEnemies[i].velocity.y = dirY / dist * speed;
}

// === Small Enemy ===

void UpdateSmallShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    //if (!smallShooterEnemiesSpawn[i]) return;
    if (smallShooterEnemies[i].size.x <= 0.0f) return;
    SmallShooterEnemyMovement(i);

    // Logique de tir vers le joueur
    if (player != NULL && smallShooterCooldowns[i] <= 0.0f)
    {
        Vector2D enemyPos = { smallShooterEnemies[i].position.x, smallShooterEnemies[i].position.y };
        Vector2D playerPos = { player->position.x, player->position.y };

        FireEnemyBullet(enemyPos, playerPos);
        smallShooterCooldowns[i] = SMALL_SHOOTER_FIRE_RATE;
    }

    if (smallShooterCooldowns[i] > 0.0f)
    {
        smallShooterCooldowns[i] -= 1.0f;
    }

    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 1.5f;  // Moitié de 2.0f
    Rectangle sourceRec = { 0, 0, assets->shooterEnemyTexture.width, assets->shooterEnemyTexture.height };
    Rectangle destRec = { smallShooterEnemies[i].position.x, smallShooterEnemies[i].position.y,
                          smallShooterEnemies[i].size.x * scale, smallShooterEnemies[i].size.y * scale };
    Vector2 origin = { (smallShooterEnemies[i].size.x * scale) / 2.0f, (smallShooterEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin,
        smallShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->smallShooterEnemiesBulletCollision[i])
    {
        smallShooterEnemies[i].size.x = 0.0f;
        smallShooterEnemies[i].size.y = 0.0f;
		//smallShooterEnemiesSpawn[i] = false;

        collision->smallShooterEnemiesBulletCollision[i] = false;
        smallShooterCooldowns[i] = 0.0f;

    }
}

void SmallShooterEnemySpawn(int i, float parentX, float parentY)
{
    smallShooterEnemiesSpawn[i] = true;
    // Position de départ = position du parent (là où le mid est mort)
    smallShooterEnemies[i].position.x = parentX;
    smallShooterEnemies[i].position.y = parentY;
    // Taille réduite (50% du parent)
    smallShooterEnemies[i].size.x = 50.0f;
    smallShooterEnemies[i].size.y = 50.0f;
    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 2.0f);
    smallShooterEnemies[i].velocity.x = cosf(Angle) * speed;
    smallShooterEnemies[i].velocity.y = sinf(Angle) * speed;
    // Rotation
    smallShooterEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallShooterEnemies[i].rotationSpeed *= -1;
}

void SmallShooterEnemyMovement(int i)
{
    smallShooterEnemies[i].position.x += smallShooterEnemies[i].velocity.x;
    smallShooterEnemies[i].position.y += smallShooterEnemies[i].velocity.y;
    smallShooterEnemies[i].angle += smallShooterEnemies[i].rotationSpeed;
    if (smallShooterEnemies[i].angle > 2 * M_PI) smallShooterEnemies[i].angle -= 2 * M_PI;
    if (smallShooterEnemies[i].angle < 0) smallShooterEnemies[i].angle += 2 * M_PI;
    
    BorderEnemyCollision(&smallShooterEnemies[i]);

}

void SmallShooterEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { smallShooterEnemies[i].position.x = -50.0f; smallShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { smallShooterEnemies[i].position.x = (float)screenWidth + 50.0f; smallShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { smallShooterEnemies[i].position.y = -50.0f; smallShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { smallShooterEnemies[i].position.y = (float)screenHeight + 50.0f; smallShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - smallShooterEnemies[i].position.x;
    float dirY = targetY - smallShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    smallShooterEnemies[i].velocity.x = dirX / dist * speed;
    smallShooterEnemies[i].velocity.y = dirY / dist * speed;
}

//////////////////////////////////      FollowerEnemy      ////////////////////////////////////////////////////////////////

void UpdateBigFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Spawn initial si nécessaire
    if (!bigFollowerEnemiesSpawn[i])
    {
        BigFollowerEnemySpawn(i);
        bigFollowerEnemiesSpawn[i] = true;
    }
    // Ne rien afficher si mort
    //if (!bigFollowerEnemiesSpawn[i]) return;
    if (bigFollowerEnemies[i].size.x <= 0.0f) return;
    BigFollowerEnemyMovement(i);
    // Affichage
    Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
    Rectangle destRec = { bigFollowerEnemies[i].position.x, bigFollowerEnemies[i].position.y,
                          bigFollowerEnemies[i].size.x * 3.5f, bigFollowerEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
        bigFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->bigFollowerEnemiesBulletCollision[i])
    {
        float x = bigFollowerEnemies[i].position.x;
        float y = bigFollowerEnemies[i].position.y;
        bigFollowerEnemies[i].size.x = 0.0f;
        bigFollowerEnemies[i].size.y = 0.0f;
		//bigFollowerEnemiesSpawn[i] = false;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        MidFollowerEnemySpawn(i * 2, x, y);
        MidFollowerEnemySpawn(i * 2 + 1, x, y);
        collision->bigFollowerEnemiesBulletCollision[i] = false;
    }
}

void BigFollowerEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigFollowerEnemies[i].position.x = -50.0f; bigFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigFollowerEnemies[i].position.x = (float)screenWidth + 50.0f; bigFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigFollowerEnemies[i].position.y = -50.0f; bigFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigFollowerEnemies[i].position.y = (float)screenHeight + 50.0f; bigFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    bigFollowerEnemies[i].speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.0f - 1.0f);
    bigFollowerEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigFollowerEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigFollowerEnemies[i].rotationSpeed *= -1;

    bigFollowerEnemies[i].size.x = 50.0f;
    bigFollowerEnemies[i].size.y = 50.0f;
}

void BigFollowerEnemyMovement(int i)
{
    // Protection contre NULL pointer
    if (player == NULL) {
        return;
    }

    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - bigFollowerEnemies[i].position.x;
    float dirY = targetY - bigFollowerEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    bigFollowerEnemies[i].velocity.x = dirX / dist * bigFollowerEnemies[i].speed;
    bigFollowerEnemies[i].velocity.y = dirY / dist * bigFollowerEnemies[i].speed;

    bigFollowerEnemies[i].position.x += bigFollowerEnemies[i].velocity.x;
    bigFollowerEnemies[i].position.y += bigFollowerEnemies[i].velocity.y;

    bigFollowerEnemies[i].angle += bigFollowerEnemies[i].rotationSpeed;
    if (bigFollowerEnemies[i].angle > 2 * M_PI) bigFollowerEnemies[i].angle -= 2 * M_PI;
    if (bigFollowerEnemies[i].angle < 0) bigFollowerEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&bigFollowerEnemies[i]);

}

// === Mid Enemy ===

void UpdateMidFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    //if (!midFollowerEnemiesSpawn[i]) return;
    if (midFollowerEnemies[i].size.x <= 0.0f) return;
    MidFollowerEnemyMovement(i);
    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 2.0f;  // Moitié de 3.5f
    Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
    Rectangle destRec = { midFollowerEnemies[i].position.x, midFollowerEnemies[i].position.y,
                          midFollowerEnemies[i].size.x * scale, midFollowerEnemies[i].size.y * scale };
    Vector2 origin = { (midFollowerEnemies[i].size.x * scale) / 2.0f, (midFollowerEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
        midFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->midFollowerEnemiesBulletCollision[i])
    {
        float x = midFollowerEnemies[i].position.x;
        float y = midFollowerEnemies[i].position.y;
        midFollowerEnemies[i].size.x = 0.0f;
        midFollowerEnemies[i].size.y = 0.0f;
		//midFollowerEnemiesSpawn[i] = false;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        SmallFollowerEnemySpawn(i * 2, x, y);
        SmallFollowerEnemySpawn(i * 2 + 1, x, y);
        collision->midFollowerEnemiesBulletCollision[i] = false;
    }
}

void MidFollowerEnemySpawn(int i, float parentX, float parentY)
{
    midFollowerEnemiesSpawn[i] = true;
    // Position de départ = position du parent (là où le big est mort)
    midFollowerEnemies[i].position.x = parentX;
    midFollowerEnemies[i].position.y = parentY;
    // Taille réduite (50% du parent)
    midFollowerEnemies[i].size.x = 60.0f;
    midFollowerEnemies[i].size.y = 60.0f;
    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midFollowerEnemies[i].velocity.x = cosf(Angle) * speed;
    midFollowerEnemies[i].velocity.y = sinf(Angle) * speed;
    // Rotation
    midFollowerEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midFollowerEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midFollowerEnemies[i].rotationSpeed *= -1;
}

void MidFollowerEnemyMovement(int i)
{
    midFollowerEnemies[i].position.x += midFollowerEnemies[i].velocity.x;
    midFollowerEnemies[i].position.y += midFollowerEnemies[i].velocity.y;
    midFollowerEnemies[i].angle += midFollowerEnemies[i].rotationSpeed;
    if (midFollowerEnemies[i].angle > 2 * M_PI) midFollowerEnemies[i].angle -= 2 * M_PI;
    if (midFollowerEnemies[i].angle < 0) midFollowerEnemies[i].angle += 2 * M_PI;
    
    BorderEnemyCollision(&midFollowerEnemies[i]);

}

void MidFollowerEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { midFollowerEnemies[i].position.x = -50.0f; midFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { midFollowerEnemies[i].position.x = (float)screenWidth + 50.0f; midFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { midFollowerEnemies[i].position.y = -50.0f; midFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { midFollowerEnemies[i].position.y = (float)screenHeight + 50.0f; midFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - midFollowerEnemies[i].position.x;
    float dirY = targetY - midFollowerEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midFollowerEnemies[i].velocity.x = dirX / dist * speed;
    midFollowerEnemies[i].velocity.y = dirY / dist * speed;
}

// === Small Enemy ===

void UpdateSmallFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    //if (!smallFollowerEnemiesSpawn[i]) return;
    if (smallFollowerEnemies[i].size.x <= 0.0f) return;
    SmallFollowerEnemyMovement(i);
    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 1.5f;  // Moitié de 2.0f
    Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
    Rectangle destRec = { smallFollowerEnemies[i].position.x, smallFollowerEnemies[i].position.y,
                          smallFollowerEnemies[i].size.x * scale, smallFollowerEnemies[i].size.y * scale };
    Vector2 origin = { (smallFollowerEnemies[i].size.x * scale) / 2.0f, (smallFollowerEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
        smallFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->smallFollowerEnemiesBulletCollision[i])
    {
        smallFollowerEnemies[i].size.x = 0.0f;
        smallFollowerEnemies[i].size.y = 0.0f;
		//smallFollowerEnemiesSpawn[i] = false;

        collision->smallFollowerEnemiesBulletCollision[i] = false;
    }
}

void SmallFollowerEnemySpawn(int i, float parentX, float parentY)
{
    smallFollowerEnemiesSpawn[i] = true;
    // Position de départ = position du parent (là où le mid est mort)
    smallFollowerEnemies[i].position.x = parentX;
    smallFollowerEnemies[i].position.y = parentY;
    // Taille réduite (50% du parent)
    smallFollowerEnemies[i].size.x = 50.0f;
    smallFollowerEnemies[i].size.y = 50.0f;
    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 2.0f);
    smallFollowerEnemies[i].velocity.x = cosf(Angle) * speed;
    smallFollowerEnemies[i].velocity.y = sinf(Angle) * speed;
    // Rotation
    smallFollowerEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallFollowerEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallFollowerEnemies[i].rotationSpeed *= -1;
}

void SmallFollowerEnemyMovement(int i)
{
    smallFollowerEnemies[i].position.x += smallFollowerEnemies[i].velocity.x;
    smallFollowerEnemies[i].position.y += smallFollowerEnemies[i].velocity.y;
    smallFollowerEnemies[i].angle += smallFollowerEnemies[i].rotationSpeed;
    if (smallFollowerEnemies[i].angle > 2 * M_PI) smallFollowerEnemies[i].angle -= 2 * M_PI;
    if (smallFollowerEnemies[i].angle < 0) smallFollowerEnemies[i].angle += 2 * M_PI;
    
    BorderEnemyCollision(&smallFollowerEnemies[i]);

}

void SmallFollowerEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { smallFollowerEnemies[i].position.x = -50.0f; smallFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { smallFollowerEnemies[i].position.x = (float)screenWidth + 50.0f; smallFollowerEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { smallFollowerEnemies[i].position.y = -50.0f; smallFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { smallFollowerEnemies[i].position.y = (float)screenHeight + 50.0f; smallFollowerEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - smallFollowerEnemies[i].position.x;
    float dirY = targetY - smallFollowerEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    smallFollowerEnemies[i].velocity.x = dirX / dist * speed;
    smallFollowerEnemies[i].velocity.y = dirY / dist * speed;
}

//////////////////////////////////      FollowerShooterEnemy      ////////////////////////////////////////////////////////////////

void UpdateBigFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Spawn initial si nécessaire
    if (!bigFollowerShooterEnemiesSpawn[i])
    {
        BigFollowerShooterEnemySpawn(i);
        bigFollowerShooterEnemiesSpawn[i] = true;
    }
    // Ne rien afficher si mort
    //if (!bigFollowerShooterEnemiesSpawn[i]) return;
    if (bigFollowerShooterEnemies[i].size.x <= 0.0f) return;
    BigFollowerShooterEnemyMovement(i);

    // Logique de tir vers le joueur
    if (player != NULL && bigFollowerShooterCooldowns[i] <= 0.0f)
    {
        Vector2D enemyPos = { bigFollowerShooterEnemies[i].position.x, bigFollowerShooterEnemies[i].position.y };
        Vector2D playerPos = { player->position.x, player->position.y };

        FireEnemyBullet(enemyPos, playerPos);
        bigFollowerShooterCooldowns[i] = BIG_SHOOTER_FIRE_RATE;
    }

    if (bigFollowerShooterCooldowns[i] > 0.0f)
    {
        bigFollowerShooterCooldowns[i] -= 1.0f;
    }

    // Affichage
    Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
    Rectangle destRec = { bigFollowerShooterEnemies[i].position.x, bigFollowerShooterEnemies[i].position.y,
                          bigFollowerShooterEnemies[i].size.x * 3.5f, bigFollowerShooterEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin,
        bigFollowerShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->bigFollowerShooterEnemiesBulletCollision[i])
    {
        float x = bigFollowerShooterEnemies[i].position.x;
        float y = bigFollowerShooterEnemies[i].position.y;
        bigFollowerShooterEnemies[i].size.x = 0.0f;
        bigFollowerShooterEnemies[i].size.y = 0.0f;
		//bigFollowerShooterEnemiesSpawn[i] = false;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        MidFollowerShooterEnemySpawn(i * 2, x, y);
        MidFollowerShooterEnemySpawn(i * 2 + 1, x, y);
        collision->bigFollowerShooterEnemiesBulletCollision[i] = false;
        bigFollowerShooterCooldowns[i] = 0.0f;

    }
}

void BigFollowerShooterEnemySpawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { bigFollowerShooterEnemies[i].position.x = -50.0f; bigFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { bigFollowerShooterEnemies[i].position.x = (float)screenWidth + 50.0f; bigFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { bigFollowerShooterEnemies[i].position.y = -50.0f; bigFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { bigFollowerShooterEnemies[i].position.y = (float)screenHeight + 50.0f; bigFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }

    bigFollowerShooterEnemies[i].speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (1.5f - 0.5f);
    bigFollowerShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.05f - 0.02f);
    bigFollowerShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigFollowerShooterEnemies[i].rotationSpeed *= -1;
    bigFollowerShooterEnemies[i].size.x = 50.0f;
    bigFollowerShooterEnemies[i].size.y = 50.0f;
}

void BigFollowerShooterEnemyMovement(int i)
{
    // Protection contre NULL pointer
    if (player == NULL) {
        return;
    }

    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - bigFollowerShooterEnemies[i].position.x;
    float dirY = targetY - bigFollowerShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    bigFollowerShooterEnemies[i].velocity.x = dirX / dist * bigFollowerShooterEnemies[i].speed;
    bigFollowerShooterEnemies[i].velocity.y = dirY / dist * bigFollowerShooterEnemies[i].speed;

    bigFollowerShooterEnemies[i].position.x += bigFollowerShooterEnemies[i].velocity.x;
    bigFollowerShooterEnemies[i].position.y += bigFollowerShooterEnemies[i].velocity.y;

    bigFollowerShooterEnemies[i].angle += bigFollowerShooterEnemies[i].rotationSpeed;
    if (bigFollowerShooterEnemies[i].angle > 2 * M_PI) bigFollowerShooterEnemies[i].angle -= 2 * M_PI;
    if (bigFollowerShooterEnemies[i].angle < 0) bigFollowerShooterEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&bigFollowerShooterEnemies[i]);

}

// === Mid Enemy ===

void UpdateMidFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    //if (!midFollowerShooterEnemiesSpawn[i]) return;
    if (midFollowerShooterEnemies[i].size.x <= 0.0f) return;
    MidFollowerShooterEnemyMovement(i);

    // Logique de tir vers le joueur
    if (player != NULL && midFollowerShooterCooldowns[i] <= 0.0f)
    {
        Vector2D enemyPos = { midFollowerShooterEnemies[i].position.x, midFollowerShooterEnemies[i].position.y };
        Vector2D playerPos = { player->position.x, player->position.y };

        FireEnemyBullet(enemyPos, playerPos);
        midFollowerShooterCooldowns[i] = MID_SHOOTER_FIRE_RATE;
    }

    if (midFollowerShooterCooldowns[i] > 0.0f)
    {
        midFollowerShooterCooldowns[i] -= 1.0f;
    }

    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 2.0f;  // Moitié de 3.5f
    Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
    Rectangle destRec = { midFollowerShooterEnemies[i].position.x, midFollowerShooterEnemies[i].position.y,
                          midFollowerShooterEnemies[i].size.x * scale, midFollowerShooterEnemies[i].size.y * scale };
    Vector2 origin = { (midFollowerShooterEnemies[i].size.x * scale) / 2.0f, (midFollowerShooterEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin,
        midFollowerShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->midFollowerShooterEnemiesBulletCollision[i])
    {
        float x = midFollowerShooterEnemies[i].position.x;
        float y = midFollowerShooterEnemies[i].position.y;
        midFollowerShooterEnemies[i].size.x = 0.0f;
        midFollowerShooterEnemies[i].size.y = 0.0f;
		//midFollowerShooterEnemiesSpawn[i] = false;

        // Spawn 2 mid enemies (indices dédiés : i*2 et i*2+1)
        SmallFollowerShooterEnemySpawn(i * 2, x, y);
        SmallFollowerShooterEnemySpawn(i * 2 + 1, x, y);
        collision->midFollowerShooterEnemiesBulletCollision[i] = false;
    }
}

void MidFollowerShooterEnemySpawn(int i, float parentX, float parentY)
{
    midFollowerShooterEnemiesSpawn[i] = true;
    // Position de départ = position du parent (là où le big est mort)
    midFollowerShooterEnemies[i].position.x = parentX;
    midFollowerShooterEnemies[i].position.y = parentY;
    // Taille réduite (50% du parent)
    midFollowerShooterEnemies[i].size.x = 60.0f;
    midFollowerShooterEnemies[i].size.y = 60.0f;
    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midFollowerShooterEnemies[i].velocity.x = cosf(Angle) * speed;
    midFollowerShooterEnemies[i].velocity.y = sinf(Angle) * speed;
    // Rotation
    midFollowerShooterEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midFollowerShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midFollowerShooterEnemies[i].rotationSpeed *= -1;
}

void MidFollowerShooterEnemyMovement(int i)
{
    midFollowerShooterEnemies[i].position.x += midFollowerShooterEnemies[i].velocity.x;
    midFollowerShooterEnemies[i].position.y += midFollowerShooterEnemies[i].velocity.y;
    midFollowerShooterEnemies[i].angle += midFollowerShooterEnemies[i].rotationSpeed;
    if (midFollowerShooterEnemies[i].angle > 2 * M_PI) midFollowerShooterEnemies[i].angle -= 2 * M_PI;
    if (midFollowerShooterEnemies[i].angle < 0) midFollowerShooterEnemies[i].angle += 2 * M_PI;
    
    BorderEnemyCollision(&midFollowerShooterEnemies[i]);

}

void MidFollowerShooterEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { midFollowerShooterEnemies[i].position.x = -50.0f; midFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { midFollowerShooterEnemies[i].position.x = (float)screenWidth + 50.0f; midFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { midFollowerShooterEnemies[i].position.y = -50.0f; midFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { midFollowerShooterEnemies[i].position.y = (float)screenHeight + 50.0f; midFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - midFollowerShooterEnemies[i].position.x;
    float dirY = targetY - midFollowerShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midFollowerShooterEnemies[i].velocity.x = dirX / dist * speed;
    midFollowerShooterEnemies[i].velocity.y = dirY / dist * speed;
}

// === Small Enemy ===

void UpdateSmallFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    // Ne rien afficher si mort
    //if (!smallFollowerShooterEnemiesSpawn[i]) return;
    if (smallFollowerShooterEnemies[i].size.x <= 0.0f) return;
    SmallFollowerShooterEnemyMovement(i);

    // Logique de tir vers le joueur
    if (player != NULL && smallFollowerShooterCooldowns[i] <= 0.0f)
    {
        Vector2D enemyPos = { smallFollowerShooterEnemies[i].position.x, smallFollowerShooterEnemies[i].position.y };
        Vector2D playerPos = { player->position.x, player->position.y };

        FireEnemyBullet(enemyPos, playerPos);
        smallFollowerShooterCooldowns[i] = SMALL_SHOOTER_FIRE_RATE;
    }

    if (smallFollowerShooterCooldowns[i] > 0.0f)
    {
        smallFollowerShooterCooldowns[i] -= 1.0f;
    }

    // Affichage (même structure que BIG mais avec scale réduit)
    const float scale = 1.5f;  // Moitié de 2.0f
    Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
    Rectangle destRec = { smallFollowerShooterEnemies[i].position.x, smallFollowerShooterEnemies[i].position.y,
                          smallFollowerShooterEnemies[i].size.x * scale, smallFollowerShooterEnemies[i].size.y * scale };
    Vector2 origin = { (smallFollowerShooterEnemies[i].size.x * scale) / 2.0f, (smallFollowerShooterEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin,
        smallFollowerShooterEnemies[i].angle * 180.0f / M_PI, WHITE);
    // Gestion collision
    if (collision->smallFollowerShooterEnemiesBulletCollision[i])
    {
        smallFollowerShooterEnemies[i].size.x = 0.0f;
        smallFollowerShooterEnemies[i].size.y = 0.0f;
		//smallFollowerShooterEnemiesSpawn[i] = false;

        collision->smallFollowerShooterEnemiesBulletCollision[i] = false;
        smallFollowerShooterCooldowns[i] = 0.0f;

    }
}

void SmallFollowerShooterEnemySpawn(int i, float parentX, float parentY)
{
    smallFollowerShooterEnemiesSpawn[i] = true;
    // Position de départ = position du parent (là où le mid est mort)
    smallFollowerShooterEnemies[i].position.x = parentX;
    smallFollowerShooterEnemies[i].position.y = parentY;
    // Taille réduite (50% du parent)
    smallFollowerShooterEnemies[i].size.x = 50.0f;
    smallFollowerShooterEnemies[i].size.y = 50.0f;
    // Direction ALÉATOIRE complète (pas vers un point spécifique)
    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 2.0f);
    smallFollowerShooterEnemies[i].velocity.x = cosf(Angle) * speed;
    smallFollowerShooterEnemies[i].velocity.y = sinf(Angle) * speed;
    // Rotation
    smallFollowerShooterEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallFollowerShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallFollowerShooterEnemies[i].rotationSpeed *= -1;
}

void SmallFollowerShooterEnemyMovement(int i)
{
    smallFollowerShooterEnemies[i].position.x += smallFollowerShooterEnemies[i].velocity.x;
    smallFollowerShooterEnemies[i].position.y += smallFollowerShooterEnemies[i].velocity.y;
    smallFollowerShooterEnemies[i].angle += smallFollowerShooterEnemies[i].rotationSpeed;
    if (smallFollowerShooterEnemies[i].angle > 2 * M_PI) smallFollowerShooterEnemies[i].angle -= 2 * M_PI;
    if (smallFollowerShooterEnemies[i].angle < 0) smallFollowerShooterEnemies[i].angle += 2 * M_PI;
    
    BorderEnemyCollision(&smallFollowerShooterEnemies[i]);

}

void SmallFollowerShooterEnemyRespawn(int i)
{
    int side = GetRandomValue(1, 4);
    if (side == 1) { smallFollowerShooterEnemies[i].position.x = -50.0f; smallFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 2) { smallFollowerShooterEnemies[i].position.x = (float)screenWidth + 50.0f; smallFollowerShooterEnemies[i].position.y = (float)GetRandomValue(0, screenHeight); }
    if (side == 3) { smallFollowerShooterEnemies[i].position.y = -50.0f; smallFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    if (side == 4) { smallFollowerShooterEnemies[i].position.y = (float)screenHeight + 50.0f; smallFollowerShooterEnemies[i].position.x = (float)GetRandomValue(0, screenWidth); }
    float targetX = (float)GetRandomValue(50, screenWidth - 50);
    float targetY = (float)GetRandomValue(50, screenHeight - 50);
    float dirX = targetX - smallFollowerShooterEnemies[i].position.x;
    float dirY = targetY - smallFollowerShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    smallFollowerShooterEnemies[i].velocity.x = dirX / dist * speed;
    smallFollowerShooterEnemies[i].velocity.y = dirY / dist * speed;
}


bool AllEnemiesDead(void)
{
    for (int i = 0; i < maxBigBasicEnemies; i++)
        if (bigBasicEnemiesSpawn[i] && bigBasicEnemies[i].size.x > 0.0f && bigBasicEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxMidBasicEnemies; i++)
        if (midBasicEnemiesSpawn[i] && midBasicEnemies[i].size.x > 0.0f && midBasicEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxSmallBasicEnemies; i++)
        if (smallBasicEnemiesSpawn[i] && smallBasicEnemies[i].size.x > 0.0f && smallBasicEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxBigShooterEnemy; i++)
        if (bigShooterEnemiesSpawn[i] && bigShooterEnemies[i].size.x > 0.0f && bigShooterEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxMidShooterEnemy; i++)
        if (midShooterEnemiesSpawn[i] && midShooterEnemies[i].size.x > 0.0f && midShooterEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxSmallShooterEnemy; i++)
        if (smallShooterEnemiesSpawn[i] && smallShooterEnemies[i].size.x > 0.0f && smallShooterEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxBigFollowerEnemy; i++)
        if (bigFollowerEnemiesSpawn[i] && bigFollowerEnemies[i].size.x > 0.0f && bigFollowerEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxMidFollowerEnemy; i++)
        if (midFollowerEnemiesSpawn[i] && midFollowerEnemies[i].size.x > 0.0f && midFollowerEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxSmallFollowerEnemy; i++)
        if (smallFollowerEnemiesSpawn[i] && smallFollowerEnemies[i].size.x > 0.0f && smallFollowerEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxBigFollowerShooterEnemy; i++)
        if (bigFollowerShooterEnemiesSpawn[i] && bigFollowerShooterEnemies[i].size.x > 0.0f && bigFollowerShooterEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxMidFollowerShooterEnemy; i++)
        if (midFollowerShooterEnemiesSpawn[i] && midFollowerShooterEnemies[i].size.x > 0.0f && midFollowerShooterEnemies[i].size.y > 0.0f)
            return false;

    for (int i = 0; i < maxSmallFollowerShooterEnemy; i++)
        if (smallFollowerShooterEnemiesSpawn[i] && smallFollowerShooterEnemies[i].size.x > 0.0f && smallFollowerShooterEnemies[i].size.y > 0.0f)
            return false;

    return true; // Tous les ennemis spawnés sont morts
}