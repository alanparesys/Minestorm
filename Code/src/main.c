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

        //Text
        DrawText("(F) : Launch Solo Game", 160, 350, 20, WHITE);
        DrawText("(K) : Launch Duo Game", 160, 400, 20, WHITE);
        DrawText("(Esc) : Quit Game", 160, 450, 20, WHITE);

        EndDrawing();
    }

    UnloadTexture(background);

    CloseWindow();

    return 0;
}