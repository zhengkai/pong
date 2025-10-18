#pragma once

#include <SDL3/SDL_render.h>
#include <vector>

class Grid {
private:
	SDL_Renderer *r;
	float startX = 0;
	float startY = 0;

public:
	Grid(SDL_Renderer *r);
	~Grid();
	void draw(const std::vector<bool> &li);
	void calcGrid();
	void drawRand();
};
