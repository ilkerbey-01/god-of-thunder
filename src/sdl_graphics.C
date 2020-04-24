// THOR - The God of Thunder
// See LICENSE for details

#include <cstdlib>
#include <stdint.h>
#include <SDL.h>

#include "sdl_man.h"
#include "pal_man.h"

extern SDL_Surface* surface;

void xpset(int16_t X, int16_t Y, uint16_t PageBase, int16_t Color);

uint16_t current_page = 0;

uint32_t convert_color(uint8_t index) {
  return SDL_MapRGBA(surface->format,
    palette_colors[index].r,
    palette_colors[index].g,
    palette_colors[index].b,
    0xff);
}

void xshowpage(unsigned page) {
  current_page = page;
  return; // TODO xshowpage
}

void xfillrectangle(int16_t StartX, int16_t StartY, int16_t EndX, int16_t EndY,
  uint16_t PageBase, int16_t Color) {
  SDL_Rect rect;
  rect.x = StartX;
  rect.y = StartY;
  rect.w = EndX - StartX;
  rect.h = EndY - StartY;
  SDL_FillRect(surface, &rect, convert_color((uint8_t)Color));
}

typedef struct {
  int16_t plane_width;
  int16_t height;
  int16_t invis_color;
  uint8_t color_indexes[];
} RAW_IMAGE;

static void xput_layer(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff, int offset) {
  int16_t width = *(int16_t*)(buff);
  int16_t height = *(int16_t*)(buff + 2);
  int16_t mask = *(int16_t*)(buff + 4);
  uint8_t* pixels = buff + 6;
  uint32_t index = width * height * offset;
  int32_t pitch = surface->pitch / surface->format->BytesPerPixel;
  for (int32_t curr_y = 0; curr_y < height; ++curr_y) {
    for (int32_t curr_x = 0; curr_x < width; ++curr_x) {
      if (pixels[index] != mask) {
        int32_t sx = x + curr_x * 4 + offset;
        int32_t sy = y + curr_y;
        if (pagebase) {
          sy += (pagebase / 19200) * NATIVE_SCREEN_HEIGHT;
        }
        //if (sx >= 0 && sx < NATIVE_SCREEN_WIDTH &&
        //    sy >= 0 && sy < NATIVE_SCREEN_HEIGHT) {
        ((uint32_t*)surface->pixels)[sy * pitch + sx] = convert_color(pixels[index]);
        //}
      }
      ++index;
    }
  }
}

void xput(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  xput_layer(x, y, pagebase, buff, 0);
  xput_layer(x, y, pagebase, buff, 1);
  xput_layer(x, y, pagebase, buff, 2);
  xput_layer(x, y, pagebase, buff, 3);
}

void xput2(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  xput(x, y, pagebase, buff);
}
void xfput(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  xput(x, y, pagebase, buff);
}
void xfarput(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff) {
  xput(x, y, pagebase, buff);
}

void xtext(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff, int16_t color) {
  for (int i = 0; i < 72; i++) {
    int16_t text_color = *(buff + i) == 0
      ? 0
      : color;
    if (text_color) {
      int offset_x = x + (i % 8);
      int offset_y = y + (i / 8);
      xpset(offset_x, offset_y, pagebase, text_color);
    }
  }
}

void xtext1(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff, int16_t color) {
  xtext(x, y, pagebase, buff, color);
}
void xtextx(int16_t x, int16_t y, uint16_t pagebase, uint8_t* buff, int16_t color) {
  xtext(x, y, pagebase, buff, color);
}

void xpset(int16_t X, int16_t Y, uint16_t PageBase, int16_t Color) {
  int bpp = surface->format->BytesPerPixel;

  uint32_t pixel = convert_color((uint8_t)Color);
  uint8_t* p = (uint8_t*)surface->pixels + Y * surface->pitch + X * bpp;

  switch (bpp) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16*)p = pixel;
    break;
  case 3:
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    }
    else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(Uint32*)p = pixel;
    break;
  }
}

int16_t xpoint(int16_t X, int16_t Y, uint16_t PageBase) {
  int bpp = surface->format->BytesPerPixel;

  uint8_t* p = (uint8_t*)surface->pixels + Y * surface->pitch + X * bpp;

  switch (bpp) {
  case 1:
    return *p;
  case 2:
    return *(Uint16*)p;
  case 3:
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      return p[0] << 16 | p[1] << 8 | p[2];
    }
    else {
      return p[0] | p[1] << 8 | p[2] << 16;
    }
  case 4:
    return *(Uint32*)p;
  default:
    return 0;
  }
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
