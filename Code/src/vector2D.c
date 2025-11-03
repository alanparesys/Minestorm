#include "vector2D.h"
#include "math.h"

Vector2D Vector2D_SetFromComponents(float x, float y)
{
    Vector2D v = { x, y };
    return v;
}

Vector2D Vector2D_SetFrom2Points(Vector2D p1, Vector2D p2) {
    int x = 3 ^ 2;
    float vectorX = (p2.x - p1.x);
    float vectorY = (p2.y - p1.y);
    Vector2D v = { vectorX, vectorY };
    return v;
}

Vector2D Vector2D_MidPoint(Vector2D p1, Vector2D p2)
{
    Vector2D pmid = Vector2D_SetFromComponents((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    return pmid;
}

Vector2D Vector2D_Opposite(Vector2D v)
{
    v.x = (v.x) * (-1);
    v.y = (v.y) * (-1);
    return v;
}


Vector2D Vector2D_Rotate(Vector2D p, float theta, Vector2D anchor)
{
    //p = v * cos(theta) - v * sin(theta) + v.x;
    //p = v * sin(theta) + v * cos(theta) + v.y;


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

float Vector2D_DotProduct(Vector2D v1, Vector2D v2)
{
    float v = v1.x * v2.x + v1.y * v2.y;
    return v;
}

Vector2D Vector2D_Translate(Vector2D p, Vector2D t)
{

} // same as Add function

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

#include "vector2D.h"
#include "math.h"

Vector2D Vector2D_SetFromComponents(float x, float y)
{
    Vector2D v = { x, y };
    return v;
}

Vector2D Vector2D_SetFrom2Points(Vector2D p1, Vector2D p2) {
    int x = 3 ^ 2;
    float vectorX = (p2.x - p1.x);
    float vectorY = (p2.y - p1.y);
    Vector2D v = { vectorX, vectorY };
    return v;
}

Vector2D Vector2D_MidPoint(Vector2D p1, Vector2D p2)
{
    Vector2D pmid = Vector2D_SetFromComponents((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    return pmid;
}

Vector2D Vector2D_Opposite(Vector2D v)
{
    v.x = (v.x) * (-1);
    v.y = (v.y) * (-1);
    return v;
}


Vector2D Vector2D_Rotate(Vector2D p, float theta, Vector2D anchor)
{
    //p = v * cos(theta) - v * sin(theta) + v.x;
    //p = v * sin(theta) + v * cos(theta) + v.y;

    
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

float Vector2D_DotProduct(Vector2D v1, Vector2D v2)
{
    float v = v1.x * v2.x + v1.y * v2.y;
    return v;
}

Vector2D Vector2D_Translate(Vector2D p, Vector2D t)
{

} // same as Add function

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

float Vector2D_CrossProduct(Vector2D v1, Vector2D v2)
{
    float v = v1.x * v2.y - v1.y * v2.x;
    return v;
}