#include <stdlib.h>
#include "jukefila.h"
#include <stdio.h>

jukefila* criar_jukefila() {
    struct jukefila *f;


    if (!(f = malloc(sizeof(jukefila))))
        return NULL;

    f->inicio = NULL;
    f->final = NULL;
    
    return f;
}

void inserir_jukefila(pedido* elemento, jukefila* fila) {
    pedido *p;


    if (fila->inicio == NULL) {
        fila->inicio = elemento;
        fila->final = elemento;
        elemento->proximo = NULL;
        elemento->anterior = NULL;
        return;
    }

    if (elemento->valor > fila->inicio->valor) {
        fila->inicio->anterior = elemento;
        elemento->proximo = fila->inicio;
        elemento->anterior = NULL;
        fila->inicio = elemento;
        return;
    }

    if (elemento->valor <= fila->final->valor) {
        fila->final->proximo = elemento;
        elemento->anterior = fila->final;
        elemento->proximo = NULL;
        fila->final = elemento;
        return;
    }

    elemento->proximo = fila->inicio;
    p = fila->inicio->proximo;
    
    // Caso genérico
    while (elemento->valor <= p->valor) {
        elemento->proximo = p;
        p = p->proximo;
    }
    
    
    p->anterior->proximo = elemento;
    elemento->proximo = p;
    elemento->anterior = p->anterior;
    p->anterior = elemento;
}


pedido* consumir_jukefila(jukefila* fila) {
    pedido* pedido_consumido;

    if (contar_jukefila(fila) == 0)
        return NULL;
    
    if (contar_jukefila(fila) == 1) {
        pedido_consumido = fila->inicio;
        fila->inicio = NULL;
        fila->final = NULL;
        return pedido_consumido;
    }

    pedido_consumido = fila->inicio;
    fila->inicio = fila->inicio->proximo;
    fila->inicio->anterior = NULL;

    return pedido_consumido;
}


unsigned int contar_jukefila(jukefila* fila) {
    pedido* aux;

    if (!(fila->inicio))
        return 0;


    aux = fila->inicio;

    int i = 1;
    for (pedido *aux = fila->inicio; aux != fila->final; aux = aux->proximo)
        i++;

    return i;
}

void destruir_jukefila(jukefila *fila) {
    for (pedido *aux = fila->inicio; aux != NULL; aux = aux->proximo)
        destruir_pedido(consumir_jukefila(fila));

    free(fila);
}
