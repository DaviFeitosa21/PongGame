#include "raylib.h"

int main(void) 
{
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Pong Game");

	Vector2 ballPosition = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	Vector2 ballSpeed = { 5.0f, 4.0f };
	float ballRadius = 20;

	int framesCounter = 0;

	SetTargetFPS(60);

	while (!WindowShouldClose()) 
	{
		ballPosition.x += ballSpeed.x;
		ballPosition.y += ballSpeed.y;
		if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeed.x *= -1.0f;
		if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeed.y *= -1.0f;

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawCircleV(ballPosition, ballRadius, BLACK);

		DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
