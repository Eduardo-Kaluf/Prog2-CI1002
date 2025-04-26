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


void read_dir_members(FILE *archiver, struct dir_member_t ***dir_members, int *dir_size) {
    struct dir_member_t first_member;

    if (is_empty(archiver))
        return;

    // Le o primeiro membro para descobrir a quantidade total de membros do diretório
    fread(&first_member, sizeof(struct dir_member_t), 1, archiver);

    *dir_size = first_member.offset / sizeof(struct dir_member_t);

    *dir_members = malloc(sizeof(struct dir_member_t *) * (*dir_size));

    rewind(archiver);
    for (int i = 0; i < *dir_size; i++) {
        (*dir_members)[i] = malloc(sizeof(struct dir_member_t));
        fread((*dir_members)[i], sizeof(struct dir_member_t), 1, archiver);
    }
}

void write_directory(FILE *archiver, struct dir_member_t **dir_members, int total_size, int append_size) {
    rewind(archiver);

    if (dir_members == NULL)
        return;

    order_dir_members(dir_members, total_size);

    // Caso algum membro tenha sido adicionado/removido, aumenta/diminui espaço para o conteúdo
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

struct dir_member_t **remove_by_name(struct dir_member_t **dir_members, char *target, int *dir_size) {
    int found = -1;

    // Procura o índice do membro cujo nome é igual ao 'target'
    for (int i = 0; i < *dir_size; i++) {
        if (strcmp(dir_members[i]->name, target) == 0) {
            found = i;
            break;
        }
    }

    // Se o membro não foi encontrado, apenas retorna o vetor original
    if (found == -1)
        return dir_members;

    // Armazena o tamanho do membro que será removido e o libera
    int removed_size = dir_members[found]->stored_size;
    free(dir_members[found]);

    struct dir_member_t **new_array = NULL;

    // Se restaram membros, aloca um novo vetor
    if (*dir_size - 1 > 0)
        new_array = malloc((*dir_size - 1) * sizeof(*new_array));

    int new_index = 0;

    // Copia todos os membros, exceto o removido, para o vetor
    for (int i = 0; i < *dir_size; i++) {
        if (i == found)
            continue;
        new_array[new_index++] = dir_members[i];
    }

    // Atualiza os campos 'order' e 'offset' dos membros que estavam após o removido
    for (int i = found; i < *dir_size - 1; i++) {
        new_array[i]->order  -= 1;
        new_array[i]->offset -= removed_size;
    }

    // Libera o vetor antigo, atualiza o tamanho e o retorna
    free(dir_members);
    (*dir_size)--;

    return new_array;
}

void order_dir_members(struct dir_member_t **dir_members, int dir_size) {
    // Insertion sort
    for (int i = 1; i < dir_size; ++i) {
        struct dir_member_t *key = dir_members[i];
        int j = i - 1;

        while (j >= 0 && dir_members[j]->order > key->order) {
            dir_members[j + 1] = dir_members[j];
            j = j - 1;
        }
        dir_members[j + 1] = key;
    }
}

struct dir_member_t *find_by_name(struct dir_member_t **dir_members, char *target, int dir_size) {
    for (int i = 0; i < dir_size; i++) {
        if (strcmp(dir_members[i]->name, target) == 0)
            return dir_members[i];
    }

    return NULL;
}

void fix_offsets(struct dir_member_t **dir_members, int dir_size, int offset_fix, int order_start, int order_end) {
    if (order_end > dir_size)
        return;

    for (int i = order_start; i < order_end; i++)
        dir_members[i]->offset = dir_members[i]->offset + offset_fix;
}

void fix_order(struct dir_member_t **dir_members, int dir_size, int order_start, int order_end, int order_increment) {
    if (order_end > dir_size)
        return;

    for (int i = order_start; i < order_end; i++)
            dir_members[i]->order = dir_members[i]->order + order_increment;
}

void free_dir_members(struct dir_member_t ***dir_members, int dir_size) {
    struct dir_member_t **members = *dir_members;

    for (int i = 0; i < dir_size; i++) {
        free(members[i]);
    }

    free(members);
    *dir_members = NULL;
}
