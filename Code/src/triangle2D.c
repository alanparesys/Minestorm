#include <math.h>
#include <stdio.h>
#include "triangle2D.h"

Triangle2D Triangle2D_SetFrom3Points(Vector2D p1, Vector2D p2, Vector2D p3)
{
    Triangle2D tri;
    tri.p1 = p1;
    tri.p2 = p2;
    tri.p3 = p3;

    float a = Vector2D_Distance(p1, p2);
    float b = Vector2D_Distance(p2, p3);
    float c = Vector2D_Distance(p3, p1);
    tri.a = a;
    tri.b = b;
    tri.c = c;

    Vector2D v1 = Vector2D_SetFrom2Points(p1, p2);
    Vector2D v2 = Vector2D_SetFrom2Points(p2, p3);
    Vector2D v3 = Vector2D_SetFrom2Points(p1, p3);

    float beta = Vector2D_GetAngle(Vector2D_Opposite(v1), v2);
    float gamma = Vector2D_GetAngle(Vector2D_Opposite(v2), Vector2D_Opposite(v3));
    float alpha = Vector2D_GetAngle(v1, v3);
    tri.alpha = alpha;
    tri.beta = beta;
    tri.gamma = gamma;

    return tri;
}

Triangle2D Triangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2)
{
    Vector2D p2 = Vector2D_Add(p1, v1);
    Vector2D p3 = Vector2D_Add(p1, v2);

    return Triangle2D_SetFrom3Points(p1, p2, p3);
}

float Triangle2D_GetSurface(Triangle2D tri)
{
    Vector2D v1 = Vector2D_SetFrom2Points(tri.p1, tri.p2);
    Vector2D v2 = Vector2D_SetFrom2Points(tri.p1, tri.p3);
    return fabs(Vector2D_CrossProduct(v1, v2)) / 2.0f; // fabs --> absolute value in floating point
}

int Triangle2D_IsSpecial(Triangle2D tri)
{
    float epsilon = 1e-3f; // epsilon 1e-3 (0.001) = average accuracy
    int special = 0;

    // Check equilateral
    if (fabs(tri.a - tri.b) < epsilon && fabs(tri.b - tri.c) < epsilon)
        special = 3;

    // Check isosceles
    else if (fabs(tri.a - tri.b) < epsilon || fabs(tri.a - tri.c) < epsilon || fabs(tri.b - tri.c) < epsilon)   // fabs --> absolute value in floating point
        special = 2;

    // Check rectangle (zero scalar product)
    Vector2D v1 = Vector2D_SetFrom2Points(tri.p1, tri.p2);
    Vector2D v2 = Vector2D_SetFrom2Points(tri.p2, tri.p3);
    Vector2D v3 = Vector2D_SetFrom2Points(tri.p3, tri.p1);

    if (fabs(Vector2D_DotProduct(v1, Vector2D_SetFrom2Points(tri.p1, tri.p3))) < epsilon ||
        fabs(Vector2D_DotProduct(v2, Vector2D_SetFrom2Points(tri.p2, tri.p1))) < epsilon || // fabs --> absolute value in floating point
        fabs(Vector2D_DotProduct(v3, Vector2D_SetFrom2Points(tri.p3, tri.p2))) < epsilon)
    {
        if (special == 2)
            special = 12; // rectangle and isosceles
        else
            special = 1;  // rectangle
    }

    return special;
}

// 0: nothing special , 1: right triangle , 2: isoceles triangle , 3: equilateral triangle, 
// 12: right and isoceles triangle

Vector2D Triangle2D_Circumcenter(Triangle2D tri)
{
    float D = 2 * (tri.p1.x * (tri.p2.y - tri.p3.y) +
        tri.p2.x * (tri.p3.y - tri.p1.y) +
        tri.p3.x * (tri.p1.y - tri.p2.y));

    float A2 = tri.p1.x * tri.p1.x + tri.p1.y * tri.p1.y;
    float B2 = tri.p2.x * tri.p2.x + tri.p2.y * tri.p2.y;
    float C2 = tri.p3.x * tri.p3.x + tri.p3.y * tri.p3.y;

    float cx = (A2 * (tri.p2.y - tri.p3.y) +
        B2 * (tri.p3.y - tri.p1.y) +
        C2 * (tri.p1.y - tri.p2.y)) / D;

    float cy = (A2 * (tri.p3.x - tri.p2.x) +
        B2 * (tri.p1.x - tri.p3.x) +
        C2 * (tri.p2.x - tri.p1.x)) / D;

    return Vector2D_SetFromComponents(cx, cy);
}

Vector2D Triangle2D_Isobarycenter(Triangle2D tri)
{
    Vector2D G;
    G.x = (tri.p1.x + tri.p2.x + tri.p3.x) / 3.0f;
    G.y = (tri.p1.y + tri.p2.y + tri.p3.y) / 3.0f;

    return G;
}

Triangle2D Triangle2D_Translate(Triangle2D tri, Vector2D t)
{
    Vector2D p1 = Vector2D_Translate(tri.p1, t);
    Vector2D p2 = Vector2D_Translate(tri.p2, t);
    Vector2D p3 = Vector2D_Translate(tri.p3, t);

    return Triangle2D_SetFrom3Points(p1, p2, p3);
}

Triangle2D Triangle2D_Scale(Triangle2D tri, float a, Vector2D anchor)
{
    Vector2D p1 = Vector2D_Scale(tri.p1, a, anchor);
    Vector2D p2 = Vector2D_Scale(tri.p2, a, anchor);
    Vector2D p3 = Vector2D_Scale(tri.p3, a, anchor);

    return Triangle2D_SetFrom3Points(p1, p2, p3);
}

Triangle2D Triangle2D_Rotate(Triangle2D tri, float theta, Vector2D anchor)
{
    Vector2D p1 = Vector2D_Rotate(tri.p1, theta, anchor);
    Vector2D p2 = Vector2D_Rotate(tri.p2, theta, anchor);
    Vector2D p3 = Vector2D_Rotate(tri.p3, theta, anchor);

    return Triangle2D_SetFrom3Points(p1, p2, p3);
}

// ------------------------------------------------------------------------------
// Bonus
Triangle2D Triangle2D_RotateAll(Triangle2D tri, float theta, Vector2D anchor)
{
    Vector2D p1 = Vector2D_Rotate(tri.p1, theta, anchor);
    Vector2D p2 = Vector2D_Rotate(tri.p2, theta, anchor);
    Vector2D p3 = Vector2D_Rotate(tri.p3, theta, anchor);

    return Triangle2D_SetFrom3Points(p1, p2, p3);
}
// ------------------------------------------------------------------------------

void Triangle_Print(Triangle2D tri)
{
    printf("pts : \n p1 = (%f, %f); \n p2 = (%f, %f); \n p3 = (%f, %f); \n\n", tri.p1.x, tri.p1.y, tri.p2.x, tri.p2.y, tri.p3.x, tri.p3.y);
    printf("normes : \n p1 p2 = %f; \n p2 p3 = %f; \n p3 p1 = %f; \n\n", tri.a, tri.b, tri.c);
    printf("angles : \n p1 = %f; \n p2 = %f; \n p3 = %f; \n\n", tri.alpha, tri.beta, tri.gamma);
}