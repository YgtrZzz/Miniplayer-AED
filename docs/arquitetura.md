# Arquitetura
## Componentes
| Módulo | Responsabilidade | Dependências |
|---|---|---|
| musica | Dados da música | biblioteca padrão |
| lse | Nós genéricos e encadeamento | biblioteca padrão |
| pilha | Política LIFO | lse |
| fila | Política FIFO | lse |
| miniplayer | Simulação e integração | demais TADs |

## Diagrama de dependências
MiniPlayer → (LSE, Pilha, Fila) → LSE → t_elem interno.

## Decisões técnicas
- **Por que `void *`?** Permite que a mesma LSE armazene qualquer tipo de dado (`int`, `t_musica`, etc.) sem duplicar código e sem acoplar `lse.c` ao módulo `musica`. Operações que precisam inspecionar ou liberar a carga recebem funções callback (`t_comparador`, `t_lse_visitar`, `destruir_carga`).
- **Quem é dono das cargas?** Quem alocou a carga (por exemplo, a biblioteca do MiniPlayer é dona das `t_musica`). A LSE é dona apenas dos nós (`t_elem`); ela só libera as cargas se `lse_destruir` receber um callback destruidor não nulo.
- **Como tratar referências compartilhadas?** Como a biblioteca possui as músicas e estruturas futuras (fila e histórico) apenas guardam ponteiros para elas, ao remover ou liberar uma música da biblioteca será necessário remover antes suas referências na fila e no histórico, e destruir fila/histórico passando `NULL` no callback de destruição para evitar *double free* ou ponteiros inválidos.
- **Quando adicionar ponteiro `fim`?** Somente na M08 (otimização opcional para a fila), quando `lse_inserir_fim` precisar de custo O(1). Até a M05, `struct lse` mantém apenas `inicio` e `tamanho`.

## M01 — Módulo `musica` e memória dinâmica
- `t_musica` representa o registro de domínio com `id`, `titulo[80]`, `artista[80]` e `duracao`.
- `musica_criar` aloca a estrutura dinamicamente no heap e copia as strings de forma independente, com truncamento seguro garantindo `'\0'` ao final.
- `musica_destruir` recebe `void *carga` para já ser compatível com o callback de destruição da LSE nas missões seguintes.

## M02 — TADs, encapsulamento e contratos da LSE
- Separação entre interface pública (`include/lse.h`) e representação interna (`src/lse.c`).
- `t_lse` é exposto apenas como tipo opaco (`typedef struct lse t_lse;`), impedindo que módulos clientes acessem `inicio`, `tamanho` ou `t_elem` diretamente.
- `t_elem` permanece exclusivamente em `src/lse.c`, garantindo que futuras estruturas (pilha e fila) reutilizem as funções `lse_*` em vez de manipular ponteiros de nó.

## M03 — Construção e percurso da lista
Encadeamento após inserir `c`, `b`, `a` (nessa ordem) no início:

```
t_lse: inicio ──► [a|•]──► [b|•]──► [c|NULL]     tamanho = 3
```

- `t_elem` e os campos de `t_lse` continuam em `src/lse.c`; `lse.h` só expõe o tipo opaco `t_lse` e o callback `t_lse_visitar`.
- Dono do nó: a LSE (aloca em `lse_inserir_inicio`, libera em `lse_destruir`). Dono da carga: quem a criou; a LSE só a libera se `lse_destruir` receber um destruidor.
- O enunciado cita `t_visitar` na assinatura de `lse_percorrer`; usamos `t_lse_visitar`, nome do `typedef`, para manter a declaração consistente.

## M04 — Inserção no fim, busca e remoção
Estados e ponteiros alterados nas operações de remoção:

```
vazia:      inicio ──► NULL                                  tamanho = 0
um nó:      inicio ──► [a|NULL]                              remover(a): inicio = NULL
três nós:   inicio ──► [a|•]──► [b|•]──► [c|NULL]            tamanho = 3
  remover início (a): inicio = a->prox            -> inicio ──► [b]──► [c]
  remover meio   (b): a->prox = b->prox           -> [a]──► [c]
  remover fim    (c): b->prox = NULL              -> [a]──► [b]──► NULL
```

