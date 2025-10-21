#pragma once

#include <SDL3/SDL_render.h>
#include <string>
#ifndef __EMSCRIPTEN__
#include <SDL3_ttf/SDL_ttf.h>
#endif

class Text {

private:
	SDL_Renderer *r;
#ifndef __EMSCRIPTEN__
	TTF_Font *fMono32;
#endif

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
#ifndef __EMSCRIPTEN__
	void render(TTF_Font *font, std::string text, int x, int y, Align align);
#endif
	void error(std::string_view msg);
};
