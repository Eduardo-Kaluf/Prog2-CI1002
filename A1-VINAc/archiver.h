#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <stdio.h>


char option_i(FILE *archiver, char **new_members, int append_size, int compress);

char option_c(FILE *archiver);

char option_x(FILE *archiver, char **members_to_extract, int extraction_size);

char option_m(FILE *archiver, char *member_to_move, char *target);

char option_r(FILE *archiver, char **removing_members, int removing_size);

#endif
