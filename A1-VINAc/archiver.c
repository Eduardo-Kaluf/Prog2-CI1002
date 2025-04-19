#include <stdlib.h>
#include <stdio.h>

#include "dir_member.h"
#include "directory.h"
#include "archiver.h"

#define DIR_MEMBER_SIZE sizeof(struct dir_member_t)

long is_empty(FILE *archiver) {
    fseek(archiver, 0, SEEK_END);
    long size = ftell(archiver); 
    rewind(archiver);
    return size == 0;
}

void option_ip(FILE *archiver, char **new_members, int append_size) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;
    int total_size = append_size;

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
        fseek(archiver, 0, SEEK_END);
        long offset = ftell(archiver);

        FILE *new_member = fopen(new_members[i], "rb");

        fseek(new_member, 0, SEEK_END);
        long member_size = ftell(new_member);
        char *member_content = malloc(member_size);

        fseek(new_member, 0, SEEK_SET);
        fread(member_content, 1, member_size, new_member);

        fwrite(member_content, 1, member_size, archiver);

        dir_members[dir_size + i] = create_dir_member(new_members[i], -1, offset, dir_size + i);

        fclose(new_member);
        free(member_content);
    }

    // Atualiza o diretório do archive
    fseek(archiver, 0, SEEK_SET);

    order_dir_members(dir_members, total_size);

    create_gap(archiver, dir_members[0]->offset, append_size * DIR_MEMBER_SIZE );

    fix_offsets(dir_members, total_size, append_size * DIR_MEMBER_SIZE );

    for (int i = 0; i < total_size; i++)
        fwrite(dir_members[i], DIR_MEMBER_SIZE , 1, archiver);

    fclose(archiver);
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

void option_x(FILE *archiver) {
    struct dir_member_t **dir_members = NULL;
    int dir_size = 0;

    // Caso o archive não seja vazio, le os membros do diretório
    if (!is_empty(archiver))
        read_dir_members(archiver, &dir_members, &dir_size);

    const long buffer_size = 1024;
    char buffer[buffer_size];
    fseek(archiver, 0, SEEK_END);
    long size = ftell(archiver);
    int bytes_to_read;

    for (int i = 0; i < dir_size; i++) {
        FILE * file_test = fopen(dir_members[i]->name, "w");

        if (i != dir_size - 1)
            bytes_to_read = dir_members[i + 1]->offset - dir_members[i]->offset;
        else
            bytes_to_read = size - dir_members[i]->offset;

        fseek(archiver, dir_members[i]->offset, SEEK_SET);

        fread(buffer, 1, bytes_to_read, archiver);

        fwrite(buffer, 1, bytes_to_read, file_test);
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

// void move_to_end(FILE *archiver, int start, int finish, int new_offset) {
//     fseek(archiver, 0, finish);
//     long filesize = ftell(archiver);

//     // SE PRECISAR TROCAR PELO TAMANHO DO MAIOR BUFFER
//     const int buffer_size = 1024;
//     char buffer[buffer_size];

//     int chunk = 0;

//     for (long i = start; i < finish;) {
//         if (start + buffer_size < finish)
//             chunk += buffer_size;
//         else
//             chunk += finish - i;

//         fseek(archiver, i, SEEK_SET);
//         fread(buffer, 1, chunk, archiver);
        
//         fseek(archiver, 0, SEEK_END);
//         fwrite(buffer, 1, chunk, archiver);


//         i += buffer_size;
//     }

//     fflush(archiver);
//     fclose(archiver);

// }

void create_gap(FILE *archiver, long pos, long shift_len) {
    fseek(archiver, 0, SEEK_END);
    long filesize = ftell(archiver);

    const long buffer_size = 1024;
    char buffer[buffer_size];

    for (long i = filesize; i > pos; ) {
        long chunk_start = i - buffer_size;

        if (chunk_start < pos)
            chunk_start = pos;

        long bytes_to_read = i - chunk_start;

        fseek(archiver, chunk_start, SEEK_SET);
        fread(buffer, 1, bytes_to_read, archiver);

        fseek(archiver, chunk_start + shift_len, SEEK_SET);
        fwrite(buffer, 1, bytes_to_read, archiver);

        i -= bytes_to_read;
    }

    fseek(archiver, 0, SEEK_SET);
}