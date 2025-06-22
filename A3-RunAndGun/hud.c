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


void reset_fonts(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal, int n) {
    for (int i = 0; i < n; i++)
        menu_fonts[i] = normal;
    menu_fonts[0] = focus;
}

int selected_font(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *target, int n) {
    for (int i = 0; i < n; i++) {
        if (menu_fonts[i] == target) {
            return i;
        }
    }

    return 0;
}

void font_down(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal, int n) {
    int k = selected_font(menu_fonts, focus, n);

    if (k == n - 1) {
        menu_fonts[k] = normal;
        menu_fonts[0] = focus;
        return;
    }

    menu_fonts[k] = normal;
    menu_fonts[k + 1] = focus;
}

void font_up(ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal, int n) {
    int k = selected_font(menu_fonts, focus, n);

    if (k == 0) {
        menu_fonts[k] = normal;
        menu_fonts[n - 1] = focus;
        return;
    }

    menu_fonts[k] = normal;
    menu_fonts[k - 1] = focus;
}

void display_options(ALLEGRO_FONT **options_fonts) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(options_fonts[0], al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H - 200, ALLEGRO_ALIGN_CENTER, "FACIL");
    al_draw_text(options_fonts[1], al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H - 100, ALLEGRO_ALIGN_CENTER, "MEDIO");
    al_draw_text(options_fonts[2], al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H, ALLEGRO_ALIGN_CENTER, "DIFICIL");
    al_draw_text(options_fonts[3], al_map_rgb(255,255,255), DISP_CENTER_W, DISP_CENTER_H + 100, ALLEGRO_ALIGN_CENTER, "VOLTAR");
}

void menu_movement(int options, ALLEGRO_FONT **options_fonts, ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal) {

    if (!options) {
        if (key == ALLEGRO_KEY_S)
            font_down(menu_fonts, focus, normal, MENU_FONT_N);

        if (key == ALLEGRO_KEY_W)
            font_up(menu_fonts, focus, normal, MENU_FONT_N);
    }

    if (options) {
        if (key == ALLEGRO_KEY_S)
            font_down(options_fonts, focus, normal, OPTIONS_FONT_N);

        if (key == ALLEGRO_KEY_W)
            font_up(options_fonts, focus, normal, OPTIONS_FONT_N);
    }
}

int handle_menu_functions(int *start, int *options, ALLEGRO_FONT **menu_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal) {
    if (selected_font(menu_fonts, focus, MENU_FONT_N) == 0) {
        *start = 1;
        return 1;
    }

    if (selected_font(menu_fonts, focus, MENU_FONT_N) == 1) {
        *options = 1;
        reset_fonts(menu_fonts, focus, normal, MENU_FONT_N);
        return 1;
    }

    if (selected_font(menu_fonts, focus, MENU_FONT_N) == 2)
        return 0;

    return 0;
}

void handle_options_functions(int *options, struct player *player, struct entity *boss, struct entity **enemies, ALLEGRO_FONT **options_fonts, ALLEGRO_FONT *focus, ALLEGRO_FONT *normal) {

    int font_selected = selected_font(options_fonts, focus, OPTIONS_FONT_N);

    switch (font_selected) {
        // EASY
        case 0:
            player->entity->health = 15;
            for (int i = 0; i < 6; i++)
                enemies[i]->health = 2;
            boss->health = 10;
            break;
            // NORMAL
        case 1:
            player->entity->health = 10;
            for (int i = 0; i < 6; i++)
                enemies[i]->health = 3;
            boss->health = 15;
            break;
            // HARD
        case 2:
            player->entity->health = 5;
            for (int i = 0; i < 6; i++)
                enemies[i]->health = 4;
            boss->health = 20;
            break;
        default:
            *options = 0;
            reset_fonts(options_fonts, focus, normal, OPTIONS_FONT_N);
            break;
    }
}
