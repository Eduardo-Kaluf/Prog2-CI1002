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
#define MIN_ARCHIVER_SIZE 4
#define OPTION_POSITION 1
#define HYPHEN_POSITION 0
#define OPTION_CHAR 1
#define OPTION_SIZE 2
#define COMPRESSED 1
#define UNCOMPRESSED 0
#define START_OF_FILE 0
#define BLANK 0
#define DONT_CHANGE -1
#define FORWARD 1
#define BACKWARD 0
#define OVERWRITE 1

// Enumeração de códigos de erro
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
    DUPLICATED_MEMBER,
    ARCHIVER_IS_NOT_VALID
};

// Remove os três primeiros argumentos do argv
// - args: argv
// - arg_size: argc
void setup_args(char ***args, int *arg_size);

// Finaliza o programa de forma segura após um erro
// Pode restaurar o backup se necessário, fechar arquivos abertos e limpar memória
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - error: código de erro (vindo do enum Errors)
// - backup: arquivo de backup (pode ser restaurado ou descartado)
// - backup_name: nome do arquivo de backup
int graceful_shutdown(FILE* archiver, int error, FILE* backup, char **backup_name);

// Verifica se há duplicatas em um vetor de strings, 1 se houver elementos duplicados, 0 caso contrário
// - arr: vetor de strings
// - size: número de elementos no vetor
int has_duplicates(char *arr[], int size);

// Verifica se o archiver passado é um ".vc", 1 se a extensão for válida, 0 caso contrário
// - str: string contendo o nome do arquivo
int checks_extension(char *archiver_name);

#endif
