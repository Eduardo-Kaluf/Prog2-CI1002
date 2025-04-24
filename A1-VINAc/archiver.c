#define _GNU_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "dir_member.h"
#include "directory.h"
#include "archiver.h"

#include <string.h>

#include "content.h"
#include "files.h"
#include "logger.h"
#include "utils.h"


char option_i(FILE *archiver, char **new_members, int append_size, int compress) {
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

        if (new_member == NULL) {
            if (dir_members != NULL)
                free_dir_members(&dir_members, dir_size);
            return FILE_NOT_FOUND;
        }

        long member_size = file_size(new_member);

        if (member_size == 0) {
            fclose(new_member);
            if (dir_members != NULL)
                free_dir_members(&dir_members, dir_size);
            return FILE_IS_BLANK;
        }

        if (dir_intial_size > 0)
            found = find_by_name(dir_members, new_members[i], dir_size);

        if (found != NULL) {
            move_chunks(archiver, found->offset + found->stored_size, archiver_size, found->offset + member_size);

            if (member_size < found->stored_size)
                ftruncate(fileno(archiver), archiver_size - (found->stored_size - member_size));

            fix_offsets(dir_members, dir_size, -(found->stored_size - member_size), found->order + 1, dir_size);

            edit_dir_member(found, DONT_CHANGE, found->offset, found->order);

            if (compress)
                read_write(new_member, archiver, member_size, START_OF_FILE, found->offset, WRITING_COMPRESSED, found);
            else
                read_write(new_member, archiver, member_size, START_OF_FILE, found->offset, READ_WRITE_UNCOMPRESSED, NULL);
        }
        else {
            dir_size++;
            dir_members = realloc(dir_members, dir_size * sizeof(struct dir_member_t *));
            dir_members[dir_size - 1] = create_dir_member(new_members[i], DONT_CHANGE, archiver_size, dir_size - 1);

            if (compress)
                read_write(new_member, archiver, member_size, START_OF_FILE, archiver_size, WRITING_COMPRESSED, dir_members[dir_size - 1]);
            else
                read_write(new_member, archiver, member_size, START_OF_FILE, archiver_size, READ_WRITE_UNCOMPRESSED, NULL);
        }

        fclose(new_member);
    }

    // Atualiza o diretório do archive
    write_directory(archiver, dir_members, dir_size, dir_size - dir_intial_size);

    if (dir_members != NULL)
        free_dir_members(&dir_members, dir_size);


    return OK;
}

char option_c(FILE *archiver) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (is_empty(archiver))
        return ARCHIVE_IS_BLANK;

    read_dir_members(archiver, &dir_members, &dir_size);

    for (int i = 0; i < dir_size; i++)
        log_member(dir_members[i]);

    if (dir_members != NULL)
        free_dir_members(&dir_members, dir_size);

    return OK;
}

char option_x(FILE *archiver, char **members_to_extract, int extraction_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;
    int extract_all = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (is_empty(archiver))
        return ARCHIVE_IS_BLANK;

    read_dir_members(archiver, &dir_members, &dir_size);

    if (extraction_size == 0) {
        extraction_size = dir_size;
        extract_all = 1;
    }

    for (int i = 0; i < extraction_size; i++) {
        struct dir_member_t *extracting;

        if (extract_all)
            extracting = dir_members[i];
        else
            extracting = find_by_name(dir_members, members_to_extract[i], dir_size);

        if (extracting == NULL) {
            log_error(MEMBER_NOT_FOUND, members_to_extract[i]);
            continue;
        }

        FILE *out_file = fopen(extracting->name, "w");

        if (extracting->original_size == extracting->stored_size)
            read_write(archiver, out_file, extracting->stored_size, extracting->offset, 0, READ_WRITE_UNCOMPRESSED, NULL);
        else
            read_write(archiver, out_file, extracting->stored_size, extracting->offset, 0, READING_COMPRESSED, extracting);

        edit_dir_member(extracting, DONT_CHANGE, DONT_CHANGE, DONT_CHANGE);

        fclose(out_file);
    }

    if (dir_members != NULL)
        free_dir_members(&dir_members, dir_size);

    return OK;
}

