#include "entity.h"
#include "player.h"
#include "utils.h"


struct entity *create_entity(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet, int health, int cooldown) {

    struct entity *entity = malloc(sizeof(struct entity));
    entity->spritesheet = spritesheet;
    entity->width = width;
    entity->height = height;
    entity->dx = dx;
    entity->dy = dy;
    entity->x = x;
    entity->y = y;
    entity->jumping = 0;
    entity->shot_time = 0;
    entity->foot = 0;
    entity->side = 0;
    entity->in_range = 0;
    entity->health = health;
    entity->especial = 0;
    entity->cooldown = cooldown;
    return entity;
}

void move_background(struct entity *bg, struct player *main_player, struct entity **enemies, struct entity *boss) {
    if (main_player->joystick->right || main_player->joystick->left) {
        if (main_player->entity->x + main_player->entity->width / 2 + PLAYER_STEP >= DISP_W && bg->x + DISP_W < bg->width) {
            bg->x += 2*PLAYER_STEP;
            for (int i = 0; i < 6; i++) {
                enemies[i]->x -= 2*PLAYER_STEP;
            }
            boss->x -= 2*PLAYER_STEP;
        }
        if (main_player->entity->x - main_player->entity->width / 2 - PLAYER_STEP <= 0 && bg->x > PLAYER_STEP) {
            bg->x -= 2*PLAYER_STEP;
            for (int i = 0; i < 6; i++) {
                enemies[i]->x += 2*PLAYER_STEP;
            }
            boss->x += 2*PLAYER_STEP;
        }
    }
}

enum Directions get_fox_direction(ALLEGRO_TIMER *timer) {
    return movement_1[((int) (al_get_timer_count(timer) % 240)) / 24];
}

int get_fox_sprite(struct entity *player, struct entity *fox, ALLEGRO_TIMER *timer) {
    if (fox->in_range) {
        if (fox->x <= player->x)
            fox->side = RIGHT;
        else
            fox->side = LEFT;

        if (fox->shot_time <= 5) {
            fox_offset_h = 15;
            return 1;
        }

        fox_offset_h = FOX_OFFSET_H;

        return 0;
    }

    enum Directions fox_direction = get_fox_direction(timer);

    if (fox_direction == NONE)
        return 0;

    fox->side = fox_direction;

    if ((int) al_get_timer_count(timer) % 12 == 0)
        fox->foot = !fox->foot;

    return 2 + fox->foot;
}

void destroy_entity(struct entity *element) {
    free(element);
}
