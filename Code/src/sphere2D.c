#define _USE_MATH_DEFINES


#include "sphere2D.h"
#include "vector2D.h"
#include <math.h>
#include <stdio.h>


Sphere2D Sphere2D_SetFromCenterRadius(Vector2D center, float radius)
{
    Sphere2D sph;
    sph.center = center;
    sph.radius = radius;
    return sph;
}

Sphere2D Sphere2D_SetFrom2Points(Vector2D p1, Vector2D p2)
{
    float radius = Vector2D_Distance(p1, p2);
    return Sphere2D_SetFromCenterRadius(p1, radius);
}

float Sphere2D_GetSurface(Sphere2D sph)
{
    return M_PI * sph.radius * sph.radius; // ?r�
}

float Sphere2D_GetPerimeter(Sphere2D sph)
{
    return 2 * M_PI * sph.radius; // 2?r
}

Sphere2D Sphere2D_Translate(Sphere2D sph, Vector2D t)
{
    Vector2D nwCenter = Vector2D_Translate(sph.center, t);
    Sphere2D nwSph = Sphere2D_SetFromCenterRadius(nwCenter, sph.radius);
    return nwSph;
}

Sphere2D Sphere2D_Scale(Sphere2D sph, float a, Vector2D anchor)
{
    Sphere2D result;
    result.center = Vector2D_Scale(sph.center, a, anchor);
    result.radius = sph.radius * a;
    return result;
}

void Sphere2D_Print(Sphere2D sph)
{
    printf("Sphere2D :\n");
    printf(" Centre : (%.2f, %.2f)\n", sph.center.x, sph.center.y);
    printf(" Rayon  : %.2f\n", sph.radius);
    printf(" Perimetre : %.2f\n", Sphere2D_GetPerimeter(sph));
    printf(" Surface : %.2f\n", Sphere2D_GetSurface(sph));

}