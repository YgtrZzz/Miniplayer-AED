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
- **Quando adicionar ponteiro `fim`?** Somente na M08 (otimização opcional para a fila), quando `lse_inserir_fim` precisar de custo O(1). Até a M07, `struct lse` mantém apenas `inicio` e `tamanho`.

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

## M06 — TAD Pilha por composição
Evolução do estado da pilha na sequência da Atividade 1 (empilhar A, B, C; consultar topo; desempilhar; empilhar D):

```
inicial:              topo ──► NULL
empilhar(A):          topo ──► [A|NULL]
empilhar(B):          topo ──► [B]──► [A|NULL]
empilhar(C):          topo ──► [C]──► [B]──► [A|NULL]
topo():               retorna C (pilha inalterada)
desempilhar():        retorna C -> topo ──► [B]──► [A|NULL]
empilhar(D):          topo ──► [D]──► [B]──► [A|NULL]
```

Ordem de saída restante ao desempilhar até esvaziar: **D, B, A** (política LIFO).

- **Composição:** `struct pilha { t_lse *lista; };` fica privada em `src/pilha.c`, e `include/pilha.h` expõe só o tipo opaco `t_pilha`. Todas as funções apenas chamam `lse.h` (`lse_criar`, `lse_inserir_inicio`, `lse_remover_inicio`, `lse_primeiro` e `lse_destruir`), sem mexer em `t_elem`, nos campos da `t_lse` ou incluir `lse.c`.
- **Propriedade das cargas:** a pilha não copia as cargas e não valida carga `NULL` (na M06 usamos só cargas não nulas para não confundir com pilha vazia). No histórico (M07), as músicas pertencem à biblioteca, então a pilha será destruída com `destruir_carga = NULL`. O callback só deve ser passado quando a pilha for a única dona das cargas.
- **Complexidade:** `pilha_criar`, `pilha_empilhar`, `pilha_desempilhar` e `pilha_topo` são **O(1)** (operações no início da LSE); `pilha_destruir` é **O(n)** (sem contar o callback).
- **Reflexões da M06:**
  - *Início O(1) vs. remover no fim da LSE:* no início basta ajustar `inicio`, em O(1). Remover no fim da LSE seria **O(n)** porque precisaríamos percorrer a lista inteira até achar o penúltimo nó para apontar o `prox` dele para `NULL`.
  - *Por que `pilha_topo` não remove:* consultar o topo é só leitura; se removesse, perderíamos o elemento do topo só por olhar.
  - *Por que destruir o histórico não libera as músicas:* a biblioteca é dona das músicas; liberar no histórico deixaria ponteiros inválidos na biblioteca e causaria liberação dupla depois.
  - *Composição vs. duplicação:* na composição, `pilha` reutiliza `lse` pela interface pública sem repetir código de nós e ponteiros; duplicar a lista repetiria lógica e daria mais margem para bugs e retrabalho.

## M07 — Histórico e navegação (`player`)
Novo módulo `player` (`include/player.h` e `src/player.c`) dependendo apenas de `pilha.h` e `musica.h` (sem acessar `lse.h` nem `t_elem`):

```
player -> (pilha historico, pilha avanco, t_musica *atual) -> pilha -> lse -> t_elem
```

Exemplo de estado (pilhas da base para o topo):
- `Criar`: `historico = []`, `atual = NULL`, `avanco = []`
- `Reproduzir A, B, C`: `historico = [A, B]`, `atual = C`, `avanco = []`
- `Anterior`, `Anterior`: `historico = []`, `atual = A`, `avanco = [C, B]`
- `Avançar`: `historico = [A]`, `atual = B`, `avanco = [C]`
- `Reproduzir D`: `historico = [A, B]`, `atual = D`, `avanco = []` (ramificação limpa o avanço)

- **Composição e encapsulamento:** `struct player { t_musica *atual; t_pilha *historico; t_pilha *avanco; };` fica privada em `src/player.c`. Toda navegação usa apenas `pilha_criar`, `pilha_empilhar`, `pilha_desempilhar`, `pilha_topo` e `pilha_destruir`.
- **Referências emprestadas:** o player não é dono das músicas; ele só guarda ponteiros. `player_destruir` chama `pilha_destruir(..., NULL)` nas duas pilhas e nunca libera as músicas. Também nunca empilhamos `NULL`.
- **Ordem segura em falha de alocação:** em `player_anterior` e `player_avancar`, primeiro consultamos `pilha_topo` na pilha de origem; depois tentamos `pilha_empilhar` a atual na pilha de destino; só se der certo chamamos `pilha_desempilhar` da origem. Em `player_reproduzir`, empilhamos a atual no histórico antes de limpar `avanco` e trocar `atual`. Assim, se faltar memória, o estado anterior fica intacto.
- **Complexidade:** `player_criar`, `player_anterior`, `player_avancar` e `player_atual` são **O(1)**; `player_reproduzir` é **O(1)** quando repete a mesma música ou `avanco` está vazio e **O(k)** quando precisa esvaziar `k` itens de `avanco`; `player_destruir` é **O(h + k)** (`h` itens no histórico e `k` no avanço).
- **Reflexões da M07:**
  - *Por que anterior e avançar custam O(1)?* Porque fazem só `pilha_topo`, `pilha_empilhar` e `pilha_desempilhar`, todas O(1) no topo da pilha.
  - *Por que selecionar outra música pode custar O(k)?* Porque ao tocar uma música diferente precisamos limpar todos os `k` elementos da pilha `avanco` um por um com `pilha_desempilhar`.
  - *Como impedir referências pendentes se a biblioteca excluir uma música?* Antes de liberar uma música na biblioteca, será preciso remover todas as referências a ela em `atual`, `historico` e `avanco` (ou limpar essas referências antes da exclusão, o que ficará para missões futuras).
  - *A mesma música pode aparecer em posições diferentes do histórico?* Sim. Ao tocar `A → B → A → C`, a música `A` aparece duas vezes no histórico porque ele guarda a ordem de reprodução (apenas ponteiros para a mesma música).
