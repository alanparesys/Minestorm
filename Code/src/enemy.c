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
#define BIG_FOLLOWER_SHOOTER_FIRE_RATE 120.0f
#define MID_FOLLOWER_SHOOTER_FIRE_RATE 90.0f
#define SMALL_FOLLOWER_SHOOTER_FIRE_RATE 60.0f

Vector2D spawnPoints[150];  // Augmenté pour avoir plus de points disponibles
bool spawnPointsInitialized = false;
bool motherShipActive = false;
bool motherShipSpawned = false;
int maxMotherShip = 1;
Enemy motherShip[1];
bool enemiesDropped = false;
bool enemiesActive = false;

int currentSpawnIndex = 0;
float spawnTimer = 0.0f;
float spawnInterval = 0.05f;  // Réduit pour spawn plus rapide

// NOUVEAU : Timer pour attendre 1 seconde
float spawnDelayTimer = 0.0f;
const float SPAWN_DELAY = 1.0f; // 1 seconde de délai

// Compteur de points utilisés
int usedSpawnPoints = 0;

// Distance minimum de spawn par rapport au joueur
#define MIN_SPAWN_DISTANCE_FROM_PLAYER 100.0f

// Fonction helper pour trouver une position valide à au moins 100 pixels du joueur
static Vector2D GetValidSpawnPosition(float proposedX, float proposedY)
{
    Vector2D validPos = { proposedX, proposedY };
    
    if (player == NULL)
    {
        return validPos; // Si pas de joueur, retourner la position proposée
    }
    
    // Calculer la distance entre la position proposée et le joueur
    float dx = proposedX - player->position.x;
    float dy = proposedY - player->position.y;
    float distance = sqrtf(dx * dx + dy * dy);
    
    // Si la distance est suffisante, retourner la position proposée
    if (distance >= MIN_SPAWN_DISTANCE_FROM_PLAYER)
    {
        return validPos;
    }
    
    // Sinon, trouver une nouvelle position en s'éloignant du joueur
    // On va chercher une position dans différentes directions jusqu'à trouver une valide
    float angle = atan2f(dy, dx); // Angle actuel vers le joueur
    float newDistance = MIN_SPAWN_DISTANCE_FROM_PLAYER + 50.0f; // Distance de sécurité
    
    // Essayer plusieurs angles autour de la position actuelle
    for (int attempt = 0; attempt < 8; attempt++)
    {
        float testAngle = angle + (attempt * M_PI / 4.0f); // Tester 8 directions
        float testX = player->position.x + cosf(testAngle) * newDistance;
        float testY = player->position.y + sinf(testAngle) * newDistance;
        
        // Vérifier que la position est dans les limites de l'écran
        if (testX >= 0 && testX <= screenWidth && testY >= 0 && testY <= screenHeight)
        {
            validPos.x = testX;
            validPos.y = testY;
            return validPos;
        }
    }
    
    // Si aucune position valide n'a été trouvée, utiliser la position proposée quand même
    // (mieux que de ne pas spawner du tout)
    return validPos;
}

// =========================== Basic Enemies ===========================
int maxBigBasicEnemies = 3;
Enemy bigBasicEnemies[3];
bool bigBasicEnemiesSpawn[3] = { false };

int maxMidBasicEnemies = 6;
Enemy midBasicEnemies[6];
bool midBasicEnemiesSpawn[6] = { false };

int maxSmallBasicEnemies = 12;
Enemy smallBasicEnemies[12];
bool smallBasicEnemiesSpawn[12] = { false };

// =========================== Shooter Enemies ===========================
int maxBigShooterEnemy = 2;
Enemy bigShooterEnemies[2];
bool bigShooterEnemiesSpawn[2] = { false };

int maxMidShooterEnemy = 4;
Enemy midShooterEnemies[4];
bool midShooterEnemiesSpawn[4] = { false };

int maxSmallShooterEnemy = 8;
Enemy smallShooterEnemies[8];
bool smallShooterEnemiesSpawn[8] = { false };

// =========================== Follower Enemies ===========================
int maxBigFollowerEnemy = 2;
Enemy bigFollowerEnemies[2];
bool bigFollowerEnemiesSpawn[2] = { false };

int maxMidFollowerEnemy = 4;
Enemy midFollowerEnemies[4];
bool midFollowerEnemiesSpawn[4] = { false };

int maxSmallFollowerEnemy = 8;
Enemy smallFollowerEnemies[8];
bool smallFollowerEnemiesSpawn[8] = { false };

// =========================== Follower-Shooter Enemies ===========================
int maxBigFollowerShooterEnemy = 1;
Enemy bigFollowerShooterEnemies[1];
bool bigFollowerShooterEnemiesSpawn[1] = { false };

int maxMidFollowerShooterEnemy = 2;
Enemy midFollowerShooterEnemies[2];
bool midFollowerShooterEnemiesSpawn[2] = { false };

int maxSmallFollowerShooterEnemy = 4;
Enemy smallFollowerShooterEnemies[4];
bool smallFollowerShooterEnemiesSpawn[4] = { false };


// ===================== FONCTIONS DE HITBOX UTILISANT SPHERE2D =====================
typedef enum EnemyScaleType {
    SCALE_BIG,
    SCALE_MID,
    SCALE_SMALL
} EnemyScaleType;

static Sphere2D BuildEnemyHitboxGeneric(Enemy enemies[], int index, EnemyScaleType scaleType)
{
    if (enemies[index].size.x <= 0.0f || enemies[index].size.y <= 0.0f)
    {
        return Sphere2D_SetFromCenterRadius(Vector2D_SetFromComponents(0.0f, 0.0f), 0.0f);
    }

    float scale = 1.0f;
    switch (scaleType)
    {
    case SCALE_BIG: scale = BIG_SCALE; break;
    case SCALE_MID: scale = MID_SCALE; break;
    case SCALE_SMALL: scale = SMALL_SCALE; break;
    default: break;
    }

    float renderedSize = enemies[index].size.x * scale;
    Vector2D center = Vector2D_SetFromComponents(enemies[index].position.x, enemies[index].position.y);
    float radius = renderedSize * 0.5f * 0.8f;
    return Sphere2D_SetFromCenterRadius(center, radius);
}

