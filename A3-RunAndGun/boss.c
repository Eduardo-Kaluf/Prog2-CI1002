#include "boss.h"

#include "entity.h"
#include "shot.h"
#include "utils.h"

void update_boss_position(struct entity *boss) {
    if (boss->jumping)
        boss->y -= PLAYER_STEP;

    if (boss->y + boss->height / 2 < GROUND && !boss->jumping) {
        if ((boss->y + PLAYER_STEP) + boss->width/2 < GROUND)
            boss->y = boss->y + PLAYER_STEP;
        else
            boss->y = GROUND;
    }
}

void update_boss_status(struct entity *boss, struct entity **enemies, int *boss_fight, ALLEGRO_TIMER* timer, ALLEGRO_BITMAP* bullet_type) {
    if (!(*boss_fight)) {
        for (int i = 0; i < 6; i++) {
            *boss_fight = 1;

            if (enemies[i]->health > 0) {
                *boss_fight = 0;
                return;
            }
        }
    }

    int offset_factor = 1;
    if (boss->side == RIGHT)
        offset_factor = -1;

    int offseted_width = boss->width - boss_offset_w;
    int offseted_x = boss->x + offset_factor * (boss_offset_w / 2);

    int offseted_height = boss->height - boss_offset_h;
    int offseted_y = boss->y + (boss_offset_h / 2);
    // AGORA ESTAMOS DE FATO NOS BOUNDS DO PERSONAGEM, SEM NENHUM PIXEL EM BRANCO PARA DIREITA OU ESQUERDA

    // QUEREMOS QUE A COLISÃO SEJA PARTIR DO x DO CORPO E NÃO DAS ORELHAS
    int actual_x_collision = offseted_x - offseted_width / 2 + BOSS_BODY_OFFSET_W;
    // FATOR * 4 DE ESCALA POIS O LADO DIRETO DO CORPO É MAIOR QUE O ESQUERDO

    if (shots_collide(BOSS, actual_x_collision, offseted_y - offseted_height / 2, offseted_width - BOSS_BODY_OFFSET_W * (4 * BOSS_SCALE), offseted_height))
        boss->health--;

    if (boss->especial && boss->y + boss->height/2 >= GROUND) {
        boss->especial = 0;

        for (int i = 1; i < between(4, 10); i++)
            shots_create(BOSS, DOWN, bullet_type, boss->x - 10 * i * between(5, 10), 0);
    }

    if ((int) al_get_timer_count(timer) % 300 == 0) {
        boss->jumping = 1;
        boss->especial = 1;
    }

    if (boss->y + boss->height / 2 <= 350)
        boss->jumping = 0;
}
int get_boss_sprite(struct entity *enemy) {
    boss_offset_w = BOSS_OFFSET_W;
    boss_offset_h =	BOSS_OFFSET_H;

    if (enemy->jumping == 1 || enemy->y + enemy->height/2 < GROUND) {
        boss_offset_w = 70;
        boss_offset_h =	20;
        return 1;
    }

    boss_ground_offset = 0;
    if (enemy->shot_time / 20 <= 0) {
        boss_ground_offset = BOSS_GROUND_OFFSET;
        return 2;
    }

    return 0;
}

