// THOR - The God of Thunder
// See LICENSE for details

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "res_man.h"
#include "DEFINE.H"
#include "PROTO.H"
#include "pal_man.h"
//===========================================================================
extern uint8_t text[94][72];
uint8_t pbuff[768];
extern uint8_t dialog_color[16];
extern uint8_t cheat;
extern uint16_t display_page, draw_page;
extern volatile uint8_t key_flag[100];
extern ACTOR actor[MAX_ACTORS];
//===========================================================================
void xprint(int16_t x, int16_t y, const char *string, uint16_t page, int16_t color)
{
  uint8_t ch;
  uint8_t str[4];

  str[3] = 0;
  x &= 0xfffc;
  while (*string)
  {
    ch = *string++;
    if (ch == '~' && isxdigit(*string))
    {
      ch = *string++;
      if (isdigit(ch))
        ch -= 48;
      else
      {
        ch = toupper(ch) - 55;
      }
      color = dialog_color[ch];
      continue;
    }
    if (ch > 31 && ch < 127)
    {
      xtext1(x, y, page, text[ch - 32], 0);
      xtext(x, y, page, text[ch - 32], color);
    }
    x += 8;
  }
}
//===========================================================================
void xprintx(int16_t x, int16_t y, const char *string, uint16_t page, int16_t color)
{
  uint8_t ch;
  uint8_t str[4];
  int16_t c;

  c = 26;
  str[3] = 0;
  x &= 0xfffc;
  while (*string)
  {
    ch = *string++;
    if (ch == '~' && isxdigit(*string))
    {
      ch = *string++;
      if (isdigit(ch))
        ch -= 48;
      else
      {
        ch = toupper(ch) - 55;
      }
      color = dialog_color[ch];
      continue;
    }
    if (ch > 31 && ch < 127)
    {
      xtextx(x, y - 1, page, text[ch - 32], c);
      xtextx(x, y + 1, page, text[ch - 32], c);
      xtextx(x + 1, y, page, text[ch - 32], c);
      xtextx(x - 1, y, page, text[ch - 32], c);
      xtextx(x - 1, y - 1, page, text[ch - 32], c);
      xtextx(x + 1, y + 1, page, text[ch - 32], c);
      xtextx(x + 1, y - 1, page, text[ch - 32], c);
      xtextx(x - 1, y + 1, page, text[ch - 32], c);
      xtext(x, y, page, text[ch - 32], color);
    }
    x += 8;
  }
}
//===========================================================================
void split_screen(void)
{
// TODO
//   asm {

// mov dx,0x03d4 //CRTC
// mov al,0x18 //select LINE COMPARE reg
// out dx,al
// inc dx
// mov ax,128  //set bits 0-7 of LINE COMPARE
//          //I believe this is what I changed
//          //I am loading the LINE compare with
//          //384 (192x2) (128 here + 256 in bit 8)
//          //since mode X displays each scan line
//          //twice. I was originally loading
//          //385, since that was the first
//          //scan line I wanted for the upper
//     //half of the screen.
// out dx,al
// dec dx
// mov al,7 //OVERFLOW reg
// out dx,al
// inc dx
// in  al,dx
// or  al,0x10 //set bit 8 of LINE COMPARE
// out dx,al
// dec dx
// mov al,9
// out dx,al
// inc dx
// in  al,dx
// and al,0xbf //clear bit 9 of LINE COMPARE
// out dx,al
//   }
}
//===========================================================================
int16_t load_palette(void)
{
  int16_t i;

  if (res_read("palette", (uint8_t *)pbuff) < 0)
    return 0;

  uint8_t temp;
  for (int i = 0; i < 255; i++) {
    temp = pbuff[i * 3];
    pbuff[i * 3] = pbuff[i * 3 + 2];
    pbuff[i * 3 + 2] = temp;
  }

  int index = 0;
  for (int i = 0; i < 0x300; i += 3) {
    PALETTE_COLOR color = PALETTE_COLOR();
    color.b = pbuff[i];
    color.g = pbuff[i + 1];
    color.r = pbuff[i + 2];
    palette_colors[index++] = color;
  }

  return 1;
}
/*=========================================================================*/
void xbox(int16_t x1, int16_t y1, int16_t x2, int16_t y2, unsigned page, int16_t color)
{

  xline(x1, y1, x2, y1, page, color);
  xline(x1, y2, x2, y2, page, color);
  xline(x1, y1, x1, y2, page, color);
  xline(x2, y1, x2, y2, page, color);
}
/*=========================================================================*/
void fade_in(void)
{

  pal_fade_in(pbuff);
}
/*=========================================================================*/
void fade_out(void)
{

  pal_fade_out(pbuff);
}
//===========================================================================
void unsplit_screen(void)
{
  // TODO
//   asm {

// mov dx,0x03d4 //CRTC
// mov al,0x18 //select LINE COMPARE reg
// out dx,al
// inc dx
// mov ax,255 //set bits 0-7 of LINE COMPARE
// out dx,al
// dec dx
// mov al,7 //OVERFLOW reg
// out dx,al
// inc dx
// in  al,dx
// or  al,0x10 //set bit 8 of LINE COMPARE
// out dx,al
// dec dx
// mov al,9
// out dx,al
// inc dx
// in  al,dx
// or  al,64
//     //and al,0xbf      //clear bit 9 of LINE COMPARE
// out dx,al
//   }
}
//==========================================================================
void screen_dump(void)
{

  if (cheat)
  {
    if (display_page != PAGE0)
      return;
    play_sound(YAH, 1);
    unsplit_screen();
    xshowpage(display_page);
    xcopyd2d(0, 0, 320, 48, 0, 192, PAGES, display_page, 320, 320);
    while (key_flag[_K])
      ;
    while (!key_flag[ESC])
      ;
    split_screen();
    xcopyd2d(0, 0, 320, 48, 0, 193, display_page, display_page, 320, 320);
  }
}
//==========================================================================
void show_all_actors(void)
{
  int16_t i;

  for (i = 0; i < MAX_ACTORS; i++)
    actor[i].show = 0;
  xdisplay_actors(&actor[MAX_ACTORS - 1], display_page);
}
