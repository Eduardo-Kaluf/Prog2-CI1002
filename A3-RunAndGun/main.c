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

	if (trajectory == LEFT) {
		enemy->x = enemy->x - PLAYER_STEP / 2;
	}
	else if (trajectory == RIGHT) {
		enemy->x = enemy->x + PLAYER_STEP / 2;
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

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	struct entity *bg = create_entity(7350, 720, 0, 0, 0, 0, al_load_bitmap("sprites/bg.png"));

	ALLEGRO_BITMAP* shot = al_load_bitmap("sprites/snowBall.png");
	ALLEGRO_BITMAP* cupcake = al_load_bitmap("sprites/bunny.png");
	ALLEGRO_BITMAP* fox = al_load_bitmap("sprites/fox.png");
	struct player* player = create_player(SPRITE_W, SPRITE_H, 100, 300, X_VELOCITY, Y_VELOCITY, cupcake);

	struct entity* enemies[6];
	for (int i = 1; i < 7; i++) {
		enemies[i - 1] = create_entity(FOX_W, FOX_H, 500 * i, GROUND, X_VELOCITY, Y_VELOCITY, fox);
	}

	al_start_timer(timer);
	int player_sprite = 0;
	int fox_sprite = 0;
	ALLEGRO_FONT* fonts[2] = {focused_font, normal_font};
	while(1) {
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			if (start) {
				jump(player);

				shots_update();

				update_position(player);

				for (int i = 0; i < 6; i++) {
					enum Directions fox_direction = movement_1[((int) (al_get_timer_count(timer) % 240)) / 24];
					update_enemy_position(enemies[i], fox_direction);
				}

				shots_draw(shot);

				move_background(bg, player, enemies);

				player_sprite = get_player_sprite(player, timer);


				al_draw_tinted_scaled_rotated_bitmap_region(bg->spritesheet, bg->x, bg->y, bg->width, bg->height, al_map_rgba(255, 255, 255, 255),
															0, 0, 0, 0, 1, 1, 0, 0);

				al_draw_tinted_scaled_rotated_bitmap_region(player->entity->spritesheet, player_sprite * SPRITE_W, 0, SPRITE_W, SPRITE_H, al_map_rgba(255, 255, 255, 255),
															SPRITE_W / 2, SPRITE_H / 2, player->entity->x, player->entity->y, 0.8, 0.8, 0, player->entity->side * ALLEGRO_FLIP_HORIZONTAL);

				for (int i = 0; i < 6; i++) {
					fox_sprite = get_fox_sprite(player->entity, enemies[i], timer);

					al_draw_tinted_scaled_rotated_bitmap_region(enemies[i]->spritesheet, fox_sprite * FOX_W, 0, FOX_W, FOX_H, al_map_rgba(255, 255, 255, 255),
						FOX_W / 2, FOX_H / 2, enemies[i]->x, enemies[i]->y, 2, 2, 0, enemies[i]->side * ALLEGRO_FLIP_HORIZONTAL);

				}
				shots_draw(shot);
			}
			else {
				al_clear_to_color(al_map_rgb(0, 0, 0));

				al_draw_text(title_font,
				al_map_rgb_f(255,1,1),
				DISP_W / 2, DISP_H / 6,
				ALLEGRO_ALIGN_CENTER, "SNOWY MOIR");
				al_draw_text(fonts[0],
				al_map_rgb_f(255,1,1),
				DISP_W / 2, DISP_H / 2,
				ALLEGRO_ALIGN_CENTER, "JOGAR");
				al_draw_text(fonts[1],
				al_map_rgb_f(1,1,1),
				DISP_W / 2, DISP_H / 2 + 100,
				ALLEGRO_ALIGN_CENTER, "SAIR");
			}



			for (int i = 0; i < 6; i++) {
				if (enemies[i]->shot_time)
					enemies[i]->shot_time--;
				else if (enemies[i]->in_range) {
					int x = enemies[i]->x + (enemies[i]->width / 2);
					if (shots_add(NULL, enemies[i], enemies[i]->side, false, x, enemies[i]->y))
						enemies[i]->shot_time = 10;
				}
			}


			al_flip_display();
		}
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {

        	if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        		if (player->entity->shot_time)
        			player->entity->shot_time--;
        		if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
        			int x = player->entity->x + (player->entity->width / 2);
        			if (shots_add(player, NULL, player->entity->side, true, x, player->entity->y))
        				player->entity->shot_time = 5;
        		}
        	}

        	if (event.keyboard.keycode == ALLEGRO_KEY_A) {
				joystick_left(player->joystick);
			}

        	if (event.keyboard.keycode == ALLEGRO_KEY_D) {
				joystick_right(player->joystick);
			}

        	if (event.keyboard.keycode == ALLEGRO_KEY_S) {
        		joystick_down(player->joystick);
        	}

        	if (event.keyboard.keycode == ALLEGRO_KEY_W) {
				joystick_up(player->joystick);
        	}

        	if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
        		joystick_fire(player->joystick);
        	}

			if (event.type == ALLEGRO_EVENT_KEY_DOWN && (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_W)) {
				ALLEGRO_FONT* temp = fonts[1];
				fonts[1] = fonts[0];
				fonts[0] = temp;
			}

        	if (event.keyboard.keycode == ALLEGRO_KEY_ENTER && start == 0) {
        		if (fonts[0] == focused_font)
        			start = 1;
        		else
        			break;
        	}

			if (player->entity->height / 2 + player->entity->y == GROUND && event.keyboard.keycode == ALLEGRO_KEY_W && event.type == ALLEGRO_EVENT_KEY_DOWN) {
				player->entity->jumping = 1;
			}
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
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