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

    // O archiver passado precisa ser um ".vc"
    if (strlen(argv[ARCHIVER_POSITION]) < MIN_ARCHIVER_SIZE || !checks_extension(argv[ARCHIVER_POSITION]))
        return graceful_shutdown(NULL, ARCHIVER_IS_NOT_VALID, NULL, NULL);

    // Aceita argumentos apenas do tipo "-x"
    if (strlen(argv[OPTION_POSITION]) != OPTION_SIZE || argv[OPTION_POSITION][HYPHEN_POSITION] != '-')
        return graceful_shutdown(NULL, OPTION_IS_NOT_CORRECT, NULL, NULL);


    // Pega a opção do usuário
    char option = argv[OPTION_POSITION][OPTION_CHAR];

    char *archiver_name;
    if (option == 'z') {
        int archiver_name_size = strlen(argv[ARCHIVER_POSITION]) - 3;
        archiver_name = malloc(archiver_name_size + 5);
        memcpy(archiver_name, argv[ARCHIVER_POSITION], archiver_name_size);
        strcat(archiver_name, "_z.vc");
    }

    // Caso o archiver não exista, ele é criado
    FILE *archiver = fopen(argv[ARCHIVER_POSITION], "r+b");
    if (archiver == NULL) {
        if (option == 'z')
            graceful_shutdown(archiver, NO_ARCHIVER_FOR_Z, NULL, NULL);
        else
            archiver = fopen(argv[ARCHIVER_POSITION], "w+b");
    }

    // exclui os args desnecessários
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
        case 'i':
            if (argc <= MIN_MEMBERS)
                return graceful_shutdown(archiver, NOT_ENOUGH_PARAMETERS, NULL, NULL);

            backup = create_backup(archiver, &backup_name);

            error_code = option_i(archiver, argv, argc, COMPRESSED);

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
        case 'z':
            printf("%d", argc);
            error_code = option_z(archiver, argv, argc, archiver_name);
            free(archiver_name);
            break;
        default:
            error_code = OPTION_DOES_NOT_EXIST;
            break;
    }

    // Ambiente de homologação
    #if HOMOLOG
        if (error_code == OK) {
            option_c(archiver);
            log_content(archiver);
        }
    #endif

    graceful_shutdown(archiver, error_code, backup, &backup_name);
}