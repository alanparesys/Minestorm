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

// Soon
Triangle2D Triangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2)
{
    Triangle2D tri;
    tri.p1 = p1;

    float a = Vector2D_Distance(p1, Vector2D_SetFromComponents(v1.x, v1.y));
    float b = Vector2D_Distance(p1, Vector2D_SetFromComponents(v2.x, v2.y));
    float c = Vector2D_Distance(Vector2D_SetFromComponents(v1.x, v1.y), Vector2D_SetFromComponents(v2.x, v2.y));
    tri.a = a;
    tri.b = b;
    tri.c = c;

    Vector2D v1 = Vector2D_SetFrom2Points(p1, Vector2D_SetFromComponents(v1.x, v1.y));
    Vector2D v2 = Vector2D_SetFrom2Points(p1, Vector2D_SetFromComponents(v2.x, v2.y));
    Vector2D v3 = Vector2D_SetFrom2Points(Vector2D_SetFromComponents(v1.x, v1.y), Vector2D_SetFromComponents(v2.x, v2.y));

    float beta = Vector2D_GetAngle(Vector2D_Opposite(v1), v2);
    float gamma = Vector2D_GetAngle(Vector2D_Opposite(v2), Vector2D_Opposite(v3));
    float alpha = Vector2D_GetAngle(v1, v3);
    tri.alpha = alpha;
    tri.beta = beta;
    tri.gamma = gamma;

    return tri;
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
    if (tri.alpha == 90.0f || tri.beta == 90.0f || tri.gamma == 90.0f)
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
    float D = 2 * (tri.p1.x * (tri.p2.y - tri.p3.y) + tri.p2.x * (tri.p2.y - tri.p1.y) + tri.p3.x * (tri.p1.y - tri.p2.y));        // D = 2 * (A.x*(B.y-C.y) + B.x*(C.y-A.y) + C.x*(A.y-B.y))

    float A2 = pow(tri.p1.x, 2) + pow(tri.p1.y, 2);                                                                                // A2 = A.x² + A.y²
    float B2 = pow(tri.p2.x, 2) + pow(tri.p2.y, 2);                                                                                // B2 = B.x² + B.y²
    float C2 = pow(tri.p3.x, 2) + pow(tri.p3.y, 2);                                                                                // C2 = C.x² + C.y²

    float centerx = (A2 * (tri.p2.y - tri.p3.y) + B2 * (tri.p3.y - tri.p1.y) + C2 * (tri.p1.y - tri.p2.y)) / D;                    // centre.x = (A2*(B.y-C.y) + B2*(C.y-A.y) + C2*(A.y-B.y)) / D
    float centery = (A2 * (tri.p3.x - tri.p2.x) + B2 * (tri.p1.x - tri.p3.x) + C2 * (tri.p2.x - tri.p1.x)) / D;                    // centre.y = (A2*(C.x-B.x) + B2*(A.x-C.x) + C2*(B.x-A.x)) / D

    Vector2D center = Vector2D_SetFromComponents(centerx, centery);
    return center;
}

Vector2D Triangle2D_Isobarycenter(Triangle2D tri)
{
    float Gx = (tri.p1.x + tri.p2.x + tri.p3.x) / 3;
    float Gy = (tri.p1.y + tri.p2.y + tri.p3.y) / 3;

    Vector2D grav = Vector2D_SetFromComponents(Gx, Gy);
    return grav;
}

Triangle2D Triangle2D_Translate(Triangle2D tri, Vector2D t)
{
    Vector2D nvp1 = Vector2D_Add(tri.p1, t);
    Vector2D nvp2 = Vector2D_Add(tri.p2, t);
    Vector2D nvp3 = Vector2D_Add(tri.p3, t);

    Triangle2D nvtri = Triangle2D_SetFrom3Points(nvp1, nvp2, nvp3);
    return nvtri;
}

Triangle2D Triangle2D_Scale(Triangle2D tri, float a, Vector2D anchor)
{
    Triangle2D nvp;
    nvp.p1.x = anchor.x + (tri.p1.x - anchor.x) * a;
    nvp.p1.y = anchor.y + (tri.p1.y - anchor.y) * a;

    nvp.p2.x = anchor.x + (tri.p2.x - anchor.x) * a;
    nvp.p2.y = anchor.y + (tri.p2.y - anchor.y) * a;

    nvp.p3.x = anchor.x + (tri.p3.x - anchor.x) * a;
    nvp.p3.y = anchor.y + (tri.p3.y - anchor.y) * a;

    Vector2D nvp1 = Vector2D_SetFromComponents(nvp.p1.x, nvp.p1.y);
    Vector2D nvp2 = Vector2D_SetFromComponents(nvp.p2.x, nvp.p2.y);
    Vector2D nvp3 = Vector2D_SetFromComponents(nvp.p3.x, nvp.p3.y);

    Triangle2D nvtri = Triangle2D_SetFrom3Points(nvp1, nvp2, nvp3);

    return nvtri;
}

Triangle2D Triangle2D_Rotate(Triangle2D tri, float theta, Vector2D anchor)
{
    Vector2D gtri = Triangle2D_Isobarycenter(tri);

    Vector2D GA = Vector2D_SetFrom2Points(gtri, tri.p1);
    Vector2D GB = Vector2D_SetFrom2Points(gtri, tri.p2);
    Vector2D GC = Vector2D_SetFrom2Points(gtri, tri.p3);

    Vector2D nvgtri;
    nvgtri.x = anchor.x * cos(theta) + gtri.y * sin(theta) - 0;
    nvgtri.y = -(anchor.x) * sin(theta) + gtri.y * cos(theta) - 0;

    Triangle2D nvp;
    nvp.p1.x = nvgtri.x + GA.x;
    nvp.p1.y = nvgtri.y + GA.y;
    nvp.p2.x = nvgtri.x + GB.x;
    nvp.p2.y = nvgtri.y + GB.y;
    nvp.p3.x = nvgtri.x + GC.x;
    nvp.p3.y = nvgtri.y + GC.y;

    Vector2D nvp1 = Vector2D_SetFromComponents(nvp.p1.x, nvp.p1.y);
    Vector2D nvp2 = Vector2D_SetFromComponents(nvp.p2.x, nvp.p2.y);
    Vector2D nvp3 = Vector2D_SetFromComponents(nvp.p3.x, nvp.p3.y);

    Triangle2D nvtri = Triangle2D_SetFrom3Points(nvp1, nvp2, nvp3);

    return nvtri;
}

void Triangle_Print(Triangle2D tri)
{
    printf("triangle : \n p1 = (%f, %f); \n p2 = (%f, %f); \n p3 = (%f, %f)", tri.p1.x, tri.p1.y, tri.p2.x, tri.p2.y, tri.p3.x, tri.p3.y);
}