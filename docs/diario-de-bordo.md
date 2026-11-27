# Diário de bordo

## Missão M01 — 11/11
- **Integrantes e papéis:** Codificador: Ygor Gutierrz | Revisor: André Silveira
- **Objetivo:** compreender alocação dinâmica em C e implementar o módulo `musica` (`musica_criar` e `musica_destruir`) com cópia segura de strings.
- **O que implementamos:** `musica_criar` (alocação dinâmica, validação de `titulo`, `artista` e `duracao >= 0`, e cópia segura com `copiar_texto`) e `musica_destruir` em `src/musica.c`; ampliação dos casos de teste em `tests/test_musica.c`.
- **Decisão técnica e justificativa:** usamos `strncpy` acompanhado da atribuição explícita de `'\0'` na última posição do vetor para evitar estouro de buffer e garantir strings sempre terminadas, além de rejeitar duração negativa. `musica_destruir` aceita `void *` para já servir como callback de destruição da LSE.
- **Problema encontrado e solução:** o teste inicial do kit verificava apenas o caso feliz; adicionamos testes para cópia independente, truncamento de strings longas, parâmetros inválidos e `musica_destruir(NULL)`.
- **Evidências de testes:** compilação com `gcc -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude tests/test_musica.c src/musica.c` sem avisos; execução de `test_musica` imprimiu `test_musica: OK`.
- **Próxima ação:** M02 — definir e implementar os contratos iniciais do TAD LSE (lista vazia).

## Missão M02 — 13/11
- **Integrantes e papéis:** Codificador: André Silveira | Revisor: Ygor Gutierrz
- **Objetivo:** consolidar o encapsulamento do TAD LSE (`t_lse` opaco, `t_elem` interno) e implementar criação, consultas básicas e destruição de lista vazia.
- **O que implementamos:** `lse_criar`, `lse_tamanho`, `lse_primeiro` e a base de `lse_destruir` em `src/lse.c`; instalação do pacote incremental em `missoes/M02/`.
- **Decisão técnica e justificativa:** definimos que `lse_tamanho(NULL)` retorna `0` e `lse_primeiro(NULL)` retorna `NULL` para evitar desreferência de ponteiro nulo. Mantivemos `t_elem` privado em `src/lse.c` para que módulos futuros não dependam da estrutura interna dos nós.
- **Problema encontrado e solução:** antes da implementação, `TESTE_INCREMENTAL_M02.c` falhava na asserção `l != NULL` porque `lse_criar` era um stub retornando `NULL`. Após implementar os 4 contratos da M02 em `src/lse.c`, o teste passou sem alterar o código de teste.
- **Evidências de testes:** compilação com `-std=c11 -Wall -Wextra -Wpedantic` sem avisos; execução do teste da M02 (`make test_m02` / `build/m02_incremental`) imprimiu `M02: teste de contrato passou.`.
- **Próxima ação:** M03 — implementar inserção no início, percurso por callback e destruição com nós.

## Missão M03 — 16/11
- **Integrantes e papéis:** Codificador: Ygor Gutierrz | Revisor: André Silveira
- **Objetivo:** construir a cadeia de nós da LSE, inserir no início, percorrer elementos por callback sem expor `t_elem` e destruir nós com callback opcional para cargas.
- **O que implementamos:** declaração de `t_lse_visitar` e `lse_percorrer` em `include/lse.h`; implementação de `lse_inserir_inicio`, `lse_percorrer` e revisão de `lse_destruir` (percorrendo nós e chamando `destruir_carga` quando não nulo) em `src/lse.c`; preenchimento dos testes em `missoes/M03/test_m03.c`.
- **Decisão técnica e justificativa:** permitimos carga `NULL` em `lse_inserir_inicio`, pois um nó com carga nula ainda é um nó válido; inserção no início é O(1). No `test_m03.c`, usamos uma `t_musica` local (automática) para testar carga genérica sem precisar linkar `src/musica.c`, preservando o `missoes/M03/Makefile.incremental` original sem modificações.
- **Problema encontrado e solução:** o enunciado da M03 citava `t_visitar` na assinatura de `lse_percorrer`, mas definia o `typedef` como `t_lse_visitar`; padronizamos como `t_lse_visitar` em `include/lse.h`. Também corrigimos o acoplamento inicial que havia exigido editar `missoes/M03/Makefile.incremental`.
- **Evidências de testes:** compilação limpa com `-std=c11 -Wall -Wextra -Wpedantic` usando os arquivos originais do `Makefile.incremental` da M03; execução imprimiu `M03: todos os testes passaram.`. Regressão da M02 aprovada.
- **Próxima ação:** M04 — implementar `lse_inserir_fim`, `lse_buscar`, `lse_remover` e `lse_remover_inicio`.

