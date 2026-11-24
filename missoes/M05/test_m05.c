/* Testes incrementais M05 — não acessar campos privados de t_lse/t_elem. */
#include <assert.h>
#include <stdio.h>
#include "lse.h"

static int comparar_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

typedef struct { int id; const char *titulo; } musica_t;
static int comparar_musica(const void *a, const void *b) {
    int x = ((const musica_t *)a)->id, y = ((const musica_t *)b)->id;
    return (x > y) - (x < y);
}

static const void *esperados[8];
static int visitas;

static void verificar(void *carga) {
    assert(visitas < 8);
    assert(carga == esperados[visitas]);
    visitas++;
}

static void conferir(t_lse *l, int n) {
    assert(lse_tamanho(l) == n);
    visitas = 0;
    lse_percorrer(l, verificar);
    assert(visitas == n);
}

static void teste_vazia_inicio_meio_fim(void) {
    int a = 10, b = 20, c = 25, d = 30;
    t_lse *l = lse_criar(); assert(l);

    /* lista vazia */
    assert(lse_inserir_ordenado(l, &b, comparar_int) == 1);
    esperados[0] = &b;
    conferir(l, 1);
    assert(lse_primeiro(l) == &b);

    /* inicio */
    assert(lse_inserir_ordenado(l, &a, comparar_int) == 1);
    esperados[0] = &a; esperados[1] = &b;
    conferir(l, 2);
    assert(lse_primeiro(l) == &a);

    /* fim */
    assert(lse_inserir_ordenado(l, &d, comparar_int) == 1);
    esperados[0] = &a; esperados[1] = &b; esperados[2] = &d;
    conferir(l, 3);

    /* meio */
    assert(lse_inserir_ordenado(l, &c, comparar_int) == 1);
    esperados[0] = &a; esperados[1] = &b; esperados[2] = &c; esperados[3] = &d;
    conferir(l, 4);
    assert(lse_primeiro(l) == &a);

    lse_destruir(l, NULL);
}

static void teste_duplicados_estaveis(void) {
    musica_t a={10,"primeira"}, b={10,"segunda"}, c={5,"menor"}, d={10,"terceira"};
    t_lse *l=lse_criar(); assert(l);

    assert(lse_inserir_ordenado(l, &a, comparar_musica) == 1);
    assert(lse_inserir_ordenado(l, &b, comparar_musica) == 1);
    assert(lse_inserir_ordenado(l, &c, comparar_musica) == 1);
    assert(lse_inserir_ordenado(l, &d, comparar_musica) == 1);

    esperados[0] = &c;
    esperados[1] = &a;
    esperados[2] = &b;
    esperados[3] = &d;
    conferir(l, 4);
    assert(lse_primeiro(l) == &c);

    lse_destruir(l, NULL);
}

static void teste_invalido_e_reinsercao(void) {
    int a = 10, b = 20, c = 30;

    assert(lse_inserir_ordenado(NULL, &a, comparar_int) == 0);

    t_lse *l = lse_criar(); assert(l);
    assert(lse_inserir_ordenado(l, &a, NULL) == 0);
    conferir(l, 0);

    assert(lse_inserir_ordenado(l, &b, comparar_int) == 1);
    assert(lse_inserir_ordenado(l, &a, NULL) == 0);
    esperados[0] = &b;
    conferir(l, 1);

    assert(lse_inserir_ordenado(l, &c, comparar_int) == 1);
    assert(lse_inserir_ordenado(l, &a, comparar_int) == 1);
    esperados[0] = &a; esperados[1] = &b; esperados[2] = &c;
    conferir(l, 3);

    /* remover do meio e reinserir mantendo ordenacao */
    assert(lse_remover(l, &b, comparar_int) == &b);
    esperados[0] = &a; esperados[1] = &c;
    conferir(l, 2);
    assert(lse_inserir_ordenado(l, &b, comparar_int) == 1);
    esperados[0] = &a; esperados[1] = &b; esperados[2] = &c;
    conferir(l, 3);

    /* esvaziar e reinserir */
    assert(lse_remover_inicio(l) == &a);
    assert(lse_remover_inicio(l) == &b);
    assert(lse_remover_inicio(l) == &c);
    conferir(l, 0);
    assert(lse_inserir_ordenado(l, &c, comparar_int) == 1);
    assert(lse_inserir_ordenado(l, &a, comparar_int) == 1);
    esperados[0] = &a; esperados[1] = &c;
    conferir(l, 2);

    lse_destruir(l, NULL);
}

int main(void) {
    teste_vazia_inicio_meio_fim();
    teste_duplicados_estaveis();
    teste_invalido_e_reinsercao();
    puts("M05: todos os testes passaram.");
    return 0;
}
