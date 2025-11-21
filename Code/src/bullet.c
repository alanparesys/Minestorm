#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "explosion.h"
#include <math.h>
#include <stdio.h>

// Explosion scaling constants for different enemy sizes
#define EXPLOSION_SCALE_BIG   2.5f
#define EXPLOSION_SCALE_MID   1.8f
#define EXPLOSION_SCALE_SMALL 1.3f

// Global bullet arrays - consider moving to game state structure
ShipBullet shipBullets[MAX_BULLETS];
EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];

// Global game state - consider encapsulating in a GameState struct


// Player Bullet
// Initialize all player bullets to inactive state
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

// Spawn a new bullet from the player's position in the given direction
// Uses first available inactive slot in the bullet pool
void FireBullet(Vector2D startPos, float angle, GameAssets* assets)
{
    // Search for first available bullet slot
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active)
        {
            shipBullets[i].active = true;
            shipBullets[i].position = (Vector2){ startPos.x, startPos.y };
            // Calculate velocity components from angle
            shipBullets[i].velocity = (Vector2){ cosf(angle) * BULLET_SPEED, sinf(angle) * BULLET_SPEED };
            shipBullets[i].radius = BULLET_RADIUS;
            return;
        }
    }
}

// Update and render all active player bullets
void UpdateBullets(GameAssets* assets, Collision* collision)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active) continue;

        // Apply velocity to position
        shipBullets[i].position.x += shipBullets[i].velocity.x;
        shipBullets[i].position.y += shipBullets[i].velocity.y;

        // Deactivate bullets that leave screen bounds (with 50px margin)
        if (shipBullets[i].position.x < -50 || shipBullets[i].position.x > 1080 + 50 ||
            shipBullets[i].position.y < -50 || shipBullets[i].position.y > 1300 + 50)
        {
            shipBullets[i].active = false;
            continue;
        }

        // Render bullet with texture or fallback circle
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

        // Skip enemy collision checks if enemies are disabled
        if (!enemiesActive) continue;

        // Big Basic Enemy Collisions
        for (int j = 0; j < maxBigBasicEnemies; j++)
        {
            // Skip invalid enemies
            if (bigBasicEnemies[j].size.x <= 0.0f || bigBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { bigBasicEnemies[j].position.x, bigBasicEnemies[j].position.y };
            float enemyRadius = (bigBasicEnemies[j].size.x * 3.5f) / 2.0f; // Magic number: collision multiplier

            // Calculate distance between bullet and enemy centers
            float dx = shipBullets[i].position.x - enemyCenter.x;
            float dy = shipBullets[i].position.y - enemyCenter.y;
            float distance = sqrtf(dx * dx + dy * dy);

            // Check circle-circle collision
            if (distance < shipBullets[i].radius + enemyRadius)
            {
                collision->bigBasicEnemiesBulletCollision[j] = true;
                shipBullets[i].active = false;
                printf("Hit Big Basic Enemy %d!\n", j);

                Vector2D explosionPos = { bigBasicEnemies[j].position.x, bigBasicEnemies[j].position.y };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_BIG);
                score += 100;
                break; // Exit after first collision
            }
        }

        if (!shipBullets[i].active) continue;

        // Mid Basic Enemy Collisions
        for (int j = 0; j < maxMidBasicEnemies; j++)
        {
            if (midBasicEnemies[j].size.x <= 0.0f || midBasicEnemies[j].size.y <= 0.0f) continue;

            Vector2 enemyCenter = { midBasicEnemies[j].position.x, midBasicEnemies[j].position.y };
            float enemyRadius = (midBasicEnemies[j].size.x * 2.0f) / 2.0f; // Different collision multiplier

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

        // Small Basic Enemy Collisions
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

        // Big Shooter Enemy Collisions
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

        // Mid Shooter Enemy Collisions
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

        // Small Shooter Enemy Collisions
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

        // Big Follower Enemy Collisions
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

        // Mid Follower Enemy Collisions
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

        // Small Follower Enemy Collisions
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

        // Big Follower Shooter Enemy Collisions
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

        // Mid Follower Shooter Enemy Collisions
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

        // Small Follower Shooter Enemy Collisions
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

// ENEMY BULLETS

// Initialize all enemy bullets to inactive state
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

// Spawn enemy bullet aimed at target position
// Normalizes direction vector to maintain consistent speed
void FireEnemyBullet(Vector2D startPos, Vector2D targetPos, GameAssets* assets)
{
    // Find first available bullet slot
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (!enemyBullets[i].active)
        {
            enemyBullets[i].active = true;
            enemyBullets[i].position = (Vector2){ startPos.x, startPos.y };

            // Calculate normalized direction vector towards target
            float dirX = targetPos.x - startPos.x;
            float dirY = targetPos.y - startPos.y;
            float dist = sqrtf(dirX * dirX + dirY * dirY);

            if (dist > 0.001f) // Avoid division by zero
            {
                enemyBullets[i].velocity.x = (dirX / dist) * ENEMY_BULLET_SPEED;
                enemyBullets[i].velocity.y = (dirY / dist) * ENEMY_BULLET_SPEED;
            }
            else
            {
                // Fallback: shoot straight down if target is at same position
                enemyBullets[i].velocity.x = 0.0f;
                enemyBullets[i].velocity.y = ENEMY_BULLET_SPEED;
            }

            enemyBullets[i].radius = BULLET_RADIUS;
            return;
        }
    }
}

