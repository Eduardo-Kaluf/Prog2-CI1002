#ifndef SHOT_H
#define SHOT_H

#include <allegro5/bitmap.h>

#include "player.h"

void shots_init();

char shots_create(enum EntityType type, enum Directions side, ALLEGRO_BITMAP* bullet_type, int x, int y);

void shots_update();

int collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

char shots_collide(enum EntityType type, int x, int y, int w, int h);

void shots_draw();

#endif
