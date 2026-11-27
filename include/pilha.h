#ifndef PILHA_H
#define PILHA_H

/* Representacao opaca: a composicao com t_lse fica privada em pilha.c. */
typedef struct pilha t_pilha;

/*
 * Cria uma pilha vazia composta por uma lista simplesmente encadeada interna.
 * Retorno: ponteiro para a pilha alocada, ou NULL em falha de memoria (liberando alocacoes parciais).
 * Memoria: o chamador deve liberar a pilha com pilha_destruir.
 */
t_pilha *pilha_criar(void);

/*
 * Empilha uma carga no topo da pilha (O(1), politica LIFO).
 * Parametros: pilha (nao nula) e carga (ponteiro generico; usar cargas nao nulas).
 * Retorno: 1 em sucesso, 0 se pilha for NULL ou houver falha de alocacao (pilha inalterada).
 * Memoria: guarda apenas o ponteiro da carga, sem copia-la.
 */
int pilha_empilhar(t_pilha *pilha, void *carga);

/*
 * Remove e devolve a carga do topo da pilha (O(1)).
 * Parametro: pilha.
 * Retorno: carga do topo, ou NULL se pilha for NULL ou estiver vazia.
 * Memoria: libera apenas o no interno; a carga e devolvida ao chamador sem ser liberada.
 */
void *pilha_desempilhar(t_pilha *pilha);

/*
 * Consulta a carga do topo da pilha sem remove-la (O(1)).
 * Parametro: pilha.
 * Retorno: carga do topo, ou NULL se pilha for NULL ou estiver vazia.
 * Memoria: operacao nao destrutiva; nao altera nem libera a carga.
 */
void *pilha_topo(const t_pilha *pilha);

/*
 * Libera a lista interna e a estrutura da pilha (O(n), sem contar o callback).
 * Parametros: pilha (se NULL, nao faz nada) e destruir_carga (callback opcional).
 * Memoria: com destruir_carga == NULL, libera apenas os nos e a pilha (referencias emprestadas);
 * com callback nao nulo, libera cada carga antes do no (apenas quando a pilha tiver posse exclusiva).
 */
void pilha_destruir(t_pilha *pilha, void (*destruir_carga)(void *));

#endif
