#ifndef POLYGONE2D_H
#define POLYGONE2D_H

#include "Vector2D.h"
#include "Rectangle2D.h"

typedef struct Points {
    Vector2D coordinate;
    struct Points* next;
    struct Points* prev;
} Points;

typedef struct Polygone2D {

    int nbsommets;
    Points* point;

} Polygone2D;


Polygone2D Polygone2D_SetFromPoints(int Nbsommets, ...);
void Points_Free(Points* firstPoint);
Polygone2D Polygone2D_Translate(Polygone2D poly, Vector2D t);
Polygone2D Polygone2D_Scale(Polygone2D poly, float a, Vector2D anchor);
float Polygone2D_Perimetre(Polygone2D poly);
float Polygone2D_Aire(Polygone2D poly);
Vector2D Polygone2D_Isobarycenter(Polygone2D poly);
Polygone2D Polygone2D_Rotate(Polygone2D poly, float theta, Vector2D anchor);
Polygone2D Polygone2D_RotateAll(Polygone2D poly, float theta, Vector2D anchor);
void Polygone2D_Print(Polygone2D poly);


#endif