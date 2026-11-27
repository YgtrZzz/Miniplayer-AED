# MiniPlayer — ICC002 (2026/2)
Projeto integrador progressivo, em duplas, em C. Reprodução **simulada**.

**Dupla:** Ygor Gutierrz e André Silveira

## Compilação e Execução
`make` e `./build/miniplayer`.

## Testes
- **Testes das missões incrementais (recomendado):**
  - `make test_m02` — contrato da lista vazia (M02)
  - `make test_m03` — construção, inserção no início, percurso e destruição (M03)
  - `make test_m04` — inserção no fim, busca e remoção (M04)
  - `make test_m05` — inserção ordenada estável e testes de regressão (M05)
  - `make test_m06` — TAD Pilha por composição com LSE (M06)
  - `make test_missoes` — executa todas as missões instaladas em `missoes/M*/`
- **Testes unitários do kit inicial:**
  - `make build/test_musica && ./build/test_musica` (M01 — passando)
  - `make build/test_lse && ./build/test_lse` (M02–M04 — passando)
  - `make build/test_pilha && ./build/test_pilha` (M06, com teste próprio de intercalação — passando)
  - `make test` — executa todos os testes do kit (`test_musica`, `test_lse`, `test_pilha`, `test_fila`). **ATENÇÃO:** `test_fila` falha intencionalmente até que a missão M08 seja implementada.

## Contratos
- `void *carga` é genérico; a lista não conhece `t_musica`.
- A lista possui os nós, **não** as cargas, salvo quando `lse_destruir` recebe callback destruidor.
- Remover devolve a carga e libera apenas o nó.
- Pilha e fila reutilizam `lse_*`, sem manipular `t_elem`.
- Biblioteca possui as músicas; histórico e fila só as referenciam.
- Antes de liberar uma música, remover referências a ela de fila e histórico.

## Como documentar
Preencher `docs/arquitetura.md`, `docs/contratos.md`, `docs/diario-de-bordo.md` e `docs/relatorio-testes.md` a cada missão. Ver `GUIA_ESTUDANTE.md`.

## Estado atual (M01 a M06)
- **M01:** módulo `musica` (`musica_criar`, `musica_destruir`) e testes em `tests/test_musica.c`.
- **M02:** TAD `lse` opaco, criação, tamanho, consulta ao primeiro elemento e destruição de lista vazia (`make test_m02`).
- **M03:** encadeamento de nós, `lse_inserir_inicio`, `lse_percorrer` com callback e `lse_destruir` com callback opcional (`make test_m03`).
- **M04:** `lse_inserir_fim`, `lse_buscar`, `lse_remover` e `lse_remover_inicio` (`make test_m04`).
- **M05:** `lse_inserir_ordenado` com comparador genérico e estabilidade em empates (`make test_m05`).
- **M06:** TAD `pilha` genérico LIFO por composição sobre a LSE (`make test_m06` e `tests/test_pilha.c`).
- **Pendentes (próximas missões):** histórico de reprodução (M07), `fila` (M08) e integração/arquivos (M09–M10).
