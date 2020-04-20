// THOR - The God of Thunder
//Source code released to the public domain on March 27th, 2020.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>

#include "1_DEFINE.H"
#include "1_PROTO.H"
#include "utility.h"
#include "g_asm.h"
//#include "mu_man.h"

//========================= Global Declarations ==============================
uint16_t page[3] = {PAGE0, PAGE1, PAGE2};
uint16_t display_page, draw_page;
uint16_t page3_offset;
uint8_t pge;
int16_t exit_flag;

volatile uint8_t key_flag[100];
volatile uint8_t joy_flag[100];
volatile uint8_t tmp_flag[100];
uint8_t break_code;
uint8_t scan_code, last_scan_code;
uint8_t diag;
uint8_t slow_mode, startup;
uint8_t shot_ok;
int16_t thor_x1, thor_y1, thor_x2, thor_y2, thor_real_y1;
int16_t thor_pos;
int16_t max_shot;

extern volatile uint16_t timer_cnt, vbl_cnt, magic_cnt, extra_cnt;

uint8_t text[94][72];
uint8_t *bg_pics;
//uint8_t bg_header[200];
uint8_t objects[NUM_OBJECTS][262];
int16_t ox, oy, of;
uint8_t object_map[240];
uint8_t object_index[240];
uint8_t *bleep;
uint8_t thor_icon1, thor_icon2, thor_icon3, thor_icon4;
uint8_t level_type;
uint8_t *song;
uint8_t music_current;
uint8_t boss_loaded;
uint8_t apple_drop;
uint8_t cheat;
uint8_t area;
uint8_t last_setup[32];

LEVEL scrn;
uint8_t *scrnp;

uint8_t *sd_data;
int16_t current_level, new_level, new_level_tile, current_area;
//uint8_t file_str[10];

SETUP setup;
uint8_t *tmp_buff;
int16_t reps;

uint8_t *mask_buff;
uint8_t *mask_buff_start;
uint8_t abuff[AMI_LEN];
uint8_t *ami_buff;
ACTOR actor[MAX_ACTORS];  //current actors
ACTOR enemy[MAX_ENEMIES]; //current enemies
ACTOR shot[MAX_ENEMIES];  //current shots
uint8_t enemy_type[MAX_ENEMIES];

ACTOR magic_item[2];
uint8_t magic_pic[2][1024];

//ACTOR enemy[4];   //4 different available actors
//uint8_t enemy_used[4];
//uint8_t enemy_type[4];
//uint8_t enemy_complex[4];
//uint16_t enemy_buff[4];
//int16_t enemy_mask_offset[4];

//ACTOR shot[4];    //4 shots
//uint16_t shot_buff[4];
//int16_t shot_mask_offset[4];
uint8_t warp_scroll;

//uint8_t *enemy_data;     //points to four actors/sounds

ACTOR *thor;
ACTOR *hammer;
ACTOR explosion;
ACTOR sparkle;
THOR_INFO thor_info;
int16_t key_fire, key_up, key_down, key_left, key_right, key_magic, key_select;
int16_t boss_dead;

int16_t warp_flag;

