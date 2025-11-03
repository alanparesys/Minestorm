#include <raylib.h>

int main()
{
    InitWindow(640, 800, "MineStorm");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("Assets/minestorm_background.png");

    while (!WindowShouldClose())
    {
        // Update
        
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw
        DrawTexture(background, 0, 0, WHITE);

        EndDrawing();
    }

    UnloadTexture(background);

    CloseWindow();

    return 0;
}//caca