# Relatório de testes

## M01 — Resultados (`musica`)
| ID | Missão | Função/contrato | Entrada/cenário | Resultado esperado | Resultado obtido | Status |
|---|---|---|---|---|---|---|
| T01 | M01 | `musica_criar` | dados válidos (`1, "Teste", "Artista", 180`) | ponteiro não nulo e campos corretos | igual | ok |
| T02 | M01 | `musica_criar` | alteração da string original após criação | `m->titulo` permanece `"Original"` (cópia independente) | igual | ok |
| T03 | M01 | `musica_criar` | strings com 199 caracteres (`> 79`) | truncamento em 79 caracteres com `'\0'` final | igual | ok |
| T04 | M01 | `musica_criar` | `titulo == NULL`, `artista == NULL` ou `duracao < 0` | retorno `NULL` | igual | ok |
| T05 | M01 | `musica_destruir` | ponteiro `NULL` | sem erro / sem acesso inválido | igual | ok |

## Casos-limite
- **Estrutura vazia:** testada em M02, M03, M04 e M05 (`lse_tamanho == 0`, `lse_primeiro == NULL`, `lse_percorrer` com 0 visitas, `lse_buscar`/`lse_remover`/`lse_remover_inicio` retornando `NULL`, `lse_inserir_fim` e `lse_inserir_ordenado` inserindo o primeiro nó corretamente).
- **Estrutura com um elemento:** testada em M03, M04 e M05 (inserção no início, no fim e ordenada antes/depois do único elemento, percurso com 1 visita, remoção por chave e `lse_remover_inicio` devolvendo a lista ao estado vazio `inicio == NULL` e `tamanho == 0`, seguida de reinserção).
- **Chave inexistente:** testada em M04 (`lse_buscar` e `lse_remover` retornam `NULL` e preservam o tamanho e o encadeamento da lista).
- **Falha de alocação (estratégia de simulação):** o código verifica o retorno `NULL` de `malloc` em `musica_criar`, `lse_criar`, `lse_inserir_inicio`, `lse_inserir_fim` e `lse_inserir_ordenado`, retornando `NULL` ou `0` sem alterar a lista. A falha não foi forçada em execução; para simulá-la em um teste controlado, pode-se substituir temporariamente `malloc` por uma função ou macro auxiliar de teste que devolva `NULL` propositalmente.

## Memória
- **Ambiente e compilação:** Windows com GCC (MSYS2 UCRT64), compilando todos os módulos e testes com `-std=c11 -Wall -Wextra -Wpedantic -g -Iinclude` sem nenhum aviso (*warning*).
- **Revisão de alocação e liberação:**
  - `musica_criar` aloca 1 `t_musica`; `musica_destruir` libera com `free(carga)`.
  - `lse_criar` aloca 1 `t_lse`; `lse_inserir_inicio`, `lse_inserir_fim` e `lse_inserir_ordenado` alocam 1 `t_elem` por inserção.
  - `lse_remover` e `lse_remover_inicio` liberam apenas o `t_elem` removido e devolvem a carga.
  - `lse_destruir` salva `atual->prox` antes de chamar `destruir_carga(atual->carga)` (quando não nulo) e `free(atual)`, encerrando com `free(lista)`.
- **Nota sobre ferramentas:** o Valgrind não está disponível nativamente no Windows; caso o projeto seja executado em Linux/WSL antes da entrega final, será rodado `valgrind --leak-check=full` sobre os binários de `build/`.

## M02 — Resultados (`lse` — contrato de lista vazia)
| ID | Missão | Função/contrato | Cenário | Esperado | Obtido | Status |
|---|---|---|---|---|---|---|
| T06 | M02 | `lse_criar` (antes da implementação) | stub retornando `NULL` | falha na asserção `l != NULL` | falha esperada confirmada | ok |
| T07 | M02 | `lse_criar`/`tamanho`/`primeiro`/`destruir` | lista recém-criada vazia | `l != NULL`, tamanho 0, primeiro `NULL`, destruição limpa | `M02: teste de contrato passou.` | ok |

