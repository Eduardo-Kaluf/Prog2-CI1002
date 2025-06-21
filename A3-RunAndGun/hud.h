#ifndef __HUD__
#define __HUD__
#include <allegro5/allegro_font.h>

#include "player.h"

void display_menu(ALLEGRO_FONT **fonts, ALLEGRO_FONT *title_font);

void display_won(ALLEGRO_FONT *title_font);

void display_game_over(ALLEGRO_FONT *title_font);

void draw_hud(struct player *player, ALLEGRO_BITMAP* heart);

#endif
