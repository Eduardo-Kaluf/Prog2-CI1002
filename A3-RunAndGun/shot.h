#ifndef SHOT_H
#define SHOT_H

#include <allegro5/bitmap.h>

#include "player.h"

void shots_init();

char shots_add(struct player *element, struct entity *enemy, int side, char ship, int x, int y);

void shots_update();

int collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

char shots_collide(char ship, int x, int y, int w, int h);

void shots_draw(ALLEGRO_BITMAP* snowBall, ALLEGRO_BITMAP* spike);

#endif
