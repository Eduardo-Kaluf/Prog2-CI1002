#ifndef DIR_MEMBER_H
#define DIR_MEMBER_H

#include <sys/types.h>

// Estrutura que representa um membro do diretório
struct dir_member_t {
    char name[128];
    int original_size;
    int stored_size;
    uid_t uid;
    long int last_modification;
    unsigned int order;
    int offset; 
};

// Cria e inicializa um novo membro de diretório, retornando-o
// - member_name: nome do membro
// - compressed_size: tamanho do arquivo comprimido (DONT_CHANGE caso o arquivo não possua compressão)
// - offset: posição no arquivo onde o membro está armazenado
// - order: ordem no diretório
struct dir_member_t *create_dir_member(char *member_name, int compressed_size, int offset, int order);

// Exibe/loga as informações de um membro do diretório
// - dir_member: ponteiro para o membro que será exibido
void log_member(struct dir_member_t *dir_member);

// Edita as informações de um membro existente no diretório
// - dir_member: ponteiro para o membro a ser editado
// - compressed_size: novo tamanho armazenado (DONT_CHANGE caso não possua compressão)
// - offset: novo offset no arquivo (DONT_CHANGE caso não deva ser alterado)
// - order: nova posição no diretório (DONT_CHANGE caso não deva ser alterado)
void edit_dir_member(struct dir_member_t *dir_member, int compressed_size, int offset, int order);

#endif
