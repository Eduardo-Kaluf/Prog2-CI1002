#ifndef __BOSS__
#define __BOSS__

#include <allegro5/bitmap.h>
#include <allegro5/timer.h>

#include "entity.h"

void update_boss_position(struct entity *boss);

void update_boss_status(struct entity *boss, struct entity **enemies, int *boss_fight, ALLEGRO_TIMER* timer, ALLEGRO_BITMAP* bullet_type);

int get_boss_sprite(struct entity *enemy);

#endif
