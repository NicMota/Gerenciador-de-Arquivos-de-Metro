#include "auxiliares.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int parse_valor_nulo_fixo(const char *valor) {
    return valor != NULL && (strcmp(valor, "NULO") == 0 || strcmp(valor, "nulo") == 0);
}

int eh_string_nula(const char *valor) {
    return valor != NULL && (strcmp(valor, "NULO") == 0 || strcmp(valor, "nulo") == 0);
}

void normalizar_token(char *s) {
    size_t i = 0;
    size_t j = 0;
    if (s == NULL) {
        return;
    }
    while (s[i] != '\0') {
        if (s[i] != ';' && s[i] != '\r' && s[i] != '\n') {
            s[j++] = s[i];
        }
        i++;
    }
    s[j] = '\0';
}

void copiar_limpo(char *dst, size_t dst_sz, const char *src) {
    if (dst == NULL || dst_sz == 0) {
        return;
    }
    if (src == NULL) {
        dst[0] = '\0';
        return;
    }
    strncpy(dst, src, dst_sz - 1);
    dst[dst_sz - 1] = '\0';
    normalizar_token(dst);
}

int ler_token_custom(char *out, size_t out_sz) {
    int ch;
    size_t i = 0;

    if (out == NULL || out_sz == 0) {
        return 0;
    }

    do {
        ch = getchar();
        if (ch == EOF) {
            return 0;
        }
    } while (isspace(ch));

    if (ch == '\"') {
        ch = getchar();
        while (ch != EOF && ch != '\"' && i + 1 < out_sz) {
            out[i++] = (char)ch;
            ch = getchar();
        }
    } else {
        while (ch != EOF && !isspace(ch) && i + 1 < out_sz) {
            out[i++] = (char)ch;
            ch = getchar();
        }
    }

    out[i] = '\0';
    normalizar_token(out);
    return 1;
}

static int casa_criterio(const reg_dados *r, const criterio_busca *criterio) {
    registro_campos c;
    char campo[64];
    char valor[128];
    int n64;
    long n32;

    extrair_campos_registro(r, &c);
    copiar_limpo(campo, sizeof(campo), criterio->campo);
    copiar_limpo(valor, sizeof(valor), criterio->valor);

    if (strcmp(campo, "codestacao") == 0 || strcmp(campo, "codEstacao") == 0) {
        if (parse_valor_nulo_fixo(valor)) n32 = -1;
        else n32 = strtol(valor, NULL, 10);
        return c.codEstacao == (int)n32;
    }
    if (strcmp(campo, "codlinha") == 0 || strcmp(campo, "codLinha") == 0) {
        if (parse_valor_nulo_fixo(valor)) n32 = -1;
        else n32 = strtol(valor, NULL, 10);
        return c.codLinha == (int)n32;
    }
    if (strcmp(campo, "codproxestacao") == 0 ||
        strcmp(campo, "codProxEstacao") == 0 ||
        strcmp(campo, "codproxest") == 0) {
        if (parse_valor_nulo_fixo(valor)) n32 = -1;
        else n32 = strtol(valor, NULL, 10);
        return c.codProxEstacao == (int)n32;
    }
    if (strcmp(campo, "distproxestacao") == 0 ||
        strcmp(campo, "distProxEstacao") == 0) {
        if (parse_valor_nulo_fixo(valor)) n32 = -1;
        else n32 = strtol(valor, NULL, 10);
        return c.distProxEstacao == (int)n32;
    }
    if (strcmp(campo, "codlinhaintegra") == 0 ||
        strcmp(campo, "codLinhaIntegra") == 0) {
        if (parse_valor_nulo_fixo(valor)) n64 = -1;
        else n64 = atoi(valor);
        return c.codLinhaIntegra == n64;
    }
    if (strcmp(campo, "codestacaointegra") == 0 ||
        strcmp(campo, "codEstIntegra") == 0 ||
        strcmp(campo, "codestintegra") == 0) {
        if (parse_valor_nulo_fixo(valor)) n64 = -1;
        else n64 = atoi(valor);
        return c.codEstIntegra == n64;
    }
    if (strcmp(campo, "nomeestacao") == 0 || strcmp(campo, "nomeEstacao") == 0) {
        if (eh_string_nula(valor)) {
            return c.nomeEstacao[0] == '\0';
        }
        return strcmp(c.nomeEstacao, valor) == 0;
    }
    if (strcmp(campo, "nomelinha") == 0 || strcmp(campo, "nomeLinha") == 0) {
        if (eh_string_nula(valor)) {
            return c.nomeLinha[0] == '\0';
        }
        return strcmp(c.nomeLinha, valor) == 0;
    }
    return 0;
}

int registro_casa_criterios_and(const reg_dados *r, const criterio_busca *criterios, int m) {
    int i;
    if (r == NULL || criterios == NULL || m <= 0) {
        return 0;
    }
    for (i = 0; i < m; i++) {
        if (!casa_criterio(r, &criterios[i])) {
            return 0;
        }
    }
    return 1;
}