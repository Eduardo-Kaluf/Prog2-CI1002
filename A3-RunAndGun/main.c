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
	struct player* player = create_player(SPRITE_W, SPRITE_H, 100, 300, X_VELOCITY, Y_VELOCITY, cupcake);

	al_start_timer(timer);
	int side = 0;
	int foot = 0;
	int current_sprite = 0;
	ALLEGRO_FONT* fonts[2] = {focused_font, normal_font};
	while(1) {
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			if (start) {
				jump(player);

				shots_update(player);

				update_position(player);

				move_background(bg, player);

				current_sprite = get_player_sprite(player, &foot, &side, timer);

				al_draw_tinted_scaled_rotated_bitmap_region(bg->spritesheet, bg->x, bg->y, bg->width, bg->height, al_map_rgba(255, 255, 255, 255),
															0, 0, 0, 0, 1, 1, 0, 0);

				al_draw_tinted_scaled_rotated_bitmap_region(player->entity->spritesheet, current_sprite * SPRITE_W, 0, SPRITE_W, SPRITE_H, al_map_rgba(255, 255, 255, 255),
															SPRITE_W / 2, SPRITE_H / 2, player->entity->x, player->entity->y, 0.8, 0.8, 0, side * ALLEGRO_FLIP_HORIZONTAL);

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

			al_flip_display();
		}
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {

        	if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        		if (player->shot_time)
        			player->shot_time--;
        		if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
        			int x = player->entity->x + (player->entity->width / 2);
        			if (shots_add(player, side, true, x, player->entity->y))
        				player->shot_time = 5;
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

    al_destroy_font(normal_font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
    //CONMSERTAR DESTROY
    destroy_player(player);

    return 0;
}