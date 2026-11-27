# Contratos dos TADs
Para **cada função**, preencher:
- Nome e assinatura:
- Pré-condições:
- Pós-condições:
- Valor de retorno em sucesso/falha:
- Quem aloca/libera a memória:
- Invariantes preservadas:
- Testes associados:

## Invariantes da LSE
- `tamanho >= 0`.
- `tamanho == 0` se e somente se `inicio == NULL`.
- Há exatamente `tamanho` nós alcançáveis a partir de `inicio`.
- O último nó aponta para `NULL` e não há ciclos.

## M01 — Contratos implementados (`musica`)
### `t_musica *musica_criar(int id, const char *titulo, const char *artista, int duracao)`
- **Pré-condições:** `titulo != NULL`, `artista != NULL` e `duracao >= 0`.
- **Pós-condições:** retorna uma nova `t_musica` com os campos preenchidos; `titulo` e `artista` são copiados de forma independente e truncados com `'\0'` caso excedam 79 caracteres.
- **Retorno:** ponteiro válido em sucesso; `NULL` se os parâmetros forem inválidos ou se `malloc` falhar.
- **Memória:** alocada por `musica_criar`; deve ser liberada pelo chamador com `musica_destruir`.
- **Invariantes preservadas:** `titulo` e `artista` sempre terminam em `'\0'`; `duracao >= 0`.
- **Testes associados:** `tests/test_musica.c` (`test_criar_valida`, `test_copia_independente`, `test_truncamento`, `test_parametros_invalidos`).

### `void musica_destruir(void *carga)`
- **Pré-condições:** `carga` deve ser `NULL` ou um ponteiro retornado por `musica_criar` ainda não liberado.
- **Pós-condições:** memória da estrutura `t_musica` liberada; para `NULL`, não faz nada.
- **Retorno:** nenhum (`void`).
- **Memória:** libera apenas a `t_musica` apontada por `carga`.
- **Invariantes preservadas:** não há referências internas alocadas separadamente para liberar.
- **Testes associados:** `tests/test_musica.c` (`test_criar_valida`, `test_destruir_nulo`).

## M02 — Contratos implementados (`lse` — lista vazia)
### `t_lse *lse_criar(void)`
- **Pré-condições:** nenhuma.
- **Pós-condições:** cria lista vazia com `inicio == NULL` e `tamanho == 0`.
- **Retorno:** ponteiro para `t_lse` em sucesso; `NULL` se `malloc` falhar.
- **Memória:** alocada pela LSE; liberada por `lse_destruir`.
- **Invariantes preservadas:** `tamanho == 0` ⇔ `inicio == NULL`.
- **Testes associados:** `missoes/M02/TESTE_INCREMENTAL_M02.c` (`teste_lista_vazia`).

### `int lse_tamanho(const t_lse *lista)`
- **Pré-condições:** nenhuma (`lista` pode ser `NULL`).
- **Pós-condições:** não altera a lista.
- **Retorno:** quantidade de nós da lista; decisão da dupla: retorna `0` quando `lista == NULL`.
- **Memória:** não aloca nem libera memória.
- **Invariantes preservadas:** todas (operação somente leitura).
- **Testes associados:** `missoes/M02/TESTE_INCREMENTAL_M02.c`, `missoes/M03/test_m03.c`.

### `void *lse_primeiro(const t_lse *lista)`
- **Pré-condições:** nenhuma (`lista` pode ser `NULL`).
- **Pós-condições:** não altera a lista.
- **Retorno:** carga do primeiro nó; `NULL` se `lista == NULL` ou se a lista estiver vazia (ambíguo caso o primeiro nó guarde carga `NULL`; usar `lse_tamanho` para distinguir).
- **Memória:** não aloca, copia nem libera memória.
- **Invariantes preservadas:** todas (operação somente leitura).
- **Testes associados:** `missoes/M02/TESTE_INCREMENTAL_M02.c`, `missoes/M03/test_m03.c`.

