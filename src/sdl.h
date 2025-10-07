#pragma once

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "input.h"

class sdl {
private:
	Input *input;
	// SDL_Texture *bg;
	SDL_Window *window;

public:
	SDL_Renderer *r;

public:
	sdl(Input *input);
	~sdl();
	bool init();
	void handleInput(SDL_Event *e);
};
