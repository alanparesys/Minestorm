#include "bullet.h"
#include "Vector2D.h"
#include "sphere2D.h"
#include "enemy.h"
#include "game.h"
#include <math.h>
#include <stdio.h>
#include "ship_player.h"

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