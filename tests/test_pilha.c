#include <assert.h>
#include <stdio.h>
#include "pilha.h"

/* Teste proprio da M06: intercalacao de empilhar, consultar topo e desempilhar. */
static void teste_intercalacao(void) {
    char a = 'A', b = 'B', c = 'C', d = 'D';
    t_pilha *p = pilha_criar();
    assert(p != NULL);

    /* Empilha A, B, C e consulta topo sem remover */
    assert(pilha_empilhar(p, &a) == 1);
    assert(pilha_empilhar(p, &b) == 1);
    assert(pilha_empilhar(p, &c) == 1);
    assert(pilha_topo(p) == &c);
    assert(pilha_topo(p) == &c);

    /* Desempilha C, empilha D e verifica ordem restante: D, B, A */
    assert(pilha_desempilhar(p) == &c);
    assert(pilha_topo(p) == &b);
    assert(pilha_empilhar(p, &d) == 1);
    assert(pilha_topo(p) == &d);
    assert(pilha_desempilhar(p) == &d);
    assert(pilha_desempilhar(p) == &b);
    assert(pilha_desempilhar(p) == &a);
    assert(pilha_topo(p) == NULL);
    assert(pilha_desempilhar(p) == NULL);

    /* Reuso apos esvaziar */
    assert(pilha_empilhar(p, &b) == 1);
    assert(pilha_empilhar(p, &d) == 1);
    assert(pilha_desempilhar(p) == &d);
    assert(pilha_topo(p) == &b);
    assert(pilha_desempilhar(p) == &b);
    assert(pilha_desempilhar(p) == NULL);

    pilha_destruir(p, NULL);
}

int main(void) {
    t_pilha *p = pilha_criar(); assert(p != NULL);
    int a=1,b=2; assert(pilha_empilhar(p,&a)); assert(pilha_empilhar(p,&b));
    assert(pilha_desempilhar(p)==&b); assert(pilha_desempilhar(p)==&a);
    assert(pilha_desempilhar(p)==NULL);
    pilha_destruir(p,NULL);
    teste_intercalacao();
    puts("test_pilha: OK"); return 0;
}
