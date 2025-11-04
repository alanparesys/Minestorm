#ifndef TRIANGLE2D_H
#define TRIANGLE2D_H

#include "Vector2D.h"
typedef struct Triangle2D
{
	Vector2D p1;
	Vector2D p2;
	Vector2D p3;

	float a; // distance between p1 and p2
	float b; // distance between p2 and p3
	float c; // distance between p3 and p1

	float alpha; // angle at p1
	float beta; // angle at p2
	float gamma; // angle at p3
} Triangle2D;

Triangle2D Triangle2D_SetFrom3Points(Vector2D p1, Vector2D p2, Vector2D p3);
Triangle2D Triangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2);
float Triangle2D_GetSurface(Triangle2D tri);
int Triangle2D_IsSpecial(Triangle2D tri); // return a number
// 0: nothing special , 1: right triangle , 2: isoceles triangle , 3: equilateral triangle, 
// 12: right and isoceles triangle
Vector2D Triangle2D_Circumcenter(Triangle2D tri);
Vector2D Triangle2D_Isobarycenter(Triangle2D tri);

Triangle2D Triangle2D_Translate(Triangle2D tri, Vector2D t);
Triangle2D Triangle2D_Scale(Triangle2D tri, float a, Vector2D anchor);
Triangle2D Triangle2D_Rotate(Triangle2D tri, float theta, Vector2D anchor);

void Triangle_Print(Triangle2D tri);

#endif
