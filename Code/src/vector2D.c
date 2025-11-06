#include "vector2D.h"
#include "math.h"
#include "stdio.h"

Vector2D Vector2D_SetFromComponents(float x, float y)
{
    Vector2D v = { x, y };

    return v;
}

Vector2D Vector2D_SetFrom2Points(Vector2D p1, Vector2D p2)
{
    int x = 3 ^ 2;
    float vectorX = (p2.x - p1.x);
    float vectorY = (p2.y - p1.y);
    Vector2D v = { vectorX, vectorY };

    return v;
}

Vector2D Vector2D_Opposite(Vector2D v)
{
    v.x = (v.x) * (-1);
    v.y = (v.y) * (-1);

    return v;
}

Vector2D Vector2D_MidPoint(Vector2D p1, Vector2D p2)
{
    Vector2D pmid = Vector2D_SetFromComponents((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);

    return pmid;
}

float Vector2D_Distance(Vector2D p1, Vector2D p2)
{
    float vx = p2.x - p1.x;
    float vy = p2.y - p1.y;
    double v = sqrt(vx * vx + vy * vy);

    return v;
}

Vector2D Vector2D_Add(Vector2D v1, Vector2D v2)
{

    float vx = v1.x + v2.x;
    float vy = v1.y + v2.y;
    Vector2D v = Vector2D_SetFromComponents(vx, vy);

    return v;
}

float Vector2D_SquaredNorm(Vector2D v)
{
    float squarednorm = pow(v.x, 2) + pow(v.y, 2);

    return squarednorm;
}

float Vector2D_Norm(Vector2D v)
{
    float vnorm = sqrt(Vector2D_SquaredNorm(v));

    return vnorm;
}

float Vector2D_DotProduct(Vector2D v1, Vector2D v2)
{
    float v = v1.x * v2.x + v1.y * v2.y;

    return v;
}

float Vector2D_CrossProduct(Vector2D v1, Vector2D v2)
{
    float v = v1.x * v2.y - v1.y * v2.x;

    return v;
}

float Vector2D_GetAngle(Vector2D v1, Vector2D v2)       // in radian
{
    float normv1 = Vector2D_Norm(v1);
    float normv2 = Vector2D_Norm(v2);

    // avoid division by zero
    if (normv1 == 0.0f || normv2 == 0.0f)
    {
        return 0.0f;
    }

    float cosTheta = Vector2D_DotProduct(v1, v2) / (normv1 * normv2);

    // Clamp the value to avoid errors due to floating point inaccuracies
    if (cosTheta > 1.0f)
    {
        cosTheta = 1.0f;
    }
    if (cosTheta < -1.0f)
    {
        cosTheta = -1.0f;
    }

    float angle = acosf(cosTheta);

    return angle;
}

Vector2D Vector2D_Translate(Vector2D p, Vector2D t)
{
    return Vector2D_Add(p, t);
}

Vector2D Vector2D_Scale(Vector2D p, float a, Vector2D anchor)
{
    if (a == 0)
    {
        return anchor;
    }
    if (a == 1)
    {
        return p;
    }
    Vector2D nvp;
    nvp.x = anchor.x + (p.x - anchor.x) * a;
    nvp.y = anchor.y + (p.y - anchor.y) * a;

    return nvp;
}

Vector2D Vector2D_Rotate(Vector2D p, float theta, Vector2D anchor)
{
    float x = p.x - anchor.x;
    float y = p.y - anchor.y;

    float p1 = anchor.x + x * cos(theta) - y * sin(theta);
    float p2 = anchor.y + x * sin(theta) + y * cos(theta);

    return Vector2D_SetFromComponents(p1, p2);
}

void Vector2D_Print(Vector2D v)
{
    printf("(% f, % f)", v.x, v.y);
}
    

