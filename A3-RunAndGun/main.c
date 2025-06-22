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
#include "boss.h"
#include "fox.h"
#include "hud.h"

int start = 0;
int key;

int fox_offset_w = FOX_OFFSET_W;
int fox_offset_h = FOX_OFFSET_H;

int player_offset_w = PLAYER_OFFSET_W;
int player_offset_h = PLAYER_OFFSET_H;

int boss_offset_w = BOSS_OFFSET_W;
int boss_offset_h =	BOSS_OFFSET_H;
int boss_ground_offset = BOSS_GROUND_OFFSET;

int main() {
    must_init(al_init(), "Allegro");

    must_init(al_init_primitives_addon(), "Primitives");
	must_init(al_init_image_addon(), "Image");
	must_init(al_install_keyboard(), "Keyboard");
	must_init(al_init_ttf_addon(), "Ttf");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	must_init(timer, "Timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "Queue");

    ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);

	ALLEGRO_FONT* title_font = al_load_ttf_font("font.TTF", 64, 0);
    ALLEGRO_FONT* normal_font = al_load_ttf_font("font.TTF", 32, 0);
    ALLEGRO_FONT* focused_font = al_load_ttf_font("font.TTF", 48, 0);
    ALLEGRO_FONT* menu_fonts[MENU_FONT_N] = {focused_font, normal_font, normal_font};
    ALLEGRO_FONT* options_fonts[OPTIONS_FONT_N] = {focused_font, normal_font, normal_font, normal_font};

    ALLEGRO_BITMAP* snowball = al_load_bitmap("sprites/snowBall.png");
    ALLEGRO_BITMAP* spike = al_load_bitmap("sprites/spike.png");

    ALLEGRO_BITMAP* cupcake = al_load_bitmap("sprites/bunny.png");
    ALLEGRO_BITMAP* fox = al_load_bitmap("sprites/fox.png");
    ALLEGRO_BITMAP* bear = al_load_bitmap("sprites/boss.png");

    ALLEGRO_BITMAP* heart = al_load_bitmap("sprites/heart.png");

    ALLEGRO_EVENT event;

	al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

    struct entity *bg = create_entity(BG_DISP_W, BG_DISP_H, 0, 0, 0, 0, al_load_bitmap("sprites/bg.png"), 0, 0);
    struct player* player = create_player(PLAYER_DISP_W, PLAYER_DISP_H, 100, GROUND - PLAYER_DISP_H/2, X_VELOCITY, Y_VELOCITY, cupcake, 10, PLAYER_STAMINA);
    struct entity *boss = create_entity(BOSS_DISP_W, BOSS_DISP_H,  BG_DISP_W - 200, GROUND - BOSS_DISP_H/2, X_VELOCITY, Y_VELOCITY, bear, 15, 80);
	boss->side = LEFT;

	struct entity* enemies[6];
	for (int i = 1; i < 7; i++)
		enemies[i - 1] = create_entity(FOX_DISP_W, FOX_DISP_H, 750 * i, GROUND - FOX_DISP_H/2, X_VELOCITY, Y_VELOCITY, fox, 3, 50);

	int boss_fight = 0;
	int pause = 0;
	int options = 0;
	al_start_timer(timer);
	while (1) {
		al_wait_for_event(queue, &event);

		key = event.keyboard.keycode;

		if (event.type == ALLEGRO_EVENT_TIMER) {
			if (start && !pause) {
				update_boss_status(boss, enemies, &boss_fight, timer, snowball);

				update_player_status(player);

				for (int i = 0; i < 6; i++)
					update_enemy_status(enemies[i], player->entity->x);

				shots_update();

				update_player_position(player);

				for (int i = 0; i < 6; i++) {
					if (!enemies[i]->in_range)
						update_fox_position(enemies[i], get_fox_direction(timer));
				}

				if (boss_fight)
					update_boss_position(boss);

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

				// NÃO REMOVER ESSA CHAMADA
				int boss_sprite = get_boss_sprite(boss);
				if (boss_fight)
					al_draw_tinted_scaled_rotated_bitmap_region(boss->spritesheet, boss_sprite * BOSS_W, 0, BOSS_W, BOSS_H, al_map_rgba(255, 255, 255, 255),
																BOSS_W / 2, BOSS_H / 2, boss->x, boss->y + boss_ground_offset, BOSS_SCALE, BOSS_SCALE, 0, ALLEGRO_FLIP_HORIZONTAL);

				draw_hud(player, heart);

				for (int i = 0; i < 6; i++)
					handle_shots(enemies[i], NULL, FOX, spike);

				if (boss_fight)
					handle_shots(boss, NULL, BOSS, spike);
			}
			else {
				if (pause)
					al_draw_text(normal_font, al_map_rgba(0,0,0, 255), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "PAUSED");
				else {
					if (player->entity->health <= 0)
						display_game_over(title_font);
					else if (boss->health <= 0)
						display_won(title_font);
					else if (options)
						display_options(options_fonts);
					else
						display_menu(menu_fonts, title_font);
				}
			}

			al_flip_display();
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
			joystick_control(player, key);

		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

			// HANDLING MENU OPTIONS
			if (!pause && key == ALLEGRO_KEY_ENTER) {
				if (player->entity->health <= 0 || boss->health <= 0)
					break;

				if (!start) {
					if (!options) {
						if (handle_menu_functions(&start, &options, menu_fonts, focused_font, normal_font))
							continue;

						break;
					}

					if (options)
						handle_options_functions(&options, player, boss, enemies, options_fonts, focused_font, normal_font);
				}
			}

			menu_movement(options, options_fonts, menu_fonts, focused_font, normal_font);

			// HANDLE THINGS WHEN THE GAME HAS ACTUALLY STARTED
			if (key == ALLEGRO_KEY_ESCAPE && !options && start)
				pause = !pause;

			if (start && !pause) {
				handle_shots(player->entity, player, PLAYER, snowball);

				if (player->entity->height / 2 + player->entity->y == GROUND && key == ALLEGRO_KEY_W)
					player->entity->jumping = 1;
			}
		}
	}


	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
    destroy_player(player);
	destroy_entity(bg);
	for (int i = 0; i < 6; i++)
		destroy_entity(enemies[i]);
	destroy_entity(boss);
	al_destroy_font(title_font);
	al_destroy_font(normal_font);
	al_destroy_font(focused_font);
	al_destroy_bitmap(snowball);
	al_destroy_bitmap(spike);
	al_destroy_bitmap(cupcake);
	al_destroy_bitmap(fox);
	al_destroy_bitmap(bear);
	al_destroy_bitmap(heart);

    return 0;
}