### `void lse_destruir(t_lse *lista, void (*destruir_carga)(void *))` (lista vazia na M02; ampliada na M03)
- **Pré-condições:** `lista` é `NULL` ou uma lista criada por `lse_criar`.
- **Pós-condições:** libera a estrutura `t_lse` (e, na M03, todos os nós encadeados); se `destruir_carga != NULL`, invoca o callback uma vez para cada carga antes de liberar o nó; se `destruir_carga == NULL`, não toca nas cargas de terceiros.
- **Retorno:** nenhum (`void`); se `lista == NULL`, não faz nada.
- **Memória:** LSE libera nós e descritor; cargas só são liberadas via callback fornecido pelo chamador.
- **Invariantes preservadas:** não há vazamento de nós nem liberação indevida de cargas automáticas.
- **Testes associados:** `missoes/M02/TESTE_INCREMENTAL_M02.c`, `missoes/M03/test_m03.c`.

## M03 — Contratos implementados (`lse` — construção e percurso)
### `int lse_inserir_inicio(t_lse *lista, void *carga)`
- **Pré-condições:** `lista != NULL`.
- **Pós-condições:** novo nó inserido na cabeça apontando para o antigo primeiro; `tamanho` incrementado em 1; ponteiro da carga guardado sem cópia. **Decisão:** carga `NULL` é aceita, pois um nó ainda é um nó.
- **Retorno:** `1` em sucesso; `0` se `lista == NULL` ou `malloc` falhar (lista permanece inalterada).
- **Memória:** LSE aloca apenas o nó (`t_elem`); a carga continua sob posse do chamador.
- **Invariantes preservadas:** `tamanho` == nós alcançáveis; `inicio != NULL` quando `tamanho > 0`; último `prox == NULL`; sem ciclos.
- **Testes associados:** `missoes/M03/test_m03.c` (`teste_encadeamento_e_percurso`, `teste_carga_generica`, `main`).

### `void lse_percorrer(const t_lse *lista, t_lse_visitar visitar)`
- **Pré-condições:** o callback `visitar`, quando fornecido, não deve inserir, remover ou destruir nós da lista durante o percurso.
- **Pós-condições:** chama `visitar(carga)` para cada nó, na ordem do início ao fim. Se a lista estiver vazia ou se `lista == NULL` ou `visitar == NULL`, não faz nada.
- **Retorno:** nenhum (`void`).
- **Memória:** não aloca nem libera memória.
- **Invariantes preservadas:** todas (operação somente leitura sobre a estrutura).
- **Testes associados:** `missoes/M03/test_m03.c` (`teste_lista_vazia`, `teste_encadeamento_e_percurso`, `teste_carga_generica`).

## M04 — Contratos implementados (`lse` — inserção no fim, busca e remoção)
### `int lse_inserir_fim(t_lse *lista, void *carga)`
- **Pré-condições:** `lista != NULL`.
- **Pós-condições:** novo nó inserido como último elemento (`prox == NULL`); `tamanho` incrementado em 1; ordem dos elementos anteriores preservada; custo O(n) sem ponteiro `fim`. Carga `NULL` é aceita pela decisão da M03.
- **Retorno:** `1` em sucesso; `0` se `lista == NULL` ou `malloc` falhar (lista intacta).
- **Memória:** LSE aloca apenas o nó (`t_elem`); carga guardada por ponteiro sem cópia.
- **Invariantes preservadas:** `tamanho` == nós alcançáveis; último `prox == NULL`; sem ciclos.
- **Testes associados:** `missoes/M04/test_m04.c` (`teste_inserir_fim_e_buscar`, `teste_remover_cabeca_meio_cauda`, `teste_remover_inicio`, `teste_argumentos_invalidos`).

### `void *lse_buscar(const t_lse *lista, const void *chave, t_comparador comparar)`
- **Pré-condições:** `comparar` não deve alterar as cargas nem a chave.
- **Pós-condições:** lista inalterada.
- **Retorno:** ponteiro da **primeira** carga tal que `comparar(carga, chave) == 0`; `NULL` se não encontrar ou se `lista == NULL` ou `comparar == NULL`.
- **Memória:** não aloca nem libera memória.
- **Invariantes preservadas:** todas (operação somente leitura).
- **Testes associados:** `missoes/M04/test_m04.c` (`teste_inserir_fim_e_buscar`, `teste_generico_musicas`, `teste_argumentos_invalidos`).

