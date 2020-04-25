// THOR - The God of Thunder
// See LICENSE for details

#include "sb_man.h"
#include "res_man.h"
#include "header.h"

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint8_t* sound;
  int32_t length;
} DIGITAL_SOUND;

DIGITAL_SOUND digital_sounds[NUM_SOUNDS];
uint8_t* sound_root = nullptr;
SDL_AudioSpec audio_spec;

DIGITAL_SOUND digital_sounds_boss[3][3];

static uint8_t* sound_chunk;
static uint32_t sound_size;
static uint8_t* sound_pos;

/* Audio Callback
 * The audio function callback takes the following parameters:
 * udata: ignored
 * stream: A pointer to the audio buffer to be filled
 * chunk_size: The length (in bytes) of the audio buffer
*/
void sb_send_pcm(void* _, uint8_t* stream, int chunk_size) {
  //SDL 2.0
  SDL_memset(stream, 0, chunk_size);
  if (sound_size == 0) {
    return;
  }
  chunk_size = (chunk_size > sound_size ? sound_size : chunk_size);

  SDL_MixAudio(stream, sound_pos, chunk_size, SDL_MIX_MAXVOLUME);
  sound_pos += chunk_size;
  sound_size -= chunk_size;
}

bool load_boss_sound(const char* res_name, DIGITAL_SOUND* digital_sound) {
  digital_sound->sound = res_falloc_read(res_name);
  if (!digital_sound->sound) {
    return false;
  }
  digital_sound->length = res_original_size(res_name);
  return true;
}

bool load_boss_sounds() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      digital_sounds_boss[i][i].length = 0;
      digital_sounds_boss[i][i].sound = nullptr;
    }
  }

  // Part 1
  if (!load_boss_sound("BOSSV11", &digital_sounds_boss[0][0])) {
    return false;
  }
  if (!load_boss_sound("BOSSV12", &digital_sounds_boss[0][1])) {
    return false;
  }
  if (!load_boss_sound("BOSSV12", &digital_sounds_boss[0][2])) {
    return false;
  }

  // Part 2
  if (!load_boss_sound("BOSSV21", &digital_sounds_boss[1][0])) {
    return false;
  }
  if (!load_boss_sound("BOSSV22", &digital_sounds_boss[1][1])) {
    return false;
  }
  if (!load_boss_sound("BOSSV22", &digital_sounds_boss[1][2])) {
    return false;
  }

  // Part 3
  if (!load_boss_sound("BOSSV31", &digital_sounds_boss[2][0])) {
    return false;
  }
  if (!load_boss_sound("BOSSV32", &digital_sounds_boss[2][1])) {
    return false;
  }
  if (!load_boss_sound("BOSSV32", &digital_sounds_boss[2][2])) {
    return false;
  }
  return true;
}

bool sdl_audio_opened = false;
bool sb_initialize() {

  audio_spec.freq = 12195;
  audio_spec.format = AUDIO_U8;
  audio_spec.channels = 1;
  audio_spec.silence = 0;
  audio_spec.samples = 1024;
  audio_spec.callback = &sb_send_pcm;

  if (SDL_OpenAudio(&audio_spec, NULL) < 0) {
    return false;
  }
  sdl_audio_opened = true;

  // The single file is made up of 16 headers
  // (including length), and then the 16 VOC files.
  sound_root = res_falloc_read("DIGSOUND");
  if (!sound_root) {
    return false;
  }
  uint8_t* p = sound_root;
  HEADER* header = (HEADER*)sound_root;
  p += (sizeof(HEADER) * 16);

  // Read in all of the sound files.
  for (int i = 0; i < 16; i++)
  {
    DIGITAL_SOUND sound = DIGITAL_SOUND();
    sound.sound = p;
    sound.length = header->length;
    digital_sounds[i] = sound;
    p += header->length;
    header++;
  }

  if (!load_boss_sounds()) {
    return false;
  }
}

void sb_close() {
  if (sdl_audio_opened) {
    SDL_CloseAudio();
  }

  if (sound_root) {
    free(sound_root);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (digital_sounds_boss[i][j].sound) {
        free(digital_sounds_boss[i][j].sound);
      }
    }
  }
}