## M03 — Resultados (`lse` — construção e percurso)
| ID | Missão | Função/contrato | Cenário | Esperado | Obtido | Status |
|---|---|---|---|---|---|---|
| T10 | M03 | `criar`/`tamanho`/`primeiro`/`percorrer` | lista vazia | 0, `NULL`, 0 visitas | igual | ok |
| T11 | M03 | `inserir_inicio`/`percorrer` | 1 elemento | tamanho 1, 1 visita | igual | ok |
| T12 | M03 | `inserir_inicio`/`percorrer` | 3 elementos | ordem 3, 2, 1 (LIFO na cabeça) | igual | ok |
| T13 | M03 | carga genérica | `t_musica` (na stack) e carga `NULL` | tamanho 2, primeiro `NULL`, 2 visitas | igual | ok |
| T14 | M03 | `destruir` com callback | 4 inteiros alocados no heap | callback chamado 4 vezes e memória liberada | igual | ok |
| T15 | M03 | robustez | `inserir_inicio(NULL, ...)`, `percorrer` com `NULL` | retorno 0 / sem efeito | igual | ok |

## M04 — Resultados (`lse` — inserção no fim, busca e remoção)
| ID | Missão | Função/contrato | Cenário | Esperado | Obtido | Status |
|---|---|---|---|---|---|---|
| T20 | M04 | `lse_inserir_fim` | lista vazia, depois mais 2 elementos | ordem a, b, c; tamanho 3 | igual | ok |
| T21 | M04 | `lse_buscar` | primeiro, meio, último | ponteiro da própria carga | igual | ok |
| T22 | M04 | `lse_buscar` | chave ausente | `NULL`, lista inalterada | igual | ok |
| T23 | M04 | `lse_remover` | meio, fim, início | carga devolvida; ordem e tamanho corretos | igual | ok |
| T24 | M04 | `lse_remover` | único elemento | lista vazia (`tamanho == 0`, `primeiro == NULL`) | igual | ok |
| T25 | M04 | `lse_remover` + reinserir | esvaziar e inserir fim/início | encadeamento íntegro | igual | ok |
| T26 | M04 | `lse_remover` | chave duplicada `(5, 5, 8)` | apenas a 1ª ocorrência é removida | igual | ok |
| T27 | M04 | `lse_remover`/`lse_remover_inicio` | chave inexistente; lista vazia | `NULL`; lista inalterada | igual | ok |
| T28 | M04 | `lse_remover_inicio` | lista com 3 elementos até esvaziar | `a`, `b`, `c` e depois `NULL` | igual | ok |
| T29 | M04 | genérico | registro `t_musica`, comparador por `id` | busca e remoção por `id` | igual | ok |
| T30 | M04 | robustez | lista `NULL`, callback `NULL` em `buscar` e `remover` | `0` / `NULL`, sem acesso indevido | igual | ok |
| T31 | M02/M03/Kit | regressão | `test_musica`, `test_lse`, `test_m02`, `test_m03` | todos passam sem alteração de asserções | igual | ok |

Comandos executados (a partir da raiz `miniplayer/`): compilação individual com `gcc -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude` (equivalente aos alvos `make test_musica`, `make build/test_lse`, `make test_m02`, `make test_m03`, `make test_m04` e `make test_missoes`).

## M05 — Resultados (`lse` — inserção ordenada)
| ID | Missão | Função/contrato | Cenário | Esperado | Obtido | Status |
|---|---|---|---|---|---|---|
| T32 | M05 | `test_m05` (antes da implementação) | stubs `assert(1==0)` | falha esperada na linha 20 | `Assertion failed: 1==0` | ok |
| T33 | M05 | `lse_inserir_ordenado` | lista vazia, início, fim e meio (`20, 10, 30, 25`) | ordem `[10, 20, 25, 30]`, tamanho 4 | igual | ok |
| T34 | M05 | `lse_inserir_ordenado` | empates com `musica_t` (`a(10), b(10), c(5), d(10)`) | ordem estável `&c, &a, &b, &d` | igual | ok |
| T35 | M05 | `lse_inserir_ordenado` | `lista == NULL` e `comparar == NULL` | retorno `0`, lista inalterada | igual | ok |
| T36 | M05 | `lse_inserir_ordenado` + remoção | remover do meio, esvaziar e reinserir ordenado | ordem e tamanho preservados | igual | ok |
| T37 | M01–M04 | regressão | `test_musica`, `test_lse`, `test_m02`, `test_m03`, `test_m04` | todos passam sem avisos | igual | ok |

Comandos executados (Windows/MSYS2): `mingw32-make -n test_missoes` (confirmando descoberta automática de `test_m02`, `test_m03`, `test_m04` e `test_m05`) e compilação/execução com `gcc -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude` de todos os binários sem avisos.
