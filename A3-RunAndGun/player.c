#include <stdlib.h>

#include "player.h"

#include <allegro5/timer.h>

#include "shot.h"
#include "utils.h"

enum Directions movement_1[10] = {LEFT, LEFT, LEFT, LEFT, NONE, NONE, RIGHT, RIGHT, RIGHT, RIGHT};


struct player* create_player(int width, int height, int x, int y, int dx, int dy, ALLEGRO_BITMAP* spritesheet, int health, int stamina) {

	struct player *new_player = malloc(sizeof(struct player));
    new_player->entity = create_entity(width, height, x, y, dx, dy, spritesheet, health, 1);
	new_player->joystick = joystick_create();
	new_player->stamina = stamina;
	new_player->resting = 0;

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


int get_player_sprite(struct player *element, ALLEGRO_TIMER *timer) {

	player_offset_h = PLAYER_OFFSET_H;

	if ((int) al_get_timer_count(timer) % 12 == 0)
		element->entity->foot = !element->entity->foot;

	if (element->joystick->right)
		element->entity->side = RIGHT;
	else if (element->joystick->left)
		element->entity->side = LEFT;

	if (element->entity->jumping)
		return 3;
	if (element->joystick->fire)
		return 5;
	if (element->joystick->down) {
		player_offset_h = 150;
		return 4;
	}
	if (element->joystick->right && element->joystick->left)
		return 0;
	if (element->joystick->right || element->joystick->left)
		return 1 + element->entity->foot;

	return 0;
}



void update_player_status(struct player *player) {
	int offset_factor = 1;
	if (player->entity->side == RIGHT)
		offset_factor = -1;

	int offseted_width = player->entity->width - player_offset_w;
	int offseted_x = player->entity->x + offset_factor * (player_offset_w / 2);

	int offseted_height = player->entity->height - player_offset_h;
	int offseted_y = player->entity->y + (player_offset_h / 2);
	// AGORA ESTAMOS DE FATO NOS BOUNDS DO PERSONAGEM, SEM NENHUM PIXEL EM BRANCO PARA DIREITA OU ESQUERDA

	// QUEREMOS QUE A COLISÃO SEJA PARTIR DO x DO CORPO E NÃO DAS ORELHAS
	int actual_x_collision = offseted_x - offseted_width / 2 + PLAYER_BODY_OFFSET_W;
	// FATOR * 4 DE ESCALA POIS O LADO DIRETO DO CORPO É MAIOR QUE O ESQUERDO

	if (shots_collide(PLAYER, actual_x_collision, offseted_y - offseted_height / 2, offseted_width - PLAYER_BODY_OFFSET_W * (4 * PLAYER_SCALE), offseted_height))
		player->entity->health--;

	if (player->entity->y <= GROUND - 250)
		player->entity->jumping = 0;

	if (player->stamina == 0)
		player->resting = 1;

	if (player->resting)
		player->stamina++;

	if (player->stamina == PLAYER_STAMINA)
		player->resting = 0;
}



void update_player_position(struct player *player) {
	if (player->joystick->left)
		move_player(player, 1, LEFT, DISP_W, DISP_H);

	if (player->joystick->right)
		move_player(player, 1, RIGHT, DISP_W, DISP_H);

	if (player->entity->jumping)
		move_player(player, 1, UP, DISP_W, DISP_H);

	if (player->entity->y + player->entity->height / 2 < GROUND && !player->entity->jumping)
		move_player(player, 1, DOWN, DISP_W, DISP_H);
}



void destroy_player(struct player *element) {
	free(element);
}
