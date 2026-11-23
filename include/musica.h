#ifndef MUSICA_H
#define MUSICA_H

typedef struct musica {
    int id;
    char titulo[80];
    char artista[80];
    int duracao;
} t_musica;

/*
 * Cria e inicializa dinamicamente uma musica.
 * Parametros: id identificador, titulo e artista (nao nulos), duracao em segundos (>= 0).
 * Retorno: ponteiro para t_musica alocada no heap, ou NULL em parametros invalidos ou falha de alocacao.
 * Memoria: quem recebe o ponteiro torna-se dono da musica e deve libera-la com musica_destruir.
 */
t_musica *musica_criar(int id, const char *titulo, const char *artista, int duracao);

/*
 * Libera a memoria de uma musica alocada por musica_criar.
 * Parametro: carga (ponteiro generico para t_musica, compativel com callback da LSE; aceita NULL).
 * Memoria: libera apenas a estrutura apontada por carga.
 */
void musica_destruir(void *carga);

#endif
