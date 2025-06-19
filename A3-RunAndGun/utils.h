#ifndef __UTILS__ 																											
#define __UTILS__																											

#define BUFFER_W 320
#define BUFFER_H 240
#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)
#define GROUND (DISP_H - 30)

#define SPRITE_W 150
#define SPRITE_H 180

#define FOX_W 60
#define FOX_H 35

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

extern enum Directions movement_1[10];

#endif
