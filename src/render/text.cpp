#include "text.h"
#include "spdlog/spdlog.h"
#include <fmt/format.h>

Text::Text() {
}

bool Text::init(SDL_Renderer *r) {

	this->r = r;

	if (not TTF_Init()) {
		error("TTF_Init fail: {}");
		return false;
	}

	fMono32 = TTF_OpenFont("/www/pong/static/JetBrainsMono-Regular.otf", 32);
	if (!fMono32) {
		error("init fMono32 fail: {}");
		return false;
	}

	return true;
}

void Text::rMono32(std::string text, int x, int y, Align align) {
	render(fMono32, text, x, y, align);
}

void Text::render(TTF_Font *font, std::string text, int x, int y, Align align) {

	// 设置字体颜色
	SDL_Color color = {255, 255, 255, 255}; // 白色

	// 使用字体渲染文本为 Surface
	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), 0, color);
	if (!surface) {
		error("Failed to create text surface, {}");
		return;
	}

	// 将 Surface 转换为 Texture
	SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
	SDL_DestroySurface(surface);
	if (!texture) {
		error("Failed to create text texture, %s");
		return;
	}

	SDL_PropertiesID pid = SDL_GetTextureProperties(texture);
	float textWidth =
		(float)SDL_GetNumberProperty(pid, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
	int textHeight =
		(int)SDL_GetNumberProperty(pid, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

	float alignedX = static_cast<float>(x);
	switch (align) {
	case Align::CENTER:
		alignedX -= textWidth / 2;
		break;
	case Align::RIGHT:
		alignedX -= textWidth;
		break;
	default:
		break;
	}

	// 设置渲染位置
	SDL_Rect dstRect = {
		static_cast<int>(alignedX), y, static_cast<int>(textWidth), textHeight};

	SDL_FRect d;
	SDL_RectToFRect(&dstRect, &d);

	// 渲染纹理
	SDL_RenderTexture(r, texture, nullptr, &d);

	// 清理纹理
	SDL_DestroyTexture(texture);
}

void Text::error(const char *msg) {
	spdlog::error("yes {}", SDL_GetError());
}

Text::~Text() {
	spdlog::info("font close");
	// TTF_CloseFont(fMono32);
	// TTF_Quit();
}
