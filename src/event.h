#pragma once

#include <SDL3/SDL_events.h>
#include <string>

void SDLEventLog(Uint32 t);
std::string getSDLEventName(Uint32 t);
std::string getSDLGamepadBtnName(Uint8 t);
