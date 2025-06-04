#ifndef LOGGER_H
#define LOGGER_H

// Loga uma mensagem de erro
// - error:
//      - OK,
//      - FILE_NOT_FOUND,
//      - FILE_IS_BLANK,
//      - ARCHIVE_IS_BLANK,
//      - MEMBER_NOT_FOUND,
//      - NOT_ENOUGH_PARAMETERS,
//      - OPTION_IS_NOT_CORRECT,
//      - WRONG_AMOUNT_OF_PARAMETERS,
//      - OPTION_DOES_NOT_EXIST,
//      - INTERNAL_ERROR,
//      - DUPLICATED_MEMBER,
//      - ARCHIVER_IS_NOT_VALID
// - member_name: nome do membro relacionado ao erro, utilizado apenas para MEMBER_NOT_FOUND
void log_error(int error, char *member_name);

// Exibe o conteúdo atual do archiver
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
void log_content(FILE *archiver);

#endif
