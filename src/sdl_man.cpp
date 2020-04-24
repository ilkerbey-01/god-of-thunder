// THOR - The God of Thunder
// See LICENSE for details

#include "sdl_man.h"

#include <SDL.h>

bool sdl_inited = false;
SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;
extern volatile uint8_t key_flag[100];
extern volatile uint8_t joy_flag[100];
extern int16_t key_fire, key_up, key_down, key_left, key_right, key_magic, key_select;
void exit_code(int16_t ex_flag);

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

void sdl_man_handle_key(SDL_KeyboardEvent* key) {
  uint8_t flag = 0;
  if (key->type == SDL_KEYDOWN) {
    flag = 1;
  }
  switch (key->keysym.sym) {
  case SDLK_UP:
    key_flag[key_up] = flag;
    break;
  case SDLK_DOWN:
    key_flag[key_down] = flag;
    break;
  case SDLK_LEFT:
    key_flag[key_left] = flag;
    break;
  case SDLK_RIGHT:
    key_flag[key_right] = flag;
    break;
  case SDLK_LCTRL:
  case SDLK_RCTRL:
    key_flag[key_magic] = flag;
    break;
  case SDLK_LALT:
  case SDLK_RALT:
    key_flag[key_fire] = flag;
    break;
  case SDLK_ESCAPE:
    key_flag[1] = flag;
    break;
  case SDLK_RETURN:
    key_flag[28] = flag;
    break;
  case SDLK_SPACE:
    key_flag[57] = flag;
    break;
  case SDLK_s:
    key_flag[31] = flag;
    break;
  case SDLK_l:
    key_flag[38] = flag;
    break;
  }
}

void sdl_man_update() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      sdl_man_handle_key(&e.key);
      break;
    case SDL_QUIT:
      exit_code(0);
      break;
    }
  }
}
