#pragma once

#include "context/entity.h"
#include "context/window.h"
#include "input.h"
#include "render/text.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <box2d/box2d.h>

struct sdlDep {
	std::shared_ptr<context::Entity> entity;
	std::shared_ptr<context::Window> window;
};

class sdl {

private:
	SDL_Renderer *r;
	Input *input;
	Text *text;
	// SDL_Texture *bg;
	SDL_Window *window;
	SDL_Texture *ballTex;
	sdlDep d;

public:
	sdl(sdlDep dep, Input *input);
	~sdl();
	bool init();
	void handleInput(SDL_Event *e);

	void render();

private:
	void renderControlMsg();
	void renderCounter();
	void renderResize();
	void renderBall(std::shared_ptr<context::Ball> b);
	void renderBrick();
};
