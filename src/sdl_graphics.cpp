#include <stdint.h>
#include <SDL.h>

#include "pal_man.h"

extern SDL_Renderer* ren;

void sdl_graphics_set_palette_color(int16_t color_index) {
  PALETTE_COLOR color = palette_colors[color_index];
  SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 0xff);
}

void xfillrectangle(int16_t StartX, int16_t StartY, int16_t EndX, int16_t EndY,
  uint16_t PageBase, int16_t Color) {

  SDL_Rect rect = SDL_Rect();
  rect.x = StartX;
  rect.y = StartY;
  rect.w = EndX - StartX;
  rect.h = EndY - StartY;

  sdl_graphics_set_palette_color(Color);
  SDL_RenderFillRect(ren, &rect);
  SDL_RenderPresent(ren);
}
