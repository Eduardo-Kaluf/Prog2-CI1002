#include <stdlib.h>

#include "joystick.h"

#include <allegro5/keycodes.h>

#include "player.h"


struct joystick* joystick_create() {				
	struct joystick *element = malloc (sizeof(struct joystick));

	element->right = 0;
	element->left = 0;
	element->up = 0;
	element->down = 0;
	element->fire = 0;

	return element;																
}


void joystick_left(struct joystick *element) {
	element->left = element->left ^ 1;
}

void joystick_right(struct joystick *element) {
	element->right = element->right ^ 1;
}

void joystick_up(struct joystick *element) {
	element->up = element->up ^ 1;
}

void joystick_down(struct joystick *element) {
	element->down = element->down ^ 1;
}

void joystick_fire(struct joystick *element) {
	element->fire = element->fire ^ 1;
}

void destroy_joystick(struct joystick *element) {
	free(element);
}

void joystick_control(struct player *player, int key) {
	if (key == ALLEGRO_KEY_W)
		joystick_up(player->joystick);

	if (key == ALLEGRO_KEY_A)
		joystick_left(player->joystick);

	if (key == ALLEGRO_KEY_S)
		joystick_down(player->joystick);

	if (key == ALLEGRO_KEY_D)
		joystick_right(player->joystick);

	if (key == ALLEGRO_KEY_SPACE)
		joystick_fire(player->joystick);

}