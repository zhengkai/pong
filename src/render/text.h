#pragma once

#include "SDL3/SDL_render.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class Text {

private:
	SDL_Renderer *r;
	TTF_Font *fMono32;

public:
	enum class Align {
		LEFT,
		CENTER,
		RIGHT,
	};
	Text();
	~Text();
	bool init(SDL_Renderer *r);
	void rMono32(std::string text, int x, int y, Align align = Align::LEFT);

private:
	void render(TTF_Font *font, std::string text, int x, int y, Align align);
	void error(const char *msg);
};
