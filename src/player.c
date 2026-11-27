#include "player.h"
#include "pilha.h"
#include <stddef.h>
#include <stdlib.h>

struct player {
    t_musica *atual;
    t_pilha *historico;
    t_pilha *avanco;
};

/* Esvazia uma pilha sem liberar as musicas apontadas por ela (O(k)). */
static void limpar_pilha(t_pilha *pilha) {
    while (pilha_desempilhar(pilha) != NULL) {
    }
}

t_player *player_criar(void) {
    t_player *p = malloc(sizeof(t_player));
    if (p == NULL) {
        return NULL;
    }

    p->historico = pilha_criar();
    if (p->historico == NULL) {
        free(p);
        return NULL;
    }

    p->avanco = pilha_criar();
    if (p->avanco == NULL) {
        pilha_destruir(p->historico, NULL);
        free(p);
        return NULL;
    }

    p->atual = NULL;
    return p;
}

int player_reproduzir(t_player *p, t_musica *m) {
    if (p == NULL || m == NULL) {
        return 0;
    }

    if (p->atual == m) {
        return 1;
    }

    if (p->atual != NULL) {
        if (pilha_empilhar(p->historico, p->atual) == 0) {
            return 0;
        }
        limpar_pilha(p->avanco);
    }

    p->atual = m;
    return 1;
}

int player_anterior(t_player *p) {
    if (p == NULL) {
        return 0;
    }

    if (pilha_topo(p->historico) == NULL) {
        return 0;
    }

    if (p->atual != NULL) {
        if (pilha_empilhar(p->avanco, p->atual) == 0) {
            return 0;
        }
    }

    p->atual = pilha_desempilhar(p->historico);
    return 1;
}

int player_avancar(t_player *p) {
    if (p == NULL) {
        return 0;
    }

    if (pilha_topo(p->avanco) == NULL) {
        return 0;
    }

    if (p->atual != NULL) {
        if (pilha_empilhar(p->historico, p->atual) == 0) {
            return 0;
        }
    }

    p->atual = pilha_desempilhar(p->avanco);
    return 1;
}

t_musica *player_atual(const t_player *p) {
    if (p == NULL) {
        return NULL;
    }
    return p->atual;
}

void player_destruir(t_player *p) {
    if (p == NULL) {
        return;
    }
    pilha_destruir(p->historico, NULL);
    pilha_destruir(p->avanco, NULL);
    free(p);
}