// Play enemy shoot sound effect
void PlayEnemyShootSound(GameAssets* assets)
{
    PlaySound(assets->laser2);
}

// Check collisions between player bullets and enemy bullets
// Destroys both bullets on impact and creates explosion effect
void CheckBulletBulletCollisions(void)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!shipBullets[i].active) continue;

        for (int j = 0; j < MAX_ENEMY_BULLETS; j++)
        {
            if (!enemyBullets[j].active) continue;

            // Calculate distance between bullet centers
            float dx = shipBullets[i].position.x - enemyBullets[j].position.x;
            float dy = shipBullets[i].position.y - enemyBullets[j].position.y;
            float distance = sqrtf(dx * dx + dy * dy);

            // Sum of both bullet radii for collision threshold
            float collisionRadius = shipBullets[i].radius + enemyBullets[j].radius;

            // Handle collision
            if (distance < collisionRadius)
            {
                // Deactivate both bullets
                shipBullets[i].active = false;
                enemyBullets[j].active = false;

                // Create explosion at midpoint between bullets
                Vector2D explosionPos = {
                    (shipBullets[i].position.x + enemyBullets[j].position.x) / 2.0f,
                    (shipBullets[i].position.y + enemyBullets[j].position.y) / 2.0f
                };
                SpawnExplosion(explosionPos, EXPLOSION_SCALE_SMALL);

                // Debug output
                printf("*** BULLET-BULLET COLLISION DETECTED! ***\n");
                printf("Explosion created at (%.1f, %.1f)\n", explosionPos.x, explosionPos.y);

                break; // Exit loop after collision
            }
        }
    }
}

// Update and render all active enemy bullets
void UpdateEnemyBullets(GameAssets* assets, Ship* player)
{
    if (player == NULL) {
        printf("ERROR: player is NULL in UpdateEnemyBullets!\n");
        return;
    }
    printf("PlayerBullet position: (%.1f, %.1f)\n", player->position.x, player->position.y);

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (!enemyBullets[i].active) continue;

        // Apply velocity to position
        enemyBullets[i].position.x += enemyBullets[i].velocity.x;
        enemyBullets[i].position.y += enemyBullets[i].velocity.y;

        // Deactivate bullets that leave screen bounds (with 50px margin)
        if (enemyBullets[i].position.x < -50 || enemyBullets[i].position.x > 1080 + 50 ||
            enemyBullets[i].position.y < -50 || enemyBullets[i].position.y > 1300 + 50)
        {
            enemyBullets[i].active = false;
            continue;
        }

        // Render bullet in red
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

        // Check collision with player
        if (player != NULL)
        {
            // Approximate player hitbox as circle using smaller dimension
            float playerRadius = (player->size.x < player->size.y ? player->size.x : player->size.y) / 2.0f;

            float dx = enemyBullets[i].position.x - player->position.x;
            float dy = enemyBullets[i].position.y - player->position.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < enemyBullets[i].radius + playerRadius)
            {
                enemyBullets[i].active = false;
                lifeNumber = lifeNumber--;
                printf("Player hit by enemy bullet! GAME OVER\n");
            }
        }
    }
}

// UTILITY FUNCTIONS

// Check collision between rectangular ship hitbox and circular enemy hitbox
// Uses closest point on rectangle to circle center approach
bool CheckCollisionShipEnemy(Rectangle2D shipBox, Sphere2D enemySphere)
{
    // Find closest X coordinate on rectangle to sphere center
    float closestX = fmaxf(
        fminf(enemySphere.center.x,
            fmaxf(shipBox.p1.x, fmaxf(shipBox.p2.x, fmaxf(shipBox.p3.x, shipBox.p4.x)))), // Max X of rectangle
        fminf(shipBox.p1.x, fminf(shipBox.p2.x, fminf(shipBox.p3.x, shipBox.p4.x)))       // Min X of rectangle
    );

    // Find closest Y coordinate on rectangle to sphere center
    float closestY = fmaxf(
        fminf(enemySphere.center.y,
            fmaxf(shipBox.p1.y, fmaxf(shipBox.p2.y, fmaxf(shipBox.p3.y, shipBox.p4.y)))), // Max Y of rectangle
        fminf(shipBox.p1.y, fminf(shipBox.p2.y, fminf(shipBox.p3.y, shipBox.p4.y)))       // Min Y of rectangle
    );

    // Calculate squared distance from closest point to sphere center
    float dx = enemySphere.center.x - closestX;
    float dy = enemySphere.center.y - closestY;

    // Return true if distance is less than sphere radius (collision detected)
    return (dx * dx + dy * dy) <= (enemySphere.radius * enemySphere.radius);
}