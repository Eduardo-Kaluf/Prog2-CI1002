#define _GNU_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "dir_member.h"
#include "directory.h"
#include "archiver.h"
#include "utils.h"
#include "content.h"


void option_ip(FILE *archiver, char **new_members, int append_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;
    int total_size = append_size;
    int append_minus = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (!is_empty(archiver)) {
        read_dir_members(archiver, &dir_members, &dir_size);
        total_size += dir_size;
        dir_members = realloc(dir_members, total_size * sizeof(struct dir_member_t *));
    }
    else
        dir_members = malloc(append_size * sizeof(struct dir_member_t *));

    // Adiciona o conteúdo dos novos membros ao arquiva e atualiza o vetor do diretório
    for (int i = 0; i < append_size; i++) {
        long archiver_size = file_size(archiver);

        FILE *new_member = fopen(new_members[i], "rb");

        long member_size = file_size(new_member);

        struct dir_member_t *found = find_by_name(dir_members, new_members[i], dir_size);

        if (found != NULL) {
            move_chunks(archiver, found->offset + found->stored_size, archiver_size, found->offset + member_size);
            if (member_size < found->stored_size) {
                ftruncate(fileno(archiver), archiver_size - (found->stored_size - member_size));
            }

            // fix_offsets(dir_members, dir_size, -(found->stored_size - member_size), found->order + 1, dir_size);

            for (int j = 0; j < dir_size; j++) {
                if (dir_members[j]->order > found->order) {
                    dir_members[j]->offset = dir_members[j]->offset - (found->stored_size - member_size);
                }
            }

            edit_dir_member(found, -1, found->offset, found->order);

            read_write(new_member, archiver, member_size, found->offset);

            append_minus += 1;
        }
        else {
            read_write(new_member, archiver, member_size, archiver_size);
            dir_members[dir_size + i - append_minus] = create_dir_member(new_members[i], -1, archiver_size, dir_size + i - append_minus);
        }

        fclose(new_member);
    }

    append_size -= append_minus;
    total_size -= append_minus;

    // Atualiza o diretório do archive
    write_directory(archiver, dir_members, total_size, append_size);
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
    if (!is_empty(archiver))
        read_dir_members(archiver, &dir_members, &dir_size);

    char buffer[BUFFER_SIZE];
    fseek(archiver, 0, SEEK_END);
    long size = ftell(archiver);
    int bytes_to_read;

    if (extraction_size == 0) {
        for (int i = 0; i < dir_size; i++) {
            FILE *out_file = fopen(dir_members[i]->name, "w");

            if (i != dir_size - 1)
                bytes_to_read = dir_members[i + 1]->offset - dir_members[i]->offset;
            else
                bytes_to_read = size - dir_members[i]->offset;

            fseek(archiver, dir_members[i]->offset, SEEK_SET);

            fread(buffer, 1, bytes_to_read, archiver);

            fwrite(buffer, 1, bytes_to_read, out_file);
        }
    }
    else {
        for (int i = 0; i < extraction_size; i++) {
            struct dir_member_t *extracting = find_by_name(dir_members, members_to_extract[i], dir_size);
            FILE *out_file = fopen(extracting->name, "w");

            bytes_to_read = extracting->stored_size;

            fseek(archiver, extracting->offset, SEEK_SET);
            fread(buffer, 1, bytes_to_read, archiver);

            fwrite(buffer, 1, bytes_to_read, out_file);
        }
    }

}

