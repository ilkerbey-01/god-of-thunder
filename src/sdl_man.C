// THOR - The God of Thunder
// See LICENSE for details

#include "sdl_man.h"

#include <SDL.h>

bool sdl_inited = false;
SDL_Window* win = nullptr;
SDL_Surface* surface = nullptr;
SDL_TimerID timer_id = 0;
extern volatile uint8_t key_flag[100];
extern volatile uint8_t joy_flag[100];
extern int16_t key_fire, key_up, key_down, key_left, key_right, key_magic, key_select;
extern volatile uint16_t timer_cnt, vbl_cnt, magic_cnt, extra_cnt;
int32_t TimerDivisor, TimerCount;
volatile int32_t TickCount2, TickCount;

void exit_code(int16_t ex_flag);

void tick_update(void) {
  SDL_UpdateWindowSurface(win);
}

uint32_t sdl_man_timer_handler(uint32_t interval, void* param) {
  SDL_UserEvent userevent;
  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = NULL;
  userevent.data2 = NULL;

  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user = userevent;
  SDL_PushEvent(&event);

  timer_cnt++;
  vbl_cnt++;
  magic_cnt++;
  extra_cnt++;

  TickCount2++;
  TickCount = TickCount2 >> 1;
  TimerCount += TimerDivisor;

  if (TimerCount >= 0x10000L) {
    TimerCount -= 0x10000L;
  }

  return interval;
}

bool sdl_man_initialize() {
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) != 0) {
    return false;
  }
  sdl_inited = true;

  win = SDL_CreateWindow("God of Thunder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, NATIVE_SCREEN_WIDTH * 4, NATIVE_SCREEN_HEIGHT * 4, 0);
  if (!win) {
    return false;
  }

  surface = SDL_GetWindowSurface(win);
  if (!surface) {
    return false;
  }

  timer_id = SDL_AddTimer(1000 / 50, sdl_man_timer_handler, NULL);

  return true;
}

void sdl_man_close() {
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
      exit(0);
      break;
    case SDL_USEREVENT:
      tick_update();
      break;
    }
  }
}
