#include <stdio.h>

#include "res_man.h"
//==========================================================================
int32_t res_length(const char* name) {
  int num;

  num = res_find_name(name);
  if (num < 0) return (long)RES_ENTRY_NOT_FOUND;
  return res_header[num].length;
}
//==========================================================================
int32_t res_original_size(const char* name) {
  int num;

  num = res_find_name(name);
  if (num < 0) return (long)RES_ENTRY_NOT_FOUND;
  return res_header[num].original_size;
}
