#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include <math.h>
#include <stdio.h>

ShipBullet shipBullets[MAX_BULLETS];


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

        // Collision BigBasicEnemies
        for (int j = 0; j < maxBigBasicEnemies; j++)
        {
            if (bigBasicEnemies[j].size.x <= 0.0f || bigBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigBasicEnemies[j].position.x + bigBasicEnemies[j].size.x * 1.25f,
                                    bigBasicEnemies[j].position.y + bigBasicEnemies[j].size.y * 1.25f };
            float enemyRadius = fmaxf(bigBasicEnemies[j].size.x * 1.25f, bigBasicEnemies[j].size.y * 1.25f);

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
            }
        }

        // Collision BigShooterEnemies
        for (int j = 0; j < maxBigShooterEnemy; j++)
        {
            if (bigShooterEnemies[j].size.x <= 0.0f || bigShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigShooterEnemies[j].position.x + bigShooterEnemies[j].size.x * 1.75f,
                                    bigShooterEnemies[j].position.y + bigShooterEnemies[j].size.y * 1.75f };
            float enemyRadius = fmaxf(bigShooterEnemies[j].size.x * 1.75f, bigShooterEnemies[j].size.y * 1.75f);

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
            }
        }

        // Collision BigFollowerEnemies
        for (int j = 0; j < maxBigFollowerEnemy; j++)
        {
            if (bigFollowerEnemies[j].size.x <= 0.0f || bigFollowerEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigFollowerEnemies[j].position.x + bigFollowerEnemies[j].size.x * 1.75f,
                                    bigFollowerEnemies[j].position.y + bigFollowerEnemies[j].size.y * 1.75f };
            float enemyRadius = fmaxf(bigFollowerEnemies[j].size.x * 1.75f, bigFollowerEnemies[j].size.y * 1.75f);

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigFollowerEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
            }
        }

        // Collision BigFollowerShooterEnemies
        for (int j = 0; j < maxBigFollowerShooterEnemy; j++)
        {
            if (bigFollowerShooterEnemies[j].size.x <= 0.0f || bigFollowerShooterEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigFollowerShooterEnemies[j].position.x + bigFollowerShooterEnemies[j].size.x * 1.75f,
                                    bigFollowerShooterEnemies[j].position.y + bigFollowerShooterEnemies[j].size.y * 1.75f };
            float enemyRadius = fmaxf(bigFollowerShooterEnemies[j].size.x * 1.75f, bigFollowerShooterEnemies[j].size.y * 1.75f);

            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigFollowerShooterEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
            }
        }
    }
}
