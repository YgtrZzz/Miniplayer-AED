/* M04: testes incrementais. Não modificar asserções para ocultar defeitos. */
#include <assert.h>
#include <stdio.h>
#include "lse.h"
#include "musica.h"

static int comparar_int(const void *carga, const void *chave) {
    int a = *(const int *)carga, b = *(const int *)chave;
    return (a > b) - (a < b);
}
static int comparar_musica_id(const void *carga, const void *chave) {
    int a = ((const t_musica *)carga)->id;
    int b = *(const int *)chave;
    return (a > b) - (a < b);
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
static void teste_inserir_fim_e_buscar(void) {
    int a=10,b=20,c=30,ausente=99;
    t_lse *l=lse_criar(); assert(l);
    // inserção no fim em lista vazia e não vazia; ordem preservada
    assert(lse_inserir_fim(l,&a)==1);
    esperados[0]=&a; conferir(l,1);
    assert(lse_primeiro(l)==&a);
    assert(lse_inserir_fim(l,&b)==1);
    assert(lse_inserir_fim(l,&c)==1);
    esperados[1]=&b; esperados[2]=&c; conferir(l,3);
    assert(lse_primeiro(l)==&a);

    // busca por elementos presentes (primeiro, intermediário, último) e ausente
    assert(lse_buscar(l,&a,comparar_int)==&a);
    assert(lse_buscar(l,&b,comparar_int)==&b);
    assert(lse_buscar(l,&c,comparar_int)==&c);
    assert(lse_buscar(l,&ausente,comparar_int)==NULL);
    conferir(l,3); // busca não altera a lista
    lse_destruir(l,NULL);
}
static void teste_remover_cabeca_meio_cauda(void) {
    int a=10,b=20,c=30,d=40;
    t_lse *l=lse_criar(); assert(l);
    assert(lse_inserir_fim(l,&a)==1);
    assert(lse_inserir_fim(l,&b)==1);
    assert(lse_inserir_fim(l,&c)==1);
    assert(lse_inserir_fim(l,&d)==1);
    // remover do meio
    assert(lse_remover(l,&b,comparar_int)==&b);
    
    esperados[0]=&a; esperados[1]=&c; esperados[2]=&d; conferir(l,3);
    assert(lse_remover(l,&b,comparar_int)==NULL); // já removido

    // remover do fim
    assert(lse_remover(l,&d,comparar_int)==&d);
    esperados[0]=&a; esperados[1]=&c; conferir(l,2);
    // inserir no fim após remover o último: encadeamento íntegro
    assert(lse_inserir_fim(l,&d)==1);
    esperados[2]=&d; conferir(l,3);
    assert(lse_remover(l,&d,comparar_int)==&d);

    // remover do inicio
    assert(lse_remover(l,&a,comparar_int)==&a);
    assert(lse_primeiro(l)==&c);
    esperados[0]=&c; conferir(l,1);

    // único elemento: lista volta a ficar vazia
    assert(lse_remover(l,&c,comparar_int)==&c);
    assert(lse_tamanho(l)==0 && lse_primeiro(l)==NULL);
    conferir(l,0);

    // reinserção após esvaziar
    assert(lse_inserir_fim(l,&b)==1);
    assert(lse_inserir_inicio(l,&a)==1);
    esperados[0]=&a; esperados[1]=&b; conferir(l,2);

    lse_destruir(l,NULL);
}
static void teste_remover_inicio(void) {
    int a=5,b=5,c=8,ausente=99;
    t_lse *l=lse_criar(); assert(l);
    assert(lse_remover_inicio(l)==NULL);
    assert(lse_inserir_fim(l,&a)==1);
    assert(lse_inserir_fim(l,&b)==1);
    assert(lse_inserir_fim(l,&c)==1);

    // remover do início de lista com elementos, na ordem de inserção
    assert(lse_remover_inicio(l)==&a);
    esperados[0]=&b; esperados[1]=&c; conferir(l,2);

    // chave duplicada: remove só a primeira ocorrência (restou b, que vale 5)
    assert(lse_inserir_fim(l,&a)==1); // lista: b(5), c(8), a(5)
    assert(lse_remover(l,&a,comparar_int)==&b); // primeira com valor 5 é b
    esperados[0]=&c; esperados[1]=&a; conferir(l,2);
    assert(lse_buscar(l,&a,comparar_int)==&a); // a segunda ocorrência permanece
    assert(lse_buscar(l,&ausente,comparar_int)==NULL);
    assert(lse_remover(l,&ausente,comparar_int)==NULL);
    conferir(l,2);

    // esvazia e tenta remover de lista vazia
    assert(lse_remover_inicio(l)==&c);
    assert(lse_remover_inicio(l)==&a);
    assert(lse_tamanho(l)==0);
    assert(lse_remover_inicio(l)==NULL);
    conferir(l,0);
    lse_destruir(l,NULL);
}
static void teste_generico_musicas(void) {
    t_musica a={11,"A","X",100}, b={22,"B","Y",200};
    int chave=22;
    t_lse *l=lse_criar(); assert(l);
    assert(lse_inserir_fim(l,&a)==1);
    assert(lse_inserir_fim(l,&b)==1);
    assert(lse_buscar(l,&chave,comparar_musica_id)==&b);
    assert(lse_remover(l,&chave,comparar_musica_id)==&b);
    esperados[0]=&a; conferir(l,1);
    lse_destruir(l,NULL);
}

static void teste_argumentos_invalidos(void) {
    int x=1;
    assert(lse_inserir_fim(NULL,&x)==0);
    assert(lse_buscar(NULL,&x,comparar_int)==NULL);
    assert(lse_remover(NULL,&x,comparar_int)==NULL);
    assert(lse_remover_inicio(NULL)==NULL);

    t_lse *l=lse_criar(); assert(l);
    assert(lse_buscar(l,&x,comparar_int)==NULL);
    assert(lse_remover(l,&x,comparar_int)==NULL);
    assert(lse_inserir_inicio(l,&x)==1);
    assert(lse_buscar(l,&x,NULL)==NULL);
    assert(lse_remover(l,&x,NULL)==NULL);
    esperados[0]=&x; conferir(l,1);
    lse_destruir(l,NULL);
}

int main(void) {
    teste_inserir_fim_e_buscar();
    teste_remover_cabeca_meio_cauda();
    teste_remover_inicio();
    teste_generico_musicas();
    teste_argumentos_invalidos();
    puts("M04: todos os testes passaram.");
    return 0;
}
