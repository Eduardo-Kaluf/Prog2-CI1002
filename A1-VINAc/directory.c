#define _GNU_SOURCE 1

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "archiver.h"
#include "content.h"
#include "utils.h"
#include "dir_member.h"
#include "files.h"
#include "directory.h"


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

void write_directory(FILE *archiver, struct dir_member_t **dir_members, int total_size, int append_size) {
    rewind(archiver);

    if (dir_members == NULL)
        return;

    order_dir_members(dir_members, total_size);

    if (append_size != 0) {
        move_chunks(archiver, dir_members[0]->offset, file_size(archiver), dir_members[0]->offset + (append_size * DIR_MEMBER_SIZE));
        fix_offsets(dir_members, total_size, append_size * DIR_MEMBER_SIZE, 0, total_size);

        if (append_size < 0)
            ftruncate(fileno(archiver), file_size(archiver) + (append_size * DIR_MEMBER_SIZE));
    }

    rewind(archiver);
    for (int i = 0; i < total_size; i++)
        fwrite(dir_members[i], DIR_MEMBER_SIZE , 1, archiver);
}

void remove_by_name(struct dir_member_t ***dir_members, char *target, int *dir_size) {
    for (int i = 0; i < *dir_size; i++) {
        if (strcmp((*dir_members)[i]->name, target) == 0) {
            int removed_size = (*dir_members)[i]->stored_size;

            free((*dir_members)[i]);

            for (int j = i; j < *dir_size - 1; j++) {
                (*dir_members)[j] = (*dir_members)[j + 1];
                (*dir_members)[j]->order -= 1;
                (*dir_members)[j]->offset -= removed_size;
            }

            (*dir_size)--;

            struct dir_member_t **tmp = realloc(*dir_members, (*dir_size) * sizeof(struct dir_member_t *));
            if (tmp != NULL || *dir_size == 0) {
                *dir_members = tmp;
            }

            return;
        }
    }
}

void order_dir_members(struct dir_member_t **v, int append_size) {
    for (int i = 1; i < append_size; ++i) {
        struct dir_member_t *key = v[i];
        int j = i - 1;

        while (j >= 0 && v[j]->order > key->order) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = key;
    }
}

struct dir_member_t *find_by_name(struct dir_member_t **dir_members, char *target, int dir_size) {
    for (int i = 0; i < dir_size; i++) {
        if (strcmp(dir_members[i]->name, target) == 0)
            return dir_members[i];
    }

    return NULL;
}

void fix_offsets(struct dir_member_t **dir_members, int n, int offset_fix, int order_start, int order_end) {
    if (order_end > n)
        return;

    for (int i = order_start; i < order_end; i++)
        dir_members[i]->offset = dir_members[i]->offset + offset_fix;
}

void fix_order(struct dir_member_t **dir_members, int n, int order_start, int order_end, int order_increment) {
    if (order_end > n)
        return;

    for (int i = order_start; i < order_end; i++)
            dir_members[i]->order = dir_members[i]->order + order_increment;
}
