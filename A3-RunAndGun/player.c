#include <stdlib.h>
#include "player.h"
#include "utils.h"

struct player* create_player(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet) {

	struct player *new_player = malloc(sizeof(struct player));
    new_player->entity = create_entity(width, height, x, y, dx, dy, spritesheet);
	new_player->joystick = joystick_create();

	return new_player;
}

void move_player(struct player *element, int steps, int trajectory, int max_x, int max_y) {

	if (!trajectory) {
		if ((element->entity->x - steps*PLAYER_STEP) - element->entity->width/2 >= 0)
			element->entity->x = element->entity->x - steps*PLAYER_STEP;
	}
	else if (trajectory == 1) {
		if ((element->entity->x + steps*PLAYER_STEP) + element->entity->width/2 <= max_x)
			element->entity->x = element->entity->x + steps*PLAYER_STEP;
	}
	else if (trajectory == 2) {
		if ((element->entity->y - steps*PLAYER_STEP) - element->entity->width/2 >= 0)
			element->entity->y = element->entity->y - steps*PLAYER_STEP;
	}

	else if (trajectory == 3) {
		if ((element->entity->y + steps*PLAYER_STEP) + element->entity->width/2 < max_y)
			element->entity->y = element->entity->y + steps*PLAYER_STEP;
		else
			element->entity->y = GROUND;
	}
}

void destroy_player(struct player *element) {
	free(element);
}

void jump(struct player *element) {

    if (element->entity->y <= GROUND - 200) {
        element->joystick->up = 0;
    }

}

int get_player_sprite(struct player *element) {
	if (element->joystick->up)
		return 3;
	if (element->joystick->down)
		return 4;
	if (element->joystick->right || element->joystick->left)
		return 1;

	return 0;
}



// void crouch(struct player *element) {

// }
