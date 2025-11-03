typedef struct Vector2D
{
	float x;
	float y;
} Vector2D;

Vector2D Vector2D_SetFromComponents(float x, float y);
Vector2D Vector2D_SetFrom2Points(Vector2D p1, Vector2D p2);
Vector2D Vector2D_Opposite(Vector2D v);
Vector2D Vector2D_MidPoint(Vector2D p1, Vector2D p2);
float Vector2D_Distance(Vector2D p1, Vector2D p2);
Vector2D Vector2D_Add(Vector2D v1, Vector2D v2);
float Vector2D_SquaredNorm(Vector2D v);
float Vector2D_Norm(Vector2D v);
float Vector2D_DotProduct(Vector2D v1, Vector2D v2);
float Vector2D_CrossProduct(Vector2D v1, Vector2D v2);
float Vector2D_GetAngle(Vector2D v1, Vector2D v2);

Vector2D Vector2D_Translate(Vector2D p, Vector2D t); // same as Add function
Vector2D Vector2D_Scale(Vector2D p, float a, Vector2D anchor);
Vector2D Vector2D_Rotate(Vector2D p, float theta, Vector2D anchor);
void Vector2D_Print(Vector2D v);