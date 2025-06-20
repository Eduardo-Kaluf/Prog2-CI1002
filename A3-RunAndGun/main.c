#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "player.h"
#include "utils.h"
#include "shot.h"

int GRAVITY = 30;
int start = 0;
int key;

void must_init(bool test, const char *description) {
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int lo, int hi) {
	return lo + (rand() % (hi - lo));
}

void display_menu(ALLEGRO_FONT **fonts) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(fonts[0], al_map_rgb_f(255,1,1), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "JOGAR");
	al_draw_text(fonts[1], al_map_rgb_f(255,1,1), DISP_CENTER_W, DISP_CENTER_H + 100, ALLEGRO_ALIGN_CENTER, "SAIR");
	al_draw_text(fonts[2], al_map_rgb_f(255,1,1), DISP_CENTER_W, (float) DISP_H / 6, ALLEGRO_ALIGN_CENTER, "SNOWY MOIR");
}

void display_won(ALLEGRO_FONT **fonts) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(fonts[2], al_map_rgb_f(255,1,1), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "VITORIA");
}

void display_game_over(ALLEGRO_FONT **fonts) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(fonts[2], al_map_rgb_f(255,1,1), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "GAME OVER");
}

void update_position(struct player *player) {
	if (player->joystick->left)
		move_player(player, 1, LEFT, DISP_W, DISP_H);

	if (player->joystick->right)
		move_player(player, 1, RIGHT, DISP_W, DISP_H);

	if (player->entity->jumping)
		move_player(player, 1, UP, DISP_W, DISP_H);

	if (player->entity->y + player->entity->height / 2 < GROUND && !player->entity->jumping)
		move_player(player, 1, DOWN, DISP_W, DISP_H);
}



void update_enemy_position(struct entity *enemy, enum Directions trajectory) {
	if (trajectory == LEFT)
		enemy->x = enemy->x - PLAYER_STEP / 2;
	else if (trajectory == RIGHT)
		enemy->x = enemy->x + PLAYER_STEP / 2;
}


extern enum Directions movement_2[10] = {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE};

int get_boss_sprite(struct entity *enemy) {
	if (enemy->jumping == 1 || enemy->y + enemy->height/2 < GROUND)
		return 1;

	if (enemy->shot_time / 20 <= 0)
		return 2;

	return 0;
}

int end_game(struct player *player, struct entity *boss, int *start) {
	if (player->entity->health <= 0 || boss->health <= 0) {
		*start = 0;
		return 1;
	}

	return 0;
}

void handle_shots(struct entity *element, struct joystick *joystick, enum EntityType type, ALLEGRO_BITMAP* bullet_type) {
	if (element->shot_time)
		element->shot_time--;
	else if (type == BOSS && !element->especial || type == PLAYER && key == ALLEGRO_KEY_SPACE || type == FOX && element->in_range) {
		enum Directions direction;
		int x = element->x;

		if (type == PLAYER && joystick->up)
			direction = UP;
		else
			direction = element->side;

		if (direction == LEFT)
			x -= - element->width / 2;
		else
			x += + element->width / 2;

		if (shots_create(type, direction, bullet_type, x, element->y))
			element->shot_time = element->cooldown;
	}
}

void update_boss_position(struct entity *boss, ALLEGRO_TIMER* timer, ALLEGRO_BITMAP* bullet_type) {

	if (shots_collide(false, boss->x, boss->y, boss->width, boss->height))
		boss->health--;

	if (boss->jumping)
		boss->y -= PLAYER_STEP;

	if (boss->y + boss->height / 2 < GROUND && !boss->jumping) {
		if ((boss->y + PLAYER_STEP) + boss->width/2 < GROUND)
			boss->y = boss->y + PLAYER_STEP;
		else
			boss->y = GROUND;
	}

	if (boss->especial && boss->y + boss->height/2 >= GROUND) {
		boss->especial = 0;

		for (int i = 1; i < between(4, 10); i++)
			shots_create(BOSS, DOWN,  bullet_type, boss->x - 10 * i * between(5, 10), 0);
	}

	if ((int) al_get_timer_count(timer) % 300 == 0) {
		boss->jumping = 1;
		boss->especial = 1;
	}

	if (boss->y + boss->height / 2 <= 350)
		boss->jumping = 0;
}

void update_player_status(struct player *player) {
	if(shots_collide(true, player->entity->x, player->entity->y, player->entity->width - 135, player->entity->height))
		player->entity->health--;

	if (player->entity->y <= GROUND - 250)
		player->entity->jumping = 0;
}

void update_enemy_status(struct entity *enemy, int player_x) {
	if (shots_collide(false, enemy->x, enemy->y, enemy->width, enemy->height))
		enemy->health--;

	if (enemy->health <= 0)
		enemy->x = -500;

	if (abs(enemy->x - player_x) <= 300)
		enemy->in_range = 1;
}


void update_boss_status(struct entity **enemies, int *boss_fight) {
	// CRIAR STRUCT BOSS QUE CONTEM BOSS_IS_FIGHTING OU ADICIONAR ACTIVE NA ENTITY
	if (!(*boss_fight)) {
		for (int i = 0; i < 6; i++) {
			*boss_fight = 1;

			if (enemies[i]->health > 0) {
				*boss_fight = 0;
				break;
			}
		}
	}
}


