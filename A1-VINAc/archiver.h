#ifndef ARCHIVER_H
#define ARCHIVER_H

#include "dir_member.h"

void option_ip(FILE *archiver, char **new_members, int n);
void order_dir_members(struct dir_member_t **v, int n);
void move_to_end(FILE *archiver, int start, int finish, int new_offset);
void create_gap(FILE *archiver, long pos, long shift_len);
void option_c(FILE *archiver);
void option_x(FILE *archiver);

#endif
