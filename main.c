#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <stdio.h>
#include <string.h>

#include "hitbox.h"
#include "h7_sdl.h"
#include "h7_gl.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

static int
mksdlwindow(SDL_Window **winpp)
{
	SDL_Window *winp;


	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "sdl error: %s\n", SDL_GetError());
		return 1;
	}
	winp = SDL_CreateWindow("SDL tut", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (!winp) {
		fprintf(stderr, "unable to make window. sdl error: %s\n", SDL_GetError());
		return 1;
	}
	*winpp = winp;
	return 0;
}

static int
loadimage(SDL_Surface **surfacepp, char *fname)
{
	SDL_Surface *surfacep = SDL_LoadBMP(fname);
	if (!surfacep) {
		fprintf(stderr, "error loading image. sdl error: %s", SDL_GetError());
		return 1;
	}
	*surfacepp = surfacep;
	return 0;
}

static int
process_sdl_events(SDL_Window *winp)
{
	SDL_Event e;
evloop:
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT)
		return -1;
	return 0;
	//goto evloop;
}

int
main(int argc, char **argv)
{
	SDL_Window *winp;
	SDL_GLContext glctx;
	if (initsdl(&winp, "sdl tut")) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return 1;
	}
	if (initgl_w32(&glctx, winp)) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return 1;
	}
	setvsync(1);

	GLuint progid1;
	GLuint progid2;
	if (mkprog(&progid1))
		return 1;
	hitbox_t hb1 = {.posarr={
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
	}};
	hitbox_t hb2 = {.posarr={
		-0.1f, -0.1f,
		0.1f, -0.1f,
		0.1f, 0.1f,
		-0.1f, 0.1f,
	}};
	for (int i=0; i<5; i++) {
		renderhitbox(progid1, &hb1);
		//renderhitbox(progid1, &hb2);
		renderframe(winp);
		if (process_sdl_events(winp) == -1)
			break;
		for (int j=0; j<8; j++) {
			hb1.posarr[j] += 0.1f;
			hb2.posarr[j] -= 0.1f;
		}
		SDL_Delay(1000 / 60);
	}
	while(process_sdl_events(winp) != -1);
	/*renderhitbox(progid, &hb1);
	SDL_GL_SwapWindow(winp);
	SDL_Delay(2000);*/
	return 0;
}