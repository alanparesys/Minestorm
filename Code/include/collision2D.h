#ifndef COLLISION2D_H
#define COLLISION2D_H

#include <stdbool.h>
#include "Vector2D.h"
#include "sphere2D.h"
#include "polygone2D.h"
#include "aabb2D.h"

bool Collision_PointPoint(Vector2D a, Vector2D b, float epsilon);
bool Collision_PointCircle(Vector2D point, Sphere2D circle);
bool Collision_CircleCircle(Sphere2D a, Sphere2D b);
bool Collision_PointPolygon(Vector2D point, Polygone2D poly);
bool Collision_CirclePolygon(Sphere2D circle, Polygone2D poly);
bool Collision_PolygonPolygon(Polygone2D polyA, Polygone2D polyB);
bool Collision_PointAABB(Vector2D point, AABB2D box);
bool Collision_CircleAABB(Sphere2D circle, AABB2D box);
bool Collision_AABBAABB(AABB2D a, AABB2D b);

#endif

