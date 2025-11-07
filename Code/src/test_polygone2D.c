#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include <stdlib.h>
#include "polygone2D.h"
#include "vector2D.h"
#include <stdarg.h>


int main()
{
    printf("=============================================\n");
    printf("Polygone\n");
    printf("=============================================\n\n");

    // 1 Polygone2D_SetFromPoints
    Vector2D p1 = Vector2D_SetFromComponents(2, 2);
    Vector2D p2 = Vector2D_SetFromComponents(5, 3);
    Vector2D p3 = Vector2D_SetFromComponents(3, 4);
    Vector2D p4 = Vector2D_SetFromComponents(3, 6);
    Vector2D p5 = Vector2D_SetFromComponents(1, 4);
    Vector2D p6 = Vector2D_SetFromComponents(0, 2);
    Vector2D p7 = Vector2D_SetFromComponents(2, 0);

    Polygone2D poly1 = Polygone2D_SetFromPoints(7, p1, p2, p3, p4, p5, p6, p7);

    printf("(Function 1) : \n");
    Polygone2D_Print(poly1);
    printf("__________________________\n");

    printf("\n");

    // 2 Polygone2D_Translate
    printf("(Function 2) : \n");
    Vector2D v1 = Vector2D_SetFromComponents(2, 3);
    Polygone2D poly2 = Polygone2D_Translate(poly1, v1);
    Polygone2D_Print(poly2);
    printf("__________________________\n");

    printf("\n");

    // 3 Polygone2D_Scale
    printf("(Function 3) : \n");
    Vector2D p8 = Vector2D_SetFromComponents(6, 1);
    Polygone2D poly3 = Polygone2D_Scale(poly1, 2.0f, p8);
    Polygone2D_Print(poly3);
    printf("__________________________\n");

    printf("\n");

    // 4 Polygone2D_Perimetre
    printf("(Function 4) : \n");
    float peri = Polygone2D_Perimetre(poly1);
    printf("Perimetre polygone 1 = %f\n", peri);
    printf("__________________________\n");

    printf("\n");

    // 5 Polygone2D_Aire
    printf("(Function 5) : \n");
    float aire = Polygone2D_Aire(poly1);
    printf("Aire polygone 1 = %f\n", aire);
    printf("__________________________\n");

    printf("\n");

    // 6 Polygone2D_Isobarycenter
    printf("(Function 6) : \n");
    Vector2D p9 = Polygone2D_Isobarycenter(poly1);
    printf("Isobarycenter of polygone 1 = ");
    Vector2D_Print(p9);
    printf("\n__________________________\n");

    printf("\n");

    // 7 Polygone2D_Rotate
    printf("(Function 7) : \n");
    Polygone2D poly4 = Polygone2D_Rotate(poly1, M_PI / 2, p8);
    Polygone2D_Print(poly4);
    printf("__________________________\n");

    printf("\n");


    // 8 Polygone2D_RotateAll
    printf("(Function 8) : \n");
    Polygone2D poly5 = Polygone2D_RotateAll(poly1, M_PI / 2, p8);
    Polygone2D_Print(poly5);
    printf("__________________________\n");

    printf("\n");

}