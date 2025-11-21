#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "polygone2D.h"
#include "vector2D.h"
#include <stdarg.h>

Polygone2D Polygone2D_SetFromPoints(int Nbsommets, ...)
{
    Polygone2D newPoly;
    newPoly.point = NULL;

    va_list args;
    va_start(args, Nbsommets);

    if (Nbsommets > 0)
    {
        Points* firstPoint = NULL;
        Points* prev = NULL;

        for (int i = 0; i < Nbsommets; i++)
        {
            Vector2D v = va_arg(args, Vector2D);

            Points* newPoint = malloc(sizeof(Points));
            if (!newPoint)
            {
                printf("Memory allocation error.\n");
                Points_Free(firstPoint);
                va_end(args);
                return newPoly;
            }

            newPoint->coordinate = v;
            newPoint->next = NULL;

            if (!firstPoint)
                firstPoint = newPoint;
            else
                prev->next = newPoint;

            prev = newPoint;
        }

        newPoly.point = firstPoint;
    }
    else if (Nbsommets == -1)
    {
        // We pass a linked list directly
        Points* firstPoint = va_arg(args, Points*);
        newPoly.point = firstPoint;
    }

    va_end(args);
    return newPoly;
}



void Points_Free(Points* firstPoint)
{
    Points* current = firstPoint;
    while (current != NULL)
    {
        Points* next = current->next;
        free(current);
        current = next;
    }
}

Polygone2D Polygone2D_Translate(Polygone2D poly, Vector2D t)
{
    if (poly.point == NULL)
    {
        printf("First point = NULL\n");
        return poly;
    }

    Points* current = poly.point;
    Points* firstPoint = NULL;
    Points* prev = NULL;

    while (current != NULL)
    {
        Points* newPoint = malloc(sizeof(Points));
        if (newPoint == NULL)
        {
            printf("Memory allocation error.\n");
            Points_Free(firstPoint);
            return poly;
        }

        newPoint->coordinate = Vector2D_Translate(current->coordinate, t);
        newPoint->next = NULL;

        if (firstPoint == NULL)
        {
            firstPoint = newPoint;
        }
        else
        {
            prev->next = newPoint;
        }

        prev = newPoint;
        current = current->next;
    }

    Polygone2D newPoly = Polygone2D_SetFromPoints(-1, firstPoint);  // -1 to say that we are passing a linked list
    return newPoly;
}

Polygone2D Polygone2D_Scale(Polygone2D poly, float a, Vector2D anchor)
{
    if (poly.point == NULL)
    {
        printf("First point = NULL\n");
        return poly;
    }

    Points* current = poly.point;
    Points* firstPoint = NULL;
    Points* prev = NULL;

    while (current != NULL)
    {
        Points* newPoint = malloc(sizeof(Points));
        if (newPoint == NULL)
        {
            printf("Memory allocation error.\n");
            Points_Free(firstPoint);
            return poly;
        }

        newPoint->coordinate = Vector2D_Scale(current->coordinate, a, anchor);
        newPoint->next = NULL;

        if (firstPoint == NULL)
        {
            firstPoint = newPoint;
        }
        else
        {
            prev->next = newPoint;
        }

        prev = newPoint;
        current = current->next;
    }

    Polygone2D newPoly = Polygone2D_SetFromPoints(-1, firstPoint);
    return newPoly;
}

float Polygone2D_Perimetre(Polygone2D poly)
{
    if (poly.point == NULL)
    {
        printf("First point = NULL\n");
        return 0.0f;
    }

    Points* first = poly.point;
    Points* current = poly.point;

    float perimetre = 0.0f;

    while (current->next != NULL)
    {
        float dist = Vector2D_Distance(current->coordinate, current->next->coordinate);
        perimetre += dist;
        current = current->next;
    }
    float dist = Vector2D_Distance(current->coordinate, first->coordinate);
    perimetre += dist;
    return perimetre;
}

float Polygone2D_Aire(Polygone2D poly)
{
    if (poly.point == NULL)
    {
        printf("First point = NULL\n");
        return 0.0f;
    }
    Points* first = poly.point;
    Points* current = poly.point;
    Points* next = current->next;

    float aire = 0.0f;

    while (next != NULL)
    {
        aire += current->coordinate.x * next->coordinate.y
            - next->coordinate.x * current->coordinate.y;

        current = next;
        next = next->next;
    }

    // fermer le polygone
    aire += current->coordinate.x * first->coordinate.y
        - first->coordinate.x * current->coordinate.y;

    return fabsf(aire * 0.5f);
}

