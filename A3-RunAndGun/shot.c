#include "shot.h"
#include "player.h"

#include <allegro5/bitmap_draw.h>

#define SHIP_SHOT_W 22
#define SHIP_SHOT_H 22

struct shot {
    int x, y, dx, dy;
    int frame;
    char side;
    char ship;
    char used;
};

#define SHOTS_N 128
struct shot shots[SHOTS_N];

void shots_init() {
    for(int i = 0; i < SHOTS_N; i++)
        shots[i].used = 0;
}

char shots_add(struct player *element, struct entity *enemy, int side, char ship, int x, int y) {
    for (int i = 0; i < SHOTS_N; i++) {
        if (shots[i].used)
            continue;

        shots[i].ship = ship;

        if (ship) {
            int offset = element->entity->width;

            if (side == 1)
                offset = 10;
            if (element->joystick->up)
                if (side == 1)
                    offset = 90;
                else
                    offset = 100;

            shots[i].x = x - offset;
            shots[i].y = y - SHIP_SHOT_H / 2;

            if (element->joystick->up)
                shots[i].side = UP;
            else if (side == 1)
                shots[i].side = LEFT;
            else
                shots[i].side = RIGHT;
        }
        else if (enemy->in_range) {
            shots[i].x = x;
            shots[i].y = y;

            if (side == 1)
                shots[i].side = LEFT;
            else
                shots[i].side = RIGHT;
        }

        shots[i].frame = 0;
        shots[i].used = 1;

        return 1;
    }
    return 0;
}

void shots_update() {
    for (int i = 0; i < SHOTS_N; i++) {
        if(!shots[i].used)
            continue;

        if(shots[i].ship) {
            if (shots[i].side == RIGHT)
                shots[i].x += PLAYER_STEP + 2;
            else if (shots[i].side == LEFT)
                shots[i].x -= PLAYER_STEP + 2;
            else if (shots[i].side == UP)
                shots[i].y -= PLAYER_STEP + 2;

            if (shots[i].y <= -DISP_H || shots[i].x <= 0 || shots[i].x >= DISP_W) {
                shots[i].used = 0;
                continue;
            }
        }
        else {
            if (shots[i].side == LEFT)
                shots[i].x += PLAYER_STEP + 2;
            else if (shots[i].side == RIGHT)
                shots[i].x -= PLAYER_STEP + 2;

            if (shots[i].x <= 0 || shots[i].x >= DISP_W) {
                shots[i].used = false;
                continue;
            }
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

char shots_collide(char ship, int x, int y, int w, int h)
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        if(shots[i].ship == ship)
            continue;

        int sw, sh;
        if(ship)
        {
            sw = 35;
            sh = 12;
        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
        }

        if(collide(x, y, x+w, y+h, shots[i].x, shots[i].y, shots[i].x+sw, shots[i].y+sh))
        {
            shots[i].used = 0;
            return 1;
        }
    }

    return 0;
}

void shots_draw(ALLEGRO_BITMAP* snowBall, ALLEGRO_BITMAP* spike) {
    for(int i = 0; i < SHOTS_N; i++) {
        if(!shots[i].used)
            continue;

        if(shots[i].ship)
            al_draw_bitmap(snowBall, shots[i].x, shots[i].y, 0);
        else
            al_draw_bitmap(spike, shots[i].x, shots[i].y, 0);
    }
}