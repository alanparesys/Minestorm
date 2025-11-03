# include " Vector2D .h"
typedef struct Rectangle2D
{
	Vector2D p1; // p4 ------- p3
	Vector2D p2; // |          |
				//  |    c     |
	Vector2D p3; // |		   |
	Vector2D p4; // p1 ------- p2
	float length; // distance between p1 and p2
	float width; // distance between p2 and p3
	center;
} Rectangle2D;
Rectangle2D Rectangle2D_SetFrom4Points(Vector2D p1, Vector2D p2, Vector2D p3,Vector2D p4);
Rectangle2D Rectangle2D_SetFrom1Point2Vectors(Vector2D p1, Vector2D v1, Vector2D v2);
Rectangle2D Rectangle2D_SetFromCenterLengthWidthAngle(Vector2D center, float length, float width, float angle);
float Rectangle2D_GetSurface(Rectangle2D rect);
Rectangle2D Rectangle2D_Translate(Rectangle2D rect, Vector2D t);
Rectangle2D Rectangle2D_Scale(Rectangle2D rect, float a, Vector2D anchor);
Rectangle2D Rectangle2D_Rotate(Rectangle2D rect, float theta, Vector2D anchor);
void Rectangle2D_Print(Rectangle2D rect);