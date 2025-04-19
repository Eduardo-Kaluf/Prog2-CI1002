#ifndef DIR_MEMBER_H
#define DIR_MEMBER_H

#include <stdio.h>
#include <sys/types.h>

struct dir_member_t {
    char name[128];
    int original_size;
    int stored_size;
    uid_t uid;
    long int last_modification;
    unsigned int order;
    int offset; 
};

struct dir_member_t *create_dir_member(char *member_name, int compressed_size, int offset, int pos);

void log_member(struct dir_member_t *dir_member);

#endif
