#ifndef SPHERE2D_H
#define SPHERE2D_H

#include "vector2D.h"

typedef struct Sphere2D
{
    Vector2D center;
    float radius;
} Sphere2D;

Sphere2D Sphere2D_SetFromCenterRadius(Vector2D center, float radius);
Sphere2D Sphere2D_SetFrom2Points(Vector2D p1, Vector2D p2);
float Sphere2D_GetSurface(Sphere2D sph);
float Sphere2D_GetPerimeter(Sphere2D sph);
Sphere2D Sphere2D_Translate(Sphere2D sph, Vector2D t);
Sphere2D Sphere2D_Scale(Sphere2D sph, float a, Vector2D anchor);

// Bonus
Sphere2D Sphere2D_Rotate(Sphere2D sph, float a, Vector2D anchor);
//

void Sphere2D_Print(Sphere2D sph);

#endif
