// THOR - The God of Thunder
// See LICENSE for details

#pragma once
#pragma pack(push, 1)

#include <stdio.h>
#include <stdint.h>

typedef struct{              //23
    char name[9];            //9 name of file
    uint32_t offset;         //4 offset from gotres.dat
    uint32_t length;         //4 compressed size
    uint32_t original_size;  //4 decompressed size
    uint16_t key;            //2 is compressed
} RES_HEADER;

#define RES_MAX_ENTRIES 256  //max # of elements

#define RES_NO_ERROR 0

#define RES_NOT_ACTIVE      -1
#define RES_CANT_OPEN       -2
#define RES_CANT_CLOSE      -3
#define RES_CANT_WRITE      -4
#define RES_CANT_READ       -5
#define RES_ENTRY_NOT_FOUND -6
#define RES_ALREADY_OPEN    -7
#define RES_FULL            -8
#define RES_CANT_SEEK       -9
#define RES_NOT_OPEN        -10
#define RES_CANT_CREATE     -11
#define RES_CANT_FIND       -12
#define RES_ALREADY_EXISTS  -13
#define RES_CANT_OPEN_SRC   -14
#define RES_CANT_OPEN_DST   -15
#define RES_SRC_TOO_LARGE   -16
#define RES_OUT_OF_MEMORY   -17

extern FILE *res_fp;
extern RES_HEADER res_header[RES_MAX_ENTRIES];
extern int16_t res_active;
extern int16_t res_changed;
extern uint8_t* res_lzss_buff;

void res_init(uint8_t* );
int16_t res_open(const char* filename);
int16_t res_close(void);
void res_encrypt(uint8_t* buff, int32_t len, char key);
void res_decrypt(uint8_t* buff, int32_t len, char key);
int16_t res_abort(void);
int16_t res_create(const char* filename);
int32_t res_encode(uint8_t* buff, int32_t length);
int32_t res_decode(uint8_t* buff);
int16_t res_find_name(const char* name);
int16_t res_find_empty(void);
int16_t res_write(const char* name, uint8_t* buff, int32_t length, int16_t encode_flag);
int32_t res_read(const char* name, uint8_t* buff);
int32_t res_read_element(const char* name, uint8_t* buff, int32_t offset, int32_t size);
uint8_t* res_falloc_read(const char* name);
int16_t res_add_file(const char* fname, uint8_t* name, int16_t encode_flag);
int16_t res_extract_file(const char* fname, const char* name);
int16_t res_delete_file(const char* name);
int16_t res_pack(const char* filename);
int16_t res_replace_file(const char* fname, const char* name, int16_t encode_flag);
int32_t res_length(const char* name);
int32_t res_original_size(const char* name);
int16_t res_rename_file(const char* oldname, const char* newname);
uint8_t* res_error(int16_t num);

#pragma pack(pop)
