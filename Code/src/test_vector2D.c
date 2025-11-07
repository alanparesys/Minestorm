#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include "test_vector2D.h"
#include "vector2D.h"

void launch_vector2D()
{
    printf("=============================================\n");
    printf("Vector2D\n");
    printf("=============================================\n");
    printf("\n");

    // 1 Vector2D_SetFromComponents
    printf("(FUNCTION 1) :\n");
    Vector2D p1 = Vector2D_SetFromComponents(3, 4);
    printf("p1 = ");
    Vector2D_Print(p1);
    printf("\n__________________________\n");
    printf("\n");

    // 2 Vector2D_SetFrom2Points
    printf("(FUNCTION 2) :\n");
    Vector2D p2 = Vector2D_SetFromComponents(5, 2);
    Vector2D v1 = Vector2D_SetFrom2Points(p1, p2);
    printf("v = ");
    Vector2D_Print(v1);
    printf("\n__________________________\n");
    printf("\n");

    // 3 Vector2D_Opposite
    printf("(FUNCTION 3) :\n");
    Vector2D v2 = Vector2D_Opposite(v1);
    printf("-v = ");
    Vector2D_Print(v2);
    printf("\n__________________________\n");
    printf("\n");

    // 4 Vector2D_MidPoint
    printf("(FUNCTION 4) :\n");
    Vector2D v3 = Vector2D_MidPoint(p1, p2);
    printf(" Mid p1, p2 = ");
    Vector2D_Print(v3);
    printf("\n__________________________\n");
    printf("\n");

    // 5 Vector2D_Distance
    printf("(FUNCTION 5) :\n");
    float dist = Vector2D_Distance(p1, p2);
    printf(" Dist p1, p2 = %f", dist);
    printf("\n__________________________\n");
    printf("\n");

    // 6 Vector2D_Add
    printf("(FUNCTION 6) :\n");
    Vector2D p3 = Vector2D_SetFromComponents(4, 1);
    Vector2D v4 = Vector2D_SetFrom2Points(p2, p3);
    Vector2D v5 = Vector2D_Add(v1, v4);
    printf(" v1 + v2 = ");
    Vector2D_Print(v5);
    printf("\n__________________________\n");
    printf("\n");

    // 7 Vector2D_SquaredNorm
    printf("(FUNCTION 7) :\n");
    float sqNorm = Vector2D_SquaredNorm(v1);
    printf(" v1 squared norm = %f", sqNorm);
    printf("\n__________________________\n");
    printf("\n");

    // 8 Vector2D_Norm
    printf("(FUNCTION 8) :\n");
    float Norm = Vector2D_Norm(v1);
    printf(" v1 norm = %f", Norm);
    printf("\n__________________________\n");
    printf("\n");

    // 9 Vector2D_DotProduct
    printf("(FUNCTION 9) :\n");
    float dotProd = Vector2D_DotProduct(v1, v4);
    printf(" v1 and v2 dot product = %f", dotProd);
    printf("\n__________________________\n");
    printf("\n");

    // 10 Vector2D_CrossProduct
    printf("(FUNCTION 10) :\n");
    float crossProd = Vector2D_CrossProduct(v1, v4);
    printf(" v1 and v2 cross product = %f", crossProd);
    printf("\n__________________________\n");
    printf("\n");

    // 11 Vector2D_GetAngle
    printf("(FUNCTION 11) :\n");
    float angle = Vector2D_GetAngle(v1, v4);
    printf(" v1 and v2 angle = %f (in radian)", angle);
    printf("\n__________________________\n");
    printf("\n");

    // 12 Vector2D_Translate
    printf("(FUNCTION 12) :\n");
    Vector2D p4 = Vector2D_Translate(p1, v4);
    printf(" p1 translated by v2 = ");
    Vector2D_Print(p4);
    printf("\n__________________________\n");
    printf("\n");

    // 13 Vector2D_Scale
    printf("(FUNCTION 13) :\n");
    Vector2D p5 = Vector2D_Scale(p1, 2, p2);
    printf(" p1 scaled by 2, with p2 anchor = ");
    Vector2D_Print(p5);
    printf("\n__________________________\n");
    printf("\n");

    // 14 Vector2D_Rotate
    printf("(FUNCTION 14) :\n");
    Vector2D p6 = Vector2D_Rotate(p1, M_PI / 6, p2);
    printf(" p1 rotated by pi/6 (30 degree), with p2 anchor = ");
    Vector2D_Print(p6);
    printf("\n__________________________\n");
    printf("\n");
}

