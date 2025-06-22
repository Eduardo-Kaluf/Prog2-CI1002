#ifndef __HUD__
#define __HUD__
#include <allegro5/allegro_font.h>

#include "player.h"

void display_menu(ALLEGRO_FONT **fonts, ALLEGRO_FONT *title_font);

void display_won(ALLEGRO_FONT *title_font);

void display_game_over(ALLEGRO_FONT *title_font);

void draw_hud(struct player *player, ALLEGRO_BITMAP* heart);

void reset_fonts(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal, int n);

int selected_font(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *target, int n);

void font_down(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal, int n);

void font_up(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal, int n);

void display_options(ALLEGRO_FONT **options_fonts);

void menu_movement(int options, ALLEGRO_FONT **options_fonts, ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal);

int handle_menu_functions(int *start, int *options, ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal);

void handle_options_functions(int *options, struct player *player, struct entity *boss, struct entity **enemies, ALLEGRO_FONT **options_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal);

#endif
