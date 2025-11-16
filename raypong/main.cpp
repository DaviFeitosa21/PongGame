#include "raylib.h"

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
	int radius;
	bool active;
}Ball;

//Váriaveis Globais
static const int screenWidth = 800;
static const int screenHeight = 600;

static Player player = { 0 };
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
	SetTargetFPS(60);

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
	//Iniciar Player
	player.position.x = 10;
	player.position.y = screenHeight / 2;
	player.size.x = 10;
	player.size.y = screenWidth / 10;

	//Iniciar Bola
	ball.radius = 7;
	ball.active = false;
	ball.position.x = player.position.x + player.size.x * 2 + ball.radius;
	ball.position.y = player.position.y;
	ball.speed.x = 0;
	ball.speed.y = 0;
}

void UpdateGame(void)
{
	//Movimentação do Player
	if (IsKeyDown(KEY_UP)) player.position.y -= 5;
	if ((player.position.y - player.size.y / 2) <= 0) player.position.x = player.size.x / 2;
	if (IsKeyDown(KEY_DOWN)) player.position.y += 5;
	if ((player.position.y + player.size.y / 2) >= screenWidth) player.position.y = screenWidth - player.size.y / 2;

	//Iniciar movimentação da Bola
	if (!ball.active) {
		if (IsKeyPressed(KEY_SPACE))
		{
			ball.active = true;
			ball.speed.x = 5;
			ball.speed.y = 0;
		}
	}

	//Movimentação da Bola
	if (ball.active) 
	{
		ball.position.x += ball.speed.x;
		ball.position.y += ball.speed.y;
	}
	else 
	{
		ball.position.x = player.position.x + player.size.x / 2 - ball.radius;
		ball.position.y = player.position.y;

	}

	//Colisão Bola vs Player
	Rectangle playerRect;
	playerRect.x = player.position.x - player.size.x / 2;
	playerRect.y = player.position.y - player.size.y / 2;
	playerRect.width = player.size.x;
	playerRect.height = player.size.y;


	if (CheckCollisionCircleRec(ball.position, ball.radius, playerRect))
	{
		if (ball.speed.x < 0) 
		{
			ball.speed.x *= -1;
			ball.speed.y = (ball.position.y - player.position.y) / (player.size.y / 2) * 5;
		}
	}

	if (ball.position.x >= (GetScreenWidth() - ball.radius)) ball.speed.x *= -1.0f;
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

		DrawRectangle(player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y, BLACK);

		DrawCircleV(ball.position, ball.radius, BLACK);

	EndDrawing();
}

void UpdateDrawFrame(void)
{
	UpdateGame();
	DrawGame();
}
