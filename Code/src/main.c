// -------------------------------------------------------------
// ToolBox2D
// -------------------------------------------------------------
// Alan , Louis & Eden
// GP1
// Vector2D / Triangle2D / Rectangle2D
// -------------------------------------------------------------

#include <stdio.h>
#include "test_vector2D.h"
#include "test_triangle2D.h"
#include "test_rectangle2D.h"
#include "test_polygone2D.h"

int main()
{
    // Selection of program
    printf("Select a program to run\n");
    printf("___________________\n\n");
    printf("> (1) : Vector2D\n");
    printf("> (2) : Triangle2D\n");
    printf("> (3) : Rectangle2D\n");
    printf("> (4) : Polygone2D\n");
    printf("___________________\n\n");

    int number;
    printf(">> ");
    int result = scanf_s("%d", &number); // Choose a number

    if (number == 1)
    {
        launch_vector2D();
        printf("\n\n\n");
    }
    else if (number == 2)
    {
        launch_triangle2D();
        printf("\n\n\n");
    }
    else if (number == 3)
    {
        launch_rectangle2D();
        printf("\n\n\n");
    }
    /*
    else if (number == 4)
    {
        launch_sphere2D();
        printf("\n\n\n");
    }
    else if (number == 5)
    {
        launch_polygone2D();
        printf("\n\n\n");
    }
    */

    return 0;
}