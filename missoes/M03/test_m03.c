/* Testes aditivos da M03. Executar a partir da raiz com o Makefile incremental. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "lse.h"
#include "musica.h"

static int esperados[8];
static int n_esperados;
static int visitas;
static int destruidas;

static void verificar_ordem(void *carga) {
    assert(visitas < n_esperados);
    assert(*(int *)carga == esperados[visitas]);
    visitas++;
}

static void destruir_int(void *carga) {
    free(carga);
    destruidas++;
}

static void contar(void *carga) {
    (void)carga;
    visitas++;
}

static void preparar(int n, const int *vals) {
    n_esperados = n;
    visitas = 0;
    for (int i = 0; i < n; i++) {
        esperados[i] = vals[i];
    }
}

static void teste_lista_vazia(void) {
    t_lse *l = lse_criar();
    assert(l != NULL);
    assert(lse_tamanho(l) == 0);
    assert(lse_primeiro(l) == NULL);
    visitas = 0;
    lse_percorrer(l, contar);
    assert(visitas == 0);
    lse_percorrer(l, NULL);
    lse_percorrer(NULL, contar);
    assert(visitas == 0);
    lse_destruir(l, NULL);
}

static void teste_encadeamento_e_percurso(void) {
    int a = 1, b = 2, c = 3;
    t_lse *l = lse_criar();
    assert(l != NULL);
    assert(lse_inserir_inicio(l, &a) == 1);
    assert(lse_tamanho(l) == 1 && lse_primeiro(l) == &a);
    int um[] = {1};
    preparar(1, um);
    lse_percorrer(l, verificar_ordem);
    assert(visitas == 1);
    assert(lse_inserir_inicio(l, &b) == 1);
    assert(lse_tamanho(l) == 2 && lse_primeiro(l) == &b);
    assert(lse_inserir_inicio(l, &c) == 1);
    assert(lse_tamanho(l) == 3 && lse_primeiro(l) == &c);
    int ordem[] = {3, 2, 1};
    preparar(3, ordem);
    lse_percorrer(l, verificar_ordem);
    assert(visitas == 3);
    lse_destruir(l, NULL);
    assert(a == 1 && b == 2 && c == 3);
}

static void teste_carga_generica(void) {
    t_lse *l = lse_criar();
    assert(l != NULL);
    t_musica m = {7, "Titulo", "Artista", 200};
    assert(lse_inserir_inicio(l, &m) == 1);
    assert(lse_primeiro(l) == &m);
    assert(((t_musica *)lse_primeiro(l))->id == 7);
    assert(lse_inserir_inicio(l, NULL) == 1);
    assert(lse_tamanho(l) == 2 && lse_primeiro(l) == NULL);
    visitas = 0;
    lse_percorrer(l, contar);
    assert(visitas == 2);
    lse_destruir(l, NULL);
    assert(m.id == 7);
}

static void teste_destruicao_com_callback(void) {
    t_lse *l = lse_criar();
    assert(l != NULL);
    destruidas = 0;
    for (int i = 0; i < 4; i++) {
        int *p = malloc(sizeof(int));
        assert(p != NULL);
        *p = i;
        assert(lse_inserir_inicio(l, p) == 1);
    }
    lse_destruir(l, destruir_int);
    assert(destruidas == 4);
    lse_destruir(NULL, NULL);
}

int main(void) {
    teste_lista_vazia();
    teste_encadeamento_e_percurso();
    teste_carga_generica();
    teste_destruicao_com_callback();
    assert(lse_inserir_inicio(NULL, NULL) == 0);
    puts("M03: todos os testes passaram.");
    return 0;
}
