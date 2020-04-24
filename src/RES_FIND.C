#include <stdio.h>
#include <string.h>

#include "res_man.h"

#ifdef WIN32
#define STRCMP_I(x, y) strcmpi(x, y)
#else
#include <strings.h>
#define STRCMP_I(x, y) strcasecmp(x, y)
#endif

//==========================================================================
int16_t res_find_name(const char* name) {
  int16_t i;

  if (!res_active) return RES_NOT_ACTIVE;
  if (!res_fp) return RES_NOT_OPEN;

  char buf[9];
  memcpy(buf, name, 9);
  buf[8] = '\0';
  for (i = 0; i < RES_MAX_ENTRIES; i++) {
    if (!STRCMP_I(buf, res_header[i].name)) return i;
  }
  return RES_ENTRY_NOT_FOUND;
}
//==========================================================================
int16_t res_find_empty(void) {
  int16_t i;

  if (!res_active) return RES_NOT_ACTIVE;
  if (!res_fp) return RES_NOT_OPEN;

  for (i = 0; i < RES_MAX_ENTRIES; i++) {
    if (res_header[i].length == 0) return i;
  }
  return RES_FULL;
}
