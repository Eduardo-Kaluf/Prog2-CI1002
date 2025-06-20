#ifndef __UTILS__ 																											
#define __UTILS__																											

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
#define PLAYER_SCALE 0.8
#define PLAYER_DISP_W PLAYER_W * PLAYER_SCALE
#define PLAYER_DISP_H PLAYER_H * PLAYER_SCALE

#define FOX_W 60
#define FOX_H 35
#define FOX_SCALE 3.5
#define FOX_DISP_W FOX_W * FOX_SCALE
#define FOX_DISP_H FOX_H * FOX_SCALE

#define BOSS_W 250
#define BOSS_H 350
#define BOSS_SCALE 1
#define BOSS_DISP_W BOSS_W * BOSS_SCALE
#define BOSS_DISP_H BOSS_H * BOSS_SCALE

#define BG_W 7350
#define BG_H 720
#define BG_SCALE 1
#define BG_DISP_W BG_W * BG_SCALE
#define BG_DISP_H BG_H * BG_SCALE


#define PLAYER_STEP 10
#define X_VELOCITY 1
#define Y_VELOCITY 1

enum Directions {
    LEFT,
    RIGHT,
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

#endif
