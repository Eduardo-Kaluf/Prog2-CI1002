#ifndef CONTENT_H
#define CONTENT_H

#include <stdio.h>

#include "dir_member.h"


void move_member(FILE *archiver, struct dir_member_t *member_to_move, int offset_to);

void move_chunks(FILE *archiver, int start, int finish, int write_position);

void move_and_shift_member(FILE *archiver, struct dir_member_t *member, long target_end, long size, int forward);

void read_write(FILE *read_file, FILE *write_file, long bytes_io, int read_offset, int write_offset, int mode, struct dir_member_t *member);

#endif