Sphere2D GetBigBasicEnemyHitbox(int i)            { return BuildEnemyHitboxGeneric(bigBasicEnemies, i, SCALE_BIG); }
Sphere2D GetMidBasicEnemyHitbox(int i)            { return BuildEnemyHitboxGeneric(midBasicEnemies, i, SCALE_MID); }
Sphere2D GetSmallBasicEnemyHitbox(int i)          { return BuildEnemyHitboxGeneric(smallBasicEnemies, i, SCALE_SMALL); }
Sphere2D GetBigShooterEnemyHitbox(int i)          { return BuildEnemyHitboxGeneric(bigShooterEnemies, i, SCALE_BIG); }
Sphere2D GetMidShooterEnemyHitbox(int i)          { return BuildEnemyHitboxGeneric(midShooterEnemies, i, SCALE_MID); }
Sphere2D GetSmallShooterEnemyHitbox(int i)        { return BuildEnemyHitboxGeneric(smallShooterEnemies, i, SCALE_SMALL); }
Sphere2D GetBigFollowerEnemyHitbox(int i)         { return BuildEnemyHitboxGeneric(bigFollowerEnemies, i, SCALE_BIG); }
Sphere2D GetMidFollowerEnemyHitbox(int i)         { return BuildEnemyHitboxGeneric(midFollowerEnemies, i, SCALE_MID); }
Sphere2D GetSmallFollowerEnemyHitbox(int i)       { return BuildEnemyHitboxGeneric(smallFollowerEnemies, i, SCALE_SMALL); }
Sphere2D GetBigFollowerShooterEnemyHitbox(int i)  { return BuildEnemyHitboxGeneric(bigFollowerShooterEnemies, i, SCALE_BIG); }
Sphere2D GetMidFollowerShooterEnemyHitbox(int i)  { return BuildEnemyHitboxGeneric(midFollowerShooterEnemies, i, SCALE_MID); }
Sphere2D GetSmallFollowerShooterEnemyHitbox(int i){ return BuildEnemyHitboxGeneric(smallFollowerShooterEnemies, i, SCALE_SMALL); }

// ================================== Border Colision ==================================

void BorderEnemyCollision(Enemy* enemy)
{
    // Si sort par la gauche, r�appara�t � droite
    if (enemy->position.x < -50)
        enemy->position.x = screenWidth + 50;

    // Si sort par la droite, r�appara�t � gauche
    if (enemy->position.x > screenWidth + 50)
        enemy->position.x = -50;

    // Si sort par le haut, r�appara�t en bas
    if (enemy->position.y < 0)
        enemy->position.y = screenHeight + 50;

    // Si sort par le bas, r�appara�t en haut
    if (enemy->position.y > screenHeight + 50)
        enemy->position.y = 0;
}

// ================================== MotherShip ==================================
void MotherShipUpdate(GameAssets* assets, Collision* collision)
{
    if (!motherShipActive)
    {
        motherShipActive = true;
        MotherShipSpawn(assets);
    }

    if (motherShipSpawned)
    {
        MotherShipMovement(assets, collision);
        DrawMotherShip(assets); // Dessiner ici dans le bon ordre
    }
}


void MotherShipSpawn(GameAssets* assets)
{
    if (!motherShipSpawned)
    {
        motherShipSpawned = true;
        enemiesDropped = false;
        // currentSpawnIndex = 0;
        spawnTimer = 0.0f;
        // usedSpawnPoints = 0;  // Reset compteur de points

        motherShip[0].position.x = 140;
        motherShip[0].position.y = screenHeight;
        motherShip[0].size.x = 64.0f;
        motherShip[0].size.y = 64.0f;
        motherShip[0].speed = 7.0f;
        motherShip[0].angle = 0.0f;
    }
}

// Modifier aussi MotherShipMovement pour NE PAS dessiner le mothership ici
void MotherShipMovement(GameAssets* assets, Collision* collision)
{
    motherShip[0].speed = 7.0f;

    if (motherShip[0].position.y > -300)
    {
        motherShip[0].position.y -= motherShip[0].speed;

        // Incrémenter le timer de délai
        spawnDelayTimer += GetFrameTime();
        // Spawner SEULEMENT après 1 seconde
        if (spawnDelayTimer >= SPAWN_DELAY && currentSpawnIndex < 56)
        {

            // Spawner progressivement les points rouges seulement
            if (currentSpawnIndex < 56)
            {
                spawnTimer += GetFrameTime();

                if (spawnTimer >= spawnInterval)
                {
                    spawnTimer = 0.0f;
                    spawnPoints[currentSpawnIndex].x = GetRandomValue(50, screenWidth - 50);
                    spawnPoints[currentSpawnIndex].y = GetRandomValue(50, screenHeight - 50);
                    currentSpawnIndex++;
                }
            }
        }

        // NE PAS DESSINER ICI - Le dessin est fait dans UpdateSoloGameplay
        // DrawSpawnedPoints(); // ENLEVER
        // DrawTextureEx(assets->motherShipTexture, ...) // ENLEVER
    }
    else
    {
        // Le mothership a disparu
        if (!enemiesDropped)
        {
            SpawnAllEnemiesFromPoints();
            enemiesDropped = true;
            enemiesActive = true;
        }

        motherShip[0].size.x = 0.0f;
        motherShip[0].size.y = 0.0f;
        motherShipSpawned = false;
    }
}

// Nouvelle fonction pour dessiner le mothership dans le bon ordre
void DrawMotherShip(GameAssets* assets)
{
    if (motherShipSpawned && motherShip[0].position.y > -300)
    {
        DrawTextureEx(assets->motherShipTexture,
            (Vector2) {
            motherShip[0].position.x,
                motherShip[0].position.y
        },
            0, 1.0f, WHITE);
    }
}



