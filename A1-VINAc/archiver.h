#ifndef ARCHIVER_H
#define ARCHIVER_H

void option_ip(FILE *archiver, char **new_members, int append_size);

void option_c(FILE *archiver);

void option_x(FILE *archiver, char **members_to_extract, int extraction_size);

void option_m(FILE *archiver, char *member_to_move, char *target);

void option_r(FILE *archiver, char **removing_members, int removing_size);

long is_empty(FILE *archiver);

long file_size(FILE *archiver);

void read_write(FILE *read_file, FILE *write_file, int member_size, int write_offset);

#endif
