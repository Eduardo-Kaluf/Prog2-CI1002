#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "logger.h"


void setup_args(char ***args, int *arg_size) {
    if (SHIFT_AMOUNT >= *arg_size) {
        *args = NULL;
        *arg_size = 0;
        return;
    }

    *arg_size -= SHIFT_AMOUNT;
    *args = *args + SHIFT_AMOUNT;
}

int graceful_shutdown(FILE* archiver, int error, FILE* backup, char *backup_name) {
    log_error(error, NULL);
    if (archiver != NULL)
        fclose(archiver);
    if (backup != NULL)
        fclose(backup);
    if (backup_name != NULL)
        remove(backup_name);

    exit(error);
    // TODO TODO TODO
}

int has_duplicates(char *arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (strcmp(arr[i], arr[j]) == 0)
                return 1;
        }
    }
    return 0;
}
