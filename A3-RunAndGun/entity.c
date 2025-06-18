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

void destroy_entity(struct entity *element) {
    free(element);
}

void move_background(struct entity *bg, struct player *player) {
    if (player->entity->x + player->entity->width / 2 + PLAYER_STEP >= DISP_W && bg->x + DISP_W < bg->width)
        bg->x += 2*PLAYER_STEP;
    if (player->entity->x - player->entity->width / 2 - PLAYER_STEP <= 0 && bg->x > PLAYER_STEP)
        bg->x -= 2*PLAYER_STEP;
}
