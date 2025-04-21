#ifndef FILES_H
#define FILES_H

enum Mode {
    READ_WRITE_UNCOMPRESSED,
    READING_COMPRESSED,
    WRITING_COMPRESSED,
};


long file_size(FILE *file);

long is_empty(FILE *file);

FILE *create_backup(FILE *archiver, char **backup_name);

void restore_backup(FILE *archiver, FILE *backup, char *backup_name);

#endif
