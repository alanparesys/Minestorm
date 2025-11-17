#ifndef AABB2D_H
#define AABB2D_H

#include <stdbool.h>
#include "Vector2D.h"
#include "rectangle2D.h"

typedef struct AABB2D
{
    Vector2D min;
    Vector2D max;
} AABB2D;

AABB2D AABB2D_FromCenterHalfSize(Vector2D center, Vector2D halfSize);
AABB2D AABB2D_FromMinMax(Vector2D min, Vector2D max);
AABB2D AABB2D_FromRectangle(Rectangle2D rect);
Vector2D AABB2D_GetCenter(AABB2D box);
Vector2D AABB2D_GetHalfSize(AABB2D box);
Vector2D AABB2D_GetSize(AABB2D box);
bool AABB2D_ContainsPoint(AABB2D box, Vector2D point);

#endif

