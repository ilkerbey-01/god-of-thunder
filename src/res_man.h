// THOR - The God of Thunder
// See LICENSE for details

#pragma once

#include <stdint.h>

typedef struct {              //23
  char name[9];               //9 name of file
  uint32_t offset;            //4 offset from gotres.dat
  uint32_t length;            //4 compressed size
  uint32_t original_size;     //4 decompressed size
  uint16_t flags;             //2 is compressed
} RES_HEADER;
extern RES_HEADER res_header[0xb1];


int32_t res_read(const char *name, uint8_t *buff);
void res_init(uint8_t* lzss);
int16_t res_open(const char* file_name);
int16_t res_find_name(const char* res_name);
uint8_t* res_falloc_read(const char* res_name);
void res_close();
