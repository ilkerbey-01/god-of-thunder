#include "sb_man.h"

int16_t SoundBlasterPresent;
uint8_t AdLibPresent;
volatile uint16_t timer_cnt, vbl_cnt, magic_cnt, extra_cnt;
int16_t music_flag, sound_flag, pcsound_flag;
uint8_t noal, nosb;
int16_t SB_VOCPlaying() {
  return 0;
}

void SB_PlayVOC(uint8_t* sound, int16_t tmp) {

}

void SB_StopVOC() {

}

void SB_Shutdown() {

}

void SB_ALOut(int16_t x, int16_t y) {

}

uint8_t* SB_Startup(uint8_t* x) {
  return 0;
}

uint8_t* SB_DetectAdLib(void) {
  return 0;
}