- `lse_inserir_fim` trata lista vazia (`inicio = novo`) e lista não vazia percorrendo até o último nó (`atual->prox == NULL`), com custo O(n) e sem adicionar ponteiro `fim` (previsto só na M08).
- `lse_remover` percorre a lista mantendo dois ponteiros (`anterior` e `atual`): se `anterior == NULL`, atualiza `lista->inicio = atual->prox`; caso contrário, religa `anterior->prox = atual->prox`.
- Callback de comparação (`t_comparador`): a lista só chama `comparar(carga, chave)`; quem conhece `t_musica` é o teste/cliente. `lse.c` continua independente de `t_musica` e `t_elem` continua privado.
- Dono do nó: a LSE (aloca em `inserir_*`, libera em `remover*` e `destruir`). Dono da carga: quem a criou; `lse_remover` e `lse_remover_inicio` **devolvem** a carga ao chamador, que decide se a libera (ex.: `musica_destruir`).
- O enunciado chama o tipo do callback de `t_comparador_lse`; o `lse.h` do projeto usa `t_comparador`, mantido para preservar o cabeçalho original.

## M05 — Inserção ordenada e testes
Ponteiros `anterior`, `atual` e `novo` nas quatro situações de `lse_inserir_ordenado`:

```
1) Lista vazia (inserir 20):
   anterior = NULL, atual = NULL
   novo->prox = NULL; inicio = novo             -> inicio ──► [20|NULL]

2) Início (inserir 10 em [20]):
   anterior = NULL, atual = [20]
   novo->prox = [20]; inicio = novo             -> inicio ──► [10]──► [20|NULL]

3) Meio (inserir 25 em [10]──►[20]──►[30]):
   anterior = [20], atual = [30]
   novo->prox = [30]; [20]->prox = novo         -> [10]──►[20]──►[25]──►[30|NULL]

4) Fim (inserir 30 em [10]──►[20]):
   anterior = [20], atual = NULL
   novo->prox = NULL; [20]->prox = novo         -> [10]──►[20]──►[30|NULL]
```

Pseudocódigo da inserção ordenada estável:

```text
se lista == NULL ou comparar == NULL então retorne 0
novo <- alocar nó com carga
se novo == NULL então retorne 0
anterior <- NULL
atual <- lista->inicio
enquanto atual != NULL e comparar(atual->carga, carga_nova) <= 0 faça
    anterior <- atual
    atual <- atual->prox
novo->prox <- atual
se anterior == NULL então lista->inicio <- novo
senão anterior->prox <- novo
lista->tamanho <- lista->tamanho + 1
retorne 1
```

- **Estabilidade em empates:** o laço avança enquanto `atual != NULL && comparar(atual->carga, carga) <= 0`. Por usar `<= 0` (em vez de `< 0`), elementos com mesma chave são inseridos **após** os já existentes, preservando a ordem de chegada.
- **Complexidade:** tempo **O(1)** no melhor caso (lista vazia ou menor que o primeiro nó) e **O(n)** no caso médio e pior caso (meio ou fim); espaço auxiliar **O(1)** (apenas ponteiros locais e alocação de um único nó `t_elem`).
- **Encapsulamento e memória:** `t_elem` permanece privado em `src/lse.c` e a lista não conhece `t_musica`; a inserção aloca apenas o nó e guarda o ponteiro da carga sem copiá-la.
- **Reflexões da M05:**
  - *Inserção no início O(1) vs. inserção ordenada O(n):* inserir no início só religa `lista->inicio` em tempo constante; inserir ordenado exige percorrer a cadeia linearmente até localizar a posição correta (até `n` comparações).
  - *Risco de critérios diferentes na mesma lista:* a pré-condição de ordenação é quebrada; o percurso para na primeira carga maior segundo o novo critério e a lista deixa de ficar globalmente ordenada para qualquer critério.
  - *Múltiplos critérios sem alterar `lse.c`:* como `lse_inserir_ordenado` delega a ordem ao callback `comparar(carga_existente, carga_nova)`, basta o chamador passar callbacks distintos (por título com `strcmp`, por duração ou por `id`) sem nenhuma alteração no módulo da lista.
  - *Ordem relativa de chaves iguais:* mantida integralmente (algoritmo estável), porque `comparar(...) <= 0` percorre todos os empates antes de inserir o novo nó.
