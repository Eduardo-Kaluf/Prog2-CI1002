#include <stdio.h>
#include <stdlib.h>


struct node_t {
    struct node_t *prox;
    
    char titulo[33];
    char texto[513];

    char idade; // !NUMERO!
};

struct fila_t {
    struct node_t *head;
    struct node_t *tail;
};

struct fila_t *cria_fila() {
    struct fila_t *f;

    if (!(f = malloc(sizeof(struct fila_t))))
        return NULL;

    f->head = NULL;
    f->tail = NULL;

    return f;
}

struct node_t *remove_noticia(struct fila_t *f) {
    struct node_t *aux;

    if (f == NULL) 
        return NULL;
    
    if (f->head == NULL)
        return NULL;

    aux = f->head;
    if (f->head->prox != NULL) {
        f->head = f->head->prox;
    }
    else {
        f->head = NULL;
        f->tail = NULL;
    }

    return aux;
}

void insere_noticia(struct fila_t *f, struct node_t *noticia) {
    struct node_t *p;
    struct node_t *aux;

    if (f == NULL || noticia == NULL)
        return;

    if (f->tail == NULL) {
        f->tail = noticia;
        f->head = noticia;
        noticia->prox = NULL;
        return;
    }

    // Caso a fila não esteja vazia
    f->tail->prox = noticia;
    noticia->prox = NULL;
    f->tail = noticia;

    return;
}

void destroi_fila(struct fila_t *f) {
    struct node_t *aux;

    while (f->head != NULL) {
        free(remove_noticia(f));
    }

    free(f);
}

void atualizar (struct fila_t *f) {
    struct node_t *p = f->head;

    if (f == NULL)
        return NULL;
    
    int i = 0;
    while (p != NULL) {
        if (p->idade == 3) 
            i++;

        p->idade++;

        p = p->prox;
    }

    for (int j = 0; i < i; j++) {
        free(remove_noticia(f));
    }

}

void requisita_noticia(char *titulo, char *texto){
    getchar();
    printf("\nDigite o título: ");
    fgets(titulo, 33, stdin);
    printf("Digite o texto: ");
    fgets(texto, 513, stdin);
}

struct node_t *criar_noticia() {
    struct node_t *noticia;
    
    noticia = malloc(sizeof(struct node_t));

    requisita_noticia(noticia->titulo, noticia->titulo);
    
    noticia->idade = 0;
    noticia->prox = NULL;

    return noticia;
}

void print_menu() {
    printf(" - (1) Cadastrar notícia");
    printf(" - (2) Fechar edição");
    printf(" - (3) Sair");
}

int main() {


    return 0;
}