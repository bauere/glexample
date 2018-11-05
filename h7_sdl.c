#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include "h7_sdl.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int
initsdl(SDL_Window **winpp, char *name)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "sdl error: %s\n", SDL_GetError());
		return 1;
	}
	*winpp = SDL_CreateWindow("SDL tut", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (!*winpp) {
		fprintf(stderr, "sdl error: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

void
process_sdl_events(SDL_Window *winp)
{
	SDL_Event e;
evloop:
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT)
		return;
	SDL_GL_SwapWindow(winp);
	goto evloop;
}