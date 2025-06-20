#ifndef __PLAYER__ 																											
#define __PLAYER__																											

#include <allegro5/bitmap.h>
#include <allegro5/timer.h>

#include "entity.h"
#include "joystick.h"
#include "utils.h"


struct player {
    struct joystick *joystick;
    struct entity *entity;
    int jumping;
};

struct player* create_player(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet, int health);

void move_player(struct player *element, int steps, enum Directions trajectory, int max_x, int max_y);

void crouch(struct player *element);

int get_player_sprite(struct player *element, ALLEGRO_TIMER *timer);

void destroy_player(struct player *element);

#endif
