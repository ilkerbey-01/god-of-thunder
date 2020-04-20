#pragma once

#include <stdint.h>

typedef struct {
  char tmp;
} PCSound;

int FX_PCPlaying();
void FX_PlayPC(PCSound* sound, int32_t size);
void FX_StopPC();
void FX_ServicePC();
