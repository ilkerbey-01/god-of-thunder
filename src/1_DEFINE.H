//Source code released to the public domain on March 27th, 2020.

#pragma once
#include <stdint.h>

#undef outportb
#undef inportb
//==========================================================================
//typedefs
//Describes one alignment of a mask-image pair

typedef struct
{
       int16_t image_width; //image width in addresses in display memory
       uint16_t image_ptr;  //offset of image bitmap in display mem
       uint8_t *mask_ptr;   //pointer to mask bitmap
} ALIGNED_MASK_IMAGE;

typedef struct
{
       ALIGNED_MASK_IMAGE *alignments[4]; //ptrs to AlignedMaskedImage
                                          //structs for four possible destination
                                          //image alignments
} MASK_IMAGE;

typedef struct
{                               //size=512
       uint8_t icon[12][20];    //0   grid of icons
       uint8_t bg_color;        //240 background color
       uint8_t type;            //241 music
       uint8_t actor_type[16];  //242 type of enemies (12 max)
       uint8_t actor_loc[16];   //254 location of enemies
       uint8_t actor_value[16]; //pass value
       uint8_t pal_colors[3];   //change 251,253,254 to these three
       uint8_t actor_invis[16];
       uint8_t extra[13];
       uint8_t static_obj[30];    //302 static objects (treasure, keys,etc)
       int16_t static_x[30];      //332 X coor of static objects
       int16_t static_y[30];      //392 Y coor of static objects
       uint8_t new_level[10];     //452 level jump for icon 200-204
       uint8_t new_level_loc[10]; //462 grid location to jump in to
       uint8_t area;              //472 game area (1=forest,etc)
       uint8_t actor_dir[16];     //initial dir
       uint8_t future[3];         //473
} LEVEL;

typedef struct
{ //size=256
       //first part loaded from disk    (size=40)
       uint8_t move;              //movement pattern (0=none)
       uint8_t width;             //physical width
       uint8_t height;            //physical height
       uint8_t directions;        //1,2 or 4 (1=uni-directional)
       uint8_t frames;            //# frames per direction
       uint8_t frame_speed;       //# cycles between frame changes
       uint8_t frame_sequence[4]; //sequence
       uint8_t speed;             //move every Nth cycle
       uint8_t size_x;            //non-physical padding on X coor
       uint8_t size_y;            //non-phsyical padding on Y coor
       uint8_t strength;          //hit strength
       uint8_t health;            //
       uint8_t num_moves;         //# of moves every <speed> cycles
       uint8_t shot_type;         //actor # of shot
       uint8_t shot_pattern;      //func number to decide to shoot
       uint8_t shots_allowed;     //# shots allowed on screen
       uint8_t solid;             //1=solid (not ghost,etc)
       uint8_t flying;            //
       uint8_t rating;            //rnd(100) < rating = jewel
       uint8_t type;              //actor (0=thor,1=hammer,2=enemy,3=shot)
       uint8_t name[9];           //actors name
       uint8_t func_num;          //special function when thor touches
       uint8_t func_pass;         //value to pass to func
       int16_t magic_hurts;       //bitwise magic hurts flags
       uint8_t future1[4];

       //the rest is dynamic    //size=216
       MASK_IMAGE pic[4][4]; //mask image pointers
       uint8_t frame_count;  //count to switch frames
       uint8_t dir;          //direction of travel
       uint8_t last_dir;     //last direction of travel
       int16_t x;            //actual X coor
       int16_t y;            //actual Y coor
       int16_t center;       //center of object
       int16_t last_x[2];    //last X coor on each page
       int16_t last_y[2];    //last Y coor on each page
       uint8_t used;         //1=active, 0=not active
       uint8_t next;         //next frame to be shown
       uint8_t speed_count;  //count down to movement
       uint8_t vunerable;    //count down to vunerability
       uint8_t shot_cnt;     //count down to another shot
       uint8_t num_shots;    //# of shots currently on screen
       uint8_t creator;      //which actor # created this actor
       uint8_t pause;        //pause must be 0 to move
       uint8_t actor_num;
       uint8_t move_count;
       uint8_t dead;
       uint8_t toggle;
       uint8_t center_x;
       uint8_t center_y;
       uint8_t show; //display or not (for blinking)
       uint8_t temp1;
       uint8_t temp2;
       uint8_t counter;
       uint8_t move_counter;
       uint8_t edge_counter;
       uint8_t temp3;
       uint8_t temp4;
       uint8_t temp5;
       uint8_t hit_thor;
       int16_t rand;
       uint8_t init_dir;
       uint8_t pass_value;
       uint8_t shot_actor;
       uint8_t magic_hit;
       uint8_t temp6;
       int16_t i1, i2, i3, i4, i5, i6;
       uint8_t init_health;
       uint8_t talk_counter;
       uint8_t etype;
       uint8_t future2[25];
} ACTOR;

