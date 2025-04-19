
#include "dir_member.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void read_dir_members(FILE *archiver, struct dir_member_t ***dir_members, int *dir_member_num) {
    struct dir_member_t first_member;

    fread(&first_member, sizeof(struct dir_member_t), 1, archiver);
    fseek(archiver, 0, SEEK_SET);

    *dir_member_num = first_member.offset / sizeof(struct dir_member_t);

    *dir_members = malloc(sizeof(struct dir_member_t *) * (*dir_member_num));

    for (int i = 0; i < *dir_member_num; i++) {
        (*dir_members)[i] = malloc(sizeof(struct dir_member_t));
        fread((*dir_members)[i], sizeof(struct dir_member_t), 1, archiver);
    }
}


void fix_offsets(struct dir_member_t **dir_members, int n, int offset_fix) {
    for (int i = 0; i < n; i++) {
        dir_members[i]->offset = dir_members[i]->offset + offset_fix;
    }
}
