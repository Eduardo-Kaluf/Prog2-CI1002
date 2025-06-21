#ifndef __FOX__
#define __FOX__

#include "entity.h"

void update_fox_position(struct entity *enemy, enum Directions trajectory);

void update_enemy_status(struct entity *enemy, int player_x);


#endif
