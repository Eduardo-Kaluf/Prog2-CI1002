#define _GNU_SOURCE 1

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "archiver.h"
#include "utils.h"
#include "files.h"
#include "content.h"
#include "dir_member.h"


long file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

long is_empty(FILE *file) {
    return file_size(file) == 0;
}

FILE *create_backup(FILE *archiver, char **backup_name) {
    *backup_name = malloc(32);
    sprintf(*backup_name, "%ld.vc", time(NULL));

    FILE *backup = fopen(*backup_name, "w+b");
    read_write(archiver, backup, file_size(archiver), START_OF_FILE, START_OF_FILE, READ_WRITE_UNCOMPRESSED, NULL);

    return backup;
}

void restore_backup(FILE *archiver, FILE *backup, char *backup_name) {
    ftruncate(fileno(archiver), START_OF_FILE);
    read_write(backup, archiver, file_size(backup), START_OF_FILE, START_OF_FILE, READ_WRITE_UNCOMPRESSED, NULL);

    fclose(backup);
    remove(backup_name);
}
