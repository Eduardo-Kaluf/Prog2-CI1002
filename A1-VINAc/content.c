//
// Created by eduardo-kaluf on 19/04/25.
//

#include "content.h"

#include <stdio.h>

#include "dir_member.h"
#include "utils.h"

void move_member(FILE *archiver, struct dir_member_t *member_to_move, int offset_to) {
    char buffer[BUFFER_SIZE];

    fseek(archiver, member_to_move->offset, SEEK_SET);
    fread(buffer, 1, member_to_move->stored_size, archiver);

    fseek(archiver, offset_to, SEEK_SET);
    fwrite(buffer, 1, member_to_move->stored_size, archiver);

    fseek(archiver, 0, SEEK_SET);
}


// ESSA FUNÇÃO DEVE FAZER FTRUNCATE TAMBEM
void move_chunks(FILE *archiver, int start, int finish, int write_position) {
    char buffer[BUFFER_SIZE];

    for (long i = start; i < finish; ) {
        long chunk_start = i;

        int bytes_to_read = BUFFER_SIZE;

        if (chunk_start + BUFFER_SIZE > finish)
            bytes_to_read = finish - chunk_start;

        fseek(archiver, chunk_start, SEEK_SET);
        fread(buffer, 1, bytes_to_read, archiver);

        fseek(archiver, write_position, SEEK_SET);
        fwrite(buffer, 1, bytes_to_read, archiver);

        write_position += bytes_to_read;
        i += bytes_to_read;
    }

    fseek(archiver, 0, SEEK_SET);
}
