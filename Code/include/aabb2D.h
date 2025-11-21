#ifndef AABB2D_H
#define AABB2D_H

#include <stdbool.h>
#include "Vector2D.h"
#include "rectangle2D.h"

// Represents an axis-aligned bounding box in 2D
// Stores minimum and maximum coordinates
typedef struct AABB2D
{
    Vector2D min; // Lower-left corner of the box
    Vector2D max; // Upper-right corner of the box
} AABB2D;

// Create an AABB from a center point and half-size (for width/height)
AABB2D AABB2D_FromCenterHalfSize(Vector2D center, Vector2D halfSize);

// Create an AABB directly from min and max coordinates
AABB2D AABB2D_FromMinMax(Vector2D min, Vector2D max);

// Build an AABB from a Rectangle2D definition
AABB2D AABB2D_FromRectangle(Rectangle2D rect);

// Compute and return the geometric center of the box
Vector2D AABB2D_GetCenter(AABB2D box);

// Get half-size vector (half-width, half-height)
Vector2D AABB2D_GetHalfSize(AABB2D box);

// Get full size of the bounding box
Vector2D AABB2D_GetSize(AABB2D box);

// Check if a given point lies inside the bounding box
bool AABB2D_ContainsPoint(AABB2D box, Vector2D point);

#endif