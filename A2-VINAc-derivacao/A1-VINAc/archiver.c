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

        // Caso já exista no arquiver irá substitui-lo
        if (dir_intial_size > 0)
            found = find_by_name(dir_members, new_members[i], dir_size);

        if (found != NULL) {
            // Cria ou reduz o espaço para substituir o membro
            move_chunks(archiver, found->offset + found->stored_size, archiver_size, found->offset + member_size);

            // Caso tenha reduzido o espaço, trunca o desnecessário
            if (member_size < found->stored_size)
                ftruncate(fileno(archiver), archiver_size - (found->stored_size - member_size));

            // Conserta dados
            fix_offsets(dir_members, dir_size, -(found->stored_size - member_size), found->order + 1, dir_size);

            edit_dir_member(found, DONT_CHANGE, OVERWRITE, found->offset, found->order);

            // Inclui o membro no arquiver
            if (compress) {
                read_write(new_member, archiver, member_size, START_OF_FILE, found->offset, READING_AND_OVERWRITTEN_COMPRESSED, found);
                fix_offsets(dir_members, dir_size, -(member_size - found->stored_size), found->order + 1, dir_size);
            }
            else
                read_write(new_member, archiver, member_size, START_OF_FILE, found->offset, READ_WRITE_UNCOMPRESSED, NULL);
        }
        else {
            // Aumenta e adiciona o membro no diretório
            dir_size++;
            dir_members = realloc(dir_members, dir_size * sizeof(struct dir_member_t *));
            dir_members[dir_size - 1] = create_dir_member(new_members[i], DONT_CHANGE, archiver_size, dir_size - 1);

            // Inclui o membro no arquiver
            if (compress)
                read_write(new_member, archiver, member_size, START_OF_FILE, archiver_size, WRITING_COMPRESSED, dir_members[dir_size - 1]);
            else
                read_write(new_member, archiver, member_size, START_OF_FILE, archiver_size, READ_WRITE_UNCOMPRESSED, NULL);
        }

        fclose(new_member);
    }

    // Atualiza o diretório do archiver
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

    // Caso não tenha sido passado nenhum argumento, extrai todos
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

        // Decide se irá extrair um membro comprimido ou não
        if (extracting->original_size == extracting->stored_size)
            read_write(archiver, out_file, extracting->stored_size, extracting->offset, START_OF_FILE, READ_WRITE_UNCOMPRESSED, NULL);
        else
            read_write(archiver, out_file, extracting->stored_size, extracting->offset, START_OF_FILE, READING_COMPRESSED, extracting);

        edit_dir_member(extracting, DONT_CHANGE, !OVERWRITE, DONT_CHANGE, DONT_CHANGE);

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
    struct dir_member_t *dir_target = NULL;

    // Caso o usuário tenha passado "null" como "target", movemos o membro para a 1 posição
    if (strcmp(target, "null") == 0) {
        struct dir_member_t mock;
        dir_target = &mock;
        mock.order = -1;
        mock.stored_size = 0;
        mock.offset = dir_size * DIR_MEMBER_SIZE;
    }
    else
        dir_target = find_by_name(dir_members, target, dir_size);

    if (dir_target == NULL || dir_member_to_move == NULL) {
        if (dir_members != NULL)
            free_dir_members(&dir_members, dir_size);
        return MEMBER_NOT_FOUND;
    }

    // Caso o usuário tenha tentando mover o membro para o mesmo lugar, não faça nada
    if (dir_member_to_move->order == dir_target->order + 1) {
        if (dir_members != NULL)
            free_dir_members(&dir_members, dir_size);
        return OK;
    }

    int target_end = dir_target->offset + dir_target->stored_size;

    // Verifica se a movimentação é da esquerda para direita, ou o contrário
    if (dir_member_to_move->order < dir_target->order && strcmp(target, "null") != 0) {
        // Move o membro para trás do target
        move_and_shift_member(archiver, dir_member_to_move, target_end, FORWARD);

        fix_offsets(dir_members, dir_size, -dir_member_to_move->stored_size,dir_member_to_move->order + 1, dir_target->order + 1);
        fix_order(dir_members, dir_size, dir_member_to_move->order + 1, dir_target->order + 1, -1);
        target_end -= dir_member_to_move->stored_size;
    } else {
        // Move o membro para trás do target
        move_and_shift_member(archiver, dir_member_to_move, target_end, BACKWARD);

        // Conserta dados
        fix_offsets(dir_members, dir_size, dir_member_to_move->stored_size,dir_target->order + 1, dir_member_to_move->order);
        fix_order(dir_members, dir_size, dir_target->order + 1, dir_member_to_move->order, 1);
    }

    edit_dir_member(dir_member_to_move, DONT_CHANGE, !OVERWRITE, target_end, dir_target->order + 1);

    // Atualiza o diretório do archiver
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

        // Move o membro a ser removido para o final do archiver
        move_and_shift_member(archiver, current_removing_member, archiver_size, FORWARD);
        // Remove o membro do diretório
        dir_members = remove_by_name(dir_members, current_removing_member->name, &dir_size);
        // Remove o conteúdo do membro do archiver
        ftruncate(fileno(archiver), archiver_size - member_size);
    }

    // Caso o tamanho do diretório seja 0, o arquiver está vazio
    if (dir_size == 0) {
        ftruncate(fileno(archiver), START_OF_FILE);
        if (dir_members != NULL)
            free_dir_members(&dir_members, dir_size);
        return OK;
    }

    // Atualiza o diretório do archiver
    write_directory(archiver, dir_members, dir_size, -removing_size);

    if (dir_members != NULL)
        free_dir_members(&dir_members, dir_size);

    return OK;
}


char option_z(FILE *archiver, char **derivate_members, int derivate_size, char *archiver_name) {
    struct dir_member_t **dir_members = NULL;
    struct dir_member_t **derivate_members_list = NULL;
    int derivated_members_size = 0;
    int dir_size = 0;
    int derivate_bytes = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (is_empty(archiver))
        return ARCHIVE_IS_BLANK;

    read_dir_members(archiver, &dir_members, &dir_size);

    // TODO TODO TODO
    FILE *derivated_archiver = fopen(archiver_name, "w+b");

    for (int i = 0; i < derivate_size; i++) {
        struct dir_member_t *current_derivate_member = find_by_name(dir_members, derivate_members[i], dir_size);

        if (current_derivate_member == NULL) {
            if (dir_members != NULL)
                free_dir_members(&dir_members, dir_size);

            fclose(derivated_archiver);

            return MEMBER_NOT_FOUND;
        }

        int member_size = current_derivate_member->stored_size;

        // Aumenta e adiciona o membro no diretório
        derivated_members_size++;
        derivate_members_list = realloc(derivate_members_list, derivated_members_size * sizeof(struct dir_member_t *));
        derivate_members_list[derivated_members_size - 1] = current_derivate_member;

        
        read_write(archiver, derivated_archiver, member_size, current_derivate_member->offset, derivate_bytes, READ_WRITE_UNCOMPRESSED, NULL);

        derivate_members_list[derivated_members_size - 1]->offset = derivate_bytes;

        derivate_bytes += member_size;
    }

    // Atualiza o diretório do archiver
    write_directory(derivated_archiver, derivate_members_list, derivated_members_size, derivated_members_size);

    return OK;
}
