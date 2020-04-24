//Source code released to the public domain on March 27th, 2020.

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "DEFINE.H"
#include "PROTO.H"
#include "res_man.h"
#include "fx_man.h"
#include "mu_man.h"
#include "sb_man.h"

extern uint8_t *song;
extern uint8_t music_current;
extern struct sup setup;
//=========================================================================
int16_t music_init(void)
{

  if (!setup.music)
    return 0;
  music_current = -1;
  return 1;
}
//=========================================================================
void music_play(int16_t num, int16_t override)
{

  if (!setup.music)
    return;
  if ((num != music_current) || override)
  {
    if (num != music_current)
      load_music(num);
    MU_StartMusic(song, 1);
    music_current = num;
  }
}
//=========================================================================
void music_pause(void)
{

  if (!setup.music)
    return;
  MU_MusicOff();
}
//=========================================================================
void music_resume(void)
{

  if (!setup.music)
    return;
  MU_MusicOn();
}
//=========================================================================
int16_t music_is_on(void)
{

  if (!setup.music)
    return 0;
  return MU_MusicPlaying();
}
