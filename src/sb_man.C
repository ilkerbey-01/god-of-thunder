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

SDL_AudioSpec actual_spec;
DIGITAL_SOUND digital_sounds[NUM_SOUNDS];
DIGITAL_SOUND digital_sounds_boss[3][3];

void sb_convert(uint8_t* sound, int32_t length, DIGITAL_SOUND* digital_sound) {
  uint8_t* end = sound + length;
  VOC_HEADER* header = (VOC_HEADER*)sound;
  sound += sizeof(VOC_HEADER);

  SDL_AudioFormat src_format = AUDIO_U8;
  uint8_t src_channels = 1;
  int src_freq = 48000;
  uint8_t* src_data = NULL;
  uint32_t src_data_len = 0;

  // Resources from GOTRES.RES only implement the
  // terminator, sound_data and extra_information data blocks,
  // and only include one sound_data block per sound.
  int i = 0;
  while (sound < end) {
    auto type = (enum VOC_DATA_BLOCK_TYPE)(uint8_t)*sound;
    sound++;

    uint32_t size = *sound;
    sound++;
    size += (*sound) << 8;
    sound++;
    size += (*sound) << 16;
    sound++;

    switch (type) {
    case terminator: {
      sound = end;
      break;
    }
    case sound_data: {
      // (1000000/256-Sample rate)
      uint8_t sampling_rate = *sound;
      src_freq = 1000000 / (256 - sampling_rate);
      sound++;

      // see table is ignored if a block of type 0x08( Extra info )
      // defines a codec (from version 1.20)
      auto codec = (enum VOC_CODEC_TYPE)*sound;

      switch (codec) {
      case unsigned_pcm_8_bits:
        src_format = AUDIO_U8;
        break;
      case signed_pcm_16_bits:
        src_format = AUDIO_S16;
        break;
      default:
        // TODO are any other formats needed?
        break;
      }
      sound++;

      // Audio data in the specified format (codec)
      src_data = sound;
      src_data_len = size;
      sound += size;
      break;
    }
    case extra_information: {
      // Sample rate = 256000000 / ((numChannels + 1) * (65536 - frequency divisor))
      uint16_t sampling_rate = *sound;
      sound += 2;

      auto codec = (enum VOC_CODEC_TYPE)*sound;
      switch (codec) {
      case unsigned_pcm_8_bits:
        src_format = AUDIO_U8;
        break;
      case signed_pcm_16_bits:
        src_format = AUDIO_S16;
        break;
      default:
        // TODO are any other formats needed?
        break;
      }
      sound++;

      uint8_t num_channels = *sound;
      sound++;

      src_channels = num_channels + 1;
      src_freq = 256000000 / ((num_channels + 1) * (65536 - sampling_rate));
      break;
    }
    }
  }

  SDL_AudioCVT cvt;
  SDL_BuildAudioCVT(&cvt
    , src_format, src_channels, src_freq
    , actual_spec.format, actual_spec.channels, actual_spec.freq);

  cvt.len = src_data_len;
  cvt.buf = (uint8_t*)malloc((size_t)src_data_len * (size_t)cvt.len_mult);
  memcpy(cvt.buf, src_data, src_data_len);

  SDL_ConvertAudio(&cvt);

  digital_sound->length = cvt.len_cvt;
  digital_sound->sound = cvt.buf;
}

uint8_t load_boss_sound(const char* res_name, DIGITAL_SOUND* digital_sound) {
  uint8_t* sound = res_falloc_read(res_name);
  if (!sound) {
    return 0;
  }
  int32_t length = res_original_size(res_name);
  sb_convert(sound, length, digital_sound);
  free(sound);
  return 1;
}

uint8_t load_boss_sounds() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      digital_sounds_boss[i][i].length = 0;
      digital_sounds_boss[i][i].sound = NULL;
    }
  }

  // Part 1
  if (!load_boss_sound("BOSSV11", &digital_sounds_boss[0][0])) {
    return 0;
  }
  if (!load_boss_sound("BOSSV12", &digital_sounds_boss[0][1])) {
    return 0;
  }
  if (!load_boss_sound("BOSSV12", &digital_sounds_boss[0][2])) {
    return 0;
  }

  // Part 2
  if (!load_boss_sound("BOSSV21", &digital_sounds_boss[1][0])) {
    return 0;
  }
  if (!load_boss_sound("BOSSV22", &digital_sounds_boss[1][1])) {
    return 0;
  }
  if (!load_boss_sound("BOSSV22", &digital_sounds_boss[1][2])) {
    return 0;
  }

  // Part 3
  if (!load_boss_sound("BOSSV31", &digital_sounds_boss[2][0])) {
    return 0;
  }
  if (!load_boss_sound("BOSSV32", &digital_sounds_boss[2][1])) {
    return 0;
  }
  if (!load_boss_sound("BOSSV32", &digital_sounds_boss[2][2])) {
    return 0;
  }
  return 1;
}

uint8_t sdl_audio_opened = 0;
uint8_t sb_initialize() {
  SDL_AudioSpec desired_spec;
  desired_spec.freq = 48000;
  desired_spec.format = AUDIO_S16;
  desired_spec.channels = 2;
  desired_spec.silence = 0;
  desired_spec.samples = 1024;
  desired_spec.callback = NULL;

  if (SDL_OpenAudio(&desired_spec, &actual_spec) < 0) {
    return 0;
  }
  sdl_audio_opened = 1;

  // The single file is made up of 16 headers
  // (including length), and then the 16 VOC files.
  uint8_t* raw_sound = res_falloc_read("DIGSOUND");
  if (!raw_sound) {
    return 0;
  }
  HEADER* header = (HEADER*)raw_sound;
  raw_sound += (sizeof(HEADER) * 16);

  // Read in all of the sound files.
  for (int i = 0; i < 16; i++)
  {
    sb_convert(raw_sound, header->length, &digital_sounds[i]);

    raw_sound += header->length;
    header++;
  }

  if (!load_boss_sounds()) {
    return 0;
  }
}

void sb_close() {
  if (sdl_audio_opened) {
    SDL_CloseAudio();
  }

  // Don't free the last sounds, because they're 
  // shared with one of the boss sound sets.
  for (int i = 0; i < NUM_SOUNDS - 3; i++) {
    if (digital_sounds[i].sound) {
      free(digital_sounds[i].sound);
    }
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
int16_t music_flag, sound_flag, pcsound_flag;
uint8_t noal, nosb;

int16_t SB_VOCPlaying() {
  return SDL_GetAudioStatus() == SDL_AUDIO_PLAYING;
}

void SB_PlayVOC(uint8_t sound_index, int16_t tmp) {
  SDL_PauseAudio(0);

  DIGITAL_SOUND digital_sound = digital_sounds[sound_index];
  SDL_QueueAudio(1, digital_sound.sound, digital_sound.length);
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