uint8_t *std_sound_start;
uint8_t *pcstd_sound_start;
uint8_t *std_sound;
uint8_t *pcstd_sounds;
uint8_t *boss_sound[3];
uint8_t *boss_pcsound[3];
int32_t pcsound_length[NUM_SOUNDS];
int16_t rand1, rand2;
int16_t restore_screen;
int16_t last_oracle;
int16_t hourglass_flag, thunder_flag, shield_on, lightning_used, tornado_used;
int16_t apple_flag, bomb_flag;
int16_t switch_flag;
uint16_t joy_x, joy_y;
uint8_t joy_b1, joy_b2;
int16_t joystick, joylx, joyly, joyhx, joyhy;
char res_file[16];
uint8_t odin[4][262];
uint8_t hampic[4][262];
int16_t load_game_flag;
extern int16_t music_flag, sound_flag, pcsound_flag;
int16_t cash1_inform, cash2_inform, door_inform, magic_inform, carry_inform;
int16_t killgg_inform;
uint8_t dialog_color[] = {14, 54, 120, 138, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//norm,good,bad,sign,white

uint8_t *std_sounds;
uint8_t *pc_sound[NUM_SOUNDS];
uint8_t *dig_sound[NUM_SOUNDS];
int16_t boss_active;
uint8_t story_flag;
char save_filename[] = "XXXXXXXX.XXX";
uint8_t *scr;
uint8_t demo_key[DEMO_LEN];
int16_t demo_cnt;
uint8_t demo, record;
uint8_t demo_enable;
int16_t rnd_index;
int16_t rnd_array[100];
uint8_t rdemo;
uint8_t test_sdf[80];
char *options_yesno[3] = {"Yes", "No", NULL};
uint8_t *lzss_buff;
uint8_t game_over;
extern uint8_t noal, nosb;
uint8_t ret;
char tempstr[80];
uint8_t auto_load;
uint8_t ide_run, fast_exit, nojoy, gr, xdos;
uint8_t main_loop;
uint8_t end_tile;

void (*old_timer_int)(void); //interrupt function pointer
void timer_int(void);
void thor_spins(int16_t flag);
void setup_load(void);

#ifndef _WIN32
void strupr(char *s)
{
  while (*s)
  {
    *s = toupper((unsigned char)*s);
    s++;
  }
}
#endif

//============================================================================
int main(int argc, char *argv[])
{
  int16_t err, i;
  char s[21];
  int16_t vbl_flag;
  int16_t ma, rp;
  int16_t po[4] = {-1, 1, -80, 80};
  int16_t loop, vl;
  int16_t opt;
  FILE *fp;

  //chdir("\\gottest");
  //for(loop=0;loop<argc;loop++) printf("%s\r\n",argv[loop]);
  //getch();
  startup = 1;
  hourglass_flag = 0;
  thunder_flag = 0;
  lightning_used = 0;
  tornado_used = 0;
  switch_flag = 0;
  boss_dead = 0;
  load_game_flag = 0;
  warp_scroll = 0;
  boss_loaded = 0;
  demo_enable = 0;
  game_over = 0;

  music_flag = 1;
  sound_flag = 1;
  pcsound_flag = 1;

  slow_mode = 0;

  cash1_inform = 0;
  cash2_inform = 0;
  door_inform = 0;
  magic_inform = 0;
  carry_inform = 0;
  killgg_inform = 0;
  story_flag = 1;
  opt = 0;

  current_level = 23;
  area = 1;
  cheat = 0;
  demo = 0;
  rdemo = 0;
  record = 0;
  noal = 0;
  nosb = 0;
  ide_run = 0;
  fast_exit = 0;
  nojoy = 0;
  gr = 0;
  joystick = 0;
  xdos = 0;
  main_loop = 0;
  end_tile = 0;

  //ultoa(coreleft(),ts,10);
  //printf("\r\n%s",ts);
  //getch();

  if (argc > 1)
  { //parse the command line
    for (loop = 0; loop < argc; loop++)
    {
      strupr(argv[loop]);
      if (argv[loop][0] == '/' || argv[loop][0] == '\\')
      {
        strcpy(&argv[loop][0], &argv[loop][1]);
        if (strstr(argv[loop], "SAVEGAME:"))
        {
          strcpy(save_filename, (strchr(argv[loop], ':') + 1));
        }
        else if (strstr(argv[loop], "JOY:"))
        {
          strcpy(tempstr, strchr(argv[loop], ':'));
          set_joy();
        }
        else if (!strcmp(argv[loop], "VOLSTAGG"))
          cheat = 1;
        else if (!strcmp(argv[loop], "NOJOY"))
          nojoy = 1;
        else if (!strcmp(argv[loop], "GR"))
          gr = 1;
        else if (!strcmp(argv[loop], "DEMO"))
        {
          demo = 1;
          cheat = 1;
        }
        else if (!strcmp(argv[loop], "NOAL"))
        {
          noal = 1;
          music_flag = 0;
          sound_flag = 0;
        }
        else if (!strcmp(argv[loop], "NOSB"))
        {
          nosb = 1;
          sound_flag = 0;
          pcsound_flag = 1;
        }
        else if (!strcmp(argv[loop], "RDEMO"))
        {
          rdemo = 1;
          demo = 1;
          cheat = 1;
        }
        else if (!strcmp(argv[loop], "IDE"))
          ide_run = 1;
        else if (!strcmp(argv[loop], "RECORD"))
        {
          record = 1;
          cheat = 1;
        }
        else if (argv[loop][0] == 'S')
        {
          argv[loop][0] = ' ';
          current_level = atoi(argv[loop]);
        }
      }
    }
  }
  if (!strstr(save_filename, "SAV") && !demo)
  {
    ret = 0;
    run_gotm();
  }
  ret = 1;
  auto_load = 0;
  if (file_size(save_filename) > 32)
  {
    auto_load = 1;
    story_flag = 0;
  }

  setup.area = area;

  if (demo || record)
  {
    if (record)
      demo = 0;
    area = 1;
    setup.area = 1;
    cash1_inform = 1;
    cash2_inform = 1;
    door_inform = 1;
    magic_inform = 1;
    carry_inform = 1;
    story_flag = 0;
  }

  if (current_level != 23)
    story_flag = 0;

  err = initialize();
  if (err)
    exit_code(err);

  if (rdemo)
  {
    fp = fopen("demo.got", "rb");
    if (fp)
    {
      fread(&demo_key[0], 1, DEMO_LEN, fp);
      fclose(fp);
    }
  }

#ifdef DEBUG
  ultoa(coreleft(), ts, 10);
  xprint(272, 40, ts, PAGES, 14);
#endif

  warp_flag = 0;
  display_page = PAGE0;
  draw_page = PAGE1;
  xshowpage(PAGE0);

  if (record)
    memset(demo_key, 0, DEMO_LEN);
  if (demo || record)
  {
    thor->health = 100;
    thor_info.magic = 100;
    thor_info.jewels = 463;
    thor_info.score = 12455;
    setup.skill = 0;
    thor_info.inventory = 1 + 2;
    current_level = 54;
    thor_info.item = 2;
    display_item();
    display_health();
    display_magic();
    display_score();
  }
  new_level = current_level;
  // TODO
  // movedata(FP_SEG(sd_data + (new_level * 512)), FP_OFF(sd_data + (new_level * 512)),
  //          FP_SEG(&scrn), FP_OFF(&scrn), sizeof(LEVEL));
  show_level(current_level);
  exit_flag = 0;
  pge = 0;

  timer_cnt = 0;
  vbl_cnt = 0;
  vl = 0;

  thor->speed_count = 6;

  demo_cnt = 0;
  demo_enable = 1;
  rnd_index = 0;
  music_current = -1;

  if (auto_load)
  {
    if (load_game(0))
    {
      setup_load();
    }
    auto_load = 0;
    if (GAME1 && current_area == 59 && !setup.game_over)
    {
      auto_load = 1;
      fade_in();
      boss_level1();
    }
  }
  else if (!cheat)
    key_flag[ESC] = 1;
  if (!auto_load)
  {
    fade_in();
    music_play(level_type, 1);
  }
  auto_load = 0;

  startup = 0;
  if (record)
    memset(demo_key, 0, DEMO_LEN);

  //------------------------------------------------------------------------
  //ultoa(farcoreleft(),s,10);
  //xprint(0,0,s,PAGES,14);
  //ultoa(coreleft(),s,10);
  //xprint(0,10,s,PAGES,14);

  srand(1234);
  main_loop = 1;
  while (1)
  {
    if (demo || record)
      demo_key_set();
    demo_cnt++;
    if (record)
    {
      //    if(demo_cnt<DEMO_LEN){
      //      if(!demo_key[demo_cnt]) demo_key[demo_cnt]=scan_code;
      //      else demo_key[demo_cnt+1]=scan_code;
      //    }
      //    if(cheat){
      sprintf(s, "%d", demo_cnt);
      xfillrectangle(0, 40, 296 + 24, 40 + 8, PAGES, 0);
      xprint(0, 39, s, PAGES, 14);
      //    }
    }
    rand1 = rnd(100);
    rand2 = rnd(100);
    display_page = page[pge];
    pge = pge ^ 1;
    pge = pge;
    draw_page = page[pge];

    if (cheat)
    {
      if (key_flag[_THREE])
      {
        // asm("mov dx, status_Reg1");
        // asm("in al, dx");
        // asm("asm mov dx, atr_Index");
        // asm("mov al, atr_Overscan");
        // asm("out dx, al");
        // asm("mov al, 10"); // green
        // asm("out dx, al");
        // asm("mov al, 20h"); // normal
        // asm("out dx, al");
        vbl_flag = 1;
      }
      else
        vbl_flag = 0;
    }

    if (thunder_flag)
    {
      rp = rand1 / 25;
      xshowpage(display_page + po[rp]);
      thunder_flag--;
      if ((thunder_flag < MAX_ACTORS) && thunder_flag > 2)
        if (actor[thunder_flag].used)
        {
          actor[thunder_flag].vunerable = 0;
          actor_damaged(&actor[thunder_flag], 20);
        }
    }
    else
      xshowpage(display_page);

    if (cheat)
    {
      if (vbl_flag)
      {
        // asm("mov dx, status_Reg1");
        // asm("in al, dx");
        // asm("mov dx, atr_Index");
        // asm("mov al, atr_Overscan");
        // asm("out dx, al");
        // asm("mov al, 0"); // black
        // asm("out dx, al");
        // asm("mov al, 20h"); // normal
        // asm("out dx, al");
      }
    }
    if (restore_screen)
    {
      xcopyd2d(0, 0, 320, 192, 0, 0, PAGE2, draw_page, 320, 320);
      restore_screen = 0;
    }
    else
      xerase_actors(actor, draw_page);
    if (of)
    { //replace tile after object is picked up
      xcopyd2d(ox, oy, ox + 16, oy + 16, ox, oy, PAGE2, draw_page, 320, 320);
      of = 0;
    }
    if (exit_flag == 2)
    {
      thor_dies();
      exit_flag = 0;
    }
    if (switch_flag)
    {
      switch (switch_flag)
      {
      case 1:
      {
        switch_icons();
        break;
      }
      case 2:
      {
        rotate_arrows();
        break;
      }
      }
      switch_flag = 0;
    }
    thor_pos = ((thor->x + 7) / 16) + (((thor->y + 8) / 16) * 20);
    if (object_map[thor_pos])
      pick_up_object(thor_pos);
    shot_ok = 1;
    ma = MAX_ACTORS;
    if (slow_mode)
    {
      while (vbl_cnt < 4)
        ;
      vbl_cnt = 0;
      vl = 2;
    }
    else
      vl = 1;
    if (joystick)
    {
      joy_key();
      merge_keys();
    }
    if (cheat)
    {
      if (key_flag[_K])
        screen_dump();
      if (key_flag[_TWO])
        delay(2500);
      if (key_flag[_Z])
        add_magic(150);
      if (key_flag[45])
      {
        fast_exit = 1;
        break;
      } //X to exit fast
    }
    if (key_flag[_S])
      save_game();
    else if (key_flag[_L])
    {
      if (load_game(1))
        setup_load();
    }
    if (opt)
    {
      if (opt == 1)
      {
        if (select_sound())
        {
          d_restore();
          select_music();
        }
      }
      else if (opt == 2)
        select_skill();
      else if (opt == 3)
        save_game();
      else if (opt == 4)
      {
        if (load_game(1))
          setup_load();
      }
      else if (opt == 5)
      {
        if (!game_over)
          thor_dies();
      }
      else if (opt == 6)
        select_fastmode();
      else if (opt == 7)
        help();
      else if (opt == 8)
      {
        xdos = ask_exit();
        if (xdos == 2 || xdos == 3)
          break;
      }
      opt = 0;
    }
    if (!boss_dead)
    {
      if (key_flag[_F1])
        if (!thunder_flag)
          help();
      if (key_flag[_D])
        if (!thunder_flag && !game_over)
          thor_dies();
      if (key_flag[ESC])
        if (!thunder_flag)
        {
          key_flag[ESC] = 0;
          opt = option_menu();
        }
    }
    for (loop = 0; loop < vl; loop++)
    {
      for (i = 0; i < ma; i++)
      {
        if (actor[i].used)
        {
          if (hourglass_flag)
            if ((i > 2) && (!pge) && (!(actor[i].magic_hurts & HOURGLASS_MAGIC)))
              continue;
          actor[i].move_count = actor[i].num_moves;
          while (actor[i].move_count--)
            move_actor(&actor[i]);
          if (i == 0)
            set_thor_vars();
          if (new_level != current_level)
            goto brk_loop;
        }
      }
    }
    if (end_tile)
      break;
  brk_loop:
    if (exit_flag == 2)
    {
      thor_dies();
      exit_flag = 0;
    }
    thor->center_x = thor_pos % 20;
    thor->center_y = thor_pos / 20;
    //  save_d=thor->dir;
    //  if(scrn.icon[thor->center_y][thor->center_x]==154) thor->dir=0;
    xdisplay_actors(&actor[MAX_ACTORS - 1], draw_page);
    //  thor->dir=save_d;
    if (current_level != new_level)
    {
      i = level_type;
      thor->show = 0;
      hammer->used = 0;
      show_level(new_level);
      tornado_used = 0;
      if (cheat && !demo)
      {
        xfillrectangle(296, 0, 296 + 24, 10, PAGES, 0);
        sprintf(s, "%d", current_level);
        xprint(296, 0, s, PAGES, 14);
      }
    }
    use_item();
    if (boss_dead)
    {
      for (loop = 3; loop < 7; loop++)
        if (actor[loop].used)
          break;
      if (loop == 7)
      {
        xerase_actors(actor, display_page);
        xdisplay_actors(&actor[MAX_ACTORS - 1], display_page);
        exit_flag = 0;
        if (boss_active == 1)
        {
          closing_sequence1(); //serpent
          boss_active = 0;
          //        exit_flag=1;
        }
        if (exit_flag)
          break;
      }
    }
    if (demo || record)
    {
      if (demo_cnt >= (DEMO_LEN - 1))
        break;
      if (exit_flag == 5)
        break;
    }
  }
  //chdir("\\bc");
  if (record)
  {
    fp = fopen("demo.got", "wb");
    if (fp)
    {
      fwrite(&demo_key[0], 1, DEMO_LEN, fp);
      fclose(fp);
    }
  }
  if (ret && !fast_exit && !demo && !record)
  {
    d_restore();
    save_game();
  }
  music_pause();
  fade_out();
  exit_code(0);
  //if(xdos==3 && cheat) exit(0);
  if ((!cheat) || demo)
  {
    // TODO
    // _AX = 0x0100;
    // _CX = 0x2000;
    // geninterrupt(0x10);
  }
  run_gotm();
  exit(0);
  return 0;
}
extern int16_t SBResetCount;
//===========================================================================
void run_gotm(void)
{
  int16_t num;
  char s[36];
  char *args[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                  NULL, NULL, NULL, NULL, NULL};

  args[0] = "GOT.EXE";

  num = 1;
  if (xdos == 3)
  {
    args[num] = "/EXIT";
    num++;
  }
  if (ret)
  {
    args[num] = "/RETURN";
    num++;
  }
  if (demo && exit_flag == 5)
  {
    args[num] = "/KEYRETURN";
    num++;
  }
  if (noal)
  {
    args[num] = "/NOAL";
    num++;
  }
  if (gr)
  {
    args[num] = "/GR";
    num++;
  }
  if (nojoy)
  {
    args[num] = "/NOJOY";
    num++;
  }
  if (nosb)
  {
    args[num] = "/NOSB";
    num++;
  }
  if (cheat && !demo)
  {
    args[num] = "/VOLSTAGG";
    num++;
  }
  if (game_over)
  {
    sprintf(s, "%d", thor_info.score);
    strcpy(res_file, "/SCORE:");
    strcat(res_file, s);
    args[num] = res_file;
    num++;
    strcpy(s, "/SAVEGAME:");
    strcat(s, save_filename);
    args[num] = s;
    num++;
  }
  //num=0;
  //while(args[num]){
  //  printf("%s\r\n",args[num]);
  //  num++;
  //}
  //printf("%d\r\n",SBResetCount);
  //getch();

  // TODO
  //if (!ide_run)
  //  execv("GOT.EXE", args);
  exit(0);
}
//===========================================================================
void printt(int16_t val)
{
  char s[21];

  sprintf(s, "%d", val);
  xfillrectangle(200, 16, 224, 40, PAGES, 20);
  xprint(200, 16, s, PAGES, 12);
}
//===========================================================================
void thor_dies(void)
{
  int16_t li, ln;

  for (li = 0; li < MAX_ACTORS; li++)
    actor[li].show = 0;

  xdisplay_actors(&actor[MAX_ACTORS - 1], draw_page);

  li = thor_info.item;
  ln = thor_info.inventory;
  play_sound(DEAD, 1);
  thor_spins(0);
  xfput(thor->x, thor->y, display_page, objects[10]);
  thor->used = 1;
  timer_cnt = 0;
  while (timer_cnt < 60)
    rotate_pal();
  new_level = thor_info.last_screen;
  thor->x = (thor_info.last_icon % 20) * 16;
  thor->y = ((thor_info.last_icon / 20) * 16) - 1;
  if (thor->x < 1)
    thor->x = 1;
  if (thor->y < 0)
    thor->y = 0;
  thor->last_x[0] = thor->x;
  thor->last_x[1] = thor->x;
  thor->last_y[0] = thor->y;
  thor->last_y[1] = thor->y;
  thor->dir = thor_info.last_dir;
  thor->last_dir = thor_info.last_dir;
  thor->health = thor_info.last_health;
  thor_info.magic = thor_info.last_magic;
  thor_info.jewels = thor_info.last_jewels;
  thor_info.keys = thor_info.last_keys;
  thor_info.score = thor_info.last_score;
  thor_info.object = thor_info.last_object;
  thor_info.object_name = thor_info.last_object_name;
  if (ln == thor_info.last_inventory)
  {
    thor_info.item = li;
  }
  else
  {
    thor_info.item = thor_info.last_item;
    thor_info.inventory = thor_info.last_inventory;
  }
  memcpy(&setup, last_setup, 32);
  thor->num_moves = 1;
  thor->vunerable = 60;
  thor->show = 60;
  hourglass_flag = 0;
  apple_flag = 0;
  bomb_flag = 0;
  thunder_flag = 0;
  lightning_used = 0;
  tornado_used = 0;
  shield_on = 0;
  music_resume();
  actor[1].used = 0;
  actor[2].used = 0;
  thor->speed_count = 6;
  // TODO
  // movedata(FP_SEG(sd_data + (new_level * 512)), FP_OFF(sd_data + (new_level * 512)),
  //          FP_SEG(&scrn), FP_OFF(&scrn), sizeof(LEVEL));
  display_health();
  display_magic();
  display_jewels();
  display_keys();
  display_score();
  display_item();
  show_level(new_level);
  set_thor_vars();
}
//===========================================================================
void thor_spins(int16_t flag)
{
  int16_t i, d, c;
  uint16_t dr, di, sw;
  uint8_t da[] = {0, 2, 1, 3};

  if (of)
  { //replace tile after object is picked up
    xcopyd2d(ox, oy, ox + 16, oy + 16, ox, oy, PAGE2, display_page, 320, 320);
    of = 0;
  }
  actor[2].used = 0;
  dr = draw_page;
  di = display_page;
  d = 0;
  c = 0;
  thor->next = 0;
  for (i = 0; i < 60; i++)
  {
    thor->dir = da[d];
    thor->last_dir = da[d];
    c++;
    if (c > 4)
    {
      d++;
      d = d % 4;
      c = 0;
    }
    xerase_actors(actor, dr);
    if (shield_on)
      actor[2].used = 0;
    if (i == 59)
      thor->used = 0;
    xdisplay_actors(&actor[MAX_ACTORS - 1], dr);
    if (shield_on)
      actor[2].used = 1;
    xshowpage(dr);
    sw = dr;
    dr = di;
    di = sw;
  }
  flag = flag;
}
//===========================================================================
void setup_load(void)
{

  thor->used = 1;
  new_level = thor_info.last_screen;
  thor->x = (thor_info.last_icon % 20) * 16;
  thor->y = ((thor_info.last_icon / 20) * 16) - 1;
  if (thor->x < 1)
    thor->x = 1;
  if (thor->y < 0)
    thor->y = 0;
  thor->last_x[0] = thor->x;
  thor->last_x[1] = thor->x;
  thor->last_y[0] = thor->y;
  thor->last_y[1] = thor->y;
  thor->dir = thor_info.last_dir;
  thor->last_dir = thor_info.last_dir;
  thor->health = thor_info.last_health;
  thor_info.magic = thor_info.last_magic;
  thor_info.jewels = thor_info.last_jewels;
  thor_info.keys = thor_info.last_keys;
  thor_info.score = thor_info.last_score;
  thor_info.item = thor_info.last_item;
  thor_info.inventory = thor_info.last_inventory;
  thor_info.object = thor_info.last_object;
  thor_info.object_name = thor_info.last_object_name;
  thor->num_moves = 1;
  thor->vunerable = 60;
  thor->show = 60;
  hourglass_flag = 0;
  apple_flag = 0;
  bomb_flag = 0;
  thunder_flag = 0;
  lightning_used = 0;
  tornado_used = 0;
  shield_on = 0;
  actor[1].used = 0;
  actor[2].used = 0;
  thor->speed_count = 6;
  // TODO
  // movedata(FP_SEG(sd_data + (new_level * 512)), FP_OFF(sd_data + (new_level * 512)),
  //          FP_SEG(&scrn), FP_OFF(&scrn), sizeof(LEVEL));
  display_health();
  display_magic();
  display_jewels();
  display_keys();
  display_score();
  display_item();
  current_level = new_level;
  show_level(new_level);
}
//===========================================================================
void pause(int16_t delay)
{

  timer_cnt = 0;
  while (timer_cnt < delay)
    rotate_pal();
}
//===========================================================================
void rotate_pal(void)
{

  if (slow_mode)
  {
    while (vbl_cnt < 4)
      ;
    vbl_cnt = 0;
  }
  xshowpage(display_page);
}
//===========================================================================
int16_t rnd(int16_t max)
{
  int16_t r;

  if (demo || rdemo || record)
  {
    r = rnd_array[rnd_index] % max;
    rnd_index++;
    if (rnd_index > 99)
      rnd_index = 0;
    return r;
  }
  return rand() % max;
}
