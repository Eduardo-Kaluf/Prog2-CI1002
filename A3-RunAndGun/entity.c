#include "entity.h"
#include "player.h"
#include "utils.h"


struct entity *create_entity(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet) {
    struct entity *entity = malloc(sizeof(struct entity));
    entity->spritesheet = spritesheet;
    entity->width = width;
    entity->height = height;
    entity->dx = dx;
    entity->dy = dy;
    entity->x = x;
    entity->y = y;

    return entity;
}

void move_background(struct entity *bg, struct player *main_player) {
    if (main_player->entity->x + main_player->entity->width / 2 + PLAYER_STEP >= DISP_W && bg->x + DISP_W < bg->width)
        bg->x += 2*PLAYER_STEP;
    if (main_player->entity->x - main_player->entity->width / 2 - PLAYER_STEP <= 0 && bg->x > PLAYER_STEP)
        bg->x -= 2*PLAYER_STEP;
}

void destroy_entity(struct entity *element) {
    free(element);
}