## Missão M04 — 18/11
- **Integrantes e papéis:** Codificador: André Silveira | Revisor: Ygor Gutierrz
- **Objetivo:** implementar inserção no fim, busca por chave via callback e remoção (no início e por chave) preservando os invariantes da lista.
- **O que implementamos:** `lse_inserir_fim`, `lse_buscar`, `lse_remover` e `lse_remover_inicio` em `src/lse.c` (preservando M01–M03 e mantendo `lse_inserir_ordenado` como stub da M05); conclusão dos testes em `missoes/M04/test_m04.c`; integração do bloco completo de `test_missoes` no `Makefile` principal.
- **Decisão técnica e justificativa:** em `lse_inserir_fim`, percorremos a cadeia em O(n) sem adicionar ponteiro `fim` (deixado para M08). Em `lse_remover`, usamos dois ponteiros (`anterior` e `atual`) tratando explicitamente a remoção do primeiro nó (`anterior == NULL`) e dos demais (`anterior->prox = atual->prox`), liberando apenas o nó e devolvendo a carga. Documentamos a ambiguidade do retorno `NULL` e evitamos cargas `NULL` nos testes de busca/remoção.
- **Problema encontrado e solução:** (1) o `test_m04.c` do kit declarava `musica_t` no topo mas usava `t_musica` em `teste_generico_musicas`, impedindo a compilação; incluímos `"musica.h"` e alinhamos o comparador para `const t_musica *` sem alterar asserções. (2) o enunciado citava `t_comparador_lse`, mas `include/lse.h` já definia `t_comparador`; mantivemos o cabeçalho do projeto.
- **Evidências de testes:** compilação sem avisos com GCC (`-std=c11 -Wall -Wextra -Wpedantic`) no Windows/MSYS2; aprovados `test_musica`, `test_lse`, `test_m02`, `test_m03` e `test_m04`. `test_pilha` e `test_fila` permanecem falhando intencionalmente (stubs de M06/M08).
- **Próxima ação:** M05 — implementar `lse_inserir_ordenado` e ampliar os testes.

## Missão M05 — 23/11
- **Integrantes e papéis:** Codificador: Ygor Gutierrz | Revisor: André Silveira
- **Objetivo:** implementar `lse_inserir_ordenado` com comparador genérico, tratando lista vazia, início, meio, fim e empates estáveis, e ampliar a cobertura de testes com regressão das missões anteriores.
- **O que implementamos:** `lse_inserir_ordenado` em `src/lse.c`, documentação da função em `include/lse.h` e os 3 cenários de teste em `missoes/M05/test_m05.c` (substituindo os stubs `assert(1==0)`), instalando `missoes/M05/` sem alterar `Makefile` nem arquivos anteriores.
- **Decisão técnica e justificativa:** usamos os ponteiros `anterior` e `atual` avançando enquanto `atual != NULL && comparar(atual->carga, carga) <= 0`. O operador `<=` garante estabilidade (novos elementos com chave igual entram após os existentes, preservando a ordem de chegada). Complexidade de tempo O(n) no pior caso e espaço auxiliar O(1), mantendo `t_elem` privado e sem copiar cargas.
- **Problema encontrado e solução:** (1) na execução inicial antes da implementação, `test_m05.c` falhou conforme esperado em `assert(1==0)` na linha 20; após implementar a operação e preencher os 3 testes (usando `lse_percorrer` e o `musica_t` local do próprio teste), tudo compilou sem avisos. (2) a seção *Objetivos* do enunciado citava `t_lse_comparador`, enquanto o *Contrato* e `include/lse.h` usam `t_comparador`; mantivemos `t_comparador` para preservar o cabeçalho original.
- **Evidências de testes:** `mingw32-make -n test_missoes` descobriu `test_m05` automaticamente; compilação com GCC (`-std=c11 -Wall -Wextra -Wpedantic -g -Iinclude`) no Windows/MSYS2 sem avisos; aprovados `test_m05` (`M05: todos os testes passaram.`) e toda a regressão (`test_musica`, `test_lse`, `test_m02`, `test_m03`, `test_m04`).
- **Próxima ação:** M06 — implementar o TAD Pilha (`src/pilha.c`) como composição da LSE.

## Missão M06 — 25/11
- **Integrantes e papéis:** Codificador: André Silveira | Revisor: Ygor Gutierrz
- **Objetivo:** implementar o TAD Pilha genérico (LIFO) em `src/pilha.c` por composição sobre a LSE das M03–M05, sem expor `t_elem` nem integrar o histórico ainda.
- **O que implementamos:** `pilha_criar`, `pilha_empilhar`, `pilha_desempilhar`, `pilha_topo` e `pilha_destruir` em `src/pilha.c` delegando à interface pública `include/lse.h`; documentação das funções públicas em `include/pilha.h`; teste próprio de intercalação de operações em `tests/test_pilha.c`; instalação de `missoes/M06/` sem alterar `Makefile` nem arquivos anteriores.
- **Decisão técnica e justificativa:** mantivemos `struct pilha { t_lse *lista; };` privada em `src/pilha.c` e delegamos `empilhar`/`desempilhar`/`topo` para `lse_inserir_inicio`/`lse_remover_inicio`/`lse_primeiro`, garantindo O(1) em todas as operações de topo. Em `pilha_criar`, liberamos `pilha` se `lse_criar()` retornar `NULL` (tratamento de falha parcial). Não adicionamos validação para carga `NULL`, seguindo exatamente o contrato da M06. Colocamos o teste próprio de intercalação em `tests/test_pilha.c` para manter `missoes/M06/test_m06.c` intocado.
- **Problema encontrado e solução:** na execução inicial antes da implementação, `test_m06.c` falhou conforme esperado em `Assertion failed: p` (linha 8) devido aos stubs de `src/pilha.c`. Após implementar a delegação com `<stdlib.h>`, todos os testes passaram sem avisos.
- **Evidências de testes:** `mingw32-make -n test_missoes` detectou `test_m06` automaticamente; compilação com GCC (`-std=c11 -Wall -Wextra -Wpedantic -g -Iinclude`) no Windows/MSYS2 sem avisos; aprovados `test_m06` (`M06: todos os testes passaram.`), `test_pilha` (`test_pilha: OK`, incluindo o teste de intercalação) e toda a regressão (`test_musica`, `test_lse`, `test_m02`, `test_m03`, `test_m04`, `test_m05`). Apenas `test_fila` permanece como stub pendente para a M08.
- **Próxima ação:** M07 — integrar a pilha ao histórico de reprodução do MiniPlayer.
