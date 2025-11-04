#include <math.h>
#include <stdio.h>
#include "triangle2D.h"

Triangle2D Triangle2D_SetFrom3Points(Vector2D p1, Vector2D p2, Vector2D p3)
{
    // Vec A
    float ax = p2.x - p1.x;
    float ay = p2.y - p1.y;
    float a = ax + ay;

    // Vec B
    float bx = p3.x - p2.x;
    float by = p3.y - p2.y;
    float b = bx + by;

    // Vec C
    float cx = p3.x - p1.x;
    float cy = p3.y - p1.y;
    float c = cx + cy;

    // Angle of 2 Vectors
    float a_b = ax * bx + ay * by;  // a.b
    float b_c = bx * cx + by * cy;  // b.c
    float a_c = ax * cx + ay * cy;  // a.c

    Triangle2D tri = { a, b, c };
    Triangle2D angletri = { a_b, b_c, a_c };

    return tri;
    return angletri;
}

Triangle2D Triangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2)
{

}

float Triangle2D_GetSurface(Triangle2D tri)
{
    float a = Vector2D_Distance(tri.p1, tri.p2);
    float b = Vector2D_Distance(tri.p2, tri.p3);
    float c = Vector2D_Distance(tri.p3, tri.p1);
    float p = (a + b + c) / 2;
    float surf = sqrt(p * (p - a) * (p - b) * (p - c));
    return surf;
}

int Triangle2D_IsSpecial(Triangle2D tri)
{
    int specialtri = 0;

    //tri is equilateral
    if (tri.a == tri.b && tri.a == tri.c)
    {
        specialtri = 3;
    }
    //tri is isocele
    else if (tri.a == tri.b || tri.a == tri.c)
    {
        specialtri = 2;
    }
    //tri is right
    if (tri.alpha == 90.0f || tri.beta == 90.0f || tri.gamma == 90.0)
    {
        //tri is also isocele
        if (specialtri == 2)
        {
            specialtri == 12;
        }
        else
        {
            specialtri == 1;
        }
    }
    return specialtri;
}

// 0: nothing special , 1: right triangle , 2: isoceles triangle , 3: equilateral triangle, 
// 12: right and isoceles triangle

Vector2D Triangle2D_Circumcenter(Triangle2D tri)
{

}

Vector2D Triangle2D_Isobarycenter(Triangle2D tri)
{

}

Triangle2D Triangle2D_Translate(Triangle2D tri, Vector2D t)
{

}

Triangle2D Triangle2D_Scale(Triangle2D tri, float a, Vector2D anchor)
{

}

Triangle2D Triangle2D_Rotate(Triangle2D tri, float theta, Vector2D anchor)
{

}

void Triangle_Print(Triangle2D tri)
{

}