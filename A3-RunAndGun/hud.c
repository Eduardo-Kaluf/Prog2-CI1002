//
// Created by eduardo-kaluf on 21/06/25.
//

#include "hud.h"

#include <allegro5/allegro_primitives.h>

#include "utils.h"


void display_menu(ALLEGRO_FONT **fonts, ALLEGRO_FONT *title_font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(fonts[0], al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "JOGAR");
    al_draw_text(fonts[1], al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H + 100, ALLEGRO_ALIGN_CENTER, "OPCOES");
    al_draw_text(fonts[2], al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H + 200, ALLEGRO_ALIGN_CENTER, "SAIR");
    al_draw_text(title_font, al_map_rgb(255,255,255), DISP_CENTER_W, (float) DISP_H / 6, ALLEGRO_ALIGN_CENTER, "SNOWY MOIR");
}

void display_won(ALLEGRO_FONT *title_font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(title_font, al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "VITORIA");
}

void display_game_over(ALLEGRO_FONT *title_font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(title_font, al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "GAME OVER");
}


void draw_hud(struct player *player, ALLEGRO_BITMAP* heart) {
    int spacing = HEART_DISP_W + 5;
    for(int i = 0; i < player->entity->health; i++)
        al_draw_scaled_bitmap(heart, 0, 0, HEART_W, HEART_H, 1 + (i * spacing), 10, HEART_DISP_W, HEART_DISP_H, 0);

    int remove_factor = PLAYER_STAMINA / PLAYER_STAMINA_FACTOR;
    int green = (STAMINA_BAR_X / PLAYER_STAMINA) * remove_factor;

    al_draw_filled_rectangle(5, 35, STAMINA_BAR_X, 55, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(5, 35, (((green * player->stamina) / remove_factor)), 55, al_map_rgb(0, 255, 0));
}