Vector2D Polygone2D_Isobarycenter(Polygone2D poly)
{
    Vector2D gPoly = { 0, 0 };
    if (poly.point == NULL)
    {
        printf("First point = NULL\n");
        return gPoly;
    }

    float Gx = 0.0f;
    float Gy = 0.0f;

    Points* first = poly.point;
    Points* current = poly.point;
    Points* next = current->next;

    float A = 0.0f; // signed area for barycenter

    while (next != NULL)
    {
        float x0 = current->coordinate.x;
        float y0 = current->coordinate.y;
        float x1 = next->coordinate.x;
        float y1 = next->coordinate.y;

        float cross = (x0 * y1 - x1 * y0);
        A += cross;
        Gx += (x0 + x1) * cross;
        Gy += (y0 + y1) * cross;

        current = next;
        next = next->next;
    }

    // Close the polygon
    float x0 = current->coordinate.x;
    float y0 = current->coordinate.y;
    float x1 = first->coordinate.x;
    float y1 = first->coordinate.y;
    float cross = (x0 * y1 - x1 * y0);
    A += cross;
    Gx += (x0 + x1) * cross;
    Gy += (y0 + y1) * cross;

    A *= 0.5f;

    if (fabsf(A) < 1e-6f)
        return gPoly;

    gPoly.x = Gx / (6.0f * A);
    gPoly.y = Gy / (6.0f * A);

    return gPoly;
}


Polygone2D Polygone2D_Rotate(Polygone2D poly, float theta, Vector2D anchor)
{
    if (poly.point == NULL)
    {
        printf("Error: empty polygon.\n");
        return poly;
    }

    Vector2D gPoly = Polygone2D_Isobarycenter(poly);

    Vector2D newGPoly;
    newGPoly.x = anchor.x + (gPoly.x - anchor.x) * cos(theta) - (gPoly.y - anchor.y) * sin(theta);
    newGPoly.y = anchor.y + (gPoly.x - anchor.x) * sin(theta) + (gPoly.y - anchor.y) * cos(theta);

    Vector2D delta;
    delta.x = newGPoly.x - gPoly.x;
    delta.y = newGPoly.y - gPoly.y;

    Points* current = poly.point;
    Points* firstPoint = NULL;
    Points* prev = NULL;

    while (current != NULL)
    {
        Points* newPoint = malloc(sizeof(Points));
        if (newPoint == NULL)
        {
            printf("Memory allocation error.n");
            Points_Free(firstPoint);
            return poly;
        }

        newPoint->coordinate = Vector2D_SetFromComponents(
            current->coordinate.x + delta.x,
            current->coordinate.y + delta.y
        );
        newPoint->next = NULL;

        if (firstPoint == NULL)
        {
            firstPoint = newPoint;
        }
        else
        {
            prev->next = newPoint;
        }

        prev = newPoint;
        current = current->next;
    }

    Polygone2D newPoly = Polygone2D_SetFromPoints(-1, firstPoint);
    return newPoly;
}

Polygone2D Polygone2D_RotateAll(Polygone2D poly, float theta, Vector2D anchor)
{
    if (poly.point == NULL)
    {
        printf("Error: empty polygon.\n");
        return poly;
    }

    Points* current = poly.point;
    Points* firstPoint = NULL;
    Points* prev = NULL;

    while (current != NULL)
    {
        Points* newPoint = malloc(sizeof(Points));
        if (newPoint == NULL)
        {
            printf("Memory allocation error.\n");
            Points_Free(firstPoint);
            return poly;
        }

        newPoint->coordinate = Vector2D_Rotate(current->coordinate, theta, anchor);
        newPoint->next = NULL;

        if (firstPoint == NULL)
        {
            firstPoint = newPoint;
        }
        else
        {
            prev->next = newPoint;
        }

        prev = newPoint;
        current = current->next;
    }

    Polygone2D newPoly = Polygone2D_SetFromPoints(-1, firstPoint);
    return newPoly;
}

void Polygone2D_Print(Polygone2D poly)
{
    if (poly.point == NULL)
    {
        printf("Empty polygon.\n");
        return;
    }

    Points* current = poly.point;
    int i = 0;
    while (current != NULL)
    {
        printf("Point %i = (%.2f, %.2f)\n", i, current->coordinate.x, current->coordinate.y);
        current = current->next;
        i++;
    }
    printf("Points number = %i \n", i);
}
