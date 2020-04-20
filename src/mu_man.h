#pragma once

#include <stdint.h>

int16_t MU_MusicPlaying();
void MU_StartMusic(uint8_t* music, int16_t x);
void MU_Service();
void MU_MusicOff();
void MU_MusicOn();
