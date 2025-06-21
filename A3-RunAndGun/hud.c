//
// Created by eduardo-kaluf on 21/06/25.
//

#include "hud.h"

#include "utils.h"


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