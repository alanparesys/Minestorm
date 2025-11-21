#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "explosion.h"
#include "bonus.h"
#include "collision2D.h"
#include <math.h>
#include <stdio.h>
#include <raylib.h>

extern Ship* player;
extern int lifeNumber;
extern int score;

Bonus bonus[MAX_BONUS];
float bonusSpawnTimer = 0.0f;
float shieldTimer = 0.0f;
bool bonusActive = false;
bool shieldActive = false;
float rapidFireCountDown = 3.0f;
bool rapidFireActive = false;

float shipBulletCooldown;

#define SHIP_BULLET_COOLDOWN_RAPID  0.10f
#define SHIP_BULLET_COOLDOWN_NORMAL 0.35f

// Les échelles d'explosion pour les types d'ennemis
#define EXPLOSION_SCALE_BIG   2.5f
#define EXPLOSION_SCALE_MID   1.8f
#define EXPLOSION_SCALE_SMALL 1.3f

// Initialisation du système de bonus
void InitBonus(void)
{
    for (int i = 0; i < MAX_BONUS; i++)
    {
        bonus[i].size.x = 0.0f;
        bonus[i].size.y = 0.0f;
        bonus[i].position.x = 0.0f;
        bonus[i].position.y = 0.0f;
        bonus[i].active = false;
        bonus[i].type = 0;
        bonus[i].collected = false;
    }
    bonusSpawnTimer = 0.0f;
    bonusActive = false;
    // --- Réinitialisation des états de bonus du joueur ---
    rapidFireActive = false;
    shieldActive = false;
    // Les compteurs sont initialisés
    rapidFireCountDown = 3.0f;
    shieldTimer = 0.0f;

    shipBulletCooldown = SHIP_BULLET_COOLDOWN_NORMAL;
}

// Spawn d'un nouveau bonus à une position aléatoire
void SpawnBonus(void)
{
    for (int i = 0; i < MAX_BONUS; i++)
    {
        if (!bonus[i].active)
        {
            // Choisir un type de bonus aléatoire (0-3)
            int bonusType = GetRandomValue(0, 3);
            bonus[i].type = bonusType;

            // Position aléatoire sur l'écran
            bonus[i].position.x = (float)GetRandomValue(100, 1080 - 100);
            bonus[i].position.y = (float)GetRandomValue(150, 1300 - 100);
            bonus[i].size.x = 600.0f; // Taille de la texture
            bonus[i].size.y = 600.0f;
            bonus[i].active = true;
            bonus[i].collected = false;
            bonusActive = true;

            // Afficher le type de bonus spawné
            const char* bonusNames[] = { "Vie +1", "Bouclier", "Tir Rapide", "Kill 3 Enemies" };
            printf("Bonus spawned: %s at (%.2f, %.2f)\n",
                bonusNames[bonusType], bonus[i].position.x, bonus[i].position.y);
            break;
        }
    }
}

// Update du système de bonus
void UpdateBonus(GameAssets* assets)
{
    // Si aucun bonus n'est actif, démarrer le timer
    if (!bonusActive)
    {
        bonusSpawnTimer += GetFrameTime();

        // Spawn après 5 secondes
        if (bonusSpawnTimer >= 5.0f)
        {
            SpawnBonus();
            bonusSpawnTimer = 0.0f;
        }
    }

    // Vérifier les collisions avec le joueur
    CheckBonusPlayerCollision(assets);
}

// Dessin du bonus
void DrawBonus(GameAssets* assets)
{
    for (int i = 0; i < MAX_BONUS; i++)
    {
        if (bonus[i].active)
        {
            // Échelle pour rendre le bonus plus petit
            float scale = 0.1f;

            // Choisir la texture en fonction du type de bonus
            Texture2D* bonusTexture = NULL;

            switch (bonus[i].type)
            {
            case 0: // +1 vie - Étoile rouge
                bonusTexture = &assets->redStarTexture;
                break;
            case 1: // Bouclier - Étoile bleue
                bonusTexture = &assets->blueStarTexture;
                break;
            case 2: // Tir rapide - Étoile jaune
                bonusTexture = &assets->yellowStarTexture;
                break;
            case 3: // Kill ennemis - Étoile verte
                bonusTexture = &assets->greenStarTexture;
                break;
            default:
                break;
            }

            if (bonusTexture != NULL)
            {
                Rectangle sourceRec = { 0, 0, (float)bonusTexture->width, (float)bonusTexture->height };
                Rectangle destRec = {
                    bonus[i].position.x,
                    bonus[i].position.y,
                    bonus[i].size.x * scale,
                    bonus[i].size.y * scale
                };
                Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

                DrawTexturePro(*bonusTexture, sourceRec, destRec, origin, 0.0f, WHITE);
            }
        }
    }
}

