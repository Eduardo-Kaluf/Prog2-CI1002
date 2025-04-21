#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

int graceful_shutdown(FILE* archiver, int error) {
    log_error(error, NULL);
    if (archiver != NULL)
        fclose(archiver);
    exit(error);
    // TODO TODO TODO
}