void option_m(FILE *archiver, char *member_to_move, char *target) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    fseek(archiver, 0, SEEK_END);
    long size = ftell(archiver);
    fseek(archiver, 0, SEEK_SET);

    // Caso o archive não seja vazio, le os membros do diretório
    if (!is_empty(archiver))
        read_dir_members(archiver, &dir_members, &dir_size);

    struct dir_member_t *dir_target = find_by_name(dir_members, target, dir_size);
    struct dir_member_t *dir_member_to_move = find_by_name(dir_members, member_to_move, dir_size);

    if (dir_member_to_move->order < dir_target->order) {
        move_chunks(archiver, dir_target->offset + dir_target->stored_size, size, dir_target->offset + dir_target->stored_size + dir_member_to_move->stored_size);
        move_member(archiver, dir_member_to_move, dir_target->offset + dir_target->stored_size);
        fseek(archiver, 0, SEEK_END);
        long new_size = ftell(archiver);
        fseek(archiver, 0, SEEK_SET);
        move_chunks(archiver, dir_member_to_move->offset + dir_member_to_move->stored_size, new_size, dir_member_to_move->offset);

        fix_order(dir_members, dir_size, dir_member_to_move->order + 1, dir_target->order + 1, -1);
        fix_offsets(dir_members, dir_size, -dir_member_to_move->stored_size,dir_member_to_move->order + 1, dir_target->order + 1);

        dir_member_to_move->offset = dir_target->offset + dir_target->stored_size;
        dir_member_to_move->order = dir_target->order + 1;

        ftruncate(fileno(archiver), size);

        order_dir_members(dir_members, dir_size);
        fseek(archiver, 0, SEEK_SET);

        for (int i = 0; i < dir_size; i++)
            fwrite(dir_members[i], DIR_MEMBER_SIZE , 1, archiver);

    } else {


        fix_order(dir_members, dir_size, dir_target->order + 1, dir_member_to_move->order, 1);
        fix_offsets(dir_members, dir_size, dir_member_to_move->stored_size,dir_target->order + 1, dir_member_to_move->order);

        move_chunks(archiver, dir_target->offset + dir_target->stored_size, dir_member_to_move->offset, size);

        move_member(archiver, dir_member_to_move, dir_target->offset + dir_target->stored_size);

        fseek(archiver, 0, SEEK_END);
        long new_size = ftell(archiver);
        fseek(archiver, 0, SEEK_SET);

        move_chunks(archiver, size, new_size, dir_target->offset + dir_target->stored_size + dir_member_to_move->stored_size);

        ftruncate(fileno(archiver), new_size - (dir_member_to_move->offset - (dir_target->offset + dir_target->stored_size)));

        dir_member_to_move->offset = dir_target->offset + dir_target->stored_size;
        dir_member_to_move->order = dir_target->order + 1;

        order_dir_members(dir_members, dir_size);
        fseek(archiver, 0, SEEK_SET);

        for (int i = 0; i < dir_size; i++)
            fwrite(dir_members[i], DIR_MEMBER_SIZE , 1, archiver);

        }
    }


void option_r(FILE *archiver, char **removing_members, int removing_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    fseek(archiver, 0, SEEK_SET);

    // Caso o archive não seja vazio, le os membros do diretório
    if (!is_empty(archiver))
        read_dir_members(archiver, &dir_members, &dir_size);

    for (int i = 0; i < removing_size; i++) {
        fseek(archiver, 0, SEEK_END);
        long arc_size = ftell(archiver);
        fseek(archiver, 0, SEEK_SET);

        struct dir_member_t *current_removing_member = find_by_name(dir_members, removing_members[i], dir_size);

        move_chunks(archiver, current_removing_member->offset + current_removing_member->stored_size, arc_size, current_removing_member->offset);

        fix_offsets(dir_members, dir_size, -current_removing_member->stored_size, current_removing_member->order + 1, dir_size);

        remove_by_name(&dir_members, current_removing_member->name, &dir_size);

        ftruncate(fileno(archiver), arc_size - current_removing_member->stored_size);
    }

    if (dir_size == 0) {
        ftruncate(fileno(archiver), 0);
        return;
    }


    fseek(archiver, 0, SEEK_SET);
    order_dir_members(dir_members, dir_size);

    fseek(archiver, 0, SEEK_END);
    long size = ftell(archiver);
    fseek(archiver, 0, SEEK_SET);
    move_chunks(archiver, dir_members[0]->offset, size, dir_members[0]->offset - (removing_size * DIR_MEMBER_SIZE));

    fix_offsets(dir_members, dir_size, -(removing_size * DIR_MEMBER_SIZE), 0, dir_size);


    fseek(archiver, 0, SEEK_SET);
    for (int i = 0; i < dir_size; i++)
        fwrite(dir_members[i], DIR_MEMBER_SIZE , 1, archiver);

}

long is_empty(FILE *archiver) {
    fseek(archiver, 0, SEEK_END);
    long size = ftell(archiver);
    rewind(archiver);
    return size == 0;
}

long file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void read_write(FILE *read_file, FILE *write_file, int member_size, int write_offset) {

    char *member_content = malloc(member_size * sizeof(char));

    fseek(read_file, 0, SEEK_SET);
    fread(member_content, 1, member_size, read_file);

    fseek(write_file, write_offset, SEEK_SET);
    fwrite(member_content, 1, member_size, write_file);

    rewind(read_file);
    rewind(write_file);
    free(member_content);
}
