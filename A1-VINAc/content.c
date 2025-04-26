#define _GNU_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "dir_member.h"
#include "utils.h"
#include "content.h"
#include "lz.h"
#include "files.h"
#include "logger.h"


void move_member(FILE *archiver, struct dir_member_t *member, int offset_to) {
    int bytes_remaining = member->stored_size;
    long original_offset = member->offset;
    int bytes_to_copy;

    while (bytes_remaining > 0) {
        if (bytes_remaining < BUFFER_SIZE)
            bytes_to_copy = bytes_remaining;
        else
            bytes_to_copy = BUFFER_SIZE;

        read_write(archiver, archiver, bytes_to_copy, original_offset, offset_to, READ_WRITE_UNCOMPRESSED, NULL);

        original_offset += bytes_to_copy;
        offset_to += bytes_to_copy;
        bytes_remaining -= bytes_to_copy;
    }

    rewind(archiver);
}

void move_chunks(FILE *archiver, int start, int finish, int write_position) {
    int direction;
    long i;

    // Caso a escrita esteja dentro do intervalo original lemos os dados de trás para frente
    // Caso contrário começamos do início
    if (write_position > start && write_position < finish) {
        i = finish;
        direction = -1;
    }
    else {
        i = start;
        direction = 1;
    }

    // Continua movendo enquanto estivermos dentro dos limites, dependendo da direção
    while ((direction == 1 && i < finish) || (direction == -1 && i > start)) {
        long chunk_start;
        int bytes_to_read;

        if (direction == 1) {
            // Lendo do início para o fim
            chunk_start = i;
            bytes_to_read = BUFFER_SIZE;
            // Ajusta se ultrapassar o final
            if (chunk_start + BUFFER_SIZE > finish)
                bytes_to_read = finish - chunk_start;
        }
        else {
            // Lendo do fim até o início
            chunk_start = i - BUFFER_SIZE;
            if (chunk_start < start) {
                // Ajusta se ultrapassar o início
                chunk_start = start;
                bytes_to_read = i - start;
            }
            else
                bytes_to_read = BUFFER_SIZE;
        }

        long write_offset = write_position + (chunk_start - start);

        read_write(archiver, archiver, bytes_to_read, chunk_start, write_offset, READ_WRITE_UNCOMPRESSED, NULL);

        i += direction * bytes_to_read;
    }

    rewind(archiver);
}

void move_and_shift_member(FILE *archiver, struct dir_member_t *member, long offset_to, int forward) {

    long size = file_size(archiver);

    if (forward) {
        // Se o movimento for para frente:
        // Move os dados entre o novo local de escrita e o fim do membro atual
        move_chunks(archiver, offset_to, size, offset_to + member->stored_size);

        // Move o membro para a nova posição
        move_member(archiver, member, offset_to);

        // Em seguida move os dados após o membro para ajustar o gap deixado
        move_chunks(archiver, member->offset + member->stored_size, file_size(archiver), member->offset);
    } else {
        // Se o movimento for para trás:
        // Move os dados entre o novo local de escrita e o início do membro atual
        move_chunks(archiver, offset_to, member->offset, size);

        // Move o membro para a nova posição
        move_member(archiver, member, offset_to);

        // Move os dados restantes para fechar o espaço vazio
        move_chunks(archiver, size, file_size(archiver), offset_to + member->stored_size);
    }

    // Faz com que o arquivo tenha o mesmo tamanho que foi passado
    ftruncate(fileno(archiver), size);
}

void read_write(FILE *read_file, FILE *write_file, long bytes_io, int read_offset, int write_offset, int mode, struct dir_member_t *member) {
    char *member_content = malloc(bytes_io * sizeof(char) * 2);
    char *compressed_content = malloc(bytes_io * sizeof(char));

    fseek(read_file, read_offset, SEEK_SET);

    switch (mode) {
        case READ_WRITE_UNCOMPRESSED:
            fread(member_content, 1, bytes_io, read_file);
            fseek(write_file, write_offset, SEEK_SET);
            fwrite(member_content, 1, bytes_io, write_file);

            break;
        case READING_COMPRESSED:
            if (member == NULL) {
                log_error(MEMBER_NOT_FOUND, NULL);
                return;
            }

            fread(compressed_content, 1, bytes_io, read_file);

            LZ_Uncompress((unsigned char *) compressed_content, (unsigned char *) member_content, bytes_io);

            fseek(write_file, write_offset, SEEK_SET);
            fwrite(member_content, 1, member->original_size, write_file);

            edit_dir_member(member, DONT_CHANGE, DONT_CHANGE ,DONT_CHANGE);

            break;
        case WRITING_COMPRESSED:
            if (member == NULL) {
                log_error(MEMBER_NOT_FOUND, NULL);
                return;
            }

            fread(member_content, 1, bytes_io, read_file);

            int compressed_size = LZ_Compress((unsigned char *) member_content, (unsigned char *) compressed_content, bytes_io);

            fseek(write_file, write_offset, SEEK_SET);

            // Caso o arquivo comprimido seja maior que o arquivo descomprimido, guarda sem compressão
            // Atualiza a data de modificação e o tamanho do arquivo guardado
            if (compressed_size >= bytes_io) {
                fwrite(member_content, 1, bytes_io, write_file);
                edit_dir_member(member, DONT_CHANGE, DONT_CHANGE ,DONT_CHANGE);
            }
            else {
                fwrite(compressed_content, 1, compressed_size, write_file);
                edit_dir_member(member, compressed_size, DONT_CHANGE ,DONT_CHANGE);
            }

            break;
        default:
            log_error(INTERNAL_ERROR, NULL);
            break;
    }

    rewind(read_file);
    rewind(write_file);

    free(member_content);
    free(compressed_content);
}
