#include "musica.h"
#include <stdlib.h>
#include <string.h>

/* Copia uma string com limite de tamanho, garantindo o terminador '\0'. */
static void copiar_texto(char *dest, size_t tam, const char *orig) {
    strncpy(dest, orig, tam - 1);
    dest[tam - 1] = '\0';
}

t_musica *musica_criar(int id, const char *titulo, const char *artista, int duracao) {
    if (titulo == NULL || artista == NULL || duracao < 0) {
        return NULL;
    }

    t_musica *m = malloc(sizeof(t_musica));
    if (m == NULL) {
        return NULL;
    }

    m->id = id;
    copiar_texto(m->titulo, sizeof(m->titulo), titulo);
    copiar_texto(m->artista, sizeof(m->artista), artista);
    m->duracao = duracao;
    return m;
}

void musica_destruir(void *carga) {
    free(carga);
}
