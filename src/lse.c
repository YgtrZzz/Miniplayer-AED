#include "lse.h"
#include <stddef.h>
#include <stdlib.h>

/* Representacao interna: NAO mover para lse.h. */
typedef struct elem {
    void *carga;
    struct elem *prox;
} t_elem;

struct lse {
    t_elem *inicio;
    int tamanho;
    /* M08 (otimizacao opcional): t_elem *fim; */
};

t_lse *lse_criar(void) {
    t_lse *lista = malloc(sizeof(t_lse));
    if (lista == NULL) {
        return NULL;
    }
    lista->inicio = NULL;
    lista->tamanho = 0;
    return lista;
}

int lse_tamanho(const t_lse *lista) {
    if (lista == NULL) {
        return 0;
    }
    return lista->tamanho;
}

void *lse_primeiro(const t_lse *lista) {
    if (lista == NULL || lista->inicio == NULL) {
        return NULL;
    }
    return lista->inicio->carga;
}

int lse_inserir_inicio(t_lse *lista, void *carga) {
    if (lista == NULL) {
        return 0;
    }

    t_elem *novo = malloc(sizeof(t_elem));
    if (novo == NULL) {
        return 0;
    }

    novo->carga = carga;
    novo->prox = lista->inicio;
    lista->inicio = novo;
    lista->tamanho++;
    return 1;
}

void lse_percorrer(const t_lse *lista, t_lse_visitar visitar) {
    if (lista == NULL || visitar == NULL) {
        return;
    }

    const t_elem *atual = lista->inicio;
    while (atual != NULL) {
        visitar(atual->carga);
        atual = atual->prox;
    }
}

int lse_inserir_fim(t_lse *lista, void *carga) {
    if (lista == NULL) {
        return 0;
    }

    t_elem *novo = malloc(sizeof(t_elem));
    if (novo == NULL) {
        return 0;
    }

    novo->carga = carga;
    novo->prox = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        t_elem *atual = lista->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }

    lista->tamanho++;
    return 1;
}

void *lse_remover_inicio(t_lse *lista) {
    if (lista == NULL || lista->inicio == NULL) {
        return NULL;
    }

    t_elem *removido = lista->inicio;
    void *carga = removido->carga;
    lista->inicio = removido->prox;
    free(removido);
    lista->tamanho--;
    return carga;
}

void *lse_buscar(const t_lse *lista, const void *chave, t_comparador comparar) {
    if (lista == NULL || comparar == NULL) {
        return NULL;
    }

    const t_elem *atual = lista->inicio;
    while (atual != NULL) {
        if (comparar(atual->carga, chave) == 0) {
            return atual->carga;
        }
        atual = atual->prox;
    }
    return NULL;
}

void *lse_remover(t_lse *lista, const void *chave, t_comparador comparar) {
    if (lista == NULL || comparar == NULL) {
        return NULL;
    }

    t_elem *anterior = NULL;
    t_elem *atual = lista->inicio;

    while (atual != NULL && comparar(atual->carga, chave) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        return NULL;
    }

    if (anterior == NULL) {
        lista->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    void *carga = atual->carga;
    free(atual);
    lista->tamanho--;
    return carga;
}

int lse_inserir_ordenado(t_lse *l, void *c, t_comparador cmp) {
    (void)l;
    (void)c;
    (void)cmp;
    return 0; /* TODO M05 */
}

void lse_destruir(t_lse *lista, void (*destruir_carga)(void *)) {
    if (lista == NULL) {
        return;
    }

    t_elem *atual = lista->inicio;
    while (atual != NULL) {
        t_elem *proximo = atual->prox;
        if (destruir_carga != NULL) {
            destruir_carga(atual->carga);
        }
        free(atual);
        atual = proximo;
    }
    free(lista);
}
