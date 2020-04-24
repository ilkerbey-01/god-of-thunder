#include <stdio.h>
#include <assert.h>

#include "DEFINE.H"
#include "PROTO.H"
#include "res_man.h"
//===========================================================================
int32_t lzss_decompress(uint8_t* src, uint8_t* dest) {
  uint16_t original_size = *(uint16_t*)src; src += 2;
  uint16_t one = *(uint16_t*)src; src += 2;
  assert(one == 1);

  uint8_t* original_src = src;
  uint8_t* original_dest = dest;

  while ((uint16_t)(dest - original_dest) < original_size) {
    char b = *src; ++src;
    for (int bit = 0; bit < 8; ++bit) {
      if (b & (1 << bit)) {
        *dest = *src;
        ++dest; ++src;
        if ((uint16_t)(dest - original_dest) == original_size) {
          goto done;
        }
      }
      else {
        uint16_t d = *((uint16_t*)src); src += 2;
        uint8_t count = (uint8_t)((d >> 12) + 2);
        uint16_t offset = (d & 0x0fff);
        uint8_t* lookup = dest - (size_t)offset;
        assert(lookup >= original_dest);
        for (uint8_t c = 0; c < count; ++c) {
          *dest = *lookup;
          ++dest; ++lookup;
          if ((uint16_t)(dest - original_dest) == original_size) {
            goto done;
          }
        }
      }
    }
  }
done:
  return (int32_t)(dest - original_dest);
}
//==========================================================================
int32_t res_decode(uint8_t* buff) {
  return lzss_decompress(res_lzss_buff, buff);
}
