#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


void log_error(int error, char *member_name) {
    switch (error) {
        case OK:
            printf("OK\n");
            break;
        case FILE_NOT_FOUND:
            printf("Arquivo não encontrado\n");
            break;
        case FILE_IS_BLANK:
            printf("Arquivo está em branco\n");
            break;
        case ARCHIVE_IS_BLANK:
            printf("Arquivo de arquivamento está em branco\n");
            break;
        case MEMBER_NOT_FOUND:
            if (member_name != NULL)
                printf("Membro não encontrado: %s\n", member_name);
            else
                printf("Membro não encontrado\n");
            break;
        case NOT_ENOUGH_PARAMETERS:
            printf("Parâmetros insuficientes\n");
            break;
        case OPTION_IS_NOT_CORRECT:
            printf("Opção incorreta\n");
            break;
        case WRONG_AMOUNT_OF_PARAMETERS:
            printf("Quantidade incorreta de parâmetros\n");
            break;
        case OPTION_DOES_NOT_EXIST:
            printf("Opção não existe\n");
            break;
        case INTERNAL_ERROR:
            printf("Erro interno do servidor\n");
            break;
        default:
            printf("Erro desconhecido\n");
            break;
    }
}
