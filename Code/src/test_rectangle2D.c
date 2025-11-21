#include <stdio.h>
#include <math.h>
#include "rectangle2D.h"
#include "vector2D.h"
#include "test_rectangle2D.h"

void launch_rectangle2D()
{
    printf("=============================================\n");
    printf("Rectangle2D\n");
    printf("=============================================\n");
    printf("\n");

    // 1 Rectangle2D_SetFrom4Points
    Vector2D p1 = Vector2D_SetFromComponents(1, 0);
    Vector2D p2 = Vector2D_SetFromComponents(5, 0);
    Vector2D p3 = Vector2D_SetFromComponents(5, 2);
    Vector2D p4 = Vector2D_SetFromComponents(1, 2);
    Rectangle2D rect = Rectangle2D_SetFrom4Points(p1, p2, p3, p4);
    printf("(Function 1) : \n");
    Rectangle2D_Print(rect);
    printf("__________________________\n");

    printf("\n");

    // 2 Rectangle2D_SetFrom1Point2Vectors
    Vector2D v1 = Vector2D_SetFrom2Points(p1, p2);
    Vector2D v2 = Vector2D_SetFrom2Points(p2, p3);
    Vector2D v3 = Vector2D_SetFrom2Points(p3, p4);

    Rectangle2D rect2 = Rectangle2D_SetFrom1Point2Vectors(p1, v1, v2);
    printf("(Function 2) : \n");
    Rectangle2D_Print(rect2);
    printf("__________________________\n");

    printf("\n");

    // 3 Rectangle2D_SetFromCenterLengthWidthAngle
    Vector2D center = Vector2D_SetFromComponents(3, 1);
    float length = 4.0f;
    float width = 2.0f;
    float angle = 0; // no rotation

    Rectangle2D rect3 = Rectangle2D_SetFromCenterLengthWidthAngle(center, length, width, angle);
    printf("(Function 3) : \n");
    Rectangle2D_Print(rect3);
    printf("Surface: %.2f\n\n", Rectangle2D_GetSurface(rect3)); // length * width
    printf("__________________________\n");

    printf("\n");

    // 4 Rectangle2D_Translate
    printf("(Function 4) : \n");
    Vector2D translation = Vector2D_SetFromComponents(2, 3);
    Rectangle2D rect4 = Rectangle2D_Translate(rect, translation);
    Rectangle2D_Print(rect4);
    printf("Surface: %.2f\n\n", Rectangle2D_GetSurface(rect4));
    printf("__________________________\n");

    printf("\n");

    // 5 Rectangle2D_Scale
    printf("(Function 5) : \n");
    Rectangle2D rect5 = Rectangle2D_Scale(rect, 2.0f, center);
    Rectangle2D_Print(rect5);
    printf("Surface: %.2f\n\n", Rectangle2D_GetSurface(rect5));
    printf("__________________________\n");

    printf("\n");

    // 6 Rectangle2D_Rotate
    printf("(Function 6) : \n");
    Rectangle2D rect6 = Rectangle2D_Rotate(rect5, angle, center);
    Rectangle2D_Print(rect6);
    printf("Surface: %.2f\n\n", Rectangle2D_GetSurface(rect6));
    printf("__________________________\n");

    printf("\n");


    // 7 Rectangle2D_RotateAll (Bonus)
    printf("(Function 7) : \n");
    Rectangle2D rect7 = Rectangle2D_RotateAll(rect5, angle, center);
    Rectangle2D_Print(rect7);
    printf("Surface: %.2f\n\n", Rectangle2D_GetSurface(rect7));
    printf("__________________________\n");

    printf("\n");

}