#ifndef FILES_H
#define FILES_H

enum Mode {
    READ_WRITE_UNCOMPRESSED,
    READING_COMPRESSED,
    WRITING_COMPRESSED,
    READING_AND_OVERWRITTEN_COMPRESSED
};

// Retorna o tamanho do arquivo
// - file: ponteiro para o arquivo
long file_size(FILE *file);

// Verifica se o arquivo está vazio, 1 se o arquivo estiver vazio, 0 caso contrário
// - file: ponteiro para o arquivo
long is_empty(FILE *file);

// Cria um backup do archiver, retorna um ponteiro para o arquivo de backup criado
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - backup_name: ponteiro para uma string onde o nome do backup será armazenado
FILE *create_backup(FILE *archiver, char **backup_name);

// Restaura o archiver a partir de um backup, libera a memória do backup e de seu nome
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - backup: ponteiro para o arquivo de backup
// - backup_name: ponteiro para o nome do backup
void restore_backup(FILE *archiver, FILE **backup, char **backup_name);

#endif
