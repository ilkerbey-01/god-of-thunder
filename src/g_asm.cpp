#include <cstdlib>
#include "1_DEFINE.H"
#include "1_PROTO.H"
#include "g_asm.h"

void xdisplay_actors(ACTOR *act, uint16_t page) {
    
}

void xerase_actors(ACTOR *act, uint16_t page) {

}

void pal_fade_in(uint8_t *buff) {

}

void pal_fade_out(uint8_t *buff) {

}

void read_joystick(void) {

}

void UnLZSS(uint8_t *src, uint8_t *dest, int16_t len) {

}

void xsetmode(void) {

}
void xshowpage(unsigned page) {

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
void xpset(int16_t X, int16_t Y, uint16_t PageBase, int16_t Color) {

}
int16_t xpoint(int16_t X, int16_t Y, uint16_t PageBase) {
  return 0;
}
void xget(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t pagebase,
          uint8_t *buff, int16_t invis) {

}
void xput(int16_t x, int16_t y, uint16_t pagebase, uint8_t *buff) {

}
void xput2(int16_t x, int16_t y, uint16_t pagebase, uint8_t *buff) {

}
void xtext(int16_t x, int16_t y, uint16_t pagebase, uint8_t *buff, int16_t color) {

}
void xtext1(int16_t x, int16_t y, uint16_t pagebase, uint8_t *buff, int16_t color) {

}
void xtextx(int16_t x, int16_t y, uint16_t pagebase, uint8_t *buff, int16_t color) {

}

void xfput(int16_t x, int16_t y, uint16_t pagebase, uint8_t *buff) {

}
void xfarput(int16_t x, int16_t y, uint16_t pagebase, uint8_t *buff) {

}
void xcopyd2dmasked(int16_t SourceStartX,
                    int16_t SourceStartY, int16_t SourceEndX, int16_t SourceEndY,
                    int16_t DestStartX, int16_t DestStartY, MASK_IMAGE *Source,
                    uint16_t DestPageBase, int16_t DestBitmapWidth) {

}
void xcopyd2dmasked2(
    int16_t SourceEndX, int16_t SourceEndY,
    int16_t DestStartX, int16_t DestStartY, MASK_IMAGE *Source,
    uint16_t DestPageBase) {

}
void xcopys2d(int16_t SourceStartX, int16_t SourceStartY,
              int16_t SourceEndX, int16_t SourceEndY, int16_t DestStartX,
              int16_t DestStartY, uint8_t *SourcePtr, uint16_t DestPageBase,
              int16_t SourceBitmapWidth, int16_t DestBitmapWidth) {

}
void xcopyd2d(int16_t SourceStartX, int16_t SourceStartY,
              int16_t SourceEndX, int16_t SourceEndY, int16_t DestStartX,
              int16_t DestStartY, uint16_t SourcePageBase,
              uint16_t DestPageBase, int16_t SourceBitmapWidth,
              int16_t DestBitmapWidth) {

}
uint16_t xcreatmaskimage(MASK_IMAGE *ImageToSet,
                         uint16_t DispMemStart, uint8_t *Image, int16_t ImageWidth,
                         int16_t ImageHeight, uint8_t *Mask) {
  return 0;
}
uint16_t xcreatmaskimage2(MASK_IMAGE *ImageToSet,
                          uint16_t DispMemStart, uint8_t *Image, int16_t ImageWidth,
                          int16_t ImageHeight, uint8_t *Mask) {
  return 0;
}

void xddfast(int16_t source_x, int16_t source_y, int16_t width, int16_t height,
             int16_t dest_x, int16_t dest_y,
             uint16_t source_page, uint16_t dest_page) {

}
void xsetpal(uint8_t color, uint8_t R, uint8_t G, uint8_t B) {

}
void xgetpal(uint8_t *pal, int16_t num_colrs, int16_t start_index) {

}