// NOUVELLE FONCTION : Spawner tous les ennemis après la disparition du mothership
void SpawnAllEnemiesFromPoints()
{
    int pointIndex = 0;

    // Spawn 3 Big Basic
    for (int i = 0; i < 3; i++)
    {
        if (pointIndex >= 56) break;
        bigBasicEnemies[i].position.x = spawnPoints[pointIndex].x;
        bigBasicEnemies[i].position.y = spawnPoints[pointIndex].y;
        bigBasicEnemies[i].size.x = 50.0f;
        bigBasicEnemies[i].size.y = 50.0f;

        float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
        float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;
        bigBasicEnemies[i].velocity.x = cosf(angle) * speed;
        bigBasicEnemies[i].velocity.y = sinf(angle) * speed;

        bigBasicEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
        bigBasicEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) bigBasicEnemies[i].rotationSpeed *= -1;

        bigBasicEnemiesSpawn[i] = true;
        pointIndex++;
    }

    // Spawn 2 Big Shooter
    for (int i = 0; i < 2; i++)
    {
        if (pointIndex >= 56) break;
        bigShooterEnemies[i].position.x = spawnPoints[pointIndex].x;
        bigShooterEnemies[i].position.y = spawnPoints[pointIndex].y;
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
        pointIndex++;
    }

    // Spawn 2 Big Follower
    for (int i = 0; i < 2; i++)
    {
        if (pointIndex >= 56) break;
        bigFollowerEnemies[i].position.x = spawnPoints[pointIndex].x;
        bigFollowerEnemies[i].position.y = spawnPoints[pointIndex].y;
        bigFollowerEnemies[i].size.x = 50.0f;
        bigFollowerEnemies[i].size.y = 50.0f;
        bigFollowerEnemies[i].speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

        bigFollowerEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
        bigFollowerEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) bigFollowerEnemies[i].rotationSpeed *= -1;

        bigFollowerEnemiesSpawn[i] = true;
        pointIndex++;
    }

    // Spawn 1 Big Follower-Shooter
    for (int i = 0; i < 1; i++)
    {
        if (pointIndex >= 56) break;
        bigFollowerShooterEnemies[i].position.x = spawnPoints[pointIndex].x;
        bigFollowerShooterEnemies[i].position.y = spawnPoints[pointIndex].y;
        bigFollowerShooterEnemies[i].size.x = 50.0f;
        bigFollowerShooterEnemies[i].size.y = 50.0f;
        bigFollowerShooterEnemies[i].speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

        bigFollowerShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
        bigFollowerShooterEnemies[i].angle = 0.0f;
        if (GetRandomValue(0, 1) == 0) bigFollowerShooterEnemies[i].rotationSpeed *= -1;

        bigFollowerShooterEnemiesSpawn[i] = true;
        pointIndex++;
    }

    printf("Spawned %d big enemies from points\n", pointIndex);
}

void DrawSpawnedPoints()
{
    for (int i = 0; i < currentSpawnIndex; i++)
    {
        DrawCircle(spawnPoints[i].x, spawnPoints[i].y, 5, LIGHTGRAY);
    }
}
// ===================== FONCTION POUR OBTENIR UN POINT DE SPAWN =====================
Vector2D GetNextSpawnPoint()
{
    if (usedSpawnPoints >= 150) {
        // Reset si on dépasse
        usedSpawnPoints = 0;
    }

    Vector2D point;
    point.x = GetRandomValue(50, screenWidth - 50);
    point.y = GetRandomValue(50, screenHeight - 50);

    spawnPoints[usedSpawnPoints] = point;
    usedSpawnPoints++;

    return point;
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

// Nouvelle version de SpawnPoints qui DESSINE les positions sauvegard�es
void SpawnPoints()
{
    // Initialiser seulement au premier appel
    if (!spawnPointsInitialized)
    {
        InitSpawnPoints();
    }

    // Dessiner les points sauvegard�s
    for (int i = 0; i < 56; i++)
    {
        DrawCircle(spawnPoints[i].x, spawnPoints[i].y, 3, RED);
    }
}

*/

void UpdateEnemies(GameAssets* assets, Collision* collision)
{
    //DrawSpawnedPoints();

    // Si le mothership est encore l, ne pas update les ennemis
    if (motherShipSpawned) return;

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
// ================================== BasicEnemy ==================================

void UpdateBigBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigBasicEnemiesSpawn[i]) return;
    if (bigBasicEnemies[i].size.x <= 0.0f) return;

    BigBasicEnemyMovement(i);

    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { bigBasicEnemies[i].position.x, bigBasicEnemies[i].position.y,
                          bigBasicEnemies[i].size.x * 3.5f, bigBasicEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        bigBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->bigBasicEnemiesBulletCollision[i])
    {
        float x = bigBasicEnemies[i].position.x;
        float y = bigBasicEnemies[i].position.y;

        bigBasicEnemies[i].size.x = 0.0f;
        bigBasicEnemies[i].size.y = 0.0f;

        // Spawn 2 mid enemies à des points de spawn aléatoires
        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        MidBasicEnemySpawn(i * 2, point1.x, point1.y);
        MidBasicEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->bigBasicEnemiesBulletCollision[i] = false;
        printf("Big Basic Enemy %d destroyed, spawning mids %d and %d\n", i, i * 2, i * 2 + 1);
    }
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
    if (!midBasicEnemiesSpawn[i]) return;
    if (midBasicEnemies[i].size.x <= 0.0f) return;

    MidBasicEnemyMovement(i);

    const float scale = 2.0f;
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { midBasicEnemies[i].position.x, midBasicEnemies[i].position.y,
                          midBasicEnemies[i].size.x * scale, midBasicEnemies[i].size.y * scale };
    Vector2 origin = { (midBasicEnemies[i].size.x * scale) / 2.0f, (midBasicEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        midBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->midBasicEnemiesBulletCollision[i])
    {
        float x = midBasicEnemies[i].position.x;
        float y = midBasicEnemies[i].position.y;

        midBasicEnemies[i].size.x = 0.0f;
        midBasicEnemies[i].size.y = 0.0f;

        // Spawn 2 small enemies à des points de spawn aléatoires
        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        SmallBasicEnemySpawn(i * 2, point1.x, point1.y);
        SmallBasicEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->midBasicEnemiesBulletCollision[i] = false;
        printf("Mid Basic Enemy %d destroyed, spawning smalls %d and %d\n", i, i * 2, i * 2 + 1);
    }
}

void MidBasicEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    midBasicEnemiesSpawn[i] = true;
    midBasicEnemies[i].position.x = validPos.x;
    midBasicEnemies[i].position.y = validPos.y;
    midBasicEnemies[i].size.x = 60.0f;
    midBasicEnemies[i].size.y = 60.0f;

    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midBasicEnemies[i].velocity.x = cosf(Angle) * speed;
    midBasicEnemies[i].velocity.y = sinf(Angle) * speed;

    midBasicEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midBasicEnemies[i].rotationSpeed *= -1;
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

// === Small Enemy ===

void UpdateSmallBasicEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!smallBasicEnemiesSpawn[i]) return;
    if (smallBasicEnemies[i].size.x <= 0.0f) return;

    SmallBasicEnemyMovement(i);

    const float scale = 1.5f;
    Rectangle sourceRec = { 0, 0, assets->basicEnemyTexture.width, assets->basicEnemyTexture.height };
    Rectangle destRec = { smallBasicEnemies[i].position.x, smallBasicEnemies[i].position.y,
                          smallBasicEnemies[i].size.x * scale, smallBasicEnemies[i].size.y * scale };
    Vector2 origin = { (smallBasicEnemies[i].size.x * scale) / 2.0f, (smallBasicEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->basicEnemyTexture, sourceRec, destRec, origin,
        smallBasicEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->smallBasicEnemiesBulletCollision[i])
    {
        smallBasicEnemies[i].size.x = 0.0f;
        smallBasicEnemies[i].size.y = 0.0f;
        collision->smallBasicEnemiesBulletCollision[i] = false;
        printf("Small Basic Enemy %d destroyed\n", i);
    }
}

void SmallBasicEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    smallBasicEnemiesSpawn[i] = true;
    smallBasicEnemies[i].position.x = validPos.x;
    smallBasicEnemies[i].position.y = validPos.y;
    smallBasicEnemies[i].size.x = 50.0f;
    smallBasicEnemies[i].size.y = 50.0f;

    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 2.0f);
    smallBasicEnemies[i].velocity.x = cosf(Angle) * speed;
    smallBasicEnemies[i].velocity.y = sinf(Angle) * speed;

    smallBasicEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallBasicEnemies[i].rotationSpeed *= -1;
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

//////////////////////////////////      ShooterEnemy      ////////////////////////////////////////////////////////////////

void UpdateBigShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigShooterEnemiesSpawn[i]) return;
    if (bigShooterEnemies[i].size.x <= 0.0f) return;

    BigShooterEnemyMovement(i);

    if (player != NULL && bigShooterCooldowns[i] <= 0.0f)
    {
        // Tir depuis le centre du sprite de l'ennemi (utilise la position de l'ennemi qui est le centre)
        Vector2D enemyCenter = bigShooterEnemies[i].position;
        Vector2D playerPos = { player->position.x, player->position.y };
        FireEnemyBullet(enemyCenter, playerPos, assets);
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
    // L'angle pointe vers le joueur, mais le canon est en haut de l'image, donc on ajuste de -90°
    // Rotation supplémentaire de 180° pour la tête de l'ennemi
    float rotationAngle = (bigShooterEnemies[i].angle - M_PI / 2.0f) * 180.0f / M_PI + 180.0f;
    DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin, rotationAngle, WHITE);

    if (collision->bigShooterEnemiesBulletCollision[i])
    {
        float x = bigShooterEnemies[i].position.x;
        float y = bigShooterEnemies[i].position.y;

        bigShooterEnemies[i].size.x = 0.0f;
        bigShooterEnemies[i].size.y = 0.0f;

        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        MidShooterEnemySpawn(i * 2, point1.x, point1.y);
        MidShooterEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->bigShooterEnemiesBulletCollision[i] = false;
        bigShooterCooldowns[i] = 0.0f;
    }
}

void BigShooterEnemyMovement(int i)
{
    bigShooterEnemies[i].position.x += bigShooterEnemies[i].velocity.x;
    bigShooterEnemies[i].position.y += bigShooterEnemies[i].velocity.y;

    // Calculer l'angle vers le joueur
    if (player != NULL)
    {
        float dirX = player->position.x - bigShooterEnemies[i].position.x;
        float dirY = player->position.y - bigShooterEnemies[i].position.y;
        bigShooterEnemies[i].angle = atan2f(dirY, dirX);
    }

    BorderEnemyCollision(&bigShooterEnemies[i]);
}

// === Mid Shooter Enemy ===

void UpdateMidShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!midShooterEnemiesSpawn[i]) return;
    if (midShooterEnemies[i].size.x <= 0.0f) return;

    MidShooterEnemyMovement(i);

    if (player != NULL && midShooterCooldowns[i] <= 0.0f)
    {
        // Tir depuis le centre du sprite de l'ennemi (utilise la position de l'ennemi qui est le centre)
        Vector2D enemyCenter = midShooterEnemies[i].position;
        Vector2D playerPos = { player->position.x, player->position.y };
        FireEnemyBullet(enemyCenter, playerPos, assets);
        midShooterCooldowns[i] = MID_SHOOTER_FIRE_RATE;
    }

    if (midShooterCooldowns[i] > 0.0f)
    {
        midShooterCooldowns[i] -= 1.0f;
    }

    const float scale = 2.0f;
    Rectangle sourceRec = { 0, 0, assets->shooterEnemyTexture.width, assets->shooterEnemyTexture.height };
    Rectangle destRec = { midShooterEnemies[i].position.x, midShooterEnemies[i].position.y,
                          midShooterEnemies[i].size.x * scale, midShooterEnemies[i].size.y * scale };
    Vector2 origin = { (midShooterEnemies[i].size.x * scale) / 2.0f, (midShooterEnemies[i].size.y * scale) / 2.0f };
    // L'angle pointe vers le joueur, mais le canon est en haut de l'image, donc on ajuste de -90°
    // Rotation supplémentaire de 180° pour la tête de l'ennemi
    float rotationAngle = (midShooterEnemies[i].angle - M_PI / 2.0f) * 180.0f / M_PI + 180.0f;
    DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin, rotationAngle, WHITE);

    if (collision->midShooterEnemiesBulletCollision[i])
    {
        float x = midShooterEnemies[i].position.x;
        float y = midShooterEnemies[i].position.y;

        midShooterEnemies[i].size.x = 0.0f;
        midShooterEnemies[i].size.y = 0.0f;

        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        SmallShooterEnemySpawn(i * 2, point1.x, point1.y);
        SmallShooterEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->midShooterEnemiesBulletCollision[i] = false;
        midShooterCooldowns[i] = 0.0f;
    }
}

void MidShooterEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    midShooterEnemiesSpawn[i] = true;
    midShooterEnemies[i].position.x = validPos.x;
    midShooterEnemies[i].position.y = validPos.y;
    midShooterEnemies[i].size.x = 60.0f;
    midShooterEnemies[i].size.y = 60.0f;

    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * (2.5f - 1.5f);
    midShooterEnemies[i].velocity.x = cosf(Angle) * speed;
    midShooterEnemies[i].velocity.y = sinf(Angle) * speed;

    midShooterEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midShooterEnemies[i].rotationSpeed *= -1;
}

