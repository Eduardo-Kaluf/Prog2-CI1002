#ifndef CONTENT_H
#define CONTENT_H

#include <stdio.h>

#include "dir_member.h"

// Copia um membro para o offset indicado;
// Não remove o membro do local original;
// - archiver: ponteiro para o arquivo onde os membros estão/serão armazenados
// - member: membro a ser movido
// - offset_to: posição para onde o membro será movido
void move_member(FILE *archiver, struct dir_member_t *member, int offset_to);

// Move um chunk [start, finish] para a posição de escrita indicada
// Cuida dos overlaps
// Não remove o chunk do local original;
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - start: posição inicial de onde os dados serão lidos
// - finish: posição final até onde serão lidos os dados
// - write_position: posição onde os dados devem ser reescritos
void move_chunks(FILE *archiver, int start, int finish, int write_position);

// Copia um membro para o offset indicado;
// Remove o membro do local original;
// Garante que o archiver permanecerá com o mesmo tamanho;
// Garante que nenhum gap existirá entre os membros
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - member: membro que será movido
// - offset_to: nova posição para o membro
// - forward:
//      - FORWARD: Movendo o membro para direita dele mesmo
//      - BACKWARD: Movendo o membro para esquerda dele mesmo
void move_and_shift_member(FILE *archiver, struct dir_member_t *member, long offset_to, int forward);

// Realiza a leitura de um arquivo para outro;
// - archiver: ponteiro para o arquivo onde os membros estão armazenados
// - write_file: ponteiro para o arquivo onde os dados serão escritos
// - bytes_io: quantidade de bytes a serem transferidos
// - read_offset: posição de leitura no arquivo de origem
// - write_offset: posição de escrita no arquivo de destino
// Mode:
//       O membro deve ser passado apenas no segundo e terceiro caso, para que seu tamanho seja atualizado;
//     - READ_WRITE_UNCOMPRESSED: Realiza a leitura de um arquivo sem compressão e escreve ele da mesma forma;
//     - READING_COMPRESSED: Realiza a leitura de um membro com compressão e o escreve sem compressão;
//     - WRITING_COMPRESSED: Realiza a leitura de um membro sem compressão e o escreve com compressão;
// - member: ponteiro para o membro que terá seu tamanho e data de modificação alterados
void read_write(FILE *read_file, FILE *write_file, long bytes_io, int read_offset, int write_offset, int mode, struct dir_member_t *member);

#endif
