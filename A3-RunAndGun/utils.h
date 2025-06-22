#ifndef __UTILS__ 																											
#define __UTILS__																											

#include "entity.h"
#include <stdbool.h>

#define BUFFER_W 320
#define BUFFER_H 240
#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)
#define DISP_CENTER_W (float) DISP_W / 2
#define DISP_CENTER_H (float) DISP_H / 2

#define GROUND (DISP_H - 92)

#define PLAYER_W 150
#define PLAYER_H 180
#define PLAYER_SCALE 1
#define PLAYER_DISP_W PLAYER_W * PLAYER_SCALE
#define PLAYER_DISP_H PLAYER_H * PLAYER_SCALE
#define PLAYER_OFFSET_W 20 * PLAYER_SCALE
#define PLAYER_OFFSET_H 0 * PLAYER_SCALE
#define PLAYER_BODY_OFFSET_W 30 * PLAYER_SCALE
#define PLAYER_STAMINA 100
#define PLAYER_STAMINA_FACTOR 5
#define PLAYER_STEP 10
#define X_VELOCITY 1
#define Y_VELOCITY 1

#define STAMINA_BAR_X 200
#define STAMINA_BAR_Y 20

#define FOX_W 60
#define FOX_H 35
#define FOX_SCALE 3.5
#define FOX_DISP_W FOX_W * FOX_SCALE
#define FOX_DISP_H FOX_H * FOX_SCALE
#define FOX_OFFSET_W 20 * FOX_SCALE
#define FOX_OFFSET_H 0 * FOX_SCALE

#define BOSS_W 250
#define BOSS_H 350
#define BOSS_SCALE 1
#define BOSS_DISP_W BOSS_W * BOSS_SCALE
#define BOSS_DISP_H BOSS_H * BOSS_SCALE
#define BOSS_OFFSET_W 40 * BOSS_SCALE
#define BOSS_OFFSET_H 180 * BOSS_SCALE
#define BOSS_BODY_OFFSET_W 50 * BOSS_SCALE
#define BOSS_GROUND_OFFSET 38 * BOSS_SCALE

#define BG_W 7350
#define BG_H 720
#define BG_SCALE 1
#define BG_DISP_W BG_W * BG_SCALE
#define BG_DISP_H BG_H * BG_SCALE

#define HEART_W 100
#define HEART_H 100
#define HEART_SCALE 0.2
#define HEART_DISP_W HEART_W * HEART_SCALE
#define HEART_DISP_H HEART_H * HEART_SCALE

#define MENU_FONT_N 3
#define OPTIONS_FONT_N 4

#define OFFSCREEN_POSITION -1000

enum Directions {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    NONE
};

enum EntityType {
    PLAYER,
    FOX,
    BOSS,
};


extern enum Directions movement_1[10];

extern int key;

extern int fox_offset_w;
extern int fox_offset_h;

extern int player_offset_w;
extern int player_offset_h;

extern int boss_offset_w;
extern int boss_offset_h;
extern int boss_ground_offset;

void must_init(bool test, const char *description);

int between(int lo, int hi);

int end_game(struct player *player, struct entity *boss, int *start);

#endif
