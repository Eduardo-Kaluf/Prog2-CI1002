// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

// A COMPLETAR

#include <stdlib.h>
#include <stdio.h>

#include "fprio.h"

struct fprio_t *fprio_cria () {
    struct fprio_t *l;

    if (!(l = malloc(sizeof(struct fprio_t))))
        return NULL;

    l->prim = NULL;
    l->num = 0;

    return l;
}

struct fprio_t *fprio_destroi (struct fprio_t *f) {
    struct fpnodo_t *p;

    // Fila nula
    if (f == NULL)
        return NULL;

    // Caso genérico
    while (f->prim != NULL) {
        p = f->prim;
        f->prim = f->prim->prox;
        free(p->item);
        free(p);
    }

    free(f);

    return NULL;
}

/*
 * Função auxiliar utilizada pra verificar se o item a ser inserido é repetido.
 * Poderia ser útil ao usuário, por isso transformei em função
 * Apesar de ser chamada uma única vez no código.
 * 
 * Retorna 1 caso encontre um item repetido e 0 caso não ache ou erro. 
*/
int fprio_item_repetido (struct fprio_t *f, void *item) {
    struct fpnodo_t *p;
    
    // Trata casos inválidos
    if (f == NULL || item == NULL || f->num == 0)
        return 0;

    p = f->prim;

    // Compara o item procurado com cada item da fila
    for (int i = 0; i < f->num; i++) {

        if (p->item == item)
            return 1;

        p = p->prox;
    }

    return 0;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
    struct fpnodo_t *p;
    struct fpnodo_t *novo;

    // Trata casos inválidos e verifica se o item é repetido
    if (f == NULL || item == NULL || fprio_item_repetido(f, item))
        return -1;

    // Separado do primeiro teste para evitar linhas enormes
    if (!(novo = malloc(sizeof(struct fpnodo_t))))
        return -1;
    
    novo->tipo = tipo;
    novo->prio = prio;
    novo->item = item;

    // Fila vazia
    if (f->num == 0) {
        f->prim = novo;
        novo->prox = NULL;

        return f->num++;
    }

    // Prioridade máxima
    if (novo->prio < f->prim->prio) {
        novo->prox = f->prim;
        f->prim = novo;

        return f->num++;
    }

    novo->prox = f->prim;
    p = f->prim->prox;

    // Caso genérico
    while (p != NULL && novo->prio >= p->prio) {
        novo->prox = p;
        p = p->prox;
    }

    novo->prox->prox = novo;
    novo->prox = p; 

    return f->num++;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
    struct fpnodo_t *aux;
    void *p_item;

    // Trata casos inválidos
    if (f == NULL || tipo == NULL || prio == NULL || f->num == 0)
        return NULL;

    aux = f->prim;
    f->prim = f->prim->prox;

    p_item = aux->item;
    *tipo = aux->tipo;
    *prio = aux->prio;
    
    f->num--;
    free(aux);

    return p_item;
}

int fprio_tamanho (struct fprio_t *f) {
    if (f == NULL)
        return -1;

    return f->num;
} 

void fprio_imprime (struct fprio_t *f) {
    struct fpnodo_t *p;

    // Trata casos inválidos
    if (f == NULL || f->num == 0)
        return;

    printf("(%d %d)", f->prim->tipo, f->prim->prio);

    p = f->prim->prox;

    while (p != NULL) {
        printf(" (%d %d)", p->tipo, p->prio);
        p = p->prox;
    }
}
