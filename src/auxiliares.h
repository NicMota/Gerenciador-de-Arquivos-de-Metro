#ifndef AUXILIARES
#define AUXILIARES

#include "registro_dados.h"
#include <stddef.h>

#define MAX_CRITERIOS 32

typedef struct criterio_busca {
    char campo[64];
    char valor[128];
} criterio_busca;

/*
    Verifica se um registro satisfaz todos os criterios de busca (AND).
*/
int registro_casa_criterios_and(const reg_dados *r, const criterio_busca *criterios, int m);

/* Retorna 1 quando o valor representa nulo de campo fixo: NULO/nulo. */
int parse_valor_nulo_fixo(const char *valor);

/* Retorna 1 quando o valor representa nulo de string: NULO/nulo. */
int eh_string_nula(const char *valor);

/*
    Remove caracteres de controle usados na entrada (ex.: ';', '\n', '\r').
*/
void normalizar_token(char *s);

/*
    Copia uma string de forma segura para um buffer e normaliza o token.
*/
void copiar_limpo(char *dst, size_t dst_sz, const char *src);

/*
    Le um token da entrada padrao.
    Suporta strings entre aspas duplas e valores sem aspas.
*/
int ler_token_custom(char *out, size_t out_sz);

#endif