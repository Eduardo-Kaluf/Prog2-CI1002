#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "player.h"
#include "utils.h"


int GRAVITY = 30;

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void update_position(struct player *player) {


	if (player->joystick->left)
		move_player(player, 1, LEFT, DISP_W, DISP_H);

	if (player->joystick->right)
		move_player(player, 1, RIGHT, DISP_W, DISP_H);

	if (player->joystick->up)
		move_player(player, 2, UP, DISP_W, DISP_H);

	if (player->entity->y + player->entity->height / 2 < GROUND && !player->joystick->up)
		move_player(player, 1, DOWN, DISP_W, DISP_H);

}

int main() {
    al_init();
    al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
	ALLEGRO_EVENT event;

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	struct entity *bg = create_entity(7350, 720, 0, 0, 0, 0, al_load_bitmap("sprites/bg.png"));

	ALLEGRO_BITMAP* bunny = al_load_bitmap("sprites/bunny.png");
	struct player* player = create_player(SPRITE_W, SPRITE_H, 100, 300, X_VELOCITY, Y_VELOCITY, bunny);

	al_start_timer(timer);
	int side = 0;
	int foot = 0;
	while(1) {
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			jump(player);

			update_position(player);

			if (player->joystick->right || player->joystick->left)
				move_background(bg, player);

			int current_sprite = get_player_sprite(player);

			if ((int) al_get_timer_count(timer) % 12 == 0)
				foot = !foot;

			if (current_sprite == 1)
				current_sprite += foot;

			if (player->joystick->right)
				side = 0;
			else if (player->joystick->left)
				side = 1;


			al_draw_tinted_scaled_rotated_bitmap_region(bg->spritesheet, bg->x, bg->y, bg->width, bg->height, al_map_rgba(255, 255, 255, 255),
														0, 0, 0, 0, 1, 1, 0, 0);

			al_draw_tinted_scaled_rotated_bitmap_region(player->entity->spritesheet, current_sprite * SPRITE_W, 0, SPRITE_W, SPRITE_H, al_map_rgba(255, 255, 255, 255),
												        SPRITE_W / 2, SPRITE_H / 2, player->entity->x, player->entity->y, 0.8, 0.8, 0, side * ALLEGRO_FLIP_HORIZONTAL);

			al_flip_display();
		}
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
			if (event.keyboard.keycode == ALLEGRO_KEY_A) {
				joystick_left(player->joystick);
			}

        	if (event.keyboard.keycode == ALLEGRO_KEY_D) {
				joystick_right(player->joystick);
			}

        	if (event.keyboard.keycode == ALLEGRO_KEY_S) {
        		joystick_down(player->joystick);

        	}

			if (player->entity->height / 2 + player->entity->y == GROUND && event.keyboard.keycode == ALLEGRO_KEY_W && event.type == ALLEGRO_EVENT_KEY_DOWN) {
				player->joystick->up = 1;
			}
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
	}

    al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
    //CONMSERTAR DESTROY
    destroy_player(player);

    return 0;
}