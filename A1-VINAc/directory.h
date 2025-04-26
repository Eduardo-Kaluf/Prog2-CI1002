#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>

// Lê todos os membros do diretório armazenado dentro do arquivo
// - archiver: arquivo de onde os membros serão lidos
// - dir_members: ponteiro para vetor de ponteiros que receberá os membros lidos
// - dir_size: variável que receberá o número de membros lidos
void read_dir_members(FILE *archiver, struct dir_member_t ***dir_members, int *dir_size);

// Escreve o diretório no archiver
// - archiver: arquivo onde os membros serão escritos
// - dir_members: vetor de ponteiros para os membros
// - total_size: número total de membros no vetor
// - append_size: número de membros que foram adicionados ou removidos
void write_directory(FILE *archiver, struct dir_member_t **dir_members, int total_size, int append_size);

// Remove um membro do vetor de membros pelo seu nome e retorna um novo vetor sem ele
// - dir_members: vetor de ponteiros para os membros
// - target: nome do membro a ser removido
// - dir_size: ponteiro para o número de membros
struct dir_member_t **remove_by_name(struct dir_member_t **dir_members, char *target, int *dir_size);

// Ordena os membros do diretório com base na ordem
// - dir_members: vetor de ponteiros para os membros
// - dir_size: tamanho do diretório
void order_dir_members(struct dir_member_t **dir_members, int dir_size);

// Busca um membro pelo nome dentro do vetor de membros, retorna um ponteiro para o membro encontrado ou NULL
// - dir_members: vetor de ponteiros para os membros
// - target: nome do membro a ser encontrado
// - dir_size: tamanho do diretório
struct dir_member_t *find_by_name(struct dir_member_t **dir_members, char *target, int dir_size);

// Ajusta os offsets de membros no vetor de diretório [order_start, order_end)
// - dir_members: vetor de ponteiros para os membros
// - dir_size: tamanho do diretório
// - offset_fix: valor a ser somado ou subtraído do offset
// - order_start: ordem a partir da qual aplicar a correção
// - order_end: ordem final para aplicar a correção
void fix_offsets(struct dir_member_t **dir_members, int dir_size, int offset_fix, int order_start, int order_end);

// Ajusta a ordem dos membros no vetor [order_start, order_end)
// Útil após inserções ou remoções para manter a ordem lógica correta
// Parâmetros:
// - dir_members: vetor de ponteiros para os membros
// - dir_size: tamanho do diretório
// - order_start: ordem inicial de onde começa a ajustar
// - order_end: ordem final até onde ajustar
// - order_increment: valor que será adicionado ou subtraído da ordem
void fix_order(struct dir_member_t **dir_members, int dir_size, int order_start, int order_end, int order_increment);

// Libera a memória alocada para todos os membros do diretório e do próprio vetor
// - dir_members: ponteiro para vetor de ponteiros que será liberado
// - dir_size: número de membros a serem liberados
void free_dir_members(struct dir_member_t ***dir_members, int dir_size);

#endif