int main() {
    al_init();
    al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_ttf_addon();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* normal_font = al_load_ttf_font("font.TTF", 32, 0);
	ALLEGRO_FONT* focused_font = al_load_ttf_font("font.TTF", 48, 0);
	ALLEGRO_FONT* title_font = al_load_ttf_font("font.TTF", 64, 0);
	ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP* snowball = al_load_bitmap("sprites/snowBall.png");
	ALLEGRO_BITMAP* cupcake = al_load_bitmap("sprites/bunny.png");
	ALLEGRO_BITMAP* fox = al_load_bitmap("sprites/fox.png");
	ALLEGRO_BITMAP* spike = al_load_bitmap("sprites/spike.png");
	ALLEGRO_BITMAP* bear = al_load_bitmap("sprites/boss.png");

	ALLEGRO_FONT* fonts[3] = {focused_font, normal_font, title_font};

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	struct player* player = create_player(PLAYER_DISP_W, PLAYER_DISP_H, 100, GROUND - PLAYER_DISP_H/2, X_VELOCITY, Y_VELOCITY, cupcake, 50);
	struct entity *bg = create_entity(BG_DISP_W, BG_DISP_H, 0, 0, 0, 0, al_load_bitmap("sprites/bgcereto.png"), 0, 0);
	struct entity *boss = create_entity(BOSS_DISP_W, BOSS_DISP_H, BG_DISP_W - 200, GROUND - BOSS_DISP_H/2, X_VELOCITY, Y_VELOCITY, bear, 10, 80);
	boss->in_range = 1;

	struct entity* enemies[6];
	for (int i = 1; i < 7; i++)
		enemies[i - 1] = create_entity(FOX_DISP_W, FOX_DISP_H, 750 * i, GROUND - FOX_DISP_H/2, X_VELOCITY, Y_VELOCITY, fox, 1, 50);

	int player_sprite = 0;
	int boss_fight = 0;
	al_start_timer(timer);
	while (1) {
		al_wait_for_event(queue, &event);

		key = event.keyboard.keycode;

		if (event.type == ALLEGRO_EVENT_TIMER) {
			if (start) {
				update_boss_status(enemies, &boss_fight);

				update_player_status(player);

				for (int i = 0; i < 6; i++)
					update_enemy_status(enemies[i], player->entity->x);

				shots_update();

				update_position(player);

				for (int i = 0; i < 6; i++) {
					if (!enemies[i]->in_range)
						update_enemy_position(enemies[i], get_fox_direction(timer));
				}

				if (boss_fight)
					update_boss_position(boss, timer, snowball);

				if (end_game(player, boss, &start))
					continue;

				move_background(bg, player, enemies, boss);

				al_draw_tinted_scaled_rotated_bitmap_region(bg->spritesheet, bg->x, bg->y, DISP_W, DISP_H, al_map_rgba(255, 255, 255, 255),
															DISP_CENTER_W, DISP_CENTER_H, DISP_CENTER_W, DISP_CENTER_H, 1, 1, 0, 0);

				shots_draw(snowball, spike);

				for (int i = 0; i < 6; i++)
					al_draw_tinted_scaled_rotated_bitmap_region(enemies[i]->spritesheet, get_fox_sprite(player->entity, enemies[i], timer) * FOX_W, 0, FOX_W, FOX_H, al_map_rgba(255, 255, 255, 255),
														        FOX_W / 2, FOX_H / 2, enemies[i]->x, enemies[i]->y, FOX_SCALE, FOX_SCALE, 0, enemies[i]->side * ALLEGRO_FLIP_HORIZONTAL);

				al_draw_tinted_scaled_rotated_bitmap_region(player->entity->spritesheet, get_player_sprite(player, timer) * PLAYER_W, 0, PLAYER_W, PLAYER_H, al_map_rgba(255, 255, 255, 255),
															PLAYER_W / 2, PLAYER_H / 2, player->entity->x, player->entity->y, PLAYER_SCALE, PLAYER_SCALE, 0, player->entity->side * ALLEGRO_FLIP_HORIZONTAL);

				if (boss_fight)
					al_draw_tinted_scaled_rotated_bitmap_region(boss->spritesheet, get_boss_sprite(boss) * BOSS_W, 0, BOSS_W, BOSS_H, al_map_rgba(255, 255, 255, 255),
																BOSS_W / 2, BOSS_H / 2, boss->x, boss->y, BOSS_SCALE, BOSS_SCALE, 0, 0);

				for (int i = 0; i < 6; i++)
					handle_shots(enemies[i], NULL, FOX, spike);

				if (boss_fight)
					handle_shots(boss, NULL, BOSS, spike);
			}
			else {
				if (player->entity->health <= 0)
					display_game_over(fonts);
				else if (boss->health <= 0)
					display_won(fonts);
				else
					display_menu(fonts);
			}

			al_flip_display();
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
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

			if (key == ALLEGRO_KEY_ENTER && (player->entity->health <= 0 || boss->health <= 0))
				break;

	        if (key == ALLEGRO_KEY_ENTER && start == 0) {
        		if (fonts[0] == focused_font)
        			start = 1;
        		else
        			break;
	        }
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

			handle_shots(player->entity, player->joystick, PLAYER, snowball);

			if (key == ALLEGRO_KEY_S || key == ALLEGRO_KEY_W) {
				ALLEGRO_FONT* temp = fonts[1];
				fonts[1] = fonts[0];
				fonts[0] = temp;
			}

			if (player->entity->height / 2 + player->entity->y == GROUND && key == ALLEGRO_KEY_W)
				player->entity->jumping = 1;
		}

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
	}

    // CONSERTAR DESTROY
    al_destroy_font(normal_font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
    destroy_player(player);

    return 0;
}
