#ifndef PLAYER_H
#define PLAYER_H

#include "musica.h"

/* Tipo opaco: musica atual e pilhas de historico/avanco ficam em player.c. */
typedef struct player t_player;

/* Cria controlador sem musica atual e com pilhas vazias. Retorna NULL se faltar memoria. */
t_player *player_criar(void);

/* Reproduz m (empilha atual no historico e limpa avanco). Retorna 1 ou 0 em erro. */
int player_reproduzir(t_player *p, t_musica *m);

/* Volta para o topo do historico, movendo a atual para o avanco (O(1)). Retorna 1 ou 0. */
int player_anterior(t_player *p);

/* Avanca para o topo do avanco, movendo a atual para o historico (O(1)). Retorna 1 ou 0. */
int player_avancar(t_player *p);

/* Retorna a musica atual, ou NULL se nao houver musica ou p == NULL (O(1)). */
t_musica *player_atual(const t_player *p);

/* Libera as duas pilhas e o player, sem liberar as musicas. Aceita NULL. */
void player_destruir(t_player *p);

#endif
