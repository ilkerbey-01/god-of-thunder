// // THOR - The God of Thunder
// // See LICENSE for details

// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include <assert.h>
// #include <Windows.h>

// #include "../define.h"
// #include "../proto.h"
// #include "../res_man.h"
// #include "new.h"

// SDL_Window* sdl_window = NULL;
// SDL_Surface* sdl_window_surface = NULL;
// SDL_TimerID sdl_timer = 0;
// palette_t sdl_palette;

// //===========================================================================
// void* movedata(void* src0, void* src1, void* dest0, void* dest1, size_t bytes) {
//     assert(dest0 == dest1);
//     assert(src0 == src1);
//     return memmove(dest0, src0, bytes);
// }
// //===========================================================================
// void delay(int16_t ms) {
//     Sleep((DWORD)ms);
// }
// //===========================================================================
// void read_joystick(void) {
//     return; // TODO read_joystick
// }
// //===========================================================================
// void xsetmode(void) {
//     assert(SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) == 0);
//     sdl_window = SDL_CreateWindow("God of Thunder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, NATIVE_SCREEN_WIDTH * 4, NATIVE_SCREEN_HEIGHT * 4, 0);
//     assert(sdl_window);
//     sdl_window_surface = SDL_GetWindowSurface(sdl_window);
//     assert(sdl_window_surface);
//     sdl_timer = SDL_AddTimer(1000 / 50, t0Service, NULL);
// }

// TODO revisit this
// void xtext(int16_t x,int16_t y,uint16_t pagebase,char *buff,int16_t color) {
//     int oldx = x;
//     for (int c = 0; buff[c] != '\0'; ++c) {
//         if (buff[c] == '\n') {
//             y += 16;
//             x = oldx;
//         } else {
//             xput(x, y, pagebase, text[buff[c] * 16 * 16]);
//         }
//     }
//     return; // TODO xtext
// }
// //===========================================================================
// void xcopys2d(int16_t SourceStartX, int16_t SourceStartY,
//     int16_t SourceEndX, int16_t SourceEndY, int16_t DestStartX,
//     int16_t DestStartY, char* SourcePtr, uint16_t DestPageBase,
//     int16_t SourceBitmapWidth, int16_t DestBitmapWidth) {
//     return; // TODO xcopys2d
// }
// //===========================================================================
// void xcopyd2d(int16_t SourceStartX, int16_t SourceStartY,
//     int16_t SourceEndX, int16_t SourceEndY, int16_t DestStartX,
//     int16_t DestStartY, uint16_t SourcePageBase,
//     uint16_t DestPageBase, int16_t SourceBitmapWidth,
//     int16_t DestBitmapWidth) {
//     return; // TODO xcopyd2d
// }
// //===========================================================================
// void xdisplay_actors(ACTOR *act,uint16_t page) {
//     return; // TODO xdisplay_actors
// }
// //===========================================================================
// void xerase_actors(ACTOR *act,uint16_t page) {
//     return; // TODO xerase_actors
// }
// //===========================================================================
// void pal_fade_in(char *buff) {
//     return; // TODO pal_fade_in
// }
// //===========================================================================
// void pal_fade_out(char *buff) {
//     return; // TODO pal_fade_out
// }
