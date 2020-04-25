// THOR - The God of Thunder
// See LICENSE for details

#pragma once

#include <stdint.h>

#define NUM_SOUNDS 19

typedef struct {
  char label[19];        // ASCII-StringCreative Voice File
  uint8_t signature;
  uint16_t header_size;
  uint16_t version;
  uint16_t checksum;
} VOC_HEADER;

enum VOC_DATA_BLOCK_TYPE {
  terminator          = 0x00,
  sound_data          = 0x01,
  sound_continue      = 0x02,
  silence             = 0x03,
  marker              = 0x04,
  ascii_string        = 0x05,
  repeat              = 0x06,
  end_repeat          = 0x07,
  extra_information   = 0x08
};

enum VOC_CODEC_TYPE {
  unsigned_pcm_8_bits = 0x00,
  creative_adpcm_4_bits_to_8_bits = 0x01,
  creative_adpcm_3_bits_to_8_bits = 0x02, // (AKA 2.6 bits)
  creative_adpcm_2_bits_to_8_bits = 0x03,
  signed_pcm_16_bits = 0x04,
  alaw = 0x06,
  ulaw = 0x07,
  creative_adpcm_4_bits_to_16_bits = 0x200, // (only valid in block type 0x09)
};

extern int16_t SoundBlasterPresent;
uint8_t sb_initialize();
void sb_close();
void sb_swap_boss_sounds(int16_t boss_num);
int16_t SB_VOCPlaying();
void SB_PlayVOC(uint8_t sound_index, int16_t tmp);
void SB_StopVOC();
void SB_Shutdown();
void SB_ALOut(int16_t x, int16_t y);
uint8_t* SB_Startup(char* x);
uint8_t* SB_DetectAdLib(void);
