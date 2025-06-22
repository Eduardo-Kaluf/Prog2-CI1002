#ifndef __SHOT__
#define __SHOT__

#include <allegro5/bitmap.h>

#include "player.h"
#include "utils.h"


void shots_init();

char shots_create(enum EntityType type, enum Directions side, ALLEGRO_BITMAP* bullet_type, int x, int y);

void shots_update();

int collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

char shots_collide(enum EntityType shooter, int x, int y, int w, int h);

void handle_shots(struct entity *element, struct player *player, enum EntityType type, ALLEGRO_BITMAP* bullet_type);

void shots_draw();

void shots_move_with_bg(int mult_factor);

#endif
