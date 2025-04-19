//
// Created by eduardo-kaluf on 19/04/25.
//

#ifndef CONTENT_H
#define CONTENT_H
#include <stdio.h>
#include "dir_member.h"

void move_member(FILE *archiver, struct dir_member_t *member_to_move, int offset_to);

void move_chunks(FILE *archiver, int start, int finish, int write_position);

#endif //CONTENT_H
