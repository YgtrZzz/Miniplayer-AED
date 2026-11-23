#ifndef LSE_H
#define LSE_H

/* Representacao opaca: t_elem e campos de t_lse ficam em lse.c. */
typedef struct lse t_lse;
typedef int (*t_comparador)(const void *carga, const void *chave);
typedef void (*t_lse_visitar)(void *carga);

/*
 * Cria uma lista simplesmente encadeada vazia.
 * Retorno: ponteiro para a lista alocada, ou NULL em falha de memoria.
 * Memoria: o chamador deve liberar a lista com lse_destruir.
 */
t_lse *lse_criar(void);

/*
 * Retorna a quantidade de elementos armazenados na lista.
 * Parametro: lista (se NULL, retorna 0).
 */
int lse_tamanho(const t_lse *lista);

/*
 * Consulta a carga do primeiro no sem remove-lo.
 * Parametro: lista.
 * Retorno: ponteiro da primeira carga, ou NULL se lista for NULL ou estiver vazia.
 * Memoria: nao transfere nem libera a carga.
 */
void *lse_primeiro(const t_lse *lista);

/*
 * Percorre a lista do inicio ao fim aplicando o callback em cada carga.
 * Parametros: lista e visitar (se algum for NULL ou a lista estiver vazia, nao faz nada).
 * Memoria: nao altera nos nem cargas; o callback nao deve modificar a estrutura da lista.
 */
void lse_percorrer(const t_lse *lista, t_lse_visitar visitar);

/*
 * Insere um novo no no inicio da lista (O(1)).
 * Parametros: lista (nao nula) e carga (ponteiro generico; NULL e aceito).
 * Retorno: 1 em sucesso, 0 se lista for NULL ou houver falha de alocacao.
 * Memoria: a lista aloca o no e guarda apenas o ponteiro da carga, sem copia-la.
 */
int lse_inserir_inicio(t_lse *lista, void *carga);

/*
 * Insere um novo no no final da lista percorrendo a cadeia (O(n)).
 * Parametros: lista (nao nula) e carga (ponteiro generico; NULL e aceito).
 * Retorno: 1 em sucesso, 0 se lista for NULL ou houver falha de alocacao.
 * Memoria: a lista aloca o no e guarda apenas o ponteiro da carga, sem copia-la.
 */
int lse_inserir_fim(t_lse *lista, void *carga);

/*
 * Remove o primeiro no da lista e devolve sua carga.
 * Parametro: lista.
 * Retorno: carga removida, ou NULL se lista for NULL ou estiver vazia.
 * Memoria: libera somente o no; a carga continua sob responsabilidade do chamador.
 */
void *lse_remover_inicio(t_lse *lista);

/*
 * Busca a primeira carga que satisfaz comparar(carga, chave) == 0.
 * Parametros: lista, chave de busca e callback comparar (nao altera carga nem chave).
 * Retorno: ponteiro da carga encontrada, ou NULL se nao encontrar ou se lista/comparar forem NULL.
 * Memoria: nao modifica a lista nem libera memoria.
 */
void *lse_buscar(const t_lse *lista, const void *chave, t_comparador comparar);

/*
 * Remove apenas o primeiro no cuja carga satisfaz comparar(carga, chave) == 0.
 * Parametros: lista, chave de busca e callback comparar.
 * Retorno: carga removida, ou NULL se nao encontrar ou se lista/comparar forem NULL.
 * Memoria: libera somente o no; a carga e devolvida ao chamador sem ser liberada.
 */
void *lse_remover(t_lse *lista, const void *chave, t_comparador comparar);

/*
 * M05: insere a carga mantendo a ordenacao definida por comparar.
 * Retorno: 1 em sucesso, 0 em erro.
 */
int lse_inserir_ordenado(t_lse *lista, void *carga, t_comparador comparar);

/*
 * Libera todos os nos e a estrutura da lista.
 * Parametros: lista (se NULL, nao faz nada) e destruir_carga (callback opcional).
 * Memoria: libera todos os nos; se destruir_carga != NULL, chama-o para cada carga antes de liberar o no.
 */
void lse_destruir(t_lse *lista, void (*destruir_carga)(void *));

#endif
