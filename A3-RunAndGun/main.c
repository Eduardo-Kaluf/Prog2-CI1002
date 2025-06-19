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


void must_init(bool test, const char *description) {
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int lo, int hi)
{
	return lo + (rand() % (hi - lo));
}

void display_menu(ALLEGRO_FONT **fonts) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(fonts[0], al_map_rgb_f(255,1,1), DISP_W / 2, DISP_H / 2, ALLEGRO_ALIGN_CENTER, "JOGAR");
	al_draw_text(fonts[1], al_map_rgb_f(255,1,1), DISP_W / 2, DISP_H / 2 + 100, ALLEGRO_ALIGN_CENTER, "SAIR");
	al_draw_text(fonts[2], al_map_rgb_f(255,1,1), DISP_W / 2, DISP_H / 6, ALLEGRO_ALIGN_CENTER, "SNOWY MOIR");
}

void display_game_over(ALLEGRO_FONT **fonts) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(fonts[2], al_map_rgb_f(255,1,1), DISP_W / 2, DISP_H / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
}

void update_position(struct player *player) {

	if(shots_collide(true, player->entity->x, player->entity->y, player->entity->width - 135, player->entity->height))
		player->entity->health--;

	if (player->joystick->left)
		move_player(player, 1, LEFT, DISP_W, DISP_H);

	if (player->joystick->right)
		move_player(player, 1, RIGHT, DISP_W, DISP_H);

	if (player->entity->jumping)
		move_player(player, 1, UP, DISP_W, DISP_H);

	if (player->entity->y + player->entity->height / 2 < GROUND && !player->entity->jumping)
		move_player(player, 1, DOWN, DISP_W, DISP_H);
}

void update_enemy_position(struct entity *enemy, int player_x, ALLEGRO_TIMER* timer) {
	if (shots_collide(false, enemy->x, enemy->y, enemy->width, enemy->height))
		enemy->health--;
	if (enemy->health <= 0)
		enemy->x = -100;

	enum Directions trajectory = get_fox_direction(timer);

	if (abs(enemy->x - player_x) <= 300)
		enemy->in_range = 1;
	else {
		enemy->in_range = 0;

		if (trajectory == LEFT)
			enemy->x = enemy->x - PLAYER_STEP / 2;
		else if (trajectory == RIGHT)
			enemy->x = enemy->x + PLAYER_STEP / 2;
	}
}


extern enum Directions movement_2[10] = {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE};

int get_boss_sprite(struct entity *enemy) {
	if (enemy->jumping == 1)
		return 1;

	if (enemy->shot_time / 20 <= 0)
		return 2;

	return 0;
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
	ALLEGRO_BITMAP* shot = al_load_bitmap("sprites/snowBall.png");
	ALLEGRO_BITMAP* cupcake = al_load_bitmap("sprites/bunny.png");
	ALLEGRO_BITMAP* fox = al_load_bitmap("sprites/fox.png");
	ALLEGRO_BITMAP* spike = al_load_bitmap("sprites/spike.png");
	ALLEGRO_BITMAP* bear = al_load_bitmap("sprites/boss.png");

	ALLEGRO_FONT* fonts[3] = {focused_font, normal_font, title_font};

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	struct player* player = create_player(SPRITE_W, SPRITE_H, 100, 300, X_VELOCITY, Y_VELOCITY, cupcake);
	struct entity *bg = create_entity(7350, 720, 0, 0, 0, 0, al_load_bitmap("sprites/bg.png"));
	struct entity* enemies[6];
	struct entity *boss = create_entity(750, 350, 500, 300, X_VELOCITY, Y_VELOCITY, bear);
	boss->in_range = 1;

	for (int i = 1; i < 7; i++)
		enemies[i - 1] = create_entity(FOX_W, FOX_H, 750 * i, GROUND - 60, X_VELOCITY, Y_VELOCITY, fox);
	int player_sprite = 0;
	al_start_timer(timer);
	while (1) {
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			if (start) {
				jump(player);

				shots_update();

				update_position(player);

				if (player->entity->health <= 0) {
					start = 0;
					continue;
				}

				for (int i = 0; i < 6; i++)
					update_enemy_position(enemies[i], player->entity->x, timer);

				move_background(bg, player, enemies);

				al_draw_tinted_scaled_rotated_bitmap_region(bg->spritesheet, bg->x, bg->y, bg->width, bg->height, al_map_rgba(255, 255, 255, 255),
															0, 0, 0, 0, 1, 1, 0, 0);

				shots_draw(shot, spike);

				for (int i = 0; i < 6; i++)
					al_draw_tinted_scaled_rotated_bitmap_region(enemies[i]->spritesheet, get_fox_sprite(player->entity, enemies[i], timer) * FOX_W, 0, FOX_W, FOX_H, al_map_rgba(255, 255, 255, 255),
														        FOX_W / 2, FOX_H / 2, enemies[i]->x, enemies[i]->y, 2, 2, 0, enemies[i]->side * ALLEGRO_FLIP_HORIZONTAL);

				player_sprite = get_player_sprite(player, timer);
				al_draw_tinted_scaled_rotated_bitmap_region(player->entity->spritesheet, player_sprite * SPRITE_W, 0, SPRITE_W, SPRITE_H, al_map_rgba(255, 255, 255, 255),
															SPRITE_W / 2, SPRITE_H / 2, player->entity->x, player->entity->y, 0.8, 0.8, 0, player->entity->side * ALLEGRO_FLIP_HORIZONTAL);

				al_draw_tinted_scaled_rotated_bitmap_region(boss->spritesheet, get_boss_sprite(boss) * 250, 0, 250, 350, al_map_rgba(255, 255, 255, 255),
															250 / 2, 350 / 2, boss->x, boss->y, 1, 1, 0, 0);


				for (int i = 0; i < 6; i++) {
					if (enemies[i]->shot_time)
						enemies[i]->shot_time--;
					else if (enemies[i]->in_range) {
						int x = enemies[i]->x + (enemies[i]->width / 2);
						if (shots_add(NULL, enemies[i], enemies[i]->side, false, x, enemies[i]->y))
							enemies[i]->shot_time = 30;
					}
				}

				if (boss->shot_time)
					boss->shot_time--;
				else {
					int x = boss->x - 250 / 2;
					if (shots_add(NULL, boss, LEFT, false, x, boss->y))
						boss->shot_time = 80;
				}

			}
			else {
				if (player->entity->health <= 0)
					display_game_over(fonts);
				else
					display_menu(fonts);

			}

			al_flip_display();
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
	        int key = event.keyboard.keycode;

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

			if (key == ALLEGRO_KEY_ENTER && player->entity->health <= 0)
				break;

	        if (key == ALLEGRO_KEY_ENTER && start == 0) {
        		if (fonts[0] == focused_font)
        			start = 1;
        		else
        			break;
	        }
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			int key = event.keyboard.keycode;

			if (player->entity->shot_time)
				player->entity->shot_time--;
			else if (key == ALLEGRO_KEY_SPACE) {
				int x = player->entity->x + (player->entity->width / 2);
				if (shots_add(player, NULL, player->entity->side, true, x, player->entity->y))
					player->entity->shot_time = 1;
			}

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
