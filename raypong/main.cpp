#include "raylib.h"
#include <math.h>

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

typedef struct Player {
	Vector2 position;
	Vector2 size;
}Player;

typedef struct Ball {
	Vector2 position;
	Vector2 speed;
	float speedTime;
	float speedAcum = 100.0f;
	int radius;
	bool active;
}Ball;

//Váriaveis Globais
static const int screenWidth = 1024;
static const int screenHeight = 768;

static float deltaTime = 0;
static const int currentFPS = 75;

static Player player1 = { 0 };
static Player player2 = { 0 };
static Ball ball = { 0 };

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawFrame(void);

int main(void) 
{
	InitWindow(screenWidth, screenHeight, "Pong Game");

	InitGame();

#if defined(PLATAFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(currentFPS);

	while (!WindowShouldClose()) 
	{
		UpdateDrawFrame();
	}
#endif
	CloseWindow();

	return 0;
}

void InitGame(void) 
{
	//Iniciar Player1
	player1.position.x = 10;
	player1.position.y = screenHeight / 2;
	player1.size.x = 10;
	player1.size.y = screenWidth / 10;

	//Incializar Player2
	player2.position.x = screenWidth - 10;
	player2.position.y = screenHeight / 2;
	player2.size.x = 10;
	player2.size.y = screenWidth / 10;

	//Iniciar Bola
	ball.position.x = player1.position.x + player1.size.x * 2 + ball.radius;
	ball.position.y = player1.position.y;
	ball.speed.x = 0;
	ball.speed.y = 0;
	ball.radius = 7;
	ball.active = false;
}

void UpdateGame(void)
{
	deltaTime = GetFrameTime();

	//Movimentação do Player1
	if (IsKeyDown(KEY_W)) player1.position.y -= 5;
	if ((player1.position.y - player1.size.y / 2) <= 0) player1.position.x = player1.size.x / 2;
	if (IsKeyDown(KEY_S)) player1.position.y += 5;
	if ((player1.position.y + player1.size.y / 2) >= screenHeight) player1.position.y = screenWidth - player1.size.y / 2;

	//Movimentação do Player2
	if (IsKeyDown(KEY_UP)) player2.position.y -= 5;
	if ((player2.position.y - player2.size.y / 2) <= 0) player2.position.x = player2.size.x / 2;
	if (IsKeyDown(KEY_DOWN)) player2.position.y += 5;
	if ((player2.position.y + player2.size.y / 2) >= screenHeight) player2.position.y = screenHeight - player2.size.y / 2;

	//Iniciar movimentação da Bola
	if (!ball.active) {
		if (IsKeyPressed(KEY_SPACE))
		{
			ball.active = true;
			ball.speed.x = 300.0f;
			ball.speed.y = 0;
			ball.speedTime = 0.0f;
		}
	}

	//Movimentação da Bola
	if (ball.active) 
	{
		ball.speedTime += deltaTime;

		if (ball.speedTime >= 2.0f) {
			float speedMag = sqrtf(ball.speed.x * ball.speed.x + ball.speed.y * ball.speed.y);

			speedMag += ball.speedAcum;

			float dirX = ball.speed.x / sqrtf(ball.speed.x * ball.speed.x + ball.speed.y * ball.speed.y);
			float dirY = ball.speed.y / sqrtf(ball.speed.x * ball.speed.x + ball.speed.y * ball.speed.y);

			ball.speed.x = dirX * speedMag;
			ball.speed.y = dirY * speedMag;

			ball.speedTime = 0.0f;
		}

		ball.position.x += ball.speed.x * deltaTime;
		ball.position.y += ball.speed.y * deltaTime;
	}
	else 
	{
		ball.position.x = player1.position.x + player1.size.x / 2 + ball.radius;
		ball.position.y = player1.position.y;

	}

	//Colisão Bola vs Player1
	Rectangle player1Rect;
	player1Rect.x = player1.position.x - player1.size.x / 2;
	player1Rect.y = player1.position.y - player1.size.y / 2;
	player1Rect.width = player1.size.x;
	player1Rect.height = player1.size.y;

	if (CheckCollisionCircleRec(ball.position, ball.radius, player1Rect))
	{
		if (ball.speed.x < 0) 
		{
			ball.speed.x *= -1;
			ball.speed.y = (ball.position.y - player1.position.y) / (player1.size.y / 2) * 150.0f;
		}
	}

	//Colisão Bola vs Player2
	Rectangle player2Rect;
	player2Rect.x = player2.position.x - player2.size.x / 2;
	player2Rect.y = player2.position.y - player2.size.y / 2;
	player2Rect.width = player2.size.x;
	player2Rect.height = player2.size.y;

	if (CheckCollisionCircleRec(ball.position, ball.radius, player2Rect))
	{
		if (ball.speed.x > 0)
		{
			ball.speed.x *= -1;
			ball.speed.y = (ball.position.y - player2.position.y) / (player2.size.y / 2) * 5;
		}
	}

	//if (ball.position.x >= (GetScreenWidth() - ball.radius)) ball.speed.x *= -1.0f;
	if ((ball.position.y >= (GetScreenHeight() - ball.radius)) || (ball.position.y <= ball.radius)) ball.speed.y *= -1.0f;

	if (IsKeyPressed(KEY_ENTER))
	{
		InitGame();
	}
}

void DrawGame(void) 
{
	BeginDrawing();
		
		ClearBackground(RAYWHITE);

		DrawRectangle(player1.position.x - player1.size.x / 2, player1.position.y - player1.size.y / 2, player1.size.x, player1.size.y, BLACK);
		DrawRectangle(player2.position.x - player2.size.x / 2, player2.position.y - player2.size.y / 2, player2.size.x, player2.size.y, BLACK);

		DrawCircleV(ball.position, ball.radius, BLACK);

		//DrawText(TextFormat("speed count: %02.02f\n", ball.speedTime), 0, 0, 30, BLACK);
		DrawText(TextFormat("FPS count: %d", currentFPS), 0, 0, 20, BLACK);

	EndDrawing();
}

void UpdateDrawFrame(void)
{
	UpdateGame();
	DrawGame();
}
