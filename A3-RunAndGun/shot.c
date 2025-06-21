#include "shot.h"
#include "player.h"

#include <allegro5/bitmap_draw.h>
#include <allegro5/keycodes.h>

#define SHIP_SHOT_W 22
#define SHIP_SHOT_H 22

struct shot {
    int x, y, dx, dy;
    int frame;
    enum Directions side;
    char used;
    enum EntityType shooter;
    ALLEGRO_BITMAP* bullet_type;
};

#define SHOTS_N 1024
struct shot shots[SHOTS_N];

void shots_init() {
    for(int i = 0; i < SHOTS_N; i++)
        shots[i].used = 0;
}

char shots_create(enum EntityType shooter, enum Directions side, ALLEGRO_BITMAP* bullet_type, int x, int y) {
    for (int i = 0; i < SHOTS_N; i++) {
        if (shots[i].used)
            continue;

        shots[i].shooter = shooter;

        shots[i].x = x;
        shots[i].y = y;

        shots[i].side = side;

        shots[i].frame = 0;
        shots[i].used = 1;

        shots[i].bullet_type = bullet_type;

        return 1;
    }
    return 0;
}

void shots_update() {
    for (int i = 0; i < SHOTS_N; i++) {
        if (!shots[i].used)
            continue;

        if (shots[i].side == RIGHT)
            shots[i].x += PLAYER_STEP + 2;
        else if (shots[i].side == LEFT)
            shots[i].x -= PLAYER_STEP + 2;
        else if (shots[i].side == UP)
            shots[i].y -= PLAYER_STEP + 2;
        else if (shots[i].side == DOWN)
            shots[i].y += PLAYER_STEP + 2;

        if (shots[i].y <= -DISP_H || shots[i].x <= -200 || shots[i].x >= DISP_W + 200 || shots[i].y >= DISP_H) {
            shots[i].used = 0;
            continue;
        }

        shots[i].frame++;
    }
}

int collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
    if(ax1 > bx2) return 0;
    if(ax2 < bx1) return 0;
    if(ay1 > by2) return 0;
    if(ay2 < by1) return 0;

    return 1;
}

void handle_shots(struct entity *element, struct joystick *joystick, enum EntityType type, ALLEGRO_BITMAP* bullet_type) {
    if (element->shot_time)
        element->shot_time--;
    else if ((type == BOSS && !element->especial) || (type == PLAYER && key == ALLEGRO_KEY_SPACE) || (type == FOX && element->in_range)) {
        enum Directions direction;

        if (type == PLAYER && joystick->up)
            direction = UP;
        else
            direction = element->side;

        if (shots_create(type, direction, bullet_type, element->x - element->width / 2, element->y))
            element->shot_time = element->cooldown;
    }
}

char shots_collide(enum EntityType shooter, int x, int y, int w, int h) {
    for (int i = 0; i < SHOTS_N; i++) {
        if(!shots[i].used)
            continue;

        if(shots[i].shooter == shooter)
            continue;

        int sw, sh;
        if (shooter == PLAYER) {
            sw = 35 / 2;
            sh = 12 / 2;
        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
        }

        if(collide(x, y, x+w, y+h, shots[i].x - sw, shots[i].y - sh, shots[i].x + sw, shots[i].y + sh))
        {
            shots[i].used = 0;
            return 1;
        }
    }

    return 0;
}

void shots_draw() {
    for(int i = 0; i < SHOTS_N; i++) {
        if(!shots[i].used)
            continue;

        al_draw_bitmap(shots[i].bullet_type, shots[i].x, shots[i].y, 0);
    }
}