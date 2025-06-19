#ifndef SHOT_H
#define SHOT_H

#include <allegro5/bitmap.h>

#include "player.h"

void shots_init();

char shots_add(struct player *element, struct entity *enemy, int side, char ship, int x, int y);

void shots_update();

// char shots_collide(char ship, int x, int y, int w, int h);

void shots_draw(ALLEGRO_BITMAP* snowBall, ALLEGRO_BITMAP* spike);

#endif
