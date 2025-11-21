#include <stdio.h>
#include <math.h>
#include "sphere2D.h"
#include "vector2D.h"
#include "test_sphere2D.h"

void launch_sphere2D()
{
    printf("=============================================\n");
    printf("Sphere2D\n");
    printf("=============================================\n");
    printf("\n");

    // 1 Sphere2D_SetFromCenterRadius
    printf("(Function 1) : \n");
    Vector2D center = Vector2D_SetFromComponents(4, 3);
    float radius = 3.0f;
    Sphere2D sph1 = Sphere2D_SetFromCenterRadius(center, radius);
    Sphere2D_Print(sph1);
    printf("__________________________\n\n");

    // 2 Sphere2D_SetFrom2Points
    printf("(Function 2) : \n");
    Vector2D p1 = Vector2D_SetFromComponents(1, 2);
    Vector2D p2 = Vector2D_SetFromComponents(5, 6);
    Sphere2D sph2 = Sphere2D_SetFrom2Points(p1, p2);
    Sphere2D_Print(sph2);
    printf("__________________________\n\n");

    // 3 Sphere2D_Translate
    printf("(Function 3) : \n");
    Vector2D v1 = Vector2D_SetFrom2Points(p1, p2);
    Sphere2D sph3 = Sphere2D_Translate(sph1, v1);
    Sphere2D_Print(sph3);
    printf("__________________________\n\n");

    // 4 Sphere2D_Scale
    printf("(Function 4) : \n");
    Sphere2D sph4 = Sphere2D_Scale(sph1, radius, p1);
    Sphere2D_Print(sph4);
    printf("__________________________\n\n");
}