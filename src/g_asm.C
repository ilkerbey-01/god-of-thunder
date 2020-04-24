// THOR - The God of Thunder
// See LICENSE for details

#include <cstdlib>
#include "DEFINE.H"
#include "PROTO.H"
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

void xsetmode(void) {

}
void xshowpage(unsigned page) {

}

int16_t xpoint(int16_t X, int16_t Y, uint16_t PageBase) {
  return 0;
}
void xget(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t pagebase,
          uint8_t *buff, int16_t invis) {

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
