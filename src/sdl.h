#pragma once

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "context/entity.h"
#include "input.h"
#include "render/grid.h"
#include "render/text.h"
#include <box2d/box2d.h>

struct sdlDep {
	std::shared_ptr<context::Entity> entity;
};

class sdl {
private:
	Input *input;
	Text *text;
	Grid *grid;
	// SDL_Texture *bg;
	SDL_Window *window;
	SDL_Texture *ballTex;
	sdlDep d;

public:
	SDL_Renderer *r;

public:
	sdl(sdlDep dep, Input *input);
	~sdl();
	bool init();
	void handleInput(SDL_Event *e);
	void counter(int i);
	void renderStart();
	void renderEnd();
	void renderGrid();
	void renderBall(context::Ball b);
	void renderBrick();
};
