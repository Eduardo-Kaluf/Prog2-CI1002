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


void move_member(FILE *archiver, struct dir_member_t *member_to_move, int offset_to) {
    char buffer[BUFFER_SIZE];
    int bytes_remaining = member_to_move->stored_size;
    long original_offset = member_to_move->offset;

    fseek(archiver, original_offset, SEEK_SET);

    fseek(archiver, offset_to, SEEK_SET);

    while (bytes_remaining > 0) {
        int bytes_to_copy = (bytes_remaining < BUFFER_SIZE) ? bytes_remaining : BUFFER_SIZE;

        fseek(archiver, original_offset, SEEK_SET);
        fread(buffer, 1, bytes_to_copy, archiver);

        fseek(archiver, offset_to, SEEK_SET);
        fwrite(buffer, 1, bytes_to_copy, archiver);

        original_offset += bytes_to_copy;
        offset_to += bytes_to_copy;
        bytes_remaining -= bytes_to_copy;
    }

    rewind(archiver);
}

void move_chunks(FILE *archiver, int start, int finish, int write_position) {
    char buffer[BUFFER_SIZE];

    int direction = 1;

    if (write_position > start && write_position < finish)
        direction = -1;


    long i = finish;

    if (direction == 1)
        i = start;

    while ((direction == 1 && i < finish) || (direction == -1 && i > start)) {
        long chunk_start;
        int bytes_to_read;

        if (direction == 1) {
            chunk_start = i;
            bytes_to_read = BUFFER_SIZE;
            if (chunk_start + BUFFER_SIZE > finish)
                bytes_to_read = finish - chunk_start;
        } else {
            chunk_start = i - BUFFER_SIZE;
            if (chunk_start < start) {
                chunk_start = start;
                bytes_to_read = i - start;
            } else {
                bytes_to_read = BUFFER_SIZE;
            }
        }

        fseek(archiver, chunk_start, SEEK_SET);
        fread(buffer, 1, bytes_to_read, archiver);

        long write_offset = write_position + (chunk_start - start);
        fseek(archiver, write_offset, SEEK_SET);
        fwrite(buffer, 1, bytes_to_read, archiver);

        i += direction * bytes_to_read;
    }

    fseek(archiver, 0, SEEK_SET);
}

void move_and_shift_member(FILE *archiver, struct dir_member_t *member, long target_end, long size, int forward) {

    if (forward) {
        move_chunks(archiver, target_end, size, target_end + member->stored_size);
        move_member(archiver, member, target_end);
        move_chunks(archiver, member->offset + member->stored_size, file_size(archiver), member->offset);
    } else {
        move_chunks(archiver, target_end, member->offset, size);
        move_member(archiver, member, target_end);
        move_chunks(archiver, size, file_size(archiver), target_end + member->stored_size);
    }

    ftruncate(fileno(archiver), size);
}

void read_write(FILE *read_file, FILE *write_file, long bytes_io, int read_offset, int write_offset, int mode, struct dir_member_t *member) {
    char *member_content = malloc(bytes_io * sizeof(char) * 2);
    char *compressed_content = malloc(bytes_io * sizeof(char));

    fseek(read_file, read_offset, SEEK_SET);
    fseek(write_file, write_offset, SEEK_SET);

    switch (mode) {
        case READ_WRITE_UNCOMPRESSED:
            fread(member_content, 1, bytes_io, read_file);
            fwrite(member_content, 1, bytes_io, write_file);

            break;
        case READING_COMPRESSED:
            if (member == NULL) {
                log_error(MEMBER_NOT_FOUND, NULL);
                return;
            }

            fread(compressed_content, 1, bytes_io, read_file);

            LZ_Uncompress((unsigned char *) compressed_content, (unsigned char *) member_content, bytes_io);
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

            if (compressed_size >= bytes_io) {
                fwrite(member_content, 1, bytes_io, write_file);
                edit_dir_member(member, DONT_CHANGE, DONT_CHANGE ,DONT_CHANGE);
            } else {
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
