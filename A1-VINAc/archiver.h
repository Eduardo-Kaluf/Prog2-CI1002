#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <stdio.h>

// TODAS A FUNÇÕES RETORNAM CÓDIGOS DE ERRO QUE DEVEM SER TRATADAS PELO GRACEFUL_SHUTDOWN

// Insere/acrescenta um ou mais membros com, ou sem compressão ao archive;
// Caso o membro já exista no archive, ele será substituído;
// Novos membros são inseridos respeitando a ordem da linha de comando, ao final do archive;
// - archiver: ponteiro para o arquivo onde os membros estão/serão armazenados
// - new_members: lista de nomes dos novos membros a serem inseridos
// - append_size: quantidade de novos membros
// - compress:
//      - COMPRESSED: Insere com compressão
//      - UNCOMPRESSED: Insere sem compressão
char option_i(FILE *archiver, char **new_members, int append_size, int compress);

// Lista o conteúdo de archive em ordem;
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
char option_c(FILE *archiver);

// Extrai os membros indicados de archive;
// Se os membros não forem indicados, todos são extraídos;
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - members_to_extract: lista de nomes dos membros que devem ser extraídos
// - extraction_size: quantidade de membro que serão extraídos
char option_x(FILE *archiver, char **members_to_extract, int extraction_size);

// Move o membro indicado na linha de comando para imediatamente depois do membro "target" existente em archive;
// Caso o "target" seja passado como "null", o membro será movimentado para a primeira posição do archive;
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - member_to_move: nome do membro que será movido
// - target: nome do membro de referência
char option_m(FILE *archiver, char *member_to_move, char *target);

// Remove os membros indicados de archive;
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - removing_members: lista de nomes dos membros que serão removidos
// - removing_size: quantidade de membros que serão removidos
char option_r(FILE *archiver, char **removing_members, int removing_size);

#endif
