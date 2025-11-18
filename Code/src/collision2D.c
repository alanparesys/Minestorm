#include "collision2D.h"
#include <math.h>
#include <float.h>

static const float COLLISION_EPSILON = 0.001f;

static bool PolygonIsValid(Polygone2D poly)
{
    return poly.point != NULL;
}

static Vector2D Normalize(Vector2D v)
{
    float norm = Vector2D_Norm(v);
    if (norm < COLLISION_EPSILON)
    {
        return Vector2D_SetFromComponents(0.0f, 0.0f);
    }
    return Vector2D_SetFromComponents(v.x / norm, v.y / norm);
}

static void ProjectPolygonOntoAxis(Polygone2D poly, Vector2D axis, float* min, float* max)
{
    if (!PolygonIsValid(poly))
    {
        *min = 0.0f;
        *max = 0.0f;
        return;
    }

    Points* current = poly.point;
    bool first = true;
    while (current != NULL)
    {
        float projection = Vector2D_DotProduct(current->coordinate, axis);
        if (first)
        {
            *min = *max = projection;
            first = false;
        }
        else
        {
            if (projection < *min) *min = projection;
            if (projection > *max) *max = projection;
        }
        current = current->next;
    }
}

static bool OverlapIntervals(float minA, float maxA, float minB, float maxB)
{
    return !(maxA < minB || maxB < minA);
}

static bool TestPolygonAxis(Polygone2D polyA, Polygone2D polyB, Vector2D axis)
{
    Vector2D normalizedAxis = Normalize(axis);
    if (Vector2D_Norm(normalizedAxis) < COLLISION_EPSILON)
    {
        return true;
    }

    float minA, maxA, minB, maxB;
    ProjectPolygonOntoAxis(polyA, normalizedAxis, &minA, &maxA);
    ProjectPolygonOntoAxis(polyB, normalizedAxis, &minB, &maxB);
    return OverlapIntervals(minA, maxA, minB, maxB);
}

static bool TestCircleAxis(Polygone2D poly, Sphere2D circle, Vector2D axis)
{
    Vector2D normalizedAxis = Normalize(axis);
    if (Vector2D_Norm(normalizedAxis) < COLLISION_EPSILON)
    {
        return true;
    }

    float minPoly, maxPoly;
    ProjectPolygonOntoAxis(poly, normalizedAxis, &minPoly, &maxPoly);

    float centerProjection = Vector2D_DotProduct(circle.center, normalizedAxis);
    float minCircle = centerProjection - circle.radius;
    float maxCircle = centerProjection + circle.radius;

    return OverlapIntervals(minPoly, maxPoly, minCircle, maxCircle);
}

static Vector2D EdgeNormal(Vector2D a, Vector2D b)
{
    Vector2D edge = Vector2D_SetFrom2Points(a, b);
    Vector2D normal = Vector2D_SetFromComponents(-edge.y, edge.x);
    return Normalize(normal);
}

bool Collision_PointPoint(Vector2D a, Vector2D b, float epsilon)
{
    return (fabsf(a.x - b.x) <= epsilon) && (fabsf(a.y - b.y) <= epsilon);
}

bool Collision_PointCircle(Vector2D point, Sphere2D circle)
{
    return Vector2D_Distance(point, circle.center) <= circle.radius;
}

bool Collision_CircleCircle(Sphere2D a, Sphere2D b)
{
    float distance = Vector2D_Distance(a.center, b.center);
    return distance <= (a.radius + b.radius);
}

bool Collision_PointPolygon(Vector2D point, Polygone2D poly)
{
    if (!PolygonIsValid(poly))
    {
        return false;
    }

    bool inside = false;
    Points* current = poly.point;
    while (current != NULL)
    {
        Points* next = current->next ? current->next : poly.point;
        Vector2D a = current->coordinate;
        Vector2D b = next->coordinate;

        bool intersect = ((a.y > point.y) != (b.y > point.y)) &&
            (point.x < (b.x - a.x) * (point.y - a.y) / ((b.y - a.y) + COLLISION_EPSILON) + a.x);

        if (intersect)
        {
            inside = !inside;
        }

        current = current->next;
    }

    return inside;
}

bool Collision_CirclePolygon(Sphere2D circle, Polygone2D poly)
{
    if (!PolygonIsValid(poly))
    {
        return false;
    }

    // Check if circle center is inside polygon
    if (Collision_PointPolygon(circle.center, poly))
    {
        return true;
    }

    // SAT against polygon edges
    Points* current = poly.point;
    while (current != NULL)
    {
        Points* next = current->next ? current->next : poly.point;
        Vector2D axis = EdgeNormal(current->coordinate, next->coordinate);
        if (!TestCircleAxis(poly, circle, axis))
        {
            return false;
        }
        current = current->next;
    }

    // Axis from circle center to closest vertex
    float closestDistance = FLT_MAX;
    Vector2D closestVertex = poly.point->coordinate;
    current = poly.point;
    while (current != NULL)
    {
        float distance = Vector2D_Distance(current->coordinate, circle.center);
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestVertex = current->coordinate;
        }
        current = current->next;
    }

    Vector2D axis = Vector2D_SetFrom2Points(circle.center, closestVertex);
    axis = Normalize(axis);
    if (Vector2D_Norm(axis) < COLLISION_EPSILON)
    {
        return true;
    }

    return TestCircleAxis(poly, circle, axis);
}

bool Collision_PolygonPolygon(Polygone2D polyA, Polygone2D polyB)
{
    if (!PolygonIsValid(polyA) || !PolygonIsValid(polyB))
    {
        return false;
    }

    Points* current = polyA.point;
    while (current != NULL)
    {
        Points* next = current->next ? current->next : polyA.point;
        Vector2D axis = EdgeNormal(current->coordinate, next->coordinate);
        if (!TestPolygonAxis(polyA, polyB, axis))
        {
            return false;
        }
        current = current->next;
    }

    current = polyB.point;
    while (current != NULL)
    {
        Points* next = current->next ? current->next : polyB.point;
        Vector2D axis = EdgeNormal(current->coordinate, next->coordinate);
        if (!TestPolygonAxis(polyA, polyB, axis))
        {
            return false;
        }
        current = current->next;
    }

    return true;
}

bool Collision_PointAABB(Vector2D point, AABB2D box)
{
    return (point.x >= box.min.x && point.x <= box.max.x &&
            point.y >= box.min.y && point.y <= box.max.y);
}

bool Collision_CircleAABB(Sphere2D circle, AABB2D box)
{
    float clampedX = fmaxf(box.min.x, fminf(circle.center.x, box.max.x));
    float clampedY = fmaxf(box.min.y, fminf(circle.center.y, box.max.y));
    Vector2D closestPoint = Vector2D_SetFromComponents(clampedX, clampedY);
    return Collision_PointCircle(closestPoint, circle);
}

bool Collision_AABBAABB(AABB2D a, AABB2D b)
{
    if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
    return true;
}