void MidShooterEnemyMovement(int i)
{
    midShooterEnemies[i].position.x += midShooterEnemies[i].velocity.x;
    midShooterEnemies[i].position.y += midShooterEnemies[i].velocity.y;
    
    // Calculer l'angle vers le joueur
    if (player != NULL)
    {
        float dirX = player->position.x - midShooterEnemies[i].position.x;
        float dirY = player->position.y - midShooterEnemies[i].position.y;
        midShooterEnemies[i].angle = atan2f(dirY, dirX);
    }

    BorderEnemyCollision(&midShooterEnemies[i]);
}

// === Small Shooter Enemy ===

void UpdateSmallShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!smallShooterEnemiesSpawn[i]) return;
    if (smallShooterEnemies[i].size.x <= 0.0f) return;

    SmallShooterEnemyMovement(i);

    if (player != NULL && smallShooterCooldowns[i] <= 0.0f)
    {
        // Tir depuis le centre du sprite de l'ennemi (utilise la position de l'ennemi qui est le centre)
        Vector2D enemyCenter = smallShooterEnemies[i].position;
        Vector2D playerPos = { player->position.x, player->position.y };
        FireEnemyBullet(enemyCenter, playerPos, assets);
        smallShooterCooldowns[i] = SMALL_SHOOTER_FIRE_RATE;
    }

    if (smallShooterCooldowns[i] > 0.0f)
    {
        smallShooterCooldowns[i] -= 1.0f;
    }

    const float scale = 1.5f;
    Rectangle sourceRec = { 0, 0, assets->shooterEnemyTexture.width, assets->shooterEnemyTexture.height };
    Rectangle destRec = { smallShooterEnemies[i].position.x, smallShooterEnemies[i].position.y,
                          smallShooterEnemies[i].size.x * scale, smallShooterEnemies[i].size.y * scale };
    Vector2 origin = { (smallShooterEnemies[i].size.x * scale) / 2.0f, (smallShooterEnemies[i].size.y * scale) / 2.0f };
    // L'angle pointe vers le joueur, mais le canon est en haut de l'image, donc on ajuste de -90°
    // Rotation supplémentaire de 180° pour la tête de l'ennemi
    float rotationAngle = (smallShooterEnemies[i].angle - M_PI / 2.0f) * 180.0f / M_PI + 180.0f;
    DrawTexturePro(assets->shooterEnemyTexture, sourceRec, destRec, origin, rotationAngle, WHITE);

    if (collision->smallShooterEnemiesBulletCollision[i])
    {
        smallShooterEnemies[i].size.x = 0.0f;
        smallShooterEnemies[i].size.y = 0.0f;
        collision->smallShooterEnemiesBulletCollision[i] = false;
        smallShooterCooldowns[i] = 0.0f;
    }
}

void SmallShooterEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    smallShooterEnemiesSpawn[i] = true;
    smallShooterEnemies[i].position.x = validPos.x;
    smallShooterEnemies[i].position.y = validPos.y;
    smallShooterEnemies[i].size.x = 50.0f;
    smallShooterEnemies[i].size.y = 50.0f;

    float Angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * (3.0f - 2.0f);
    smallShooterEnemies[i].velocity.x = cosf(Angle) * speed;
    smallShooterEnemies[i].velocity.y = sinf(Angle) * speed;

    smallShooterEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallShooterEnemies[i].rotationSpeed *= -1;
}

void SmallShooterEnemyMovement(int i)
{
    smallShooterEnemies[i].position.x += smallShooterEnemies[i].velocity.x;
    smallShooterEnemies[i].position.y += smallShooterEnemies[i].velocity.y;
    
    // Calculer l'angle vers le joueur
    if (player != NULL)
    {
        float dirX = player->position.x - smallShooterEnemies[i].position.x;
        float dirY = player->position.y - smallShooterEnemies[i].position.y;
        smallShooterEnemies[i].angle = atan2f(dirY, dirX);
    }

    BorderEnemyCollision(&smallShooterEnemies[i]);
}

//////////////////////////////////      FollowerEnemy      ////////////////////////////////////////////////////////////////

void UpdateBigFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigFollowerEnemiesSpawn[i]) return;
    if (bigFollowerEnemies[i].size.x <= 0.0f) return;

    BigFollowerEnemyMovement(i);

    Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
    Rectangle destRec = { bigFollowerEnemies[i].position.x, bigFollowerEnemies[i].position.y,
                          bigFollowerEnemies[i].size.x * 3.5f, bigFollowerEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
        bigFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->bigFollowerEnemiesBulletCollision[i])
    {
        float x = bigFollowerEnemies[i].position.x;
        float y = bigFollowerEnemies[i].position.y;

        bigFollowerEnemies[i].size.x = 0.0f;
        bigFollowerEnemies[i].size.y = 0.0f;

        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        MidFollowerEnemySpawn(i * 2, point1.x, point1.y);
        MidFollowerEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->bigFollowerEnemiesBulletCollision[i] = false;
    }
}

void BigFollowerEnemyMovement(int i)
{
    if (player == NULL) return;

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

// === Mid Follower Enemy ===

void UpdateMidFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!midFollowerEnemiesSpawn[i]) return;
    if (midFollowerEnemies[i].size.x <= 0.0f) return;

    MidFollowerEnemyMovement(i);

    const float scale = 2.0f;
    Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
    Rectangle destRec = { midFollowerEnemies[i].position.x, midFollowerEnemies[i].position.y,
                          midFollowerEnemies[i].size.x * scale, midFollowerEnemies[i].size.y * scale };
    Vector2 origin = { (midFollowerEnemies[i].size.x * scale) / 2.0f, (midFollowerEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
        midFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->midFollowerEnemiesBulletCollision[i])
    {
        float x = midFollowerEnemies[i].position.x;
        float y = midFollowerEnemies[i].position.y;

        midFollowerEnemies[i].size.x = 0.0f;
        midFollowerEnemies[i].size.y = 0.0f;

        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        SmallFollowerEnemySpawn(i * 2, point1.x, point1.y);
        SmallFollowerEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->midFollowerEnemiesBulletCollision[i] = false;
    }
}

void MidFollowerEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    midFollowerEnemiesSpawn[i] = true;
    midFollowerEnemies[i].position.x = validPos.x;
    midFollowerEnemies[i].position.y = validPos.y;
    midFollowerEnemies[i].size.x = 60.0f;
    midFollowerEnemies[i].size.y = 60.0f;
    midFollowerEnemies[i].speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

    midFollowerEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midFollowerEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midFollowerEnemies[i].rotationSpeed *= -1;
}

void MidFollowerEnemyMovement(int i)
{
    if (player == NULL) return;

    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - midFollowerEnemies[i].position.x;
    float dirY = targetY - midFollowerEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    midFollowerEnemies[i].velocity.x = dirX / dist * midFollowerEnemies[i].speed;
    midFollowerEnemies[i].velocity.y = dirY / dist * midFollowerEnemies[i].speed;

    midFollowerEnemies[i].position.x += midFollowerEnemies[i].velocity.x;
    midFollowerEnemies[i].position.y += midFollowerEnemies[i].velocity.y;

    midFollowerEnemies[i].angle += midFollowerEnemies[i].rotationSpeed;
    if (midFollowerEnemies[i].angle > 2 * M_PI) midFollowerEnemies[i].angle -= 2 * M_PI;
    if (midFollowerEnemies[i].angle < 0) midFollowerEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&midFollowerEnemies[i]);
}

// === Small Follower Enemy ===

void UpdateSmallFollowerEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!smallFollowerEnemiesSpawn[i]) return;
    if (smallFollowerEnemies[i].size.x <= 0.0f) return;

    SmallFollowerEnemyMovement(i);

    const float scale = 1.5f;
    Rectangle sourceRec = { 0, 0, assets->followerEnemyTexture.width, assets->followerEnemyTexture.height };
    Rectangle destRec = { smallFollowerEnemies[i].position.x, smallFollowerEnemies[i].position.y,
                          smallFollowerEnemies[i].size.x * scale, smallFollowerEnemies[i].size.y * scale };
    Vector2 origin = { (smallFollowerEnemies[i].size.x * scale) / 2.0f, (smallFollowerEnemies[i].size.y * scale) / 2.0f };
    DrawTexturePro(assets->followerEnemyTexture, sourceRec, destRec, origin,
        smallFollowerEnemies[i].angle * 180.0f / M_PI, WHITE);

    if (collision->smallFollowerEnemiesBulletCollision[i])
    {
        smallFollowerEnemies[i].size.x = 0.0f;
        smallFollowerEnemies[i].size.y = 0.0f;
        collision->smallFollowerEnemiesBulletCollision[i] = false;
    }
}

void SmallFollowerEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    smallFollowerEnemiesSpawn[i] = true;
    smallFollowerEnemies[i].position.x = validPos.x;
    smallFollowerEnemies[i].position.y = validPos.y;
    smallFollowerEnemies[i].size.x = 50.0f;
    smallFollowerEnemies[i].size.y = 50.0f;
    smallFollowerEnemies[i].speed = 2.5f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

    smallFollowerEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallFollowerEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallFollowerEnemies[i].rotationSpeed *= -1;
}

void SmallFollowerEnemyMovement(int i)
{
    if (player == NULL) return;

    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - smallFollowerEnemies[i].position.x;
    float dirY = targetY - smallFollowerEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    smallFollowerEnemies[i].velocity.x = dirX / dist * smallFollowerEnemies[i].speed;
    smallFollowerEnemies[i].velocity.y = dirY / dist * smallFollowerEnemies[i].speed;

    smallFollowerEnemies[i].position.x += smallFollowerEnemies[i].velocity.x;
    smallFollowerEnemies[i].position.y += smallFollowerEnemies[i].velocity.y;

    smallFollowerEnemies[i].angle += smallFollowerEnemies[i].rotationSpeed;
    if (smallFollowerEnemies[i].angle > 2 * M_PI) smallFollowerEnemies[i].angle -= 2 * M_PI;
    if (smallFollowerEnemies[i].angle < 0) smallFollowerEnemies[i].angle += 2 * M_PI;

    BorderEnemyCollision(&smallFollowerEnemies[i]);
}



//////////////////////////////////      Follower-Shooter Enemy      ////////////////////////////////////////////////////////////////

void UpdateBigFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!bigFollowerShooterEnemiesSpawn[i]) return;
    if (bigFollowerShooterEnemies[i].size.x <= 0.0f) return;

    BigFollowerShooterEnemyMovement(i);

    // Tirer vers le joueur
    if (player != NULL && bigFollowerShooterCooldowns[i] <= 0.0f)
    {
        // Tir depuis le centre du sprite de l'ennemi (utilise la position de l'ennemi qui est le centre)
        Vector2D enemyCenter = bigFollowerShooterEnemies[i].position;
        Vector2D playerPos = { player->position.x, player->position.y };
        FireEnemyBullet(enemyCenter, playerPos, assets);
        bigFollowerShooterCooldowns[i] = BIG_FOLLOWER_SHOOTER_FIRE_RATE;
    }

    if (bigFollowerShooterCooldowns[i] > 0.0f)
    {
        bigFollowerShooterCooldowns[i] -= 1.0f;
    }

    Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
    Rectangle destRec = { bigFollowerShooterEnemies[i].position.x, bigFollowerShooterEnemies[i].position.y,
                          bigFollowerShooterEnemies[i].size.x * 3.5f, bigFollowerShooterEnemies[i].size.y * 3.5f };
    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };
    // L'angle pointe vers le joueur, mais le canon est en haut de l'image, donc on ajuste de -90°
    // Rotation supplémentaire de 180° pour la tête de l'ennemi
    float rotationAngle = (bigFollowerShooterEnemies[i].angle - M_PI / 2.0f) * 180.0f / M_PI + 180.0f;
    DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin, rotationAngle, WHITE);

    if (collision->bigFollowerShooterEnemiesBulletCollision[i])
    {
        bigFollowerShooterEnemies[i].size.x = 0.0f;
        bigFollowerShooterEnemies[i].size.y = 0.0f;

        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        MidFollowerShooterEnemySpawn(i * 2, point1.x, point1.y);
        MidFollowerShooterEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->bigFollowerShooterEnemiesBulletCollision[i] = false;
        bigFollowerShooterCooldowns[i] = 0.0f;
    }
}

