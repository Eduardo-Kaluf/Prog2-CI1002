#ifndef __ENTITY__ 																											
#define __ENTITY__

#include <allegro5/bitmap.h>
#include <allegro5/timer.h>


// Using forward declaration !
struct player;

struct entity {
    int width;
    int height;
	int x;
	int y;
    int dx;
    int dy;
    int life;
	int jumping;
	int shot_time;
	int foot;
	int side;
	int in_range;
	int health;
	ALLEGRO_BITMAP* spritesheet;
};

struct entity *create_entity(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet);

int get_fox_sprite(struct entity *player, struct entity *fox, ALLEGRO_TIMER *timer);

enum Directions get_fox_direction(ALLEGRO_TIMER *timer);

void move_background(struct entity *bg, struct player *main_player, struct entity **enemies);

void destroy_entity(struct entity *element);

#endif