typedef struct
{                                 //size=40
       uint8_t move;              //movement pattern (0=none)
       uint8_t width;             //physical width
       uint8_t height;            //physical height
       uint8_t directions;        //1,2 or 4 (1=uni-directional)
       uint8_t frames;            //# frames per direction
       uint8_t frame_speed;       //# cycles between frame changes
       uint8_t frame_sequence[4]; //sequence
       uint8_t speed;             //move every Nth cycle
       uint8_t size_x;            //non-physical padding on X coor
       uint8_t size_y;            //non-phsyical padding on Y coor
       uint8_t strength;          //hit strength
       uint8_t health;            //
       uint8_t num_moves;         //# of moves every <speed> cycles
       uint8_t shot_type;         //actor # of shot
       uint8_t shot_pattern;      //func number to decide to shoot
       uint8_t shots_allowed;     //# shots allowed on screen
       uint8_t solid;             //1=solid (not ghost,etc)
       uint8_t flying;            //
       uint8_t rating;            //rnd(100) < rating = jewel
       uint8_t type;              //actor (0=thor,1=hammer,2=enemy,3=shot)
       uint8_t name[9];           //actors name
       uint8_t func_num;          //special function when thor touches
       uint8_t func_pass;         //value to pass to func
       uint8_t future1[6];
} ACTOR_NFO;

typedef struct
{                            //5200
       uint8_t pic[16][256]; //4096
       uint8_t shot[4][256]; //1024
       ACTOR_NFO actor_info; //40
       ACTOR_NFO shot_info;  //40
} ACTOR_DATA;

typedef struct sup
{
       uint16_t f00 : 1;
       uint16_t f01 : 1;
       uint16_t f02 : 1;
       uint16_t f03 : 1;
       uint16_t f04 : 1;
       uint16_t f05 : 1;
       uint16_t f06 : 1;
       uint16_t f07 : 1;

       uint16_t f08 : 1;
       uint16_t f09 : 1;
       uint16_t f10 : 1;
       uint16_t f11 : 1;
       uint16_t f12 : 1;
       uint16_t f13 : 1;
       uint16_t f14 : 1;
       uint16_t f15 : 1;

       uint16_t f16 : 1;
       uint16_t f17 : 1;
       uint16_t f18 : 1;
       uint16_t f19 : 1;
       uint16_t f20 : 1;
       uint16_t f21 : 1;
       uint16_t f22 : 1;
       uint16_t f23 : 1;

       uint16_t f24 : 1;
       uint16_t f25 : 1;
       uint16_t f26 : 1;
       uint16_t f27 : 1;
       uint16_t f28 : 1;
       uint16_t f29 : 1;
       uint16_t f30 : 1;
       uint16_t f31 : 1;

       uint16_t f32 : 1;
       uint16_t f33 : 1;
       uint16_t f34 : 1;
       uint16_t f35 : 1;
       uint16_t f36 : 1;
       uint16_t f37 : 1;
       uint16_t f38 : 1;
       uint16_t f39 : 1;

       uint16_t f40 : 1;
       uint16_t f41 : 1;
       uint16_t f42 : 1;
       uint16_t f43 : 1;
       uint16_t f44 : 1;
       uint16_t f45 : 1;
       uint16_t f46 : 1;
       uint16_t f47 : 1;

       uint16_t f48 : 1;
       uint16_t f49 : 1;
       uint16_t f50 : 1;
       uint16_t f51 : 1;
       uint16_t f52 : 1;
       uint16_t f53 : 1;
       uint16_t f54 : 1;
       uint16_t f55 : 1;

       uint16_t f56 : 1;
       uint16_t f57 : 1;
       uint16_t f58 : 1;
       uint16_t f59 : 1;
       uint16_t f60 : 1;
       uint16_t f61 : 1;
       uint16_t f62 : 1;
       uint16_t f63 : 1;

       uint8_t value[16];
       uint8_t junk;
       uint8_t game;
       uint8_t area;        //1,2,3
       uint8_t pc_sound;    //1=enabled
       uint8_t dig_sound;   //1 & !pc_sound = enabled
       uint8_t music;       //1=enabled
       uint8_t speed;       //1=slow mode (for slower 286's)
       uint8_t scroll_flag; //unused
       uint8_t boss_dead[3];
       uint8_t skill; //0=easy, 1=normal, 2=hard
       uint8_t game_over;
       uint8_t future[19]; //probably not needed
} SETUP;

typedef struct
{
       uint8_t width;
       uint8_t height;
} PIC_HEADER;

