#include "utility.h"

#include <stdlib.h>

char* end;

int8_t parse_decimal_int8_t(char* str) {
    return strtol(str, &end, 10);
}

int16_t parse_decimal_int16_t(char* str) {
    return strtol(str, &end, 10);
}
