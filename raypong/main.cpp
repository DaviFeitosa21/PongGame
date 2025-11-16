#include "raylib.h"

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Jogo Pong");

    int boxPositionY = screenHeight / 2 - 40;
    int scrollSpeed = 4;            

    SetTargetFPS(60);               

    while (!WindowShouldClose())
    {
        boxPositionY -= (int)(GetMouseWheelMove() * scrollSpeed);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(screenWidth / 2 - 40, boxPositionY, 10, 10, MAROON);
        DrawRectangle(screenWidth / 2 - 10, boxPositionY, 10, 10, MAROON);

        DrawText("Use mouse wheel to move the cube up and down!", 10, 10, 20, GRAY);
        DrawText(TextFormat("Box position Y: %03i", boxPositionY), 10, 40, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}