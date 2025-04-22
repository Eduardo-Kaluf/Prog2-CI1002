#define _GNU_SOURCE 1

// Define o enviroment
#ifndef HOMOLOG
    #define PROD
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"
#include "archiver.h"
#include "files.h"
#include "logger.h"


int main(int argc, char **argv) {
    FILE *backup = NULL;
    char *backup_name = NULL;
    char error_code;

    // Uma opção e o archiver devem ser obrigatoriamente passados
    if (argc < MIN_ARGUMENTS)
        return graceful_shutdown(NULL, NOT_ENOUGH_PARAMETERS, NULL, NULL);

    // Aceita argumentos apenas do tipo "-x"
    if (strlen(argv[OPTION_POSITION]) != OPTION_SIZE || argv[OPTION_POSITION][HYPHEN_POSITION] != '-')
        return graceful_shutdown(NULL, OPTION_IS_NOT_CORRECT, NULL, NULL);

    // Caso o archiver não exista, ele é criado
    FILE *archiver = fopen(argv[ARCHIVER_POSITION], "r+b");

    if (archiver == NULL)
        archiver = fopen(argv[ARCHIVER_POSITION], "w+b");

    // Pega a opção do usuário e exclui os args desnecessários
    char option = argv[OPTION_POSITION][OPTION_CHAR];

    setup_args(&argv, &argc);

    if (has_duplicates(argv, argc))
        return graceful_shutdown(archiver, DUPLICATED_MEMBER, NULL, NULL);

    switch (option) {
        case 'p':
            if (argc <= MIN_MEMBERS)
                return graceful_shutdown(archiver, NOT_ENOUGH_PARAMETERS, NULL, NULL);

            backup = create_backup(archiver, &backup_name);

            error_code = option_i(archiver, argv, argc, UNCOMPRESSED);

            if (error_code != OK)
                restore_backup(archiver, &backup, &backup_name);

            break;
        case 'z':
            if (argc <= MIN_MEMBERS)
                return graceful_shutdown(archiver, NOT_ENOUGH_PARAMETERS, NULL, NULL);

            backup = create_backup(archiver, &backup_name);

            error_code = option_i(archiver, argv, argc, COMPRESS);

            if (error_code != OK)
                restore_backup(archiver, &backup, &backup_name);

            break;
        case 'm':

            if (argc != NUM_EXPECTED_MEMBERS)
                return graceful_shutdown(archiver, WRONG_AMOUNT_OF_PARAMETERS, NULL, NULL);

            error_code = option_m(archiver, argv[MEMBER_TO_MOVE], argv[TARGET]);

            break;
        case 'x':
            error_code = option_x(archiver, argv, argc);
            break;
        case 'r':
            if (argc <= MIN_MEMBERS)
                return graceful_shutdown(archiver, NOT_ENOUGH_PARAMETERS, NULL, NULL);

            backup = create_backup(archiver, &backup_name);

            error_code = option_r(archiver, argv, argc);

            if (error_code != OK)
                restore_backup(archiver, &backup, &backup_name);

            break;
        case 'c':
            if (argc != 0)
                return graceful_shutdown(archiver, WRONG_AMOUNT_OF_PARAMETERS, NULL, NULL);

            error_code = option_c(archiver);
            break;
        default:
            error_code = OPTION_DOES_NOT_EXIST;
            break;
    }

    #if HOMOLOG
        if (error_code == OK) {
            option_c(archiver);
            log_content(archiver);
        }
    #endif

    graceful_shutdown(archiver, error_code, backup, &backup_name);
}