#define _GNU_SOURCE 1


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "dir_member.h"
#include "directory.h"
#include "archiver.h"

#include <stdbool.h>
#include "utils.h"
#include "content.h"


void option_ip(FILE *archiver, char **new_members, int append_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (!is_empty(archiver))
        read_dir_members(archiver, &dir_members, &dir_size);

    int dir_intial_size = dir_size;

    // Adiciona o conteúdo dos novos membros ao arquiva e atualiza o vetor do diretório
    for (int i = 0; i < append_size; i++) {
        struct dir_member_t *found = NULL;

        long archiver_size = file_size(archiver);

        FILE *new_member = fopen(new_members[i], "rb");

        long member_size = file_size(new_member);

        if (dir_intial_size > 0)
            found = find_by_name(dir_members, new_members[i], dir_size);

        if (found != NULL) {
            move_chunks(archiver, found->offset + found->stored_size, archiver_size, found->offset + member_size);

            if (member_size < found->stored_size)
                ftruncate(fileno(archiver), archiver_size - (found->stored_size - member_size));

            fix_offsets(dir_members, dir_size, -(found->stored_size - member_size), found->order + 1, dir_size);

            edit_dir_member(found, -1, found->offset, found->order);

            read_write(new_member, archiver, member_size, 0, found->offset);
        }
        else {
            dir_size++;
            read_write(new_member, archiver, member_size, 0, archiver_size);
            dir_members = realloc(dir_members, dir_size * sizeof(struct dir_member_t *));
            dir_members[dir_size - 1] = create_dir_member(new_members[i], -1, archiver_size, dir_size - 1);
        }

        fclose(new_member);
    }

    // Atualiza o diretório do archive
    write_directory(archiver, dir_members, dir_size, dir_size - dir_intial_size);
}

void option_c(FILE *archiver) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (!is_empty(archiver))
        read_dir_members(archiver, &dir_members, &dir_size);

    for (int i = 0; i < dir_size; i++)
        log_member(dir_members[i]);
}

void option_x(FILE *archiver, char **members_to_extract, int extraction_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (is_empty(archiver))
        return;

    read_dir_members(archiver, &dir_members, &dir_size);

    if (extraction_size == 0) {
        for (int i = 0; i < dir_size; i++) {
            FILE *out_file = fopen(dir_members[i]->name, "w");
            read_write(archiver, out_file, dir_members[i]->stored_size, dir_members[i]->offset, 0);
            fclose(out_file);
        }
    }
    else {
        for (int i = 0; i < extraction_size; i++) {
            struct dir_member_t *extracting = find_by_name(dir_members, members_to_extract[i], dir_size);
            FILE *out_file = fopen(extracting->name, "w");
            read_write(archiver, out_file, extracting->stored_size, extracting->offset, 0);
            fclose(out_file);
        }
    }
}

void option_m(FILE *archiver, char *member_to_move, char *target) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    long size = file_size(archiver);

    // Caso o archive não seja vazio, le os membros do diretório
    if (!is_empty(archiver))
        read_dir_members(archiver, &dir_members, &dir_size);

    // VERIFICAR SE DE FATO FORAM ENCONTRADOS
    struct dir_member_t *dir_target = find_by_name(dir_members, target, dir_size);
    struct dir_member_t *dir_member_to_move = find_by_name(dir_members, member_to_move, dir_size);

    int target_end = dir_target->offset + dir_target->stored_size;

    if (dir_member_to_move->order < dir_target->order) {
        move_and_shift_member(archiver, dir_member_to_move, target_end, size, true);

        fix_order(dir_members, dir_size, dir_member_to_move->order + 1, dir_target->order + 1, -1);
        fix_offsets(dir_members, dir_size, -dir_member_to_move->stored_size,dir_member_to_move->order + 1, dir_target->order + 1);
    } else {
        move_and_shift_member(archiver, dir_member_to_move, target_end, size, false);

        fix_order(dir_members, dir_size, dir_target->order + 1, dir_member_to_move->order, 1);
        fix_offsets(dir_members, dir_size, dir_member_to_move->stored_size,dir_target->order + 1, dir_member_to_move->order);
    }

    edit_dir_member(dir_member_to_move, -1, target_end, dir_target->order + 1);

    write_directory(archiver, dir_members, dir_size, 0);
}

void option_r(FILE *archiver, char **removing_members, int removing_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (is_empty(archiver))
        return;

    read_dir_members(archiver, &dir_members, &dir_size);

    for (int i = 0; i < removing_size; i++) {
        long archiver_size = file_size(archiver);
        struct dir_member_t *current_removing_member = find_by_name(dir_members, removing_members[i], dir_size);

        move_and_shift_member(archiver, current_removing_member, archiver_size, archiver_size, true);

        fix_offsets(dir_members, dir_size, -current_removing_member->stored_size, current_removing_member->order + 1, dir_size);
        // O CONCERTO DOS OFFSETS DEVE SER FEITO DENTRO DO REMOVE_BY_NAME
        remove_by_name(&dir_members, current_removing_member->name, &dir_size);

        ftruncate(fileno(archiver), archiver_size - current_removing_member->stored_size);
    }

    if (dir_size == 0) {
        ftruncate(fileno(archiver), 0);
        return;
    }

    write_directory(archiver, dir_members, dir_size, removing_size);
}

long is_empty(FILE *archiver) {
    return file_size(archiver) == 0;
}

long file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

void read_write(FILE *read_file, FILE *write_file, int member_size, int read_offset, int write_offset) {
    char *member_content = malloc(member_size * sizeof(char));

    fseek(read_file, read_offset, SEEK_SET);
    fread(member_content, 1, member_size, read_file);

    fseek(write_file, write_offset, SEEK_SET);
    fwrite(member_content, 1, member_size, write_file);

    rewind(read_file);
    rewind(write_file);
    free(member_content);
}

void move_and_shift_member(FILE *archiver, struct dir_member_t *member, long target_end, long size, int forward) {

    if (forward) {
        // Make room for the member ahead
        move_chunks(archiver, target_end, size, target_end + member->stored_size);
        move_member(archiver, member, target_end);
        // Shift everything after the old member up to fill the gap
        move_chunks(archiver, member->offset + member->stored_size, file_size(archiver), member->offset);
    } else {
        // Shift everything before the member down to make room
        move_chunks(archiver, target_end, member->offset, size);
        move_member(archiver, member, target_end);
        // Move tail content up
        move_chunks(archiver, size, file_size(archiver), target_end + member->stored_size);
    }

    // TODO TODO
    // truncate(fileno(archiver), target_end);
}