void sb_swap_boss_sounds(int16_t boss_num) {
  digital_sounds[NUM_SOUNDS - 3].length = digital_sounds_boss[boss_num - 1][0].length;
  digital_sounds[NUM_SOUNDS - 3].sound = digital_sounds_boss[boss_num - 1][0].sound;
  digital_sounds[NUM_SOUNDS - 2].length = digital_sounds_boss[boss_num - 1][1].length;
  digital_sounds[NUM_SOUNDS - 2].sound = digital_sounds_boss[boss_num - 1][1].sound;
  digital_sounds[NUM_SOUNDS - 1].length = digital_sounds_boss[boss_num - 1][2].length;
  digital_sounds[NUM_SOUNDS - 1].sound = digital_sounds_boss[boss_num - 1][2].sound;
}

int16_t SoundBlasterPresent = 1;
uint8_t AdLibPresent;
volatile uint16_t timer_cnt, vbl_cnt, magic_cnt, extra_cnt;
int16_t music_flag, sound_flag, pcsound_flag;
uint8_t noal, nosb;

int16_t SB_VOCPlaying() {
  return sound_size > 0;
}

void SB_PlayVOC(uint8_t sound_index, int16_t tmp) {
  DIGITAL_SOUND digital_sound = digital_sounds[sound_index];
  uint8_t* sound = digital_sound.sound;
  uint8_t* end = sound + digital_sound.length;
  VOC_HEADER* header = (VOC_HEADER*)sound;
  sound += sizeof(VOC_HEADER);

  SDL_PauseAudio(0);

  int i = 0;
  while (sound < end) {
    VOC_DATA_BLOCK_TYPE type = (VOC_DATA_BLOCK_TYPE)*sound;
    sound++;

    uint32_t size = *sound;
    sound++;
    size += (*sound) << 8;
    sound++;
    size += (*sound) << 16;
    sound++;

    switch (type) {
    case VOC_DATA_BLOCK_TYPE::terminator: {
      return;
    }
    case VOC_DATA_BLOCK_TYPE::ascii_string: {
      // Null terminated string
      char* ascii_string = (char*)sound;
      sound += strlen(ascii_string);

      // TODO?
      break;
    }
    case VOC_DATA_BLOCK_TYPE::end_repeat: {
      // TODO
      break;
    }
    case VOC_DATA_BLOCK_TYPE::marker: {
      // The last marker remains in an intermediate memory during playback and can be jumped on again. 
      uint8_t* marker_number = sound;
      sound += 2;

      // TODO
      break;
    }
    case VOC_DATA_BLOCK_TYPE::repeat: {
      // Number of repetitions (2-byte integer) 
      // 0x0000 to 0xFFFE1-65,535 repetitions, 0xFFFFfor infinite 
      uint16_t to_repeat = (uint16_t)*sound;
      sound += 2;

      // TODO
      break;
    }
    case VOC_DATA_BLOCK_TYPE::silence: {
      // Length of silence in the unit of the sampling rate Integer-1
      uint16_t length = (uint16_t)*sound;
      sound += 2;

      // as with audio content
      uint8_t sampling_rate = *sound;
      audio_spec.freq = 1000000 / (256 - sampling_rate);
      sound++;
      break;
    }
    case VOC_DATA_BLOCK_TYPE::sound_continue: {
      // additional audio data in the same format as the previous block 
      uint8_t* data = sound;
      sound += size;
      break;
    }
    case VOC_DATA_BLOCK_TYPE::sound_data: {
      // (1000000/256-Sample rate)
      uint8_t sampling_rate = *sound;
      audio_spec.freq = 1000000 / (256 - sampling_rate);
      sound++;

      // see table is ignored if a block of type 0x08( Extra info )
      // defines a codec (from version 1.20)
      VOC_CODEC_TYPE codec = (VOC_CODEC_TYPE)*sound;
      switch (codec) {
      case VOC_CODEC_TYPE::unsigned_pcm_8_bits:
        audio_spec.format = AUDIO_U8;
        break;
      case VOC_CODEC_TYPE::signed_pcm_16_bits:
        audio_spec.format = AUDIO_S16;
        break;
      default:
        // TODO are any other formats needed?
        break;
      }
      sound++;

      // Audio data in the specified format (codec)
      // Set audio buffer (PCM data)
      sound_chunk = sound;
      sound_size = size;
      sound_pos = sound_chunk;
      sound += size;
      break;
    }
    }
  }
}

void SB_StopVOC() {
  SDL_PauseAudio(1);
}

void SB_Shutdown() {

}

void SB_ALOut(int16_t x, int16_t y) {

}

uint8_t* SB_Startup(char* x) {
  return 0;
}

uint8_t* SB_DetectAdLib(void) {
  return 0;
}