char option_m(FILE *archiver, char *member_to_move, char *target) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (is_empty(archiver))
        return ARCHIVE_IS_BLANK;

    read_dir_members(archiver, &dir_members, &dir_size);

    struct dir_member_t *dir_member_to_move = find_by_name(dir_members, member_to_move, dir_size);
    struct dir_member_t *dir_target = find_by_name(dir_members, target, dir_size);
    // struct dir_member_t *dir_target = NULL;
    //
    // if (strcmp(target, 'null')) {
    //     int target_end = 0;
    //
    //     fix_order(dir_members, dir_size, 0, dir_member_to_move->order, 1);
    //
    //     fix_offsets(dir_members, dir_size, dir_member_to_move->stored_size,0, dir_member_to_move->order);
    //
    //     edit_dir_member(dir_member_to_move, DONT_CHANGE, dir_size * DIR_MEMBER_SIZE, 0);
    // }
    // else
    //     dir_target = find_by_name(dir_members, target, dir_size);

    if (dir_target == NULL || dir_member_to_move == NULL) {
        if (dir_members != NULL)
            free_dir_members(&dir_members, dir_size);
        return MEMBER_NOT_FOUND;
    }

    if (dir_member_to_move->order == dir_target->order + 1) {
        if (dir_members != NULL)
            free_dir_members(&dir_members, dir_size);
        return OK;
    }

    long size = file_size(archiver);

    int target_end = dir_target->offset + dir_target->stored_size;

    if (dir_member_to_move->order < dir_target->order) {
        move_and_shift_member(archiver, dir_member_to_move, target_end, size, true);

        fix_offsets(dir_members, dir_size, -dir_member_to_move->stored_size,dir_member_to_move->order + 1, dir_target->order + 1);
        fix_order(dir_members, dir_size, dir_member_to_move->order + 1, dir_target->order + 1, -1);
        target_end -= dir_member_to_move->stored_size;
    } else {
        move_and_shift_member(archiver, dir_member_to_move, target_end, size, false);

        fix_order(dir_members, dir_size, dir_target->order + 1, dir_member_to_move->order, 1);
        fix_offsets(dir_members, dir_size, dir_member_to_move->stored_size,dir_target->order + 1, dir_member_to_move->order);
    }

    edit_dir_member(dir_member_to_move, DONT_CHANGE, target_end, dir_target->order + 1);

    write_directory(archiver, dir_members, dir_size, 0);

    if (dir_members != NULL)
        free_dir_members(&dir_members, dir_size);

    return OK;
}

char option_r(FILE *archiver, char **removing_members, int removing_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (is_empty(archiver))
        return ARCHIVE_IS_BLANK;

    read_dir_members(archiver, &dir_members, &dir_size);

    for (int i = 0; i < removing_size; i++) {
        long archiver_size = file_size(archiver);
        struct dir_member_t *current_removing_member = find_by_name(dir_members, removing_members[i], dir_size);

        if (current_removing_member == NULL) {
            if (dir_members != NULL)
                free_dir_members(&dir_members, dir_size);
            return MEMBER_NOT_FOUND;
        }

        int member_size = current_removing_member->stored_size;

        move_and_shift_member(archiver, current_removing_member, archiver_size, archiver_size, true);
        dir_members = remove_by_name(dir_members, current_removing_member->name, &dir_size);
        ftruncate(fileno(archiver), archiver_size - member_size);
    }

    if (dir_size == 0) {
        ftruncate(fileno(archiver), START_OF_FILE);
        if (dir_members != NULL)
            free_dir_members(&dir_members, dir_size);
        return OK;
    }

    write_directory(archiver, dir_members, dir_size, -removing_size);

    if (dir_members != NULL)
        free_dir_members(&dir_members, dir_size);

    return OK;
}
