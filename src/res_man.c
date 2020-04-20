#include "res_man.h"

RES_HEADER res_header[0xb1];

int32_t res_read(const char *name, uint8_t *buff) {
    return 0;
}

void res_init(uint8_t* lzss) {

}

int16_t res_open(const char* file_name) {
  return -1;
}

int16_t res_find_name(const char* res_name) {
  return -1;
}

uint8_t* res_falloc_read(const char* res_name) {
  return 0;
}

void res_close() {

}