### `void *lse_remover(t_lse *lista, const void *chave, t_comparador comparar)`
- **Pré-condições:** `comparar` não deve alterar cargas nem chave.
- **Pós-condições:** remove apenas o **primeiro** nó correspondente, religa `inicio` (se primeiro nó) ou `anterior->prox` (demais casos), decrementa `tamanho` em 1 e devolve a carga. Se não encontrar ou se `lista`/`comparar` forem `NULL`, a lista permanece inalterada.
- **Retorno:** ponteiro da carga removida em sucesso; `NULL` se não encontrar ou argumentos inválidos.
- **Memória:** LSE libera **somente o nó** (`t_elem`); a carga **não** é liberada e volta à responsabilidade do chamador.
- **Invariantes preservadas:** `tamanho` == nós alcançáveis; `tamanho == 0` ⇔ `inicio == NULL`; último `prox == NULL`; sem ciclos.
- **Testes associados:** `missoes/M04/test_m04.c` (`teste_remover_cabeca_meio_cauda`, `teste_remover_inicio`, `teste_generico_musicas`, `teste_argumentos_invalidos`).

### `void *lse_remover_inicio(t_lse *lista)`
- **Pré-condições:** nenhuma (`lista` pode ser `NULL`).
- **Pós-condições:** remove o primeiro nó, atualiza `inicio` para o próximo e decrementa `tamanho` em 1. Com um único nó, a lista volta a `inicio == NULL` e `tamanho == 0`.
- **Retorno:** carga do nó removido; `NULL` se `lista == NULL` ou lista vazia.
- **Memória:** LSE libera somente o nó; a carga não é liberada.
- **Invariantes preservadas:** `tamanho` == nós alcançáveis; `tamanho == 0` ⇔ `inicio == NULL`; último `prox == NULL`.
- **Testes associados:** `missoes/M04/test_m04.c` (`teste_remover_inicio`, `teste_argumentos_invalidos`), `tests/test_lse.c`.

### Ambiguidade do retorno `NULL` (M03/M04)
A carga `NULL` é permitida pela decisão da M03, mas `lse_buscar`, `lse_remover` e `lse_remover_inicio` também retornam `NULL` para indicar "não encontrado" ou "lista vazia". Por esse motivo, os cenários de busca e remoção da M04 não utilizam cargas `NULL`. Em uma API futura, essa ambiguidade poderia ser resolvida de três formas:
1. Retornar um código de status (`int`: `1` encontrado / `0` ausente) e entregar a carga por parâmetro de saída (`void **carga_saida`).
2. Proibir carga `NULL` já nas funções de inserção (`lse_inserir_*` retornando `0`).
3. Oferecer função auxiliar booleana (`lse_contem`) ou verificar `lse_tamanho` antes da remoção no início.

## M05 — Contratos implementados (`lse` — inserção ordenada)
### `int lse_inserir_ordenado(t_lse *lista, void *carga, t_comparador comparar)`
- **Pré-condições:** `lista != NULL` e `comparar != NULL`. A lista deve estar previamente ordenada segundo o mesmo critério definido por `comparar`; a operação não tem responsabilidade de reordenar uma lista previamente desordenada. O callback recebe `(carga_existente, carga_nova)` — ambos apontando para cargas — e não deve alterá-las.
- **Pós-condições:** aloca um novo nó e o insere na posição que mantém a ordenação; em caso de empate (`comparar(carga_existente, carga_nova) == 0`), insere **após** os elementos existentes com a mesma chave (*estabilidade*). Incrementa `tamanho` em 1. Em erro, a lista permanece inalterada.
- **Retorno:** `1` em sucesso; `0` se `lista == NULL`, `comparar == NULL` ou `malloc` falhar.
- **Memória:** a LSE aloca apenas o nó (`t_elem`) e guarda o ponteiro `carga` sem copiá-lo nem assumir automaticamente sua propriedade.
- **Complexidade:** tempo O(1) no melhor caso (vazia ou início) e O(n) no caso médio/pior caso (meio ou fim); espaço auxiliar O(1).
- **Invariantes preservadas:** `tamanho` aumenta exatamente em 1; `tamanho` == nós alcançáveis; `inicio != NULL` quando `tamanho > 0`; último `prox == NULL`; sem ciclos; ordenação mantida com estabilidade.
- **Testes associados:** `missoes/M05/test_m05.c` (`teste_vazia_inicio_meio_fim`, `teste_duplicados_estaveis`, `teste_invalido_e_reinsercao`).

