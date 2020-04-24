// THOR - The God of Thunder
// See LICENSE for details

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>

#include "res_man.h"
#include "DEFINE.H"
#include "PROTO.H"
//============================================================================
extern uint8_t *bg_pics;
extern uint8_t objects[NUM_OBJECTS][262];
extern uint8_t *sd_data;
extern uint8_t *tmp_buff;
//extern uint8_t file_str[10];
extern uint8_t res_file[];
extern THOR_INFO thor_info;
extern int16_t current_area;
extern ACTOR *thor;
extern char save_filename[];
extern union REGS in, out;
extern SETUP setup;
extern uint8_t level_type, slow_mode;
extern int16_t boss_active;
extern uint8_t area;
extern uint8_t test_sdf[];
extern uint8_t *song;
extern uint8_t *lzss_buff;
extern char *options_yesno[3];
extern int16_t music_flag, sound_flag, pcsound_flag;
extern uint8_t game_over;
extern uint16_t display_page, draw_page;
extern volatile uint8_t key_flag[100];

char gotres[] = "GOTRES.00";
//===========================================================================
int32_t file_size(char *path)
{
  int32_t tmpl;
  FILE *tmp_fp;

  tmp_fp = fopen(path, "rb");
  if (!tmp_fp)
    return -1;
  fseek(tmp_fp, 0l, SEEK_END);
  tmpl = ftell(tmp_fp);
  fclose(tmp_fp);
  return tmpl;
}
//===========================================================================
int16_t load_bg_data(void)
{

  bg_pics = (uint8_t*)malloc(60460l);
  if (!bg_pics)
    return 0;
  if (GAME1)
    if (res_read("BPICS1", bg_pics) < 0)
      return 0;
  return 1;
}
//===========================================================================
int16_t load_sd_data(void)
{
  char s[21];
  char str[21];

  strcpy(s, "SDAT");
  sprintf(str, "%d", area);
  strcat(s, str);

  if (!sd_data)
    sd_data = (uint8_t*)malloc(61440l);
  if (!sd_data)
    return 0;
  if (res_read(s, sd_data) < 0)
    return 0;
  return 1;
}
//===========================================================================
int16_t load_objects(void)
{

  if (res_read("OBJECTS", (uint8_t *)objects) < 0)
    return 0;
  return 1;
}
//===========================================================================
int16_t load_actor(int16_t file, int16_t num)
{
  char s[21];
  char rs[21];

  sprintf(s, "%d", num);
  strcpy(rs, "ACTOR");
  strcat(rs, s);
  if (res_read(rs, tmp_buff) < 0)
    return 0;
  file = file;
  return 1;
}
//===========================================================================
int16_t load_speech(int16_t index)
{
  int16_t cnt;
  char tmps[30];
  char str[30];
  char *p;
  char *pm;
  char *sp;

  sp = (char*)malloc(30000l);
  if (!sp)
    return 0;

  strcpy(str, "SPEAK");
  sprintf(tmps, "%d", area);
  strcat(str, tmps);
  if (res_read(str, (uint8_t*)sp) < 0)
  {
    free(sp);
    return 0;
  }

  p = sp;

  cnt = 0;
  while (1)
  {
    if (*p == ':')
    {
      p++;
      cnt++;
      strncpy(tmps, p, 4);
      tmps[4] = 0;
      if (atoi(tmps) == index)
      {
        break;
      }
    }
    p++;
    cnt++;
  }
  while (*p != 10)
    p++;
  p++;
  pm = p;
  cnt = 0;
  while (1)
  {
    if (*p == 13)
      *p = 32;
    if (*p == ':')
    {
      if ((*(p + 1) == 'E') && (*(p + 2) == 'N') && (*(p + 3) == 'D'))
        break;
    }
    p++;
    cnt++;
    if (cnt > 5799)
    {
      free(sp);
      return 0;
    }
  }
  if (*(p - 1) == 10)
    *(p - 1) = 0;
  *p = 0;
  memcpy(tmp_buff, pm, cnt);
  tmp_buff[cnt] = 0;
  free(sp);
  return 1;
}
//===========================================================================
void setup_filenames(int16_t level)
{
  //uint8_t s[21];

  //strcpy(res_file,gotres);
  //itoa(level,s,10);
  //strcat(res_file,s);
  level = level;
}
//===========================================================================
void help(void)
{

  odin_speaks(2008, -1);
}
//===========================================================================
void save_game(void)
{
  uint8_t buff[32];

  if (game_over)
    return;
  setup.area = area;
  setup.game_over = game_over;
  if (select_option(options_yesno, "Save Game?", 0) != 1)
  {
    d_restore();
    return;
  }
  d_restore();

  FILE* fp = fopen(save_filename, "rb");
  if (fp == NULL) {
    return;
  }
  fread(buff, 1, 32, fp);
  fclose(fp);

  fp = fopen(save_filename, "wb");
  if (fp == NULL) {
    return;
  }
  fwrite(buff, 1, 32, fp);
  fwrite(&setup, sizeof(SETUP), 1, fp);
  fwrite(&thor_info, sizeof(THOR_INFO), 1, fp);
  fwrite(&sd_data, 1, 61440u, fp);
  fclose(fp);
  odin_speaks(2009, 0);
}
//===========================================================================
int16_t load_game(int16_t flag)
{
  int16_t handle;
  uint16_t total;
  uint8_t buff[32];

  if (flag)
  {
    if (select_option(options_yesno, "Load Game?", 0) != 1)
    {
      d_restore();
      return 0;
    }
    d_restore();
  }
  
  FILE* fp = fopen(save_filename, "rb");
  if (fp == NULL) {
    return 0;
  }

  fread(buff, 1, 32, fp);
  fread(&setup, sizeof(SETUP), 1, fp);
  fread(&thor_info, sizeof(THOR_INFO), 1, fp);
  fread(sd_data, 61440u, 1, fp);
  fclose(fp);

  current_area = thor_info.last_screen;
  area = setup.area;
  if (area == 0)
    area = 1;

  thor->x = (thor_info.last_icon % 20) * 16;
  thor->y = ((thor_info.last_icon / 20) * 16) - 1;
  if (thor->x < 1)
    thor->x = 1;
  if (thor->y < 0)
    thor->y = 0;
  thor->dir = thor_info.last_dir;
  thor->last_dir = thor_info.last_dir;
  thor->health = thor_info.last_health;
  thor->num_moves = 1;
  thor->vunerable = 60;
  thor->show = 60;
  thor->speed_count = 6;
  load_new_thor();
  display_health();
  display_magic();
  display_jewels();
  display_keys();
  display_item();
  if (!music_flag)
    setup.music = 0;
  if (!sound_flag)
    setup.dig_sound = 0;
  if (setup.music == 1)
  {
    if (GAME1 == 1 && current_area == 59)
    {
      if (flag)
        music_play(5, 1);
    }
    else if (flag)
      music_play(level_type, 1);
  }
  else
  {
    setup.music = 1;
    music_pause();
    setup.music = 0;
  }
  game_over = setup.game_over;
  slow_mode = setup.speed;
  return 1;
}
//==========================================================================
int16_t load_music(int16_t num)
{

  switch (num)
  {
  case 0:
    res_read("SONG1", song);
    break;
  case 1:
    res_read("SONG2", song);
    break;
  case 2:
    res_read("SONG3", song);
    break;
  case 3:
    res_read("SONG4", song);
    break;
  case 4:
    res_read("WINSONG", song);
    break;
  case 5:
    res_read("BOSSSONG", song);
    break;
  }
  if (!song)
    return 0;
  return 1;
}
