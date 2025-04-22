#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define _GNU_SOURCE 1
#define DIR_MEMBER_SIZE sizeof(struct dir_member_t)
#define BUFFER_SIZE 1024
#define SHIFT_AMOUNT 3
#define MIN_ARGUMENTS 3
#define MIN_MEMBERS 0
#define NUM_EXPECTED_MEMBERS 2
#define MEMBER_TO_MOVE 0
#define TARGET 1
#define ARCHIVER_POSITION 2
#define OPTION_POSITION 1
#define HYPHEN_POSITION 0
#define OPTION_CHAR 1
#define OPTION_SIZE 2
#define COMPRESS 1
#define UNCOMPRESSED 0
#define START_OF_FILE 0
#define BLANK 0
#define DONT_CHANGE -1

enum Errors {
    OK,
    FILE_NOT_FOUND,
    FILE_IS_BLANK,
    ARCHIVE_IS_BLANK,
    MEMBER_NOT_FOUND,
    NOT_ENOUGH_PARAMETERS,
    OPTION_IS_NOT_CORRECT,
    WRONG_AMOUNT_OF_PARAMETERS,
    OPTION_DOES_NOT_EXIST,
    INTERNAL_ERROR,
    DUPLICATED_MEMBER
};

void setup_args(char ***args, int *arg_size);

int graceful_shutdown(FILE* archiver, int error, FILE* backup, char **backup_name);

int has_duplicates(char *arr[], int size);

#endif