typedef struct
{
       uint8_t magic;
       uint8_t keys;
       int16_t jewels;
       uint8_t last_area;
       uint8_t last_screen;
       uint8_t last_icon;
       uint8_t last_dir;
       int16_t inventory;
       uint8_t item; //currently selected item
       uint8_t last_health;
       uint8_t last_magic;
       int16_t last_jewels;
       uint8_t last_keys;
       uint8_t last_item;
       int16_t last_inventory;
       uint8_t level; //current level (1,2,3)
       int32_t score;
       int32_t last_score;
       uint8_t object;
       uint8_t *object_name;
       uint8_t last_object;
       uint8_t *last_object_name;
       uint8_t armor;
       uint8_t future[65];
} THOR_INFO;

typedef struct
{
       int32_t offset;
       int32_t length;
} HEADER;
//==========================================================================
//defines

#define PAGES 0u
//#define PAGE0 3840u
//#define PAGE1 19200u
//#define PAGE2 34560u
//#define PAGE3 49920u

#define PAGE0 3840u
#define PAGE1 19280u
#define PAGE2 34720u
#define PAGE3 50160u

#define X_MAX 319
#define Y_MAX 191
#define MO_BUFF 56688u
#define MO_OFFSET 55968u
#define ENEMY_OFFSET 59664u
#define ENEMY_SHOT_OFFSET 64272u
#define MAX_ACTORS 35
#define MAX_ENEMIES 16
#define MAX_SHOTS 16
#define STAMINA 20

#define THOR 0
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define HOME 71
#define PGUP 73
#define END 79
#define PGDN 81
#define ESC 1
#define SPACE 57
#define ENTER 28
#define ALT 56
#define CTRL 29
#define TAB 15
#define LSHIFT 42
#define _Z 44
#define _ONE 2
#define _TWO 3
#define _THREE 4
#define _FOUR 5
#define _S 31
#define _L 38
#define _K 37
#define _D 32
#define _B 48
#define _F1 59
#define AMI_LEN 1800
#define TMP_SIZE 5800

#define sc_Index 0x3C4
enum
{
       sc_Reset,
       sc_Clock,
       sc_MapMask,
       sc_uint8_tMap,
       sc_MemMode
};

#define crtc_Index 0x3D4

enum
{
       crtc_H_Total,
       crtc_H_DispEnd,
       crtc_H_Blank,
       crtc_H_EndBlank,
       crtc_H_Retrace,
       crtc_H_EndRetrace,
       crtc_V_Total,
       crtc_OverFlow,
       crtc_RowScan,
       crtc_MaxScanLine,
       crtc_CursorStart,
       crtc_CursorEnd,
       crtc_StartHigh,
       crtc_StartLow,
       crtc_CursorHigh,
       crtc_CursorLow,
       crtc_V_Retrace,
       crtc_V_EndRetrace,
       crtc_V_DispEnd,
       crtc_Offset,
       crtc_Underline,
       crtc_V_Blank,
       crtc_V_EndBlank,
       crtc_Mode,
       crtc_LineCompare
};

#define gc_Index 0x3CE
enum
{
       gc_SetReset,
       gc_EnableSetReset,
       gc_ColorCompare,
       gc_DataRotate,
       gc_ReadMap,
       gc_Mode,
       gc_Misc,
       gc_ColorDontCare,
       gc_BitMask
};

#define atr_Index 0x3c0
enum
{
       atr_Mode = 16,
       atr_Overscan,
       atr_ColorPlaneEnable,
       atr_PelPan,
       atr_ColorSelect
};
#define status_Reg1 0x3da

enum
{
       OW,
       GULP,
       SWISH,
       YAH,
       ELECTRIC,
       THUNDER,
       DOOR,
       FALL,
       ANGEL,
       WOOP,
       DEAD,
       BRAAPP,
       WIND,
       PUNCH1,
       CLANG,
       EXPLODE,
       BOSS11,
       BOSS12,
       BOSS13,
};

#define status_Reg1 0x3da

#define GAME1 (area == 1)
#define GAME2 (area == 2)
#define GAME3 (area == 3)
#define BP (key_flag[_B])

#define NUM_SOUNDS 19
#define NUM_OBJECTS 32

#define APPLE_MAGIC 1
#define HOURGLASS_MAGIC 2
#define LIGHTNING_MAGIC 4

#define BOOTS_MAGIC 8
#define BOMB_MAGIC 16
#define WIND_MAGIC 32

#define QUESTION_MAGIC 64
#define SHIELD_MAGIC 128
#define THUNDER_MAGIC 256

#define BOSS_LEVEL1 59
#define BOSS_LEVEL21 200
#define BOSS_LEVEL22 118

//#define DEBUG 1
//#define IDE 1
//#define CRIPPLED 1
#define DEMO_LEN 3600