## M06 — Contratos implementados (`pilha`)
### `t_pilha *pilha_criar(void)`
- **Pré-condições:** nenhuma.
- **Pós-condições:** aloca `t_pilha` e sua `t_lse` interna vazia (`lse_criar`). Se `lse_criar` falhar, libera `t_pilha` antes de retornar `NULL`.
- **Retorno:** ponteiro da pilha em sucesso; `NULL` se faltar memória.
- **Memória:** alocada por `pilha_criar`; quem chama deve liberar com `pilha_destruir`.
- **Complexidade:** O(1).
- **Invariantes preservadas:** `pilha->lista != NULL` (LSE vazia válida).
- **Testes associados:** `missoes/M06/test_m06.c`, `tests/test_pilha.c`.

### `int pilha_empilhar(t_pilha *pilha, void *carga)`
- **Pré-condições:** `pilha != NULL`; na M06 usar `carga != NULL` (não validamos `carga == NULL` na função, conforme enunciado).
- **Pós-condições:** chama `lse_inserir_inicio(pilha->lista, carga)` sem copiar a carga. Em erro, não altera a pilha.
- **Retorno:** `1` em sucesso; `0` se `pilha == NULL` ou se falhar a alocação do nó.
- **Memória:** a LSE interna aloca só o nó; a carga continua sendo de quem chamou.
- **Complexidade:** O(1).
- **Invariantes preservadas:** novo elemento fica no topo; invariantes da LSE mantidos.
- **Testes associados:** `missoes/M06/test_m06.c` (`lifo`, `independentes`, `propriedade`, `invalidos`), `tests/test_pilha.c`.

### `void *pilha_desempilhar(t_pilha *pilha)`
- **Pré-condições:** nenhuma (`pilha` pode ser `NULL`).
- **Pós-condições:** chama `lse_remover_inicio(pilha->lista)` e devolve a carga do topo sem liberá-la.
- **Retorno:** carga do topo; `NULL` se `pilha == NULL` ou vazia.
- **Memória:** libera apenas o nó interno; a carga volta para quem chamou.
- **Complexidade:** O(1).
- **Invariantes preservadas:** ordem LIFO mantida; topo passa para o elemento anterior (ou `NULL`).
- **Testes associados:** `missoes/M06/test_m06.c` (`lifo`, `independentes`, `propriedade`, `invalidos`), `tests/test_pilha.c`.

### `void *pilha_topo(const t_pilha *pilha)`
- **Pré-condições:** nenhuma (`pilha` pode ser `NULL`).
- **Pós-condições:** chama `lse_primeiro(pilha->lista)` sem alterar a pilha.
- **Retorno:** carga do topo; `NULL` se `pilha == NULL` ou vazia.
- **Memória:** não aloca nem libera memória.
- **Complexidade:** O(1).
- **Invariantes preservadas:** todas (só leitura, não remove).
- **Testes associados:** `missoes/M06/test_m06.c` (`lifo`, `independentes`, `invalidos`), `tests/test_pilha.c`.

### `void pilha_destruir(t_pilha *pilha, void (*destruir_carga)(void *))`
- **Pré-condições:** `pilha` é `NULL` ou veio de `pilha_criar`. Só passar `destruir_carga != NULL` se a pilha for a única dona das cargas.
- **Pós-condições:** chama `lse_destruir(pilha->lista, destruir_carga)` e libera `t_pilha`. Se `pilha == NULL`, não faz nada.
- **Retorno:** nenhum (`void`).
- **Memória:** libera os nós e a estrutura `t_pilha`; cargas só são liberadas se o callback não for `NULL` (no histórico futuro, usar `NULL`).
- **Complexidade:** O(n) (sem contar o custo do callback).
- **Invariantes preservadas:** nenhum nó nem a estrutura `t_pilha` ficam vazando.
- **Testes associados:** `missoes/M06/test_m06.c` (`lifo`, `independentes`, `propriedade`, `invalidos`), `tests/test_pilha.c`.

