#include "aabb2D.h"
#include <math.h>

// Build an AABB using a center and half-size (absolute values ensure correctness)
AABB2D AABB2D_FromCenterHalfSize(Vector2D center, Vector2D halfSize)
{
    AABB2D box;
    box.min = Vector2D_SetFromComponents(center.x - fabsf(halfSize.x), center.y - fabsf(halfSize.y));
    box.max = Vector2D_SetFromComponents(center.x + fabsf(halfSize.x), center.y + fabsf(halfSize.y));
    return box;
}

// Build an AABB directly from min and max coordinates
AABB2D AABB2D_FromMinMax(Vector2D min, Vector2D max)
{
    AABB2D box;
    box.min = min;
    box.max = max;
    return box;
}

// Compute an AABB that fully contains a Rectangle2D
AABB2D AABB2D_FromRectangle(Rectangle2D rect)
{
    Vector2D min = rect.p1; // Start with first point
    Vector2D max = rect.p1;

    Vector2D points[4] = { rect.p1, rect.p2, rect.p3, rect.p4 }; // All rectangle corners

    // Expand min/max bounds based on all points
    for (int i = 0; i < 4; i++)
    {
        if (points[i].x < min.x) min.x = points[i].x;
        if (points[i].y < min.y) min.y = points[i].y;
        if (points[i].x > max.x) max.x = points[i].x;
        if (points[i].y > max.y) max.y = points[i].y;
    }

    return AABB2D_FromMinMax(min, max);
}

// Return the center point of the AABB
Vector2D AABB2D_GetCenter(AABB2D box)
{
    return Vector2D_SetFromComponents(
        (box.min.x + box.max.x) * 0.5f,
        (box.min.y + box.max.y) * 0.5f
    );
}

// Return half-size vector (half width, half height)
Vector2D AABB2D_GetHalfSize(AABB2D box)
{
    return Vector2D_SetFromComponents(
        (box.max.x - box.min.x) * 0.5f,
        (box.max.y - box.min.y) * 0.5f
    );
}

// Return full width and height
Vector2D AABB2D_GetSize(AABB2D box)
{
    return Vector2D_SetFromComponents(
        (box.max.x - box.min.x),
        (box.max.y - box.min.y)
    );
}

// Check if a point lies inside the AABB
bool AABB2D_ContainsPoint(AABB2D box, Vector2D point)
{
    return (point.x >= box.min.x && point.x <= box.max.x &&
        point.y >= box.min.y && point.y <= box.max.y);
}