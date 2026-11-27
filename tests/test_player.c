#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "musica.h"

/* Teste proprio da M07: tabela do enunciado, repeticao no historico e referencias emprestadas. */
static void teste_exemplo_e_ramificacao(void) {
    t_musica a = {.id = 1}, b = {.id = 2}, c = {.id = 3}, d = {.id = 4};
    t_player *p = player_criar();
    assert(p != NULL);

    /* Sequencia da tabela: A -> B -> C -> Anterior -> Anterior -> Avancar -> D */
    assert(player_reproduzir(p, &a) == 1 && player_atual(p) == &a);
    assert(player_reproduzir(p, &b) == 1 && player_atual(p) == &b);
    assert(player_reproduzir(p, &c) == 1 && player_atual(p) == &c);
    assert(player_anterior(p) == 1 && player_atual(p) == &b);
    assert(player_anterior(p) == 1 && player_atual(p) == &a);
    assert(player_avancar(p) == 1 && player_atual(p) == &b);
    assert(player_reproduzir(p, &d) == 1 && player_atual(p) == &d);

    /* Avanco foi limpo ao tocar D; historico tem A, B */
    assert(player_avancar(p) == 0);
    assert(player_anterior(p) == 1 && player_atual(p) == &b);
    assert(player_anterior(p) == 1 && player_atual(p) == &a);
    assert(player_anterior(p) == 0);

    player_destruir(p);
}

static void teste_repeticao_e_posse_heap(void) {
    t_musica *m1 = malloc(sizeof(t_musica));
    t_musica *m2 = malloc(sizeof(t_musica));
    t_musica *m3 = malloc(sizeof(t_musica));
    assert(m1 != NULL && m2 != NULL && m3 != NULL);
    m1->id = 10; m2->id = 20; m3->id = 30;

    t_player *p = player_criar();
    assert(p != NULL);

    /* Mesma musica (m1) aparece em posicoes diferentes do historico: m1 -> m2 -> m1 -> m3 */
    assert(player_reproduzir(p, m1) == 1);
    assert(player_reproduzir(p, m2) == 1);
    assert(player_reproduzir(p, m1) == 1);
    assert(player_reproduzir(p, m3) == 1);

    /* Volta e preserva avanco ao reproduzir a mesma musica atual */
    assert(player_anterior(p) == 1 && player_atual(p) == m1);
    assert(player_reproduzir(p, m1) == 1 && player_atual(p) == m1);
    assert(player_avancar(p) == 1 && player_atual(p) == m3);

    /* Volta por todo o historico: m3 -> m1 -> m2 -> m1 */
    assert(player_anterior(p) == 1 && player_atual(p) == m1);
    assert(player_anterior(p) == 1 && player_atual(p) == m2);
    assert(player_anterior(p) == 1 && player_atual(p) == m1);
    assert(player_anterior(p) == 0);

    /* Destruir player nao libera as musicas do chamador */
    player_destruir(p);
    assert(m1->id == 10 && m2->id == 20 && m3->id == 30);

    free(m1);
    free(m2);
    free(m3);
}

int main(void) {
    teste_exemplo_e_ramificacao();
    teste_repeticao_e_posse_heap();
    puts("test_player: OK");
    return 0;
}