## M07 — Contratos implementados (`player`)
### `t_player *player_criar(void)`
- **Pré-condições:** nenhuma.
- **Pós-condições:** cria o controlador com `atual == NULL` e duas pilhas vazias (`historico` e `avanco`). Se alguma alocação falhar, libera o que já foi alocado antes de retornar `NULL`.
- **Retorno:** ponteiro para `t_player` em sucesso; `NULL` se faltar memória.
- **Memória:** alocada por `player_criar`; liberar com `player_destruir`.
- **Complexidade:** O(1).
- **Invariantes preservadas:** `historico != NULL`, `avanco != NULL`, ambos sem `NULL` internos.
- **Testes associados:** `missoes/M07/test_m07.c`, `tests/test_player.c`.

### `int player_reproduzir(t_player *p, t_musica *m)`
- **Pré-condições:** `p != NULL` e `m != NULL`.
- **Pós-condições:** se `p->atual == m`, retorna `1` sem mudar nada (preserva `avanco`). Se `p->atual == NULL`, define `p->atual = m` sem empilhar `NULL`. Se `p->atual != m`, empilha `p->atual` em `historico`, esvazia `avanco` e define `p->atual = m`. Se `pilha_empilhar` falhar, mantém o estado anterior intacto.
- **Retorno:** `1` em sucesso; `0` se `p == NULL`, `m == NULL` ou erro de memória.
- **Memória:** só aloca/libera nós das pilhas; não copia nem libera músicas.
- **Complexidade:** O(1) se mesma música ou `avanco` vazio; O(k) se precisar esvaziar `k` itens de `avanco`.
- **Invariantes preservadas:** nunca guarda `NULL` nas pilhas; estado intacto em erro.
- **Testes associados:** `missoes/M07/test_m07.c` (`navegacao`, `independentes`, `invalidos`), `tests/test_player.c`.

### `int player_anterior(t_player *p)`
- **Pré-condições:** `p != NULL`.
- **Pós-condições:** se `historico` estiver vazio, retorna `0` sem mudar nada. Caso contrário, empilha `p->atual` em `avanco` e move o topo de `historico` para `p->atual`. Se empilhar falhar, nada é removido de `historico`.
- **Retorno:** `1` em sucesso; `0` se `p == NULL`, histórico vazio ou falha de alocação.
- **Memória:** só manipula nós internos das pilhas; não libera músicas.
- **Complexidade:** O(1).
- **Invariantes preservadas:** nenhum `NULL` nas pilhas; estado preservado em falha.
- **Testes associados:** `missoes/M07/test_m07.c`, `tests/test_player.c`.

### `int player_avancar(t_player *p)`
- **Pré-condições:** `p != NULL`.
- **Pós-condições:** simétrica a `player_anterior`: se `avanco` estiver vazio, retorna `0`; senão empilha `p->atual` em `historico` e move o topo de `avanco` para `p->atual`, mantendo tudo intacto se empilhar falhar.
- **Retorno:** `1` em sucesso; `0` se `p == NULL`, avanço vazio ou falha de alocação.
- **Memória:** só manipula nós internos das pilhas; não libera músicas.
- **Complexidade:** O(1).
- **Invariantes preservadas:** nenhum `NULL` nas pilhas; estado preservado em falha.
- **Testes associados:** `missoes/M07/test_m07.c`, `tests/test_player.c`.

### `t_musica *player_atual(const t_player *p)`
- **Pré-condições:** nenhuma (`p` pode ser `NULL`).
- **Pós-condições:** não altera o player.
- **Retorno:** ponteiro da música atual, ou `NULL` se `p == NULL` ou sem música.
- **Memória:** só leitura.
- **Complexidade:** O(1).
- **Invariantes preservadas:** todas.
- **Testes associados:** `missoes/M07/test_m07.c`, `tests/test_player.c`.

### `void player_destruir(t_player *p)`
- **Pré-condições:** `p` é `NULL` ou veio de `player_criar`.
- **Pós-condições:** destrói `historico` e `avanco` com callback `NULL` e libera `p`. Se `p == NULL`, não faz nada.
- **Retorno:** nenhum (`void`).
- **Memória:** libera apenas as pilhas e `t_player`; **nunca** libera as músicas.
- **Complexidade:** O(h + k), onde `h` é o tamanho do histórico e `k` o tamanho do avanço.
- **Invariantes preservadas:** sem vazamento das pilhas/controlador e sem liberar músicas emprestadas.
- **Testes associados:** `missoes/M07/test_m07.c`, `tests/test_player.c`.