void BigFollowerShooterEnemyMovement(int i)
{
    if (player == NULL) return;

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

    // Calculer l'angle vers le joueur
    bigFollowerShooterEnemies[i].angle = atan2f(dirY, dirX);

    BorderEnemyCollision(&bigFollowerShooterEnemies[i]);
}

// === Mid Follower Shooter Enemy ===

void UpdateMidFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!midFollowerShooterEnemiesSpawn[i]) return;
    if (midFollowerShooterEnemies[i].size.x <= 0.0f) return;

    MidFollowerShooterEnemyMovement(i);

    if (player != NULL && midFollowerShooterCooldowns[i] <= 0.0f)
    {
        // Tir depuis le centre du sprite de l'ennemi (utilise la position de l'ennemi qui est le centre)
        Vector2D enemyCenter = midFollowerShooterEnemies[i].position;
        Vector2D playerPos = { player->position.x, player->position.y };
        FireEnemyBullet(enemyCenter, playerPos, assets);
        midFollowerShooterCooldowns[i] = MID_FOLLOWER_SHOOTER_FIRE_RATE;
    }

    if (midFollowerShooterCooldowns[i] > 0.0f)
    {
        midFollowerShooterCooldowns[i] -= 1.0f;
    }

    const float scale = 2.0f;
    Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
    Rectangle destRec = { midFollowerShooterEnemies[i].position.x, midFollowerShooterEnemies[i].position.y,
                          midFollowerShooterEnemies[i].size.x * scale, midFollowerShooterEnemies[i].size.y * scale };
    Vector2 origin = { (midFollowerShooterEnemies[i].size.x * scale) / 2.0f, (midFollowerShooterEnemies[i].size.y * scale) / 2.0f };
    // L'angle pointe vers le joueur, mais le canon est en haut de l'image, donc on ajuste de -90°
    // Rotation supplémentaire de 180° pour la tête de l'ennemi
    float rotationAngle = (midFollowerShooterEnemies[i].angle - M_PI / 2.0f) * 180.0f / M_PI + 180.0f;
    DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin, rotationAngle, WHITE);

    if (collision->midFollowerShooterEnemiesBulletCollision[i])
    {
        midFollowerShooterEnemies[i].size.x = 0.0f;
        midFollowerShooterEnemies[i].size.y = 0.0f;

        Vector2D point1 = GetNextSpawnPoint();
        Vector2D point2 = GetNextSpawnPoint();

        SmallFollowerShooterEnemySpawn(i * 2, point1.x, point1.y);
        SmallFollowerShooterEnemySpawn(i * 2 + 1, point2.x, point2.y);

        collision->midFollowerShooterEnemiesBulletCollision[i] = false;
        midFollowerShooterCooldowns[i] = 0.0f;
    }
}

void MidFollowerShooterEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    midFollowerShooterEnemiesSpawn[i] = true;
    midFollowerShooterEnemies[i].position.x = validPos.x;
    midFollowerShooterEnemies[i].position.y = validPos.y;
    midFollowerShooterEnemies[i].size.x = 60.0f;
    midFollowerShooterEnemies[i].size.y = 60.0f;
    midFollowerShooterEnemies[i].speed = 2.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

    midFollowerShooterEnemies[i].rotationSpeed = 0.03f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.08f - 0.03f);
    midFollowerShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) midFollowerShooterEnemies[i].rotationSpeed *= -1;
}

void MidFollowerShooterEnemyMovement(int i)
{
    if (player == NULL) return;

    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - midFollowerShooterEnemies[i].position.x;
    float dirY = targetY - midFollowerShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    midFollowerShooterEnemies[i].velocity.x = dirX / dist * midFollowerShooterEnemies[i].speed;
    midFollowerShooterEnemies[i].velocity.y = dirY / dist * midFollowerShooterEnemies[i].speed;

    midFollowerShooterEnemies[i].position.x += midFollowerShooterEnemies[i].velocity.x;
    midFollowerShooterEnemies[i].position.y += midFollowerShooterEnemies[i].velocity.y;

    // Calculer l'angle vers le joueur
    midFollowerShooterEnemies[i].angle = atan2f(dirY, dirX);

    BorderEnemyCollision(&midFollowerShooterEnemies[i]);
}

// === Small Follower Shooter Enemy ===

void UpdateSmallFollowerShooterEnemy(int i, GameAssets* assets, Collision* collision)
{
    if (!smallFollowerShooterEnemiesSpawn[i]) return;
    if (smallFollowerShooterEnemies[i].size.x <= 0.0f) return;

    SmallFollowerShooterEnemyMovement(i);

    if (player != NULL && smallFollowerShooterCooldowns[i] <= 0.0f)
    {
        // Tir depuis le centre du sprite de l'ennemi (utilise la position de l'ennemi qui est le centre)
        Vector2D enemyCenter = smallFollowerShooterEnemies[i].position;
        Vector2D playerPos = { player->position.x, player->position.y };
        FireEnemyBullet(enemyCenter, playerPos, assets);
        smallFollowerShooterCooldowns[i] = SMALL_FOLLOWER_SHOOTER_FIRE_RATE;
    }

    if (smallFollowerShooterCooldowns[i] > 0.0f)
    {
        smallFollowerShooterCooldowns[i] -= 1.0f;
    }

    const float scale = 1.5f;
    Rectangle sourceRec = { 0, 0, assets->followerShooterEnemyTexture.width, assets->followerShooterEnemyTexture.height };
    Rectangle destRec = { smallFollowerShooterEnemies[i].position.x, smallFollowerShooterEnemies[i].position.y,
                          smallFollowerShooterEnemies[i].size.x * scale, smallFollowerShooterEnemies[i].size.y * scale };
    Vector2 origin = { (smallFollowerShooterEnemies[i].size.x * scale) / 2.0f, (smallFollowerShooterEnemies[i].size.y * scale) / 2.0f };
    // L'angle pointe vers le joueur, mais le canon est en haut de l'image, donc on ajuste de -90°
    // Rotation supplémentaire de 180° pour la tête de l'ennemi
    float rotationAngle = (smallFollowerShooterEnemies[i].angle - M_PI / 2.0f) * 180.0f / M_PI + 180.0f;
    DrawTexturePro(assets->followerShooterEnemyTexture, sourceRec, destRec, origin, rotationAngle, WHITE);

    if (collision->smallFollowerShooterEnemiesBulletCollision[i])
    {
        smallFollowerShooterEnemies[i].size.x = 0.0f;
        smallFollowerShooterEnemies[i].size.y = 0.0f;
        collision->smallFollowerShooterEnemiesBulletCollision[i] = false;
        smallFollowerShooterCooldowns[i] = 0.0f;
    }
}

