#ifndef __PLAYER__ 																											
#define __PLAYER__																											

#include <allegro5/bitmap.h>

#include "entity.h"
#include "joystick.h"

#define PLAYER_STEP 10

struct player {
    struct joystick *joystick;
    struct entity *entity;
    int jumping;
};

struct player* create_player(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet);
void move_player(struct player *element, int steps, int trajectory, int max_x, int max_y);
void destroy_player(struct player *element);
void jump(struct player *element);
void crouch(struct player *element);

#endif
