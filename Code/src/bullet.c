#include "bullet.h"
#include "enemy.h"
#include "game.h"
//#include "sphere2D.h"
#include <math.h>
#include <stdio.h>

ShipBullet shipBullets[MAX_BULLETS];

int score = 0;
int actualLevel = 1;
int lifeNumber = 3;                          // number of lives


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

        // Vérifier si hors écran
        if (shipBullets[i].position.x < -50 || shipBullets[i].position.x > 1080 + 50 ||
            shipBullets[i].position.y < -50 || shipBullets[i].position.y > 1300 + 50)
        {
            shipBullets[i].active = false;
            continue;
        }

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

        
        // ===== Collision BigBasicEnemies =====
        for (int j = 0; j < maxBigBasicEnemies; j++)
        {
            if (bigBasicEnemies[j].size.x <= 0.0f || bigBasicEnemies[j].size.y <= 0.0f) continue;

            // Le centre est la position de l'ennemi (car DrawTexturePro utilise origin au centre)
            Vector2 enemyCenter = { bigBasicEnemies[j].position.x, bigBasicEnemies[j].position.y };
            float enemyRadius = (bigBasicEnemies[j].size.x * 3.5f) / 2.0f; // scale de 3.5f

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
                printf("Hit Big Basic Enemy %d!\n", j); // Debug
                score += 100;
                break;
            }
        }
        
        // Désactiver si hors écran
        if (shipBullets[i].position.x < -50 || shipBullets[i].position.x > 1130 ||
            shipBullets[i].position.y < -50 || shipBullets[i].position.y > 1350)
        {
            shipBullets[i].active = false;
            continue;
        }

        // ===== Collision MidBasicEnemies =====
        for (int j = 0; j < maxMidBasicEnemies; j++)
        {
            if (midBasicEnemies[j].size.x <= 0.0f || midBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { midBasicEnemies[j].position.x, midBasicEnemies[j].position.y };
            float enemyRadius = (midBasicEnemies[j].size.x * 2.0f) / 2.0f; // scale de 2.0f

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->midBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
                printf("Hit Mid Basic Enemy %d!\n", j); // Debug
                score += 135;
                break;
            }
        }

        // ===== Collision SmallBasicEnemies =====
        for (int j = 0; j < maxSmallBasicEnemies; j++)
        {
            if (smallBasicEnemies[j].size.x <= 0.0f || smallBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { smallBasicEnemies[j].position.x, smallBasicEnemies[j].position.y };
            float enemyRadius = (smallBasicEnemies[j].size.x * 1.5f) / 2.0f; // scale de 1.5f

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->smallBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
                printf("Hit Small Basic Enemy %d!\n", j); // Debug
                score += 200;
                break;
            }
        }

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
                score += 325;
                break;
            }
        }

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
                score += 360;
                break;
            }
        }

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
                score += 425;
                break;
            }
        }

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
                score += 500;
                break;
            }
        }

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
                score += 535;
                break;
            }
        }

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
                score += 600;
                break;
            }
        }

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
                score += 750;
                break;
            }
        }

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
                score += 785;
                break;
            }
        }

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
                score += 850;
                break;
            }
        }
    }
}


bool CheckCollisionShipEnemy(Rectangle2D shipBox, Sphere2D enemySphere)
{
    // Find the closest point of the rectangle to the center of the sphere
    float closestX = fmaxf(fminf(enemySphere.center.x,                                  // selected coordonates of the center on x
        fmaxf(shipBox.p1.x, fmaxf(shipBox.p2.x, fmaxf(shipBox.p3.x, shipBox.p4.x)))),   // find the closest points of the rectangle on x to the center
        fminf(shipBox.p1.x, fminf(shipBox.p2.x, fminf(shipBox.p3.x, shipBox.p4.x))));   // find the farthest points of the rectangle on x to the center
    float closestY = fmaxf(fminf(enemySphere.center.y,                                  // selected coordonates of the center on y
        fmaxf(shipBox.p1.y, fmaxf(shipBox.p2.y, fmaxf(shipBox.p3.y, shipBox.p4.y)))),   // find the closest points of the rectangle on x to the center
        fminf(shipBox.p1.y, fminf(shipBox.p2.y, fminf(shipBox.p3.y, shipBox.p4.y))));   // find the farthest points of the rectangle on x to the center

    // Distance beetween this point and the center of the sphere
    float dx = enemySphere.center.x - closestX;
    float dy = enemySphere.center.y - closestY;

    return (dx * dx + dy * dy) <= (enemySphere.radius * enemySphere.radius);
}

void LevelProgress(void)
{
    if (score >= 5000) {
        actualLevel = actualLevel++;
        score = 0;
        lifeNumber = 3;
    }

}