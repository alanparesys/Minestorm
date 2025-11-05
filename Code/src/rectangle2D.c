#include "rectangle2D.h"
#include "vector2D.h"
#include <stdio.h>

Rectangle2D Rectangle2D_SetFrom4Points(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Rectangle2D rect;
    rect.p1 = p1;
    rect.p2 = p2;
    rect.p3 = p3;
    rect.p4 = p4;

    // Center = average of the 4 points
    rect.center = Vector2D_SetFromComponents((p1.x + p2.x + p3.x + p4.x) / 4.0f, (p1.y + p2.y + p3.y + p4.y) / 4.0f);
    rect.length = Vector2D_Distance(p1, p2);
    rect.width = Vector2D_Distance(p2, p3);

    return rect;
}

Rectangle2D Rectangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2)
{
    Vector2D p2 = Vector2D_Add(p1, v1);
    Vector2D p3 = Vector2D_Add(p2, v2);
    Vector2D p4 = Vector2D_Add(p1, v2);

    return Rectangle2D_SetFrom4Points(p1, p2, p3, p4);
}

Rectangle2D Rectangle2D_SetFromCenterLengthWidthAngle(Vector2D center, float length, float width, float angle)
{
    // half --> moitié
    float half_length = length / 2.0f;
    float half_width = width / 2.0f;

    Vector2D p1 = Vector2D_SetFromComponents(-half_length, -half_width);
    Vector2D p2 = Vector2D_SetFromComponents(half_length, -half_width);
    Vector2D p3 = Vector2D_SetFromComponents(half_length, half_width);
    Vector2D p4 = Vector2D_SetFromComponents(-half_length, half_width);

    // Rotation and translation toward the center
    p1 = Vector2D_Add(Vector2D_Rotate(p1, angle, Vector2D_SetFromComponents(0, 0)), center);
    p2 = Vector2D_Add(Vector2D_Rotate(p2, angle, Vector2D_SetFromComponents(0, 0)), center);
    p3 = Vector2D_Add(Vector2D_Rotate(p3, angle, Vector2D_SetFromComponents(0, 0)), center);
    p4 = Vector2D_Add(Vector2D_Rotate(p4, angle, Vector2D_SetFromComponents(0, 0)), center);

    return Rectangle2D_SetFrom4Points(p1, p2, p3, p4);
}

float Rectangle2D_GetSurface(Rectangle2D rect)
{
    return rect.length * rect.width;
}

Rectangle2D Rectangle2D_Translate(Rectangle2D rect, Vector2D t)
{
    rect.p1 = Vector2D_Add(rect.p1, t);
    rect.p2 = Vector2D_Add(rect.p2, t);
    rect.p3 = Vector2D_Add(rect.p3, t);
    rect.p4 = Vector2D_Add(rect.p4, t);
    rect.center = Vector2D_Add(rect.center, t);
    return rect;
}

Rectangle2D Rectangle2D_Scale(Rectangle2D rect, float a, Vector2D anchor)
{
    rect.p1 = Vector2D_Scale(rect.p1, a, anchor);
    rect.p2 = Vector2D_Scale(rect.p2, a, anchor);
    rect.p3 = Vector2D_Scale(rect.p3, a, anchor);
    rect.p4 = Vector2D_Scale(rect.p4, a, anchor);
    rect.center = Vector2D_Scale(rect.center, a, anchor);
    rect.length *= a; // *= --> Assignment after multiplication
    rect.width *= a;
    return rect;
}

Rectangle2D Rectangle2D_Rotate(Rectangle2D rect, float theta, Vector2D anchor)
{
    rect.p1 = Vector2D_Rotate(rect.p1, theta, anchor);
    rect.p2 = Vector2D_Rotate(rect.p2, theta, anchor);
    rect.p3 = Vector2D_Rotate(rect.p3, theta, anchor);
    rect.p4 = Vector2D_Rotate(rect.p4, theta, anchor);
    rect.center = Vector2D_Rotate(rect.center, theta, anchor);
    return rect;
}

void Rectangle2D_Print(Rectangle2D rect)
{
    printf("Rectangle:\n");
    printf("\n");
    printf("p1=(%.2f,%.2f)\np2=(%.2f,%.2f)\np3=(%.2f,%.2f)\np4=(%.2f,%.2f)\n", rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, rect.p3.x, rect.p3.y, rect.p4.x, rect.p4.y);
    printf("\n");
    printf("Center=(%.2f,%.2f)\nLength=%.2f\nWidth=%.2f\n", rect.center.x, rect.center.y, rect.length, rect.width);
}
