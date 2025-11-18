#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "explosion.h"
#include <math.h>
#include <stdio.h>

#define EXPLOSION_SCALE_BIG   2.5f
#define EXPLOSION_SCALE_MID   1.8f
#define EXPLOSION_SCALE_SMALL 1.3f

ShipBullet shipBullets[MAX_BULLETS];
EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];

int score = 0;
int actualLevel = 7;
int lifeNumber = 3;

// ======================== BULLETS DU JOUEUR ========================

void InitBullets(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        shipBullets[i].active = false;
        shipBullets[i].radius = BULLET_RADIUS;
        shipBullets[i].position = (Vector2){ 0.0f, 0.0f };
        shipBullets[i].velocity = (Vector2){ 0.0f, 0.0f };
    }
}

void FireBullet(Vector2D startPos, float angle)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active)
        {
            shipBullets[i].active = true;
            shipBullets[i].position = (Vector2){ startPos.x, startPos.y };
            shipBullets[i].velocity = (Vector2){ cosf(angle) * BULLET_SPEED, sinf(angle) * BULLET_SPEED };
            shipBullets[i].radius = BULLET_RADIUS;
            return;
        }
    }
}

void UpdateBullets(GameAssets* assets, Collision* collision)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active) continue;
      
        // Mouvement
        shipBullets[i].position.x += shipBullets[i].velocity.x;
        shipBullets[i].position.y += shipBullets[i].velocity.y;

        // V�rifier si hors �cran
        if (shipBullets[i].position.x < -50 || shipBullets[i].position.x > 1080 + 50 ||
            shipBullets[i].position.y < -50 || shipBullets[i].position.y > 1300 + 50)
        {
            shipBullets[i].active = false;
            continue;
        }

        // Note: Les collisions bullet-bullet sont vérifiées dans CheckBulletBulletCollisions() après les updates

        // Affichage
        if (assets != NULL)
        {
            Rectangle src = { 0, 0, (float)assets->bulletTexture.width, (float)assets->bulletTexture.height };
            Rectangle dst = { shipBullets[i].position.x, shipBullets[i].position.y, shipBullets[i].radius * 2.0f, shipBullets[i].radius * 2.0f };
            Vector2 origin = { shipBullets[i].radius, shipBullets[i].radius };
            DrawTexturePro(assets->bulletTexture, src, dst, origin, 0.0f, WHITE);
        }
        else
        {
            DrawCircle((int)shipBullets[i].position.x, (int)shipBullets[i].position.y, shipBullets[i].radius, WHITE);
        }

        if (!enemiesActive) continue;

        // ===== Collision BigBasicEnemies =====
        for (int j = 0; j < maxBigBasicEnemies; j++)
        {
            if (bigBasicEnemies[j].size.x <= 0.0f || bigBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigBasicEnemies[j].position.x, bigBasicEnemies[j].position.y };
            float enemyRadius = (bigBasicEnemies[j].size.x * 3.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
                printf("Hit Big Basic Enemy %d!\n", j);

                Vector2D explosionPos = { bigBasicEnemies[j].position.x, bigBasicEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_BIG);
                score += 100;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision MidBasicEnemies =====
        for (int j = 0; j < maxMidBasicEnemies; j++)
        {
            if (midBasicEnemies[j].size.x <= 0.0f || midBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { midBasicEnemies[j].position.x, midBasicEnemies[j].position.y };
            float enemyRadius = (midBasicEnemies[j].size.x * 2.0f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->midBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
                printf("Hit Mid Basic Enemy %d!\n", j);

                Vector2D explosionPos = { midBasicEnemies[j].position.x, midBasicEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_MID);
                score += 135;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision SmallBasicEnemies =====
        for (int j = 0; j < maxSmallBasicEnemies; j++)
        {
            if (smallBasicEnemies[j].size.x <= 0.0f || smallBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { smallBasicEnemies[j].position.x, smallBasicEnemies[j].position.y };
            float enemyRadius = (smallBasicEnemies[j].size.x * 1.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->smallBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
                printf("Hit Small Basic Enemy %d!\n", j);

                Vector2D explosionPos = { smallBasicEnemies[j].position.x, smallBasicEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_SMALL);
                score += 200;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision BigShooterEnemies =====
        for (int j = 0; j < maxBigShooterEnemy; j++)
        {
            if (bigShooterEnemies[j].size.x <= 0.0f || bigShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigShooterEnemies[j].position.x, bigShooterEnemies[j].position.y };
            float enemyRadius = (bigShooterEnemies[j].size.x * 3.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { bigShooterEnemies[j].position.x, bigShooterEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_BIG);
                score += 325;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision MidShooterEnemies =====
        for (int j = 0; j < maxMidShooterEnemy; j++)
        {
            if (midShooterEnemies[j].size.x <= 0.0f || midShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { midShooterEnemies[j].position.x, midShooterEnemies[j].position.y };
            float enemyRadius = (midShooterEnemies[j].size.x * 2.0f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->midShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { midShooterEnemies[j].position.x, midShooterEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_MID);
                score += 360;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision SmallShooterEnemies =====
        for (int j = 0; j < maxSmallShooterEnemy; j++)
        {
            if (smallShooterEnemies[j].size.x <= 0.0f || smallShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { smallShooterEnemies[j].position.x, smallShooterEnemies[j].position.y };
            float enemyRadius = (smallShooterEnemies[j].size.x * 1.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->smallShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { smallShooterEnemies[j].position.x, smallShooterEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_SMALL);
                score += 425;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision BigFollowerEnemies =====
        for (int j = 0; j < maxBigFollowerEnemy; j++)
        {
            if (bigFollowerEnemies[j].size.x <= 0.0f || bigFollowerEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigFollowerEnemies[j].position.x, bigFollowerEnemies[j].position.y };
            float enemyRadius = (bigFollowerEnemies[j].size.x * 3.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigFollowerEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { bigFollowerEnemies[j].position.x, bigFollowerEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_BIG);
                score += 500;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision MidFollowerEnemies =====
        for (int j = 0; j < maxMidFollowerEnemy; j++)
        {
            if (midFollowerEnemies[j].size.x <= 0.0f || midFollowerEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { midFollowerEnemies[j].position.x, midFollowerEnemies[j].position.y };
            float enemyRadius = (midFollowerEnemies[j].size.x * 2.0f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->midFollowerEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { midFollowerEnemies[j].position.x, midFollowerEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_MID);
                score += 535;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision SmallFollowerEnemies =====
        for (int j = 0; j < maxSmallFollowerEnemy; j++)
        {
            if (smallFollowerEnemies[j].size.x <= 0.0f || smallFollowerEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { smallFollowerEnemies[j].position.x, smallFollowerEnemies[j].position.y };
            float enemyRadius = (smallFollowerEnemies[j].size.x * 1.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->smallFollowerEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { smallFollowerEnemies[j].position.x, smallFollowerEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_SMALL);
                score += 600;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision BigFollowerShooterEnemies =====
        for (int j = 0; j < maxBigFollowerShooterEnemy; j++)
        {
            if (bigFollowerShooterEnemies[j].size.x <= 0.0f || bigFollowerShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigFollowerShooterEnemies[j].position.x, bigFollowerShooterEnemies[j].position.y };
            float enemyRadius = (bigFollowerShooterEnemies[j].size.x * 3.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigFollowerShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { bigFollowerShooterEnemies[j].position.x, bigFollowerShooterEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_BIG);
                score += 750;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision MidFollowerShooterEnemies =====
        for (int j = 0; j < maxMidFollowerShooterEnemy; j++)
        {
            if (midFollowerShooterEnemies[j].size.x <= 0.0f || midFollowerShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { midFollowerShooterEnemies[j].position.x, midFollowerShooterEnemies[j].position.y };
            float enemyRadius = (midFollowerShooterEnemies[j].size.x * 2.0f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->midFollowerShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { midFollowerShooterEnemies[j].position.x, midFollowerShooterEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_MID);
                score += 785;
                break;
            }
        }

        if (!shipBullets[i].active) continue;

        // ===== Collision SmallFollowerShooterEnemies =====
        for (int j = 0; j < maxSmallFollowerShooterEnemy; j++)
        {
            if (smallFollowerShooterEnemies[j].size.x <= 0.0f || smallFollowerShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { smallFollowerShooterEnemies[j].position.x, smallFollowerShooterEnemies[j].position.y };
            float enemyRadius = (smallFollowerShooterEnemies[j].size.x * 1.5f) / 2.0f;

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->smallFollowerShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;

                Vector2D explosionPos = { smallFollowerShooterEnemies[j].position.x, smallFollowerShooterEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_SMALL);
                score += 850;
                break;
            }
        }
    }
}

// ======================== BULLETS ENNEMIES ========================

void InitEnemyBullets(void)
{
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        enemyBullets[i].active = false;
        enemyBullets[i].radius = BULLET_RADIUS;
        enemyBullets[i].position = (Vector2){ 0.0f, 0.0f };
        enemyBullets[i].velocity = (Vector2){ 0.0f, 0.0f };
    }
}

void FireEnemyBullet(Vector2D startPos, Vector2D targetPos, GameAssets* assets)
{
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (!enemyBullets[i].active)
        {
            enemyBullets[i].active = true;
            enemyBullets[i].position = (Vector2){ startPos.x, startPos.y };

            // Calculer la direction vers la cible
            float dirX = targetPos.x - startPos.x;
            float dirY = targetPos.y - startPos.y;
            float dist = sqrtf(dirX * dirX + dirY * dirY);

            if (dist > 0.001f)
            {
                enemyBullets[i].velocity.x = (dirX / dist) * ENEMY_BULLET_SPEED;
                enemyBullets[i].velocity.y = (dirY / dist) * ENEMY_BULLET_SPEED;
            }
            else
            {
                enemyBullets[i].velocity.x = 0.0f;
                enemyBullets[i].velocity.y = ENEMY_BULLET_SPEED;
            }

            enemyBullets[i].radius = BULLET_RADIUS;
            PlaySound(assets->laser2);
            return;
        }
    }
}

// Fonction pour vérifier les collisions entre bullets du joueur et bullets ennemis
void CheckBulletBulletCollisions(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active) continue;

        for (int j = 0; j < MAX_ENEMY_BULLETS; j++)
        {
            if (!enemyBullets[j].active) continue;

            // Calculer la distance entre les deux bullets
            float dx = shipBullets[i].position.x - enemyBullets[j].position.x;
            float dy = shipBullets[i].position.y - enemyBullets[j].position.y;
            float distance = sqrtf(dx * dx + dy * dy);

            // Rayon de collision = somme des rayons des deux bullets
            float collisionRadius = shipBullets[i].radius + enemyBullets[j].radius;

            // Si collision détectée
            if (distance < collisionRadius)
            {
                // Désactiver les deux bullets
                shipBullets[i].active = false;
                enemyBullets[j].active = false;

                // Créer une explosion à la position de collision (milieu entre les deux bullets)
                Vector2D explosionPos = {
                    (shipBullets[i].position.x + enemyBullets[j].position.x) / 2.0f,
                    (shipBullets[i].position.y + enemyBullets[j].position.y) / 2.0f
                };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_SMALL);
                
                // Debug message
                printf("*** COLLISION BULLET-BULLET DETECTEE! ***\n");
                printf("Explosion creee a (%.1f, %.1f)\n", explosionPos.x, explosionPos.y);
                
                break; // Sortir de la boucle des bullets ennemis
            }
        }
    }
}

void UpdateEnemyBullets(GameAssets* assets, Ship* player)
{
    // DEBUG
    if (player == NULL) {
        printf("ERREUR : player est NULL dans UpdateEnemyBullets !\n");
        return;
    }
    printf("PlayerBullet position: (%.1f, %.1f)\n", player->position.x, player->position.y);

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (!enemyBullets[i].active) continue;

        // Mouvement
        enemyBullets[i].position.x += enemyBullets[i].velocity.x;
        enemyBullets[i].position.y += enemyBullets[i].velocity.y;

        // V�rifier si hors �cran
        if (enemyBullets[i].position.x < -50 || enemyBullets[i].position.x > 1080 + 50 ||
            enemyBullets[i].position.y < -50 || enemyBullets[i].position.y > 1300 + 50)
        {
            enemyBullets[i].active = false;
            continue;
        }

        // Note: Les collisions bullet-bullet sont vérifiées dans UpdateBullets() pour éviter les doubles vérifications

        // Affichage (couleur rouge pour les distinguer)
        if (assets != NULL)
        {
            Rectangle src = { 0, 0, (float)assets->bulletTexture.width, (float)assets->bulletTexture.height };
            Rectangle dst = { enemyBullets[i].position.x, enemyBullets[i].position.y,
                            enemyBullets[i].radius * 2.0f, enemyBullets[i].radius * 2.0f };
            Vector2 origin = { enemyBullets[i].radius, enemyBullets[i].radius };
            DrawTexturePro(assets->bulletTexture, src, dst, origin, 0.0f, RED);
        }
        else
        {
            DrawCircle((int)enemyBullets[i].position.x, (int)enemyBullets[i].position.y,
                enemyBullets[i].radius, RED);
        }

        // Collision avec le joueur
        if (player != NULL)
        {
            // Calculer la hitbox du joueur (approximation circulaire)
            float playerRadius = (player->size.x < player->size.y ? player->size.x : player->size.y) / 2.0f;

            float dx = enemyBullets[i].position.x - player->position.x;
            float dy = enemyBullets[i].position.y - player->position.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < enemyBullets[i].radius + playerRadius)
            {
                enemyBullets[i].active = false;
                lifeNumber = lifeNumber--; // -1 life
                printf("Player hit by enemy bullet! GAME OVER\n");
            }
        }
    }
}

// ======================== UTILITAIRES ========================

bool CheckCollisionShipEnemy(Rectangle2D shipBox, Sphere2D enemySphere)
{
    // Find the closest point of the rectangle to the center of the sphere
    float closestX = fmaxf(
        fminf(enemySphere.center.x,                                                         // selected coordonates of the center on x
            fmaxf(shipBox.p1.x, fmaxf(shipBox.p2.x, fmaxf(shipBox.p3.x, shipBox.p4.x)))),   // find the closest points of the rectangle on x to the center
        fminf(shipBox.p1.x, fminf(shipBox.p2.x, fminf(shipBox.p3.x, shipBox.p4.x)))         // find the farthest points of the rectangle on x to the center
    );

    float closestY = fmaxf(
        fminf(enemySphere.center.y,
            fmaxf(shipBox.p1.y, fmaxf(shipBox.p2.y, fmaxf(shipBox.p3.y, shipBox.p4.y)))),
        fminf(shipBox.p1.y, fminf(shipBox.p2.y, fminf(shipBox.p3.y, shipBox.p4.y)))
    );

    // Distance beetween this point and the center of the sphere
    float dx = enemySphere.center.x - closestX;
    float dy = enemySphere.center.y - closestY;

    return (dx * dx + dy * dy) <= (enemySphere.radius * enemySphere.radius);
}