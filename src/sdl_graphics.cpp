#include <stdint.h>
#include <SDL.h>

#include "pal_man.h"

extern SDL_Renderer* ren;

bool sdl_graphics_set_palette_color(int16_t color_index) {
  if (color_index) {
    PALETTE_COLOR color = palette_colors[color_index];
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    return true;
  }
  else {
    SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, SDL_ALPHA_TRANSPARENT);
    return false;
  }
}

void xfillrectangle(int16_t StartX, int16_t StartY, int16_t EndX, int16_t EndY,
  uint16_t PageBase, int16_t Color) {

  SDL_Rect rect = SDL_Rect();
  rect.x = StartX;
  rect.y = StartY;
  rect.w = EndX - StartX;
  rect.h = EndY - StartY;

  if (sdl_graphics_set_palette_color(Color)) {
    SDL_RenderFillRect(ren, &rect);
    SDL_RenderPresent(ren);
  }
}


void sdl_graphics_render_image(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* buff) {
  int size = width * height;
  for (int i = 0; i < size; i++) {
    if (sdl_graphics_set_palette_color(*(buff + i))) {
      int offset_x = x + (i % width);
      int offset_y = y + (i / width);
      SDL_RenderDrawPoint(ren, offset_x, offset_y);
    }
  }
  SDL_RenderPresent(ren);
}

void xput(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  // TODO is this 16x16?
  sdl_graphics_render_image(x, y, 16, 16, buff);
}

void xput2(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  // TODO determine the difference with xput
  xput(x, y, pagebase, buff);
}
void xfput(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  // TODO determine the difference with xput
  xput(x, y, pagebase, buff);
}
void xfarput(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  sdl_graphics_render_image(x, y, 320, 48, buff);
}

void xtext(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff, int16_t color) {
  for (int i = 0; i < 72; i++) {
    int16_t text_color = *(buff + i) == 0
      ? 0
      : color;
    if (sdl_graphics_set_palette_color(text_color)) {
      int offset_x = x + (i % 8);
      int offset_y = y + (i / 8);
      SDL_RenderDrawPoint(ren, offset_x, offset_y);
    }
  }
  SDL_RenderPresent(ren);
}

void xtext1(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff, int16_t color) {
  // TODO determine the difference with xtext
  xtext(x, y, pagebase, buff, color);
}
void xtextx(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff, int16_t color) {
  // TODO determine the difference with xtext
  xtext(x, y, pagebase, buff, color);
}

