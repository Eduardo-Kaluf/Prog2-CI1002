#ifndef __PLAYER__ 																											
#define __PLAYER__																											

#include <allegro5/bitmap.h>
#include <allegro5/timer.h>

#include "entity.h"
#include "joystick.h"


struct player {
    struct joystick *joystick;
    struct entity *entity;
    int jumping;
    int stamina;
    int resting;
};

struct player* create_player(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet, int health, int stamina);

void move_player(struct player *element, int steps, enum Directions trajectory, int max_x, int max_y);

void crouch(struct player *element);

int get_player_sprite(struct player *element, ALLEGRO_TIMER *timer);

void update_player_position(struct player *player);


void update_player_status(struct player *player);

void destroy_player(struct player *element);

#endif
