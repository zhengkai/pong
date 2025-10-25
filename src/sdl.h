#pragma once

#include "context/entity.h"
#include "context/window.h"
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
	Text *text;
	// SDL_Texture *bg;
	SDL_Window *w;
	SDL_Texture *ballTex;
	sdlDep d;

public:
	sdl(sdlDep dep);
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
	void calcGrid(int winW, int winH);
	void initWinSize();
};
