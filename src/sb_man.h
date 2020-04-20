#pragma once

#include <stdint.h>

extern int16_t SoundBlasterPresent;
int16_t SB_VOCPlaying();
void SB_PlayVOC(uint8_t* sound, int16_t tmp);
void SB_StopVOC();
void SB_Shutdown();
void SB_ALOut(int16_t x, int16_t y);
uint8_t* SB_Startup(char* x);
uint8_t* SB_DetectAdLib(void);
