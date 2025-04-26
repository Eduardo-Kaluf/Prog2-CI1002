#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "dir_member.h"
#include "logger.h"


struct dir_member_t *create_dir_member(char *member_name, int compressed_size, int offset, int order) {
    struct dir_member_t *dir_member = malloc(sizeof(struct dir_member_t));
    struct stat buf;

    stat(member_name, &buf);

    strcpy(dir_member->name, member_name);
    dir_member->original_size = buf.st_size;

    if (compressed_size != DONT_CHANGE)
        dir_member->stored_size = compressed_size;
    else
        dir_member->stored_size = dir_member->original_size;

    dir_member->uid = buf.st_uid;
    dir_member->last_modification = buf.st_mtime;
    dir_member->order = order;
    dir_member->offset = offset;

    return dir_member;
}

void edit_dir_member(struct dir_member_t *dir_member, int compressed_size, int overwrite, int offset, int order) {
    struct stat buf;

    stat(dir_member->name, &buf);

    dir_member->original_size = buf.st_size;
    dir_member->uid = buf.st_uid;
    dir_member->last_modification = buf.st_mtime;

    if (compressed_size != DONT_CHANGE)
        dir_member->stored_size = compressed_size;

    if (order != DONT_CHANGE)
        dir_member->order = order;

    if (offset != DONT_CHANGE)
        dir_member->offset = offset;

    if (overwrite == OVERWRITE)
        dir_member->stored_size = dir_member->original_size;

}

void log_member(struct dir_member_t *dir_member) {

    // Ambiente de homologação
    #if HOMOLOG
        printf("Nome: %s, Id: %d, Tamanho Original: %d, Tamanho guardado: %d, Offset: %d, Posição: %u",
                dir_member->name,
                dir_member->uid,
                dir_member->original_size,
                dir_member->stored_size,
                dir_member->offset,
                dir_member->order
        );
        printf("\n");
        return;
    #endif

    printf("Nome: %s, Id: %d, Tamanho Original: %d, Tamanho guardado: %d, Última modificação: %li, Offset: %d, Posição: %u",
            dir_member->name,
            dir_member->uid,
            dir_member->original_size,
            dir_member->stored_size,
            dir_member->last_modification,
            dir_member->offset,
            dir_member->order
    );

    printf("\n");

}
