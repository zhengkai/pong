#include "game.h"
#include "config.hpp"
#include "input.hpp"
#include <algorithm>

static std::string speedMsg = "Speed Level: ";

static void handleInput(SDL_Event *e, std::shared_ptr<Input> input) {

	SDL_Gamepad *gamepad;

	switch (e->type) {
	case SDL_EVENT_KEY_DOWN:
		input->key(&e->key);
		break;
	case SDL_EVENT_WINDOW_RESIZED: {
		input->winResize(&e->window);
		break;
	}

	default:
		break;
	}
}

Game::Game(GameDep dep) : d(std::move(dep)), input(std::make_shared<Input>()) {
}

Game::~Game() {
}

bool Game::parse() {

	if (input->quit) {
		return false;
	}

	// control speed

	if (input->speed != 0) {
		int slv = d.entity->speedLevel + input->speed;
		slv = std::max(
			-config::speedLevelMax, std::min(config::speedLevelMax, slv));
		d.entity->speedLevel = slv;
		d.entity->speed = std::pow(2, slv);

		context::ControlMsg *cm = d.window->controlMsg;
		if (cm == nullptr) {
			cm = new context::ControlMsg();
			d.window->controlMsg = cm;
		}

		std::string m = std::to_string((int)std::pow(2, std::abs(slv))) + "x";
		if (slv < 0) {
			m = "1/" + m;
		}
		cm->msg = speedMsg + m;
		cm->expireSerial = d.window->serial + cfgFPS * 2;
	}

	// window resize

	if (input->winW > 0 && input->winH > 0) {
		auto wr = d.window->winResize;
		if (wr == nullptr) {
			wr = new context::WinResize();
			d.window->winResize = wr;
		}
		wr->w = input->winW;
		wr->h = input->winH;
	}

	// show ball

	if (input->space) {
		d.window->showBall = !d.window->showBall;
	}

	// fullscreen toggle
	if (input->fullscreen) {
		spdlog::info("toggling fullscreen");
		d.window->toggleFullscreen = true;
	}

	return true;
}

void Game::loopEvent() {
	SDL_Event e;
	input->Reset();
	while (SDL_PollEvent(&e)) {
		util::SDLEventLog(e.type);
		if (e.type == SDL_EVENT_QUIT) {
			input->quit = true;
			break;
		}
		handleInput(&e, input);
	}
}
