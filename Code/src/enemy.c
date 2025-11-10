#include "stdlib.h"
#include "enemy.h"

Enemy* basicEnemy;

bool allowEnemySpawn = false;

bool basicEnemyMax = true;
int basicEnemyNumber = 0;


void BasicEnemySpawn(int screenHeight, int screenWigth, Enemy enemy)
{
    float positionX = 0;
    float positionY = 0;


    Vector2D bigSize = Vector2D_SetFromComponents(100, 100);
    Vector2D midSize = Vector2D_SetFromComponents(50, 50);
    Vector2D smallSize = Vector2D_SetFromComponents(25, 25);
    if (basicEnemyNumber == 3)
    {
        basicEnemyMax = false;
    }

    if (allowEnemySpawn == true)
    {
        basicEnemy = (Enemy*)malloc(sizeof(Enemy));
        if (basicEnemy == NULL)
        {
            printf("Allocation du basicEnemy non réussi");
            return;
        }

        int XorYSpawn = GetRandomValue(1, 2);

        // enemy spawn at X
        if (XorYSpawn == 1)
        {
            positionX = GetRandomValue(-50, screenWigth + 50);
            positionY = GetRandomValue(1, 2);
            if (positionY == 1) positionY = -50;
            else positionY = screenHeight + 50;
        }
        // enemy spawn a Y
        else
        {
            positionX = GetRandomValue(1, 2);
            positionY = GetRandomValue(-50, screenHeight + 50);
            if (positionX == 1) positionX = -50;
            else positionX = screenWigth + 50;
        }

        basicEnemy->position = Vector2D_SetFromComponents(positionX, positionY);

        basicEnemy->angle = 0.0f;

        basicEnemyNumber += 1;
    }
}

/*

// Implémentation et vérification des controles du jeu

Vector2D CheckInput(void) {
    Vector2D input = Vector2D_SetFromComponents(0, 0);
    Vector2D OY = Vector2D_SetFromComponents(0, 1);
    Vector2D OX = Vector2D_SetFromComponents(1, 0);

    //tourner à droite
    if (IsKeyDown('D')) {
        player->angle = player->angle + ROTATION_SPEED;

    }
    // tourner à gauche
    if (IsKeyDown('A') | IsKeyDown('Q')) {
        player->angle = player->angle - ROTATION_SPEED;

    }

    if (IsKeyDown('Z') | IsKeyDown('W')) {
        Vector2D pushVector = Vector2D_SetFromComponents(
            cos(player->angle) * PUSH, sin(player->angle) * PUSH);
        player->velocity = Vector2D_Add(player->velocity, pushVector);


    }

    if (IsKeyDown(KEY_SPACE)) {
        // les tirs ici (Alan)
    }

    Vector2D_Print(player->velocity);
    return input;
}

void UpdateControlGame(void) {
    if (true) {
        CheckInput();

        // on applique l'inertie : la position = la position + la vélocité
        player->position = Vector2D_Add(player->position, player->velocity);

        // on applique la friction : la vélocity = la vélocité x la friction
        player->velocity = Vector2D_Scale(player->velocity, FRICTION, Vector2D_SetFromComponents(0, 0););
    }
}
*/