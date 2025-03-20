#include <stdio.h>
#include <stdlib.h>


struct node_t {
    struct node_t *prox;
    
    char titulo[33];
    char texto[513];

    char idade;
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

    // Caso a fila nao esteja vazia
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
        return;
    
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
    printf("\nDigite o titulo: ");
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
    printf(" - (1) Cadastrar noticia");
    printf(" - (2) Fechar edicao");
    printf(" - (3) Sair");
}

int tam_fila(struct fila_t *f) {
    struct node_t *aux;
    int tam = 0;

    if (f == NULL) {
        return tam;
    }

    aux = f->head;

    while (aux != NULL) {
        tam++;

        aux = aux->prox;
    }

    return tam;
}

void print_noticia(struct node_t *noticia) {    
    char c = 'e';
    int i = 0;
    while (c != '\0') {
        c = noticia->titulo[i];

        printf("%c", c);
        
        i++;
    }
    
    printf("\n\n");

    i = 0;
    c = 'e';

    while (c != '\0') {
        c = noticia->texto[i];

        printf("%c", c);
        
        i++;
    }

    printf("\n\n");

}

int main() {
    int option = 0;
    int tipo_noticia;
    struct fila_t* breaking_fila = cria_fila();
    struct fila_t* informe_fila = cria_fila();
    struct node_t* noticia1;
    struct node_t* noticia2;
    

    while (option != 3) {
        print_menu();
        scanf("%d", &option);

        if (option == 1) {
            scanf("%d", &tipo_noticia);
            if (!tipo_noticia)
                insere_noticia(breaking_fila, criar_noticia());
            else
                insere_noticia(informe_fila, criar_noticia());
        }

        if (option == 2) {
            int tam_b = tam_fila(breaking_fila);
            int tam_i = tam_fila(informe_fila);

            if (tam_b >= 2) {
                noticia1 = remove_noticia(breaking_fila);
                noticia2 = remove_noticia(breaking_fila);
            }
            else if (tam_b == 1 && tam_fila(informe_fila) >= 1) {
                noticia1 = remove_noticia(breaking_fila);
                noticia2 = remove_noticia(informe_fila);
            }   
            else if (tam_b == 1 && tam_i == 0) {
                noticia1 = remove_noticia(breaking_fila);
                noticia2 = NULL;
            }  
            else if (tam_i >= 2) {
                noticia1 = remove_noticia(informe_fila);
                noticia2 = remove_noticia(informe_fila);

            } 
            else if (tam_i == 1) {
                noticia1 = remove_noticia(informe_fila);
                noticia2 = NULL;
            }
            else {
                noticia1 = NULL;
                noticia2 = NULL;
            }

            if (noticia1) {
                printf("=======================================================\n");
                print_noticia(noticia1);
            }
            if (noticia2) {
                printf("==");
                print_noticia(noticia2);
                printf("=======================================================\n");
            }

            atualizar(breaking_fila);
            atualizar(informe_fila);
        }

    }

    return 0;
}