#include "rectangle2D.h"
#include "vector2D.h"
#include <math.h>
#include <stdio.h>

Rectangle2D Rectangle2D_SetFrom4Points(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
{
    Rectangle2D new_rectangle;
    new_rectangle.p1 = p1;
    new_rectangle.p2 = p2;
    new_rectangle.p3 = p3;
    new_rectangle.p4 = p4;

    Vector2D center = { ((p1.x + p2.x + p3.x + p4.x) / 4), ((p1.y + p2.y + p3.y + p4.y) / 4) };
    new_rectangle.center = center;

    float length = Vector2D_Distance(p1, p2);
    float width = Vector2D_Distance(p2, p3);
    new_rectangle.length = length;
    new_rectangle.width = width;

    return new_rectangle;
}

Rectangle2D Rectangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2)
{
    Rectangle2D my_rectangle;
    my_rectangle.p1 = p1;

    my_rectangle.p2 = my_rectangle.p1 + Vector2D_SetFromComponents(v1.x, v1.y);

    my_rectangle.p4 = my_rectangle.p1 + Vector2D_SetFromComponents(v2.x, v2.y);

    Vector2D center = { ((p1.x + my_rectangle.p2.x + my_rectangle.p3.x + my_rectangle.p4.x) / 4), ((p1.y + my_rectangle.p2.y + my_rectangle.p3.y + my_rectangle.p4.y) / 4) };
    my_rectangle.center = center;

    float length = Vector2D_Distance(my_rectangle.p1, my_rectangle.p2);
    float width = Vector2D_Distance(my_rectangle.p1, my_rectangle.p4);
    my_rectangle.length = length;
    my_rectangle.width = width;
}

Rectangle2D Rectangle2D_SetFromCenterLengthWidthAngle(Vector2D center, float length, float width, float angle)
{
}

float Rectangle2D_GetSurface(Rectangle2D rect)
{
    float length = Vector2D_Distance(rect.p1, rect.p2);
    float width = Vector2D_Distance(rect.p2, rect.p3);
    float Surface = length * width;

    return Surface;
}
Rectangle2D Rectangle2D_Translate(Rectangle2D rect, Vector2D t)
{
    Rectangle2D nv_rectangle = Rectangle2D_SetFrom4Points(rect.p1, rect.p2, rect.p3, rect.p4);
    nv_rectangle.p1 = Vector2D_Add(nv_rectangle.p1, t);
    nv_rectangle.p2 = Vector2D_Add(nv_rectangle.p2, t);
    nv_rectangle.p3 = Vector2D_Add(nv_rectangle.p3, t);
    nv_rectangle.p4 = Vector2D_Add(nv_rectangle.p4, t);
}

Rectangle2D Rectangle2D_Scale(Rectangle2D rect, float a, Vector2D anchor)
{
}

Rectangle2D Rectangle2D_Rotate(Rectangle2D rect, float theta, Vector2D anchor)
{
}

void Rectangle2D_Print(Rectangle2D rect)
{
    printf("rectangle : \n p1 = (%f, %f); \n p2 = (%f, %f); \n p3 = (%f, %f) \n p4 = (%f, %f)", rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, rect.p3.x, rect.p3.y, rect.p4.x, rect.p4.y);
}