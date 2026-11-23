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
- **Quando adicionar ponteiro `fim`?** Somente na M08 (otimização opcional para a fila), quando `lse_inserir_fim` precisar de custo O(1). Até a M04, `struct lse` mantém apenas `inicio` e `tamanho`.

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
