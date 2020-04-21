#pragma once

#include <stdint.h>

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} PALETTE_COLOR;

extern PALETTE_COLOR palette_colors[0x100];
