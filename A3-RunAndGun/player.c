#include <stdlib.h>

#include "player.h"

#include <allegro5/timer.h>

#include "utils.h"


struct player* create_player(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet) {

	struct player *new_player = malloc(sizeof(struct player));
    new_player->entity = create_entity(width, height, x, y, dx, dy, spritesheet);
	new_player->joystick = joystick_create();
	new_player->shot_time = 0;

	return new_player;
}

void move_player(struct player *element, int steps, enum Directions trajectory, int max_x, int max_y) {

	if (trajectory == LEFT) {
		if ((element->entity->x - steps*PLAYER_STEP) - element->entity->width/2 >= 0)
			element->entity->x = element->entity->x - steps*PLAYER_STEP;
	}
	else if (trajectory == RIGHT) {
		if ((element->entity->x + steps*PLAYER_STEP) + element->entity->width/2 <= max_x)
			element->entity->x = element->entity->x + steps*PLAYER_STEP;
	}
	else if (trajectory == UP) {
		if ((element->entity->y - steps*PLAYER_STEP) - element->entity->width/2 >= 0)
			element->entity->y = element->entity->y - steps*PLAYER_STEP;
	}

	else if (trajectory == DOWN) {
		if ((element->entity->y + steps*PLAYER_STEP) + element->entity->width/2 < max_y)
			element->entity->y = element->entity->y + steps*PLAYER_STEP;
		else
			element->entity->y = GROUND;
	}
}



void jump(struct player *element) {

    if (element->entity->y <= GROUND - 250) {
        element->entity->jumping = 0;
    }

}

int get_player_sprite(struct player *element, int *foot, int *side, ALLEGRO_TIMER *timer) {

	if ((int) al_get_timer_count(timer) % 12 == 0)
		*foot = !(*foot);

	if (element->joystick->right)
		*side = 0;
	else if (element->joystick->left)
		*side = 1;

	if (element->entity->jumping)
		return 3;
	if (element->joystick->fire)
		return 5;
	if (element->joystick->down)
		return 4;
	if (element->joystick->right && element->joystick->left)
		return 0;
	if (element->joystick->right || element->joystick->left)
		return 1 + *foot;

	return 0;
}

void destroy_player(struct player *element) {
	free(element);
}
