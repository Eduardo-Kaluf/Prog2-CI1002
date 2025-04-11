#include <stdlib.h>
#include "jukefila.h"


jukefila* criar_jukefila() {
    struct jukefila *f;


    if (!(f = malloc(sizeof(jukefila))))
        return NULL;

    f->inicio = NULL;
    f->final = NULL;
    
    return f;
}

void inserir_jukefila(pedido* elemento, jukefila* fila);
pedido* consumir_jukefila(jukefila* fila);
unsigned int contar_jukefila(jukefila* fila);
void destruir_jukefila(jukefila *fila);
