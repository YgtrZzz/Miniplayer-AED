#include "pilha.h"
#include "lse.h"
#include <stddef.h>
#include <stdlib.h>

struct pilha {
    t_lse *lista;
};

t_pilha *pilha_criar(void) {
    t_pilha *pilha = malloc(sizeof(t_pilha));
    if (pilha == NULL) {
        return NULL;
    }

    pilha->lista = lse_criar();
    if (pilha->lista == NULL) {
        free(pilha);
        return NULL;
    }

    return pilha;
}

int pilha_empilhar(t_pilha *pilha, void *carga) {
    if (pilha == NULL) {
        return 0;
    }
    return lse_inserir_inicio(pilha->lista, carga);
}

void *pilha_desempilhar(t_pilha *pilha) {
    if (pilha == NULL) {
        return NULL;
    }
    return lse_remover_inicio(pilha->lista);
}

void *pilha_topo(const t_pilha *pilha) {
    if (pilha == NULL) {
        return NULL;
    }
    return lse_primeiro(pilha->lista);
}

void pilha_destruir(t_pilha *pilha, void (*destruir_carga)(void *)) {
    if (pilha == NULL) {
        return;
    }
    lse_destruir(pilha->lista, destruir_carga);
    free(pilha);
}
