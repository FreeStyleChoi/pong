#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>

int main(int argc, char* argv[])
{
	int window_w = 1460;
	int window_h = 960;

	int ball_w = 32;
	int ball_h = 32;

	int paddle_w = 16;
	int paddle_h = 170;

	const int FPS = 80;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, false);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	bool isRunning = true;
	SDL_Event event;
	SDL_Surface* tmpSurface;

	// background
	tmpSurface = IMG_Load("assets/tabletennis_table.png");
	SDL_Texture* backgroundTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	SDL_Rect backgroundRect{};
	backgroundRect.w = window_w;
	backgroundRect.h = window_h;
	backgroundRect.x = 0;
	backgroundRect.y = 0;

	// ball 
	tmpSurface = IMG_Load("assets/ball.png");
	SDL_Texture* ballTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	SDL_Rect ballRect{};
	ballRect.w = ball_w;
	ballRect.h = ball_h;
	ballRect.x = 0;
	ballRect.y = 0;

	int ballSpeed = 486;
	short int ballDir_x = 1;
	short int ballDir_y = 1;

	// paddle
	tmpSurface = IMG_Load("assets/paddle.png");
	SDL_Texture* paddle1Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_Texture* paddle2Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	int paddleSpeed = 486;

	/* paddle1 */
	SDL_Rect paddle1Rect{};
	paddle1Rect.w = paddle_w;
	paddle1Rect.h = paddle_h;
	paddle1Rect.x = 64;
	paddle1Rect.y = (window_h - paddle_h) / 2;
	short int paddle1_dir = 0;

	/* paddle2 */
	SDL_Rect paddle2Rect{};
	paddle2Rect.w = paddle_w;
	paddle2Rect.h = paddle_h;
	paddle2Rect.x = window_w - 64 - paddle2Rect.w;
	paddle2Rect.y = (window_h - paddle_h) / 2;
	short int paddle2_dir = 0;

	// main loop
	while (isRunning)
	{
		frameStart = (Uint32)SDL_GetTicks64();

		// event
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			/* keyboard event */
			switch (event.key.keysym.sym)
			{
			case SDLK_x:
				isRunning = false;
				break;
			case SDLK_w:
				paddle1_dir = -1;
				break;
			case SDLK_s:
				paddle1_dir = 1;
				break;
			case SDLK_UP:
				paddle2_dir = -1;
				break;
			case SDLK_DOWN:
				paddle2_dir = 1;
				break;
			case SDLK_SPACE:
				SDL_Delay(5000);
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				paddle1_dir = 0;
				break;
			case SDLK_s:
				paddle1_dir = 0;
				break;
			case SDLK_UP:
				paddle2_dir = 0;
				break;
			case SDLK_DOWN:
				paddle2_dir = 0;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		// update
		/* ball */
		ballRect.x += (int)((ballSpeed * ballDir_x) / (double)FPS);
		ballRect.y += (int)((ballSpeed * ballDir_y) / (double)FPS);
		/* paddles */
		paddle1Rect.y += (int)((paddleSpeed * paddle1_dir) / (double)FPS);
		paddle2Rect.y += (int)((paddleSpeed * paddle2_dir) / (double)FPS);

		// collision
		/* miss ball */
		if (ballRect.x <= 0)
		{
			SDL_Delay(2000);
			ballRect.x = (window_w - ballRect.w) / 2;
			ballRect.y = 0;
			ballDir_x = 1;
			ballDir_y = 1;
			paddleSpeed = 600;
		}
		if (ballRect.x >= window_w - ball_w)
		{
			SDL_Delay(2000);
			ballRect.x = (window_w - ballRect.w) / 2;
			ballRect.y = 0;
			ballDir_x = 1;
			ballDir_y = 1;
			paddleSpeed = 600;
		}

		/* ball collision (top&bottem) */
		if (ballRect.y <= 0)
		{
			ballDir_y = 1;
		}
		if (ballRect.y >= window_h - ball_h)
		{
			ballDir_y = -1;
		}

		/* paddles collision (top&bottom) */
		if (paddle1Rect.y <= 0)
		{
			paddle1Rect.y = 0;
		}
		if (paddle1Rect.y >= window_h - paddle_h)
		{
			paddle1Rect.y = window_h - paddle_h;
		}
		if (paddle2Rect.y <= 0)
		{
			paddle2Rect.y = 0;
		}
		if (paddle2Rect.y >= window_h - paddle_h)
		{
			paddle2Rect.y = window_h - paddle_h;
		}

		/* collision between ball and paddle (x) */
		if (ballRect.x <= paddle1Rect.x + paddle1Rect.w &&
			ballRect.y + ballRect.h / 2 <= paddle1Rect.y + paddle1Rect.h &&
			ballRect.y + ballRect.h / 2 >= paddle1Rect.y)
		{
			ballDir_x = 1;
			ballSpeed = ballSpeed + 50;
		}
		if (ballRect.x + ballRect.w >= paddle2Rect.x &&
			ballRect.y + ballRect.h / 2 <= paddle2Rect.y + paddle2Rect.h &&
			ballRect.y + ballRect.h / 2 >= paddle2Rect.y)
		{
			ballDir_x = -1;
			ballSpeed = ballSpeed + 50;
		}

		// render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTex, NULL, &backgroundRect);
		SDL_RenderCopy(renderer, ballTex, NULL, &ballRect);
		SDL_RenderCopy(renderer, paddle1Tex, NULL, &paddle1Rect);
		SDL_RenderCopy(renderer, paddle2Tex, NULL, &paddle2Rect);
		SDL_RenderPresent(renderer);

		// FPS
		frameTime = (Uint32)SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	// game end
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}
