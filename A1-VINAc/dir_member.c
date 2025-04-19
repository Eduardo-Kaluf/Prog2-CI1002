
#include "dir_member.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct dir_member_t *create_dir_member(char *member_name, int compressed_size, int offset, int pos) {
    struct dir_member_t *dir_member = malloc(sizeof(struct dir_member_t));
    struct stat buf;

    stat(member_name, &buf);

    strcpy(dir_member->name, member_name);
    dir_member->original_size = buf.st_size;

    if (compressed_size == -1) 
        dir_member->stored_size = dir_member->original_size;
    else
        dir_member->stored_size = compressed_size;
    
    dir_member->uid = buf.st_uid;
    dir_member->last_modification = buf.st_mtime;
    dir_member->order = pos;
    dir_member->offset = offset;

    return dir_member;
}

void log_member(struct dir_member_t *dir_member) {
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
