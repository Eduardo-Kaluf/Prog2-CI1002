#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <sys/types.h>

void read_dir_members(FILE *archiver, struct dir_member_t ***dir_members, int *dir_member_num);
void fix_offsets(struct dir_member_t **dir_members, int n, int offset_fix);


#endif
