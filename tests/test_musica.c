#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "musica.h"

static void test_criar_valida(void) {
    t_musica *m = musica_criar(1, "Teste", "Artista", 180);
    assert(m != NULL);
    assert(m->id == 1 && strcmp(m->titulo, "Teste") == 0);
    assert(strcmp(m->artista, "Artista") == 0);
    assert(m->duracao == 180);
    musica_destruir(m);
}

static void test_copia_independente(void) {
    char titulo[] = "Original";
    t_musica *m = musica_criar(2, titulo, "A", 10);
    assert(m != NULL);
    titulo[0] = 'X';
    assert(strcmp(m->titulo, "Original") == 0);
    musica_destruir(m);
}

static void test_truncamento(void) {
    char longo[200];
    memset(longo, 'a', sizeof longo - 1);
    longo[sizeof longo - 1] = '\0';
    t_musica *m = musica_criar(3, longo, longo, 0);
    assert(m != NULL);
    assert(strlen(m->titulo) == sizeof m->titulo - 1);
    assert(strlen(m->artista) == sizeof m->artista - 1);
    musica_destruir(m);
}

static void test_parametros_invalidos(void) {
    assert(musica_criar(4, NULL, "A", 10) == NULL);
    assert(musica_criar(4, "T", NULL, 10) == NULL);
    assert(musica_criar(4, "T", "A", -1) == NULL);
}

static void test_destruir_nulo(void) {
    musica_destruir(NULL);
}

int main(void) {
    test_criar_valida();
    test_copia_independente();
    test_truncamento();
    test_parametros_invalidos();
    test_destruir_nulo();
    puts("test_musica: OK");
    return 0;
}
