#include <math.h>
#include <stdio.h>

#include "triangle2D.h"

Triangle2D Triangle2D_SetFrom3Points(Vector2D p1, Vector2D p2, Vector2D p3)
{
    float vectorX = (p2.x - p1.x);
    float vectorY = (p2.y - p1.y);
    float vectorZ = (p3.z - p3.z);
    Triangle2D v = { vectorX, vectorY };
    return v;
}

Triangle2D Triangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2)
{

}

float Triangle2D_GetSurface(Triangle2D tri)
{

}

int Triangle2D_IsSpecial(Triangle2D tri) // return a number
{

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