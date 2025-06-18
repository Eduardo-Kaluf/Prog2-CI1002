#ifndef __ENTITY__ 																											
#define __ENTITY__
#include <allegro5/bitmap.h>

struct entity {
    int width;
    int height;
	int x;
	int y;
    int dx;
    int dy;
    int life;
	ALLEGRO_BITMAP* spritesheet;
};

struct entity *create_entity(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet);

void destroy_entity(struct entity *element);

void move_background(struct entity *bg, struct player *player);

#endif
