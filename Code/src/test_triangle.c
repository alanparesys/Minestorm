#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include "vector2D.h"
#include "triangle2D.h"


int main()
{
    // 1
    printf("(FUNCTION 1) :\n");
    Vector2D p1 = Vector2D_SetFromComponents(3, 4);
    Vector2D p2 = Vector2D_SetFromComponents(1, 6);
    Vector2D p3 = Vector2D_SetFromComponents(1, 3);

    Triangle2D tri1 = Triangle2D_SetFrom3Points(p1, p2, p3);
    printf("Triangle 1 :\n\n");
    Triangle_Print(tri1);
    printf("__________________________\n");
    printf("\n");
    // 2
    printf("(FUNCTION 2) :\n");

    Vector2D v1 = Vector2D_SetFrom2Points(p1, p2);
    Vector2D v2 = Vector2D_SetFrom2Points(p1, p3);

    Triangle2D tri2 = Triangle2D_SetFrom1Point2Vectors(p1, v1, v2);
    printf("Triangle 2 :\n\n");
    Triangle_Print(tri2);
    printf("__________________________\n");
    printf("\n");

    // 3
    printf("(FUNCTION 3) :\n");

    float surf1 = Triangle2D_GetSurface(tri1);
    printf("surface triangle 1 = %f\n\n", surf1);
    printf("__________________________\n");
    printf("\n");

    // 4
    printf("(FUNCTION 4) :\n");

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
    printf("__________________________\n");
    printf("\n");

    // 5
    printf("(FUNCTION 5) :\n");

    Vector2D p10 = Triangle2D_Circumcenter(tri1);
    printf("Circumcenter of triangle 1 : ");
    Vector2D_Print(p10);
    printf("\n\n__________________________\n");
    printf("\n");

    // 6
    printf("(FUNCTION 6) :\n");
    Vector2D p11 = Triangle2D_Isobarycenter(tri1);
    printf("Isobarycenter of triangle 1 : ");
    Vector2D_Print(p11);
    printf("\n\n__________________________\n");
    printf("\n");

    // 7
    printf("(FUNCTION 7) :\n");
    Vector2D t = Vector2D_SetFrom2Points(p4, p5);
    Triangle2D tri5 = Triangle2D_Translate(tri1, t);
    printf("Translation of triangle 1 : ");
    Triangle_Print(tri5);
    printf("\n\n__________________________\n");
    printf("\n");

    // 8
    printf("(FUNCTION 8) :\n");
    Vector2D p12 = Vector2D_SetFromComponents(1, 1);
    Triangle2D tri6 = Triangle2D_Scale(tri1, 2, p12);
    printf("Scale of triangle 1 : ");
    Triangle_Print(tri6);
    printf("\n\n__________________________\n");
    printf("\n");
}