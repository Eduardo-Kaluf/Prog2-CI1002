#include "utils.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>


void must_init(bool test, const char *description) {
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int lo, int hi) {
    return lo + (rand() % (hi - lo));
}


int end_game(struct player *player, struct entity *boss, int *start) {
    if (player->entity->health <= 0 || boss->health <= 0) {
        *start = 0;
        return 1;
    }

    return 0;
}
