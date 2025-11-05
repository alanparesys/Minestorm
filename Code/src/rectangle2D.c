#include "rectangle2D.h"
#include "vector2D.h"
#include <math.h>
#include <stdio.h>

Rectangle2D Rectangle2D_SetFrom4Points(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4) {
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

Rectangle2D Rectangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2) {
    Rectangle2D my_rectangle;
    my_rectangle.p1 = p1; // le coin gauche du rectangle

    my_rectangle.p2 = Vector2D_Add(p1, v1); // on par de p1 : p1 + v1 et on arrive sur p2
    //v1 c'est genre le coté p1p2 

    my_rectangle.p3 = Vector2D_Add(my_rectangle.p2, v2); // on par de p2 : my_rectangle.p2 + v2
    //du coup v2 c'est le coté p2p3

    my_rectangle.p4 = Vector2D_Add(p1, v2); // on par de p1 : p1 + v2 et on arrive sur p4

    Vector2D center = Vector2D_SetFromComponents(
        (my_rectangle.p1.x + my_rectangle.p2.x + my_rectangle.p3.x + my_rectangle.p4.x) / 4,
        (my_rectangle.p1.y + my_rectangle.p2.y + my_rectangle.p3.y + my_rectangle.p4.y) / 4); // tout ça pour le centre
    my_rectangle.center = center;

    float length = Vector2D_Distance(my_rectangle.p1, my_rectangle.p2);
    float width = Vector2D_Distance(my_rectangle.p2, my_rectangle.p3);
    my_rectangle.length = length;
    my_rectangle.width = width;

    return my_rectangle;
}

Rectangle2D Rectangle2D_SetFromCenterLengthWidthAngle(Vector2D center, float length, float width, float angle) {
    Rectangle2D rect;

    float half_length = length / 2.0f;
    float half_width = width / 2.0f;

    Vector2D p1 = Vector2D_SetFromComponents(-half_length, -half_width);
    Vector2D p2 = Vector2D_SetFromComponents(half_length, -half_width);
    Vector2D p3 = Vector2D_SetFromComponents(half_length, half_width);
    Vector2D p4 = Vector2D_SetFromComponents(-half_length, half_width);

    rect.p1 = Vector2D_Add(Vector2D_Rotate(p1, angle), center);
    rect.p2 = Vector2D_Add(Vector2D_Rotate(p2, angle), center);
    rect.p3 = Vector2D_Add(Vector2D_Rotate(p3, angle), center);
    rect.p4 = Vector2D_Add(Vector2D_Rotate(p4, angle), center);

    rect.center = center;
    rect.length = length;
    rect.width = width;

    return rect;
}

float Rectangle2D_GetSurface(Rectangle2D rect) {
    float length = Vector2D_Distance(rect.p1, rect.p2);
    float width = Vector2D_Distance(rect.p2, rect.p3);
    float surface = length * width;

    return surface;
}

Rectangle2D Rectangle2D_Translate(Rectangle2D rect, Vector2D t) {
    Rectangle2D nv_rectangle = Rectangle2D_SetFrom4Points(rect.p1, rect.p2, rect.p3, rect.p4);
    nv_rectangle.p1 = Vector2D_Add(nv_rectangle.p1, t);
    nv_rectangle.p2 = Vector2D_Add(nv_rectangle.p2, t);
    nv_rectangle.p3 = Vector2D_Add(nv_rectangle.p3, t);
    nv_rectangle.p4 = Vector2D_Add(nv_rectangle.p4, t);

    return nv_rectangle;
}

Rectangle2D Rectangle2D_Scale(Rectangle2D rect, float a, Vector2D anchor) {
    Rectangle2D scaled_rect;

    scaled_rect.p1 = Vector2D_Scale(rect.p1, a, anchor);
    scaled_rect.p2 = Vector2D_Scale(rect.p2, a, anchor);
    scaled_rect.p3 = Vector2D_Scale(rect.p3, a, anchor);
    scaled_rect.p4 = Vector2D_Scale(rect.p4, a, anchor);

    scaled_rect.center = Vector2D_Scale(rect.center, a, anchor);

    scaled_rect.length = rect.length * a;
    scaled_rect.width = rect.width * a;

    return scaled_rect;
}

Rectangle2D Rectangle2D_Rotate(Rectangle2D rect, float theta, Vector2D anchor) {
    Rectangle2D rotated_rect;

    rotated_rect.p1 = Vector2D_Rotate(rect.p1, theta, anchor);
    rotated_rect.p2 = Vector2D_Rotate(rect.p2, theta, anchor);
    rotated_rect.p3 = Vector2D_Rotate(rect.p3, theta, anchor);
    rotated_rect.p4 = Vector2D_Rotate(rect.p4, theta, anchor);
    rotated_rect.center = Vector2D_Rotate(rect.center, theta, anchor);

    rotated_rect.length = rect.length;
    rotated_rect.width = rect.width;

    return rotated_rect;
}

void Rectangle2D_Print(Rectangle2D rect)
{
    printf("rectangle : \n p1 = (%f, %f); \n p2 = (%f, %f); \n p3 = (%f, %f) \n p4 = (%f, %f)", rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, rect.p3.x, rect.p3.y, rect.p4.x, rect.p4.y);
}