#include "Platform.h"
#include "SDL.h"
#include <crtdbg.h>

Platform::Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}
Platform::~Platform()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Platform::Update(void const* buffer, int pitch)
{
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}
bool Platform::ProcessInput(uint8_t* keys)
{
	bool quit = false;
	SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				quit = true;
				return quit;
			}

			if (SDL_KEYDOWN == event.type)
			{
				if (SDLK_ESCAPE == event.key.keysym.sym)
				{
					quit = true;
					return quit;
				}

				if (SDLK_q == event.key.keysym.sym)
				{
					keys[0] = 1;
				}
				if (SDLK_w == event.key.keysym.sym)
				{
					keys[1] = 1;
				}
				if (SDLK_e == event.key.keysym.sym)
				{
					keys[2] = 1;
				}
				if (SDLK_r == event.key.keysym.sym)
				{
					keys[3] = 1;
				}
				if (SDLK_t == event.key.keysym.sym)
				{
					keys[4] = 1;
				}
				if (SDLK_a == event.key.keysym.sym)
				{
					keys[5] = 1;
				}
				if (SDLK_s == event.key.keysym.sym)
				{
					keys[6] = 1;
				}
				if (SDLK_d == event.key.keysym.sym)
				{
					keys[7] = 1;
				}
				if (SDLK_f == event.key.keysym.sym)
				{
					keys[8] = 1;
				}
				if (SDLK_g == event.key.keysym.sym)
				{
					keys[9] = 1;
				}
				if (SDLK_z == event.key.keysym.sym)
				{
					keys[10] = 1;
				}
				if (SDLK_x == event.key.keysym.sym)
				{
					keys[11] = 1;
				}
				if (SDLK_c == event.key.keysym.sym)
				{
					keys[12] = 1;
				}
				if (SDLK_v == event.key.keysym.sym)
				{
					keys[13] = 1;
				}
				if (SDLK_1 == event.key.keysym.sym)
				{
					keys[14] = 1;
				}
				if (SDLK_2 == event.key.keysym.sym)
				{
					keys[15] = 1;
				}
			}
			else if (SDL_KEYUP == event.type)
			{
				if (SDLK_q == event.key.keysym.sym)
				{
					keys[0] = 0;
				}
				if (SDLK_w == event.key.keysym.sym)
				{
					keys[1] = 0;
				}
				if (SDLK_e == event.key.keysym.sym)
				{
					keys[2] = 0;
				}
				if (SDLK_r == event.key.keysym.sym)
				{
					keys[3] = 0;
				}
				if (SDLK_t == event.key.keysym.sym)
				{
					keys[4] = 0;
				}
				if (SDLK_a == event.key.keysym.sym)
				{
					keys[5] = 0;
				}
				if (SDLK_s == event.key.keysym.sym)
				{
					keys[6] = 0;
				}
				if (SDLK_d == event.key.keysym.sym)
				{
					keys[7] = 0;
				}
				if (SDLK_f == event.key.keysym.sym)
				{
					keys[8] = 0;
				}
				if (SDLK_g == event.key.keysym.sym)
				{
					keys[9] = 0;
				}
				if (SDLK_z == event.key.keysym.sym)
				{
					keys[10] = 0;
				}
				if (SDLK_x == event.key.keysym.sym)
				{
					keys[11] = 0;
				}
				if (SDLK_c == event.key.keysym.sym)
				{
					keys[12] = 0;
				}
				if (SDLK_v == event.key.keysym.sym)
				{
					keys[13] = 0;
				}
				if (SDLK_1 == event.key.keysym.sym)
				{
					keys[14] = 0;
				}
				if (SDLK_2 == event.key.keysym.sym)
				{
					keys[15] = 0;
				}
			}
		}
	return quit;
}