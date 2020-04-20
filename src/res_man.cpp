#include "res_man.h"

#include <stdio.h>
#include <string>

// Great details about how this is implemented:
// http://www.shikadi.net/moddingwiki/DAT_Format_(God_of_Thunder)

RES_HEADER res_header[0xb1];

FILE* res_file;
int32_t res_read(const char *name, uint8_t *buff) {
    return 0;
}

bool res_init_record(int num, int& offset) {
  uint8_t buffer[23];

  size_t read = fread(buffer, 1, 23, res_file);
  if (read != 23) {
    return false;
  }
  else {
    // Decrypt the contents using simple xor.
    int i = 0;
    for (i = 0; i < 23; i++) {
      buffer[i] = buffer[i] ^ ((offset++) % 256) ^ 128;
    }

    memcpy(&res_header[num], buffer, 23);
    return true;
  }
}

void res_init(uint8_t* lzss) {
  if (res_file == NULL) {
    return;
  }

  if (!fseek(res_file, 0, SEEK_SET)) {
    return;
  }

  int count = 0;
  int offset = 0;
  while (res_init_record(count++, offset)) {
  }
}

int16_t res_open(const char* file_name) {
  res_file = fopen(file_name, "rb");
  if (res_file != NULL) {
    return 0;
  }
  return -1;
}

int16_t res_find_name(const char* res_name) {
  for (int i = 0; i < 0xb1; i++) {
    if (strcmp(res_name, res_header[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

bool read_uint8(uint8_t& i) {
  unsigned char tmp[1];
  if (!fread(&tmp, 1, 1, res_file)) {
    return false;
  }
  else {
    i = tmp[0];
    return true;
  }
}

bool read_uint16(uint16_t& i) {
  uint8_t tmp[2];
  if (fread(&tmp, 1, 2, res_file) != 2) {
    return false;
  }

  i = (uint16_t)tmp[0];
  i += (uint16_t)tmp[1] << 8;
  return true;
}

bool res_compressed_read(uint8_t* buffer) {
  // Each file's data begins with a UINT16LE value holding the
  // decompressed size, in bytes,
  uint16_t decompressed_size;
  if (!read_uint16(decompressed_size)) {
    return false;
  }

  // followed by another UINT16LE value of unknown purpose
  // which should be ignored (the value is always 0x0001.)
  uint16_t ignore;
  if (!read_uint16(ignore)) {
    return false;
  }

  // The rest of the data decompresses as follows:
  uint16_t written = 0;

  // If the amount of data decompressed matches the target size, finish. Otherwise:
  while (written < decompressed_size) {
    // Read a byte from the input data
    uint8_t data;
    if (!read_uint8(data)) {
      return false;
    }

    // For each bit in the previous byte, from the least significant to the most:
    for (int b = 0; b < 8; b++) {
      if (written >= decompressed_size) {
        break;
      }

      uint8_t test = data >> b;

      // If the bit is 1, copy a byte unchanged from the input data to the output
      if (test & 0x1) {
        if (!read_uint8(buffer[written++])) {
          return false;
        }
      }
      // Otherwise the bit is zero:
      else {
        // Read a UINT16LE
        uint16_t control;
        if (!read_uint16(control)) {
          return false;
        }

        // Add two to the upper (most significant) four bits, and
        // treat this value as the LZSS "count"
        uint16_t upper_four_bits = control >> 12;
        uint16_t count = upper_four_bits + 2;

        // Take the lower 12 bits and treat the value as the LZSS "offset"
        uint16_t offset = (control & 0xfff);

        // Look back "offset" bytes into the newly decompressed data
        /*
         * Copy "count" bytes from here to the end of the newly decompressed data.
         * Take note that as each byte is copied to its destination, that new byte
         * may later become a source byte in this same copy operation. For example,
         * if "offset" is 1 (i.e. look back one byte) and the counter is 15, then
         * the last byte will be copied 17 times (15 + 2 = 17). This is because as
         * each byte is copied, it becomes the source byte for the next copy cycle.
         */
        for (int j = 0; j < count; j++) {
          uint8_t x = buffer[written - offset];
          buffer[written++] = x;
        }
      }
    }
  }
  return true;
}

uint8_t* res_falloc_read(const char* res_name) {
  int16_t num = res_find_name(res_name);
  if (num == -1) {
    return NULL;
  }

  RES_HEADER header = res_header[num];
  if (fseek(res_file, header.offset, SEEK_SET)) {
    return NULL;
  }

  uint8_t* buffer = (uint8_t*)malloc(header.length);

  // Compressed
  if (header.flags) {
    if (!res_compressed_read(buffer)) {
      free(buffer);
      return NULL;
    }
  }

  // Raw
  else {
    size_t read = fread(buffer, 1, header.length, res_file);
    if (read != header.length) {
      free(buffer);
      return NULL;
    }
  }

  return buffer;
}

void res_close() {
  if (res_file != NULL) {
    fclose(res_file);
  }
}
