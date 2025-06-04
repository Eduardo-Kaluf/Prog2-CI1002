#include <stdio.h>
#include <stdlib.h>

#include "files.h"
#include "utils.h"
#include "logger.h"
#include "dir_member.h"

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
            printf("Archiver está em branco\n");
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
        case DUPLICATED_MEMBER:
            printf("Membro duplicado\n");
            break;
        case ARCHIVER_IS_NOT_VALID:
            printf("Archiver não é válido, certifique-se que '.vc'\n");
            break;
        case NO_ARCHIVER_FOR_Z:
            printf("O .vc não existe para ser derivado porque não existe \n");
            break;
        default:
            printf("Erro desconhecido\n");
            break;
    }
}

void log_content(FILE *archiver) {
    struct dir_member_t first_member;
    int ch;

    if (is_empty(archiver))
        return;

    fread(&first_member, sizeof(struct dir_member_t), 1, archiver);

    fseek(archiver, first_member.offset, SEEK_SET);
    while ((ch = fgetc(archiver)) != EOF)
        putchar(ch);

    printf("\n");
}
