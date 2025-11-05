#pragma once

#include "context/entity.h"
#include "context/window.h"
#include "render/rainbow.hpp"
#include "render/text.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <box2d/box2d.h>
#include <memory>

struct sdlDep {
	std::shared_ptr<context::BallCluster> ballCluster;
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
	std::unique_ptr<Rainbow> rainbow;

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
	void renderBall(std::shared_ptr<context::Ball> b, SDL_Color c);
	void renderBrick();
	void calcGrid(int winW, int winH);
	void initWinSize();
	void calcRegionSize();
	bool toggleFullscreen();
	void renderGamepad();
};
