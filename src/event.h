#pragma once

#include "context/input.hpp"
#include <SDL3/SDL_events.h>

void loopEvent(std::shared_ptr<context::Input> input);
