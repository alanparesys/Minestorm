#include "bullet.h"
#include "Vector2D.h"
#include "sphere2D.h"
#include "enemy.h"
#include "game.h"
#include <math.h>
#include <stdio.h>

ShipBullet shipBullets[MAX_BULLETS];

int maxBigBasicEnemy = 3;
Enemy bigBasicEnemies[3];


void InitBullets(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        shipBullets[i].active = false;
        shipBullets[i].radius = BULLET_RADIUS;
        shipBullets[i].position = Vector2D_SetFromComponents(0, 0);
        shipBullets[i].velocity = Vector2D_SetFromComponents(0, 0);
    }
}

void FireBullet(Vector2D startPos, float angle)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active)
        {
            shipBullets[i].active = true;

            // tirer depuis le centre du vaisseau
            shipBullets[i].position = startPos;

            shipBullets[i].velocity = Vector2D_SetFromComponents(
                cosf(angle) * BULLET_SPEED,
                sinf(angle) * BULLET_SPEED
            );

            shipBullets[i].radius = BULLET_RADIUS;
            return;
        }
    }
}

void UpdateBullets(GameAssets* assets)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active) continue;

        // déplacer la bullet
        shipBullets[i].position = Vector2D_Add(shipBullets[i].position, shipBullets[i].velocity);

        // --- Visualisation du cercle de collision ---
        DrawCircleLines(
            (int)shipBullets[i].position.x,
            (int)shipBullets[i].position.y,
            shipBullets[i].radius,
            YELLOW
        );

        // Draw bullet avec texture si assets existe
        if (assets != NULL)
        {
            Rectangle src = { 0, 0, assets->bulletTexture.width, assets->bulletTexture.height };
            Rectangle dst = { shipBullets[i].position.x, shipBullets[i].position.y,
                             shipBullets[i].radius * 2.0f, shipBullets[i].radius * 2.0f };
            Vector2 origin = { shipBullets[i].radius, shipBullets[i].radius };
            DrawTexturePro(assets->bulletTexture, src, dst, origin, 0.0f, WHITE);
        }
        else
        {
            DrawCircle((int)shipBullets[i].position.x, (int)shipBullets[i].position.y,
                shipBullets[i].radius, WHITE);
        }

        // Sphere2D pour collision
        Sphere2D bS = Sphere2D_SetFromCenterRadius(shipBullets[i].position, shipBullets[i].radius);

        // Collision avec ennemis
        for (int j = 0; j < maxBigBasicEnemy; j++)
        {
            if (bigBasicEnemies[j].size.x <= 0 || bigBasicEnemies[j].size.y <= 0) continue;

            Rectangle destRec = { bigBasicEnemies[j].position.x, bigBasicEnemies[j].position.y,
                                  bigBasicEnemies[j].size.x * 2.5f, bigBasicEnemies[j].size.y * 2.5f };
            Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

            Vector2D enemyCenter = Vector2D_SetFromComponents(destRec.x + origin.x, destRec.y + origin.y);
            float enemyRadius = fmaxf(destRec.width, destRec.height) / 2.0f;

            Sphere2D eS = Sphere2D_SetFromCenterRadius(enemyCenter, enemyRadius);

            float dx = bS.center.x - eS.center.x;
            float dy = bS.center.y - eS.center.y;
            float dist2 = dx * dx + dy * dy;
            float radiiSum = bS.radius + eS.radius;

            if (dist2 <= radiiSum * radiiSum)
            {
                shipBullets[i].active = false;
                bigBasicEnemies[j].size.x = 0.0f;
                bigBasicEnemies[j].size.y = 0.0f;
                break;
            }
        }

        // désactiver si hors écran
        if (shipBullets[i].position.x < -50 || shipBullets[i].position.x > GetScreenWidth() + 50 ||
            shipBullets[i].position.y < -50 || shipBullets[i].position.y > GetScreenHeight() + 50)
        {
            shipBullets[i].active = false;
        }
    }
}