void SmallFollowerShooterEnemySpawn(int i, float parentX, float parentY)
{
    // Vérifier que la position est à au moins 100 pixels du joueur
    Vector2D validPos = GetValidSpawnPosition(parentX, parentY);
    
    smallFollowerShooterEnemiesSpawn[i] = true;
    smallFollowerShooterEnemies[i].position.x = validPos.x;
    smallFollowerShooterEnemies[i].position.y = validPos.y;
    smallFollowerShooterEnemies[i].size.x = 50.0f;
    smallFollowerShooterEnemies[i].size.y = 50.0f;
    smallFollowerShooterEnemies[i].speed = 2.5f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

    smallFollowerShooterEnemies[i].rotationSpeed = 0.04f + ((float)GetRandomValue(0, 100) / 100.0f) * (0.1f - 0.04f);
    smallFollowerShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) smallFollowerShooterEnemies[i].rotationSpeed *= -1;
}

void SmallFollowerShooterEnemyMovement(int i)
{
    if (player == NULL) return;

    float targetX = player->position.x;
    float targetY = player->position.y;

    float dirX = targetX - smallFollowerShooterEnemies[i].position.x;
    float dirY = targetY - smallFollowerShooterEnemies[i].position.y;
    float dist = sqrtf(dirX * dirX + dirY * dirY);
    if (dist < 0.001f) dist = 0.001f;

    smallFollowerShooterEnemies[i].velocity.x = dirX / dist * smallFollowerShooterEnemies[i].speed;
    smallFollowerShooterEnemies[i].velocity.y = dirY / dist * smallFollowerShooterEnemies[i].speed;

    smallFollowerShooterEnemies[i].position.x += smallFollowerShooterEnemies[i].velocity.x;
    smallFollowerShooterEnemies[i].position.y += smallFollowerShooterEnemies[i].velocity.y;

    // Calculer l'angle vers le joueur
    smallFollowerShooterEnemies[i].angle = atan2f(dirY, dirX);

    BorderEnemyCollision(&smallFollowerShooterEnemies[i]);
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

    return true; // Tous les ennemis spawn�s sont morts
}

// Fonction spawn pour Big Basic Enemy (manquante)
void BigBasicEnemySpawn(int i)
{
    Vector2D spawnPoint = GetNextSpawnPoint();
    bigBasicEnemiesSpawn[i] = true;
    bigBasicEnemies[i].position.x = spawnPoint.x;
    bigBasicEnemies[i].position.y = spawnPoint.y;
    bigBasicEnemies[i].size.x = 50.0f;
    bigBasicEnemies[i].size.y = 50.0f;

    float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;
    bigBasicEnemies[i].velocity.x = cosf(angle) * speed;
    bigBasicEnemies[i].velocity.y = sinf(angle) * speed;

    bigBasicEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
    bigBasicEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigBasicEnemies[i].rotationSpeed *= -1;

    printf("Big Basic Enemy %d spawned at (%.2f, %.2f)\n", i, spawnPoint.x, spawnPoint.y);
}


// Fonction spawn pour Big Shooter Enemy (manquante)
void BigShooterEnemySpawn(int i)
{
    Vector2D spawnPoint = GetNextSpawnPoint();
    bigShooterEnemiesSpawn[i] = true;
    bigShooterEnemies[i].position.x = spawnPoint.x;
    bigShooterEnemies[i].position.y = spawnPoint.y;
    bigShooterEnemies[i].size.x = 50.0f;
    bigShooterEnemies[i].size.y = 50.0f;

    float angle = ((float)GetRandomValue(0, 360)) * M_PI / 180.0f;
    float speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;
    bigShooterEnemies[i].velocity.x = cosf(angle) * speed;
    bigShooterEnemies[i].velocity.y = sinf(angle) * speed;

    bigShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
    bigShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigShooterEnemies[i].rotationSpeed *= -1;

    bigShooterCooldowns[i] = BIG_SHOOTER_FIRE_RATE;
    printf("Big Shooter Enemy %d spawned at (%.2f, %.2f)\n", i, spawnPoint.x, spawnPoint.y);
}

// Fonction spawn pour Big Follower Enemy (manquante)
void BigFollowerEnemySpawn(int i)
{
    Vector2D spawnPoint = GetNextSpawnPoint();
    bigFollowerEnemiesSpawn[i] = true;
    bigFollowerEnemies[i].position.x = spawnPoint.x;
    bigFollowerEnemies[i].position.y = spawnPoint.y;
    bigFollowerEnemies[i].size.x = 50.0f;
    bigFollowerEnemies[i].size.y = 50.0f;
    bigFollowerEnemies[i].speed = 1.5f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

    bigFollowerEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
    bigFollowerEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigFollowerEnemies[i].rotationSpeed *= -1;

    printf("Big Follower Enemy %d spawned at (%.2f, %.2f)\n", i, spawnPoint.x, spawnPoint.y);
}

// Fonction spawn pour Big Follower Shooter Enemy (manquante)
void BigFollowerShooterEnemySpawn(int i)
{
    Vector2D spawnPoint = GetNextSpawnPoint();
    bigFollowerShooterEnemiesSpawn[i] = true;
    bigFollowerShooterEnemies[i].position.x = spawnPoint.x;
    bigFollowerShooterEnemies[i].position.y = spawnPoint.y;
    bigFollowerShooterEnemies[i].size.x = 50.0f;
    bigFollowerShooterEnemies[i].size.y = 50.0f;
    bigFollowerShooterEnemies[i].speed = 1.0f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.5f;

    bigFollowerShooterEnemies[i].rotationSpeed = 0.02f + ((float)GetRandomValue(0, 100) / 100.0f) * 0.03f;
    bigFollowerShooterEnemies[i].angle = 0.0f;
    if (GetRandomValue(0, 1) == 0) bigFollowerShooterEnemies[i].rotationSpeed *= -1;

    bigFollowerShooterCooldowns[i] = BIG_FOLLOWER_SHOOTER_FIRE_RATE;
    printf("Big Follower Shooter Enemy %d spawned at (%.2f, %.2f)\n", i, spawnPoint.x, spawnPoint.y);
}