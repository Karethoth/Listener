#pragma once

#include <SDL2/SDL.h>


struct Global
{
  bool shouldQuit = false;

  int windowWidth = 1200;
  int windowHeight = 600;

  SDL_Window  *window    = nullptr;
  SDL_Renderer *renderer = nullptr;
};

extern Global global;