// Vérifier la collision avec le joueur
void CheckBonusPlayerCollision(GameAssets* assets)
{
    if (player == NULL) return;
    if (motherShipSpawned) return; // Pas de collision pendant le mothership

    for (int i = 0; i < MAX_BONUS; i++)
    {
        if (!bonus[i].active) continue;

        // Créer les sphères de collision
        Sphere2D bonusSphere = GetBonusHitbox(i);

        // Vérifier que la hitbox est valide
        if (bonusSphere.radius <= 0.0f) continue;

        float playerRadius = fmaxf(player->size.x, player->size.y) * 0.6f;
        Sphere2D playerSphere = Sphere2D_SetFromCenterRadius(player->position, playerRadius);

        // Vérifier la collision
        if (Collision_CircleCircle(bonusSphere, playerSphere))
        {
            bonus[i].collected = true;
            // Le joueur a récupéré le bonus
            printf("Bonus collected!\n");

            GiveBonus(assets, i);

            // Jouer un son
            PlaySound(assets->selected);

            // Désactiver le bonus
            bonus[i].active = false;
            bonus[i].size.x = 0.0f;
            bonus[i].size.y = 0.0f;
            bonusActive = false;
            bonus[i].collected = false;
            bonusSpawnTimer = 0.0f; // Redémarrer le timer

            break;
        }
    }
}

// Obtenir la hitbox du bonus
Sphere2D GetBonusHitbox(int i)
{
    return BuildBonusHitboxGeneric(bonus, i);
}

Sphere2D BuildBonusHitboxGeneric(Bonus* bonus, int i)
{
    if (!bonus[i].active || bonus[i].size.x <= 0.0f || bonus[i].size.y <= 0.0f)
    {
        return Sphere2D_SetFromCenterRadius(Vector2D_SetFromComponents(0.0f, 0.0f), 0.0f);
    }

    float scale = 0.1f; // Même échelle que le rendu
    float renderedSize = bonus[i].size.x * scale;
    Vector2D center = bonus[i].position;
    float radius = renderedSize * 0.5f;

    return Sphere2D_SetFromCenterRadius(center, radius);
}

// Appliquer l'effet du bonus au joueur
void GiveBonus(GameAssets* assets, int i)
{
    switch (bonus[i].type)
    {
    case 0: // +1 vie
    {
        lifeNumber++;
        printf("Life +1! Total lives: %d\n", lifeNumber);
        break;
    }
    case 1: // Bouclier
    {
        // Activer le bouclier du joueur
        shieldActive = true;
        shieldTimer = 0.0f; // Réinitialiser le timer
        printf("Shield activated for 3 seconds!\n");
        break;
    }
    case 2: // Tir rapide
    {
        rapidFireActive = true;
        rapidFireCountDown = 3.0f; // 3 secondes
        shipBulletCooldown = SHIP_BULLET_COOLDOWN_RAPID; // ACTIVER le tir rapide
        printf("Rapid fire activated for 3 seconds! Cooldown: %.2f\n", shipBulletCooldown);
        break;
    }
    case 3: // Kill 3 ennemis aléatoires
    {
        printf("Bonus collected! Killing 3 random enemies\n");
        KillRandomEnemies(assets);
        PlaySound(assets->explosionSound);
        break;
    }
    default:
    {
        break;
    }
    }
}

// Fonction pour dessiner le bouclier
void DrawShield(void)
{
    if (player == NULL) return;

    if (shieldActive)
    {
        // Calculer le rayon du bouclier (légèrement plus grand que le vaisseau)
        float shieldRadius = fmaxf(player->size.x, player->size.y) * 0.8f;

        // Dessiner un cercle vide bleu autour du joueur
        DrawCircleLines((int)player->position.x, (int)player->position.y, shieldRadius, BLUE);

        // Dessiner un deuxième cercle légèrement plus grand pour un effet plus visible
        DrawCircleLines((int)player->position.x, (int)player->position.y, shieldRadius + 2.0f, SKYBLUE);
    }
}

