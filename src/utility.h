// THOR - The God of Thunder
// See LICENSE for details

#pragma once

#include <stdint.h>

void delay(int milliseconds);
int8_t parse_decimal_int8_t(char* str);
int16_t parse_decimal_int16_t(char* str);
int32_t parse_decimal_int32_t(char* str);
void* movedata(void* src0, void* src1, void* dest0, void* dest1, size_t bytes);
