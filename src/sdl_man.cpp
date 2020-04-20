#include "sdl_man.h"

#include <SDL.h>

bool sdl_inited = false;
SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;

bool sdl_man_initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    return false;
  }
  sdl_inited = true;

  win = SDL_CreateWindow("God of Thunder", 100, 100, 320, 240, SDL_WINDOW_SHOWN);
  if (!win) {
    return false;
  }

  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) {
    return false;
  }

  SDL_RenderClear(ren);

  return true;
}

void sdl_man_close() {
  if (ren) {
    SDL_DestroyRenderer(ren);
    ren = nullptr;
  }

  if (win) {
    SDL_DestroyWindow(win);
    win = nullptr;
  }

  if (sdl_inited) {
    SDL_Quit();
  }
}
