#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include "vector2D.h"
#include "triangle2D.h"
#include "test_triangle2D.h"


void launch_triangle2D()
{
    printf("=============================================\n");
    printf("Triangle2D\n");
    printf("=============================================\n");
    printf("\n");

    // 1 Triangle2D_SetFrom3Points
    printf("(Function 1) : \n");
    Vector2D p1 = Vector2D_SetFromComponents(3, 4);
    Vector2D p2 = Vector2D_SetFromComponents(1, 6);
    Vector2D p3 = Vector2D_SetFromComponents(1, 3);

    Triangle2D tri1 = Triangle2D_SetFrom3Points(p1, p2, p3);
    printf("Triangle 1 :\n\n");
    Triangle_Print(tri1);
    printf("\n__________________________\n");
    printf("\n");

    // 2 Triangle2D_SetFrom1Point2Vectors
    printf("(Function 2) : \n");

    Vector2D v1 = Vector2D_SetFrom2Points(p1, p2);
    Vector2D v2 = Vector2D_SetFrom2Points(p1, p3);

    Triangle2D tri2 = Triangle2D_SetFrom1Point2Vectors(p1, v1, v2);
    printf("Triangle 2 :\n\n");
    Triangle_Print(tri2);
    printf("\n__________________________\n");
    printf("\n");

    // 3 Triangle2D_GetSurface
    printf("(Function 3) : \n");

    float surf1 = Triangle2D_GetSurface(tri1);
    printf("surface triangle 1 = %f\n\n", surf1);
    printf("\n__________________________\n");
    printf("\n");

    // 4 Triangle2D_IsSpecial
    printf("(Function 4) : \n");

    Vector2D p4 = Vector2D_SetFromComponents(5, 1);
    Vector2D p5 = Vector2D_SetFromComponents(7, 1);
    Vector2D p6 = Vector2D_Rotate(p4, M_PI / 3, p5);
    Triangle2D tri3 = Triangle2D_SetFrom3Points(p4, p5, p6);

    Vector2D p7 = Vector2D_SetFromComponents(0, 0);
    Vector2D p8 = Vector2D_SetFromComponents(2, 0);
    Vector2D p9 = Vector2D_SetFromComponents(1, 1);
    Triangle2D tri4 = Triangle2D_SetFrom3Points(p7, p8, p9);

    int spetri1 = Triangle2D_IsSpecial(tri1);
    int spetri3 = Triangle2D_IsSpecial(tri3);
    int spetri4 = Triangle2D_IsSpecial(tri4);

    printf("Is triangle special?\n");
    printf(" Triangle 1 : % i\n", spetri1);
    printf(" Triangle 3 : % i\n", spetri3);
    printf(" Triangle 4 : % i\n\n", spetri4);
    printf("\n__________________________\n");
    printf("\n");

    // 5 Triangle2D_Circumcenter
    printf("(Function 5) : \n");

    Vector2D p10 = Triangle2D_Circumcenter(tri1);
    printf("Circumcenter of triangle 1 : ");
    Vector2D_Print(p10);
    printf("\n__________________________\n");
    printf("\n");

    // 6 Triangle2D_Isobarycenter
    printf("(Function 6) : \n");
    Vector2D p11 = Triangle2D_Isobarycenter(tri1);
    printf("Isobarycenter of triangle 1 : ");
    Vector2D_Print(p11);
    printf("\n__________________________\n");
    printf("\n");

    // 7 Triangle2D_Translate
    printf("(Function 7) : \n");
    Vector2D t = Vector2D_SetFrom2Points(p4, p5);
    Triangle2D tri5 = Triangle2D_Translate(tri1, t);
    printf("Translation of triangle 1 : ");
    Triangle_Print(tri5);
    printf("\n__________________________\n");
    printf("\n");

    // 8 Triangle2D_Scale
    printf("(Function 8) : \n");
    Vector2D p12 = Vector2D_SetFromComponents(1, 1);
    Triangle2D tri6 = Triangle2D_Scale(tri1, 2, p12);
    printf("Scale of triangle 1 : ");
    Triangle_Print(tri6);
    printf("\n__________________________\n");
    printf("\n");

    // 9 Triangle2D_Rotate
    printf("(Function 9) : \n");
    Triangle2D tri7 = Triangle2D_Rotate(tri1, M_PI / 2, p12);
    printf("Rotation of triangle 1 : ");
    Triangle_Print(tri7);
    printf("\n__________________________\n");
    printf("\n");

    // 10 Bonus Triangle2D_RotateAll
    printf("(Function 10 (bonus) : \n");
    Triangle2D tri8 = Triangle2D_RotateAll(tri1, M_PI / 2, p12);
    printf("Rotation of triangle 1 : ");
    Triangle_Print(tri8);
    printf("\n__________________________\n");
    printf("\n");

}