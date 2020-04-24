// THOR - The God of Thunder
// See LICENSE for details

#include <cstdlib>
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

typedef struct {
  int16_t plane_width;
  int16_t height;
  int16_t invis_color;
  uint8_t color_indexes[];
} RAW_IMAGE;

void xput(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  RAW_IMAGE* image = (RAW_IMAGE*)buff;
  int16_t width = (image->plane_width * 4);
  int16_t size = width * image->height;
  for (int16_t i = 0; i < size - 1; i++) {
    int16_t offset_x = i % width;
    int16_t offset_y = i / width;

    int16_t color_index = image->color_indexes[i / 4 + i % 4 * 16 * 4];
    if (color_index != image->invis_color) {
      sdl_graphics_set_palette_color(color_index);
      SDL_RenderDrawPoint(ren, x + offset_x, y + offset_y);
    }
  }
  SDL_RenderPresent(ren);
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

  RAW_IMAGE* image = (RAW_IMAGE*)buff;
  int16_t width = (image->plane_width * 4);
  int16_t size = width * image->height;
  for (int16_t i = 0; i < size - 1; i++) {
    int16_t offset_x = i % width;
    int16_t offset_y = i / width;

    int16_t color_index = image->color_indexes[i / 4 + i % 4 * 320 * 12];
    if (color_index != image->invis_color) {
      sdl_graphics_set_palette_color(color_index);
      SDL_RenderDrawPoint(ren, x + offset_x, y + offset_y);
    }
  }
  SDL_RenderPresent(ren);
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

void xpset(int16_t X, int16_t Y, uint16_t PageBase, int16_t Color) {
  if (sdl_graphics_set_palette_color(Color)) {
    SDL_RenderDrawPoint(ren, X, Y);
  }
  SDL_RenderPresent(ren);
}
void xline(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t page, int16_t color) {
  int16_t x, y;

  if (x0 == x1 && y0 == y1)
    xpset(x0, y0, page, color);

  else if (std::abs(x1 - x0) >= std::abs(y1 - y0))
  {
    if (x1 < x0)
    {
      x = x1;
      y = y1;
      x1 = x0;
      y1 = y0;
      x0 = x;
      y0 = y;
    }
    for (x = x0; x <= x1; x++)
    {
      y = (int)(y0 + ((x - x0) * (int32_t)(y1 - y0)) / (x1 - x0));
      xpset(x, y, page, color);
    }
  }
  else
  {
    if (y1 < y0)
    {
      x = x1;
      y = y1;
      x1 = x0;
      y1 = y0;
      x0 = x;
      y0 = y;
    }
    for (y = y0; y <= y1; y++)
    {
      x = (int)(x0 + ((y - y0) * (int32_t)(x1 - x0)) / (y1 - y0));
      xpset(x, y, page, color);
    }
  }
}
