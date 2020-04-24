#include <stdio.h>
#include <string.h>

#include "DEFINE.H"
#include "PROTO.H"
#include "res_man.h"
#include "new/new.h"
//==========================================================================
int32_t res_read(const char* name, uint8_t* buff) {
  int16_t num, bytes;
  size_t len;
  size_t total;
  uint8_t bf[256];
  uint8_t* p;

  if (!res_active) return RES_NOT_ACTIVE;
  if (!res_fp) return RES_NOT_OPEN;

  num = res_find_name(name);
  if (num < 0) return RES_CANT_FIND;

  if (fseek(res_fp, res_header[num].offset, SEEK_SET)) return RES_CANT_SEEK;
  len = (size_t)res_header[num].length;

  total = 0;
  if (!res_header[num].key) p = buff;
  else p = res_lzss_buff;

  while (total < len) {
    if (((len - total) > 255) && (len > 255)) bytes = (int16_t)fread(bf, 1, 256, res_fp);
    else bytes = (int16_t)fread(bf, 1, len - total, res_fp);
    if (!bytes) break;
    total += bytes;
    memcpy(p, bf, bytes);
    p += bytes;
  }
  if (res_header[num].key) res_decode(buff);
  return res_header[num].original_size;
}