// Fonction pour mettre à jour le shield timer
void UpdateShield(void)
{
    if (shieldActive)
    {
        shieldTimer += GetFrameTime();

        // Désactiver le bouclier après 3 secondes
        if (shieldTimer >= 3.0f)
        {
            shieldActive = false;
            shieldTimer = 0.0f;
            printf("Shield deactivated!\n");
        }
    }
}

void KillRandomEnemies(GameAssets* assets)
{
    int enemiesKilled = 0;
    int maxAttempts = 100; // Éviter une boucle infinie s'il y a très peu d'ennemis
    int attempts = 0;

    // Structure pour regrouper les données de chaque type d'ennemi
    typedef struct {
        Enemy* enemies;
        int maxCount;
        bool* spawnFlags;
        int killScore; // Score pour l'ennemi tué
    } EnemyGroupInfo;

    // Définition de tous les groupes d'ennemis
    EnemyGroupInfo groups[] = {
        {bigBasicEnemies, maxBigBasicEnemies, bigBasicEnemiesSpawn, 1000},
        {midBasicEnemies, maxMidBasicEnemies, midBasicEnemiesSpawn, 800},
        {smallBasicEnemies, maxSmallBasicEnemies, smallBasicEnemiesSpawn, 500},

        {bigShooterEnemies, maxBigShooterEnemy, bigShooterEnemiesSpawn, 1500},
        {midShooterEnemies, maxMidShooterEnemy, midShooterEnemiesSpawn, 1200},
        {smallShooterEnemies, maxSmallShooterEnemy, smallShooterEnemiesSpawn, 700},

        {bigFollowerEnemies, maxBigFollowerEnemy, bigFollowerEnemiesSpawn, 1500},
        {midFollowerEnemies, maxMidFollowerEnemy, midFollowerEnemiesSpawn, 1200},
        {smallFollowerEnemies, maxSmallFollowerEnemy, smallFollowerEnemiesSpawn, 700},

        {bigFollowerShooterEnemies, maxBigFollowerShooterEnemy, bigFollowerShooterEnemiesSpawn, 2000},
        {midFollowerShooterEnemies, maxMidFollowerShooterEnemy, midFollowerShooterEnemiesSpawn, 1800},
        {smallFollowerShooterEnemies, maxSmallFollowerShooterEnemy, smallFollowerShooterEnemiesSpawn, 1000}
    };

    int numGroups = sizeof(groups) / sizeof(groups[0]);

    while (enemiesKilled < 3 && attempts < maxAttempts)
    {
        attempts++;

        // 1. Choisir un groupe d'ennemis aléatoire
        int groupIndex = GetRandomValue(0, numGroups - 1);
        EnemyGroupInfo* group = &groups[groupIndex];

        // 2. Choisir un index aléatoire dans ce groupe
        if (group->maxCount == 0) continue;
        int enemyIndex = GetRandomValue(0, group->maxCount - 1);

        // 3. Vérifier si l'ennemi est actif (flag de spawn true ET taille > 0)
        if (group->spawnFlags[enemyIndex] && group->enemies[enemyIndex].size.x > 0.0f)
        {
            Enemy* enemy = &group->enemies[enemyIndex];

            // 4. 'Tuer' l'ennemi
            group->spawnFlags[enemyIndex] = false; // Désactiver le flag
            enemy->size.x = 0.0f; // Remettre la taille à 0 pour le rendre inactif
            enemy->size.y = 0.0f;

            // 5. Créer l'Explosion et Mettre à jour le Score
            float explosionScale = EXPLOSION_SCALE_SMALL;
            if (group->killScore >= 1500) explosionScale = EXPLOSION_SCALE_BIG;
            else if (group->killScore >= 1000) explosionScale = EXPLOSION_SCALE_MID;

            SpawnExplosion(enemy->position, explosionScale);
            score += group->killScore;

            enemiesKilled++;
        }
    }

    if (enemiesKilled < 3)
    {
        printf("Warning: Could only kill %d enemies due to insufficient active enemies.\n", enemiesKilled);
    }
}