#include <stdio.h>
#include "test_vector2D.h"
#include "test_triangle2D.h"
#include "test_rectangle2D.h"
#include "test_polygone2D.h"
#include "test_sphere2D.h"

int main(void)
{
    printf("Select a program to run\n");
    printf("___________________\n\n");
    printf("> (1) : Vector2D\n");
    printf("> (2) : Triangle2D\n");
    printf("> (3) : Rectangle2D\n");
    printf("> (4) : Polygone2D\n");
    printf("> (5) : Sphere2D\n");
    printf("___________________\n\n");

    int number = 0;
    printf(">> ");
    if (scanf_s("%d", &number) != 1)
    {
        printf("Invalid input.\n");
        return 1;
    }

    switch (number)
    {
    case 1: launch_vector2D(); break;
    case 2: launch_triangle2D(); break;
    case 3: launch_rectangle2D(); break;
    case 4: launch_polygone2D(); break;
    case 5: launch_sphere2D(); break;
    default: printf("Invalid selection.\n"); break;
    }

    printf("\n\n\nPress Enter to exit...");
    getchar();
    getchar();
    return 0;
}
