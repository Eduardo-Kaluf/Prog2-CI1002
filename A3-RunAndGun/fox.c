#include "fox.h"

#include <stdlib.h>

#include "shot.h"
#include "utils.h"


void update_fox_position(struct entity *enemy, enum Directions trajectory) {
    if (trajectory == LEFT)
        enemy->x = enemy->x - PLAYER_STEP / 2;
    else if (trajectory == RIGHT)
        enemy->x = enemy->x + PLAYER_STEP / 2;
}


void update_enemy_status(struct entity *enemy, int player_x) {
    int offset_factor = 1;
    if (enemy->side == RIGHT)
        offset_factor = -1;

    int offseted_width = enemy->width - fox_offset_w;
    int offseted_x = enemy->x + offset_factor * (fox_offset_w / 2);

    int offseted_height = enemy->height - fox_offset_h;
    int offseted_y = enemy->y + (fox_offset_h / 2);

    if (shots_collide(FOX, offseted_x - offseted_width / 2, offseted_y - offseted_height / 2, offseted_width, offseted_height))
        enemy->health--;

    if (enemy->health <= 0)
        enemy->x = OFFSCREEN_POSITION;

    if (abs(enemy->x - player_x) <= 300)
        enemy->in_range = 1;
}

