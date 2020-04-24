#include <stdio.h>
#include <stdlib.h>

#include "DEFINE.H"
#include "PROTO.H"
#include "res_man.h"
//==========================================================================
uint8_t* res_falloc_read(const char* name) {
  int16_t num;
  uint8_t* fp;

  if (!res_active) return 0;
  if (!res_fp) return 0;

  num = res_find_name(name);
  if (num < 0) return 0;

  fp = (uint8_t*)malloc(res_header[num].original_size);
  if (!fp) return 0;

  res_read(name, fp);
  return fp;
}
