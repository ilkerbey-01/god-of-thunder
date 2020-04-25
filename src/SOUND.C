// See LICENSE for details

#include <stdio.h>
#include <memory.h>
#include <cstdlib>

#include "DEFINE.H"
#include "PROTO.H"
#include "res_man.h"

#include "fx_man.h"
#include "mu_man.h"
#include "sb_man.h"

void play_pc_sound(int16_t index, int16_t priority_override);
//===========================================================================
extern uint8_t *std_sounds;
extern uint8_t *pcstd_sounds;
extern uint8_t *pc_sound[NUM_SOUNDS];
extern uint8_t *pcstd_sound_start;
extern int16_t level;

//enum{OW,GULP,SWISH,YAH,ELECTRIC,THUNDER,DOOR,FALL,
//     ANGEL,WOOP,ANGEL,BRAAPP,WIND,PUNCH1,CLANG,EXPLODE
//     BOSS11,BOSS12,BOSS13
int16_t sound_priority[] = {1, 2, 3, 3, 3, 1, 4, 4, 4, 5, 4, 3, 1, 2, 2, 5, 1, 3, 1};
extern int32_t pcsound_length[NUM_SOUNDS];

int16_t current_priority;
//===========================================================================
extern SETUP setup;
extern uint8_t ds_file[];
//===========================================================================
int16_t sound_init(void)
{
  int16_t i;
  uint8_t *p;
  HEADER *header;

  if (!sb_initialize()) {
    return 0;
  }

  pcstd_sound_start = res_falloc_read("PCSOUNDS");
  if (!pcstd_sound_start)
    return 0;
  pcstd_sounds = pcstd_sound_start;
  header = (HEADER *)pcstd_sounds;
  pcstd_sounds = pcstd_sounds + (sizeof(HEADER) * 16);

  p = pcstd_sounds;
  for (i = 0; i < 16; i++)
  {
    pc_sound[i] = p;
    pc_sound[i][0] = 0;
    pc_sound[i][1] = 0;
    pcsound_length[i] = header->length;
    p += (int)header->length;
    header++;
  }
  current_priority = 255;
  return 1;
}
//===========================================================================
void sound_exit(void)
{

  FX_StopPC();
  while (FX_PCPlaying())
    ;

  SB_StopVOC();

  if (pcstd_sound_start)
    free(pcstd_sound_start);
}
//===========================================================================
void play_sound(int16_t index, int16_t priority_override)
{

  if (index >= NUM_SOUNDS)
    return;
  if (setup.pc_sound)
  {
    play_pc_sound(index, priority_override);
    return;
  }
  if (!setup.dig_sound)
    return;

  if (sound_playing())
  {
    if ((!priority_override) && current_priority < sound_priority[index])
      return;
    SB_StopVOC();
  }

  SB_PlayVOC(index, 1);
  current_priority = sound_priority[index];
}
//===========================================================================
void play_pc_sound(int16_t index, int16_t priority_override)
{

  if (FX_PCPlaying())
  {
    if ((!priority_override) && current_priority < sound_priority[index])
      return;
    FX_StopPC();
  }

  FX_PlayPC((PCSound *)pc_sound[index], pcsound_length[index]);
  current_priority = sound_priority[index];
}
//===========================================================================
int16_t sound_playing(void)
{

  return SB_VOCPlaying();
}
