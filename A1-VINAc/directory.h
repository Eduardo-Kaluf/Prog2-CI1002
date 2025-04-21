#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>

void read_dir_members(FILE *archiver, struct dir_member_t ***dir_members, int *dir_member_num);

void write_directory(FILE *archiver, struct dir_member_t **dir_members, int total_size, int append_size);

void remove_by_name(struct dir_member_t ***dir_members, char *target, int *dir_size);

void order_dir_members(struct dir_member_t **v, int append_size);

struct dir_member_t *find_by_name(struct dir_member_t **dir_members, char *target, int dir_size);

void fix_offsets(struct dir_member_t **dir_members, int n, int offset_fix, int order_start, int order_end);

void fix_order(struct dir_member_t **dir_members, int n, int order_start, int order_end, int order_increment);

#endif
