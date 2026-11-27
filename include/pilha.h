#ifndef PILHA_H
#define PILHA_H

/* Tipo opaco: struct pilha (com t_lse interna) fica em pilha.c. */
typedef struct pilha t_pilha;

/* Cria uma pilha vazia. Retorna NULL se faltar memoria. Liberar com pilha_destruir. */
t_pilha *pilha_criar(void);

/* Empilha carga no topo (O(1)) sem copia-la. Retorna 1 em sucesso ou 0 se pilha == NULL / erro. */
int pilha_empilhar(t_pilha *pilha, void *carga);

/* Remove e devolve a carga do topo (O(1)) sem libera-la. Retorna NULL se vazia ou pilha == NULL. */
void *pilha_desempilhar(t_pilha *pilha);

/* Consulta a carga do topo sem remover (O(1)). Retorna NULL se vazia ou pilha == NULL. */
void *pilha_topo(const t_pilha *pilha);

/* Libera a pilha e seus nos (O(n)); chama destruir_carga se nao for NULL. Aceita pilha == NULL. */
void pilha_destruir(t_pilha *pilha, void (*destruir_carga)(void *));

#endif
