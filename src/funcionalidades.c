#include "funcionalidades.h"

#include "auxiliares.h"
#include "cabecalho.h"
#include "fornecidas.h"
#include "registro_dados.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALHA_MSG "Falha no processamento do arquivo"

static int seek_rrn(FILE *arquivo, int rrn) {
    long offset = CABECALHO_SZ + (long)rrn * REGISTRO_SZ;
    return fseek(arquivo, offset, SEEK_SET) == 0;
}

static void trim_token(char *s) {
    size_t ini = 0;
    size_t fim;
    size_t i = 0;

    if (s == NULL) {
        return;
    }
    while (s[ini] != '\0' && isspace((unsigned char)s[ini])) {
        ini++;
    }

    fim = strlen(s);
    while (fim > ini && isspace((unsigned char)s[fim - 1])) {
        fim--;
    }

    while (ini < fim) {
        s[i++] = s[ini++];
    }
    s[i] = '\0';

    if (i >= 2 && s[0] == '"' && s[i - 1] == '"') {
        memmove(s, s + 1, i - 2);
        s[i - 2] = '\0';
    }
}

static int parse_inteiro(const char *token) {
    if (parse_valor_nulo_fixo(token)) {
        return -1;
    }
    return atoi(token);
}

static void parse_string(char *dst, size_t dst_sz, const char *token) {
    if (eh_string_nula(token)) {
        dst[0] = '\0';
        return;
    }
    copiar_limpo(dst, dst_sz, token);
}

static int ler_linha(FILE *f, char *linha, size_t tam) {
    if (fgets(linha, (int)tam, f) == NULL) {
        return 0;
    }
    normalizar_token(linha);
    return 1;
}

static int linha_csv_para_campos(char *linha, registro_campos *campos) {
    char *cursor = linha;
    char *tokens[8];
    int i = 0;
    char *inicio = linha;

    while (*cursor != '\0' && i < 8) {
        if (*cursor == ',') {
            *cursor = '\0';
            tokens[i++] = inicio;
            inicio = cursor + 1;
        }
        cursor++;
    }
    tokens[i++] = inicio;
    if (i != 8) {
        return 0;
    }

    memset(campos, 0, sizeof(*campos));
    for (i = 0; i < 8; i++) {
        trim_token(tokens[i]);
    }

    if (strcmp(tokens[0], "codEstacao") == 0 || strcmp(tokens[0], "codestacao") == 0) {
        return 2;
    }

    campos->codEstacao = parse_inteiro(tokens[0]);
    parse_string(campos->nomeEstacao, sizeof(campos->nomeEstacao), tokens[1]);
    campos->codLinha = parse_inteiro(tokens[2]);
    parse_string(campos->nomeLinha, sizeof(campos->nomeLinha), tokens[3]);
    campos->codProxEstacao = parse_inteiro(tokens[4]);
    campos->distProxEstacao = parse_inteiro(tokens[5]);
    campos->codLinhaIntegra = parse_inteiro(tokens[6]);
    campos->codEstIntegra = parse_inteiro(tokens[7]);
    return 1;
}

static int ler_criterios_da_entrada(int m, criterio_busca *criterios) {
    int i;
    for (i = 0; i < m; i++) {
        char campo[64];
        char valor[128];

        if (!ler_token_custom(campo, sizeof(campo))) {
            return 0;
        }
        if (!ler_token_custom(valor, sizeof(valor))) {
            return 0;
        }
        copiar_limpo(criterios[i].campo, sizeof(criterios[i].campo), campo);
        copiar_limpo(criterios[i].valor, sizeof(criterios[i].valor), valor);
    }
    return 1;
}

static int abrir_binario_leitura(const char *nome, FILE **arquivo, cabecalho *c) {
    FILE *f = fopen(nome, "rb");
    if (f == NULL) {
        return 0;
    }
    if (!ler_cabecalho(f, c) || c->status != '0') {
        fclose(f);
        return 0;
    }
    *arquivo = f;
    return 1;
}

static int abrir_binario_escrita(const char *nome, FILE **arquivo, cabecalho *c) {
    FILE *f = fopen(nome, "r+b");
    if (f == NULL) {
        return 0;
    }
    if (!ler_cabecalho(f, c) || c->status != '0') {
        fclose(f);
        return 0;
    }
    c->status = '1';
    if (!escrever_cabecalho(f, c)) {
        fclose(f);
        return 0;
    }
    *arquivo = f;
    return 1;
}

static int vetor_int_contem(const int *vetor, int n, int valor) {
    int i;
    for (i = 0; i < n; i++) {
        if (vetor[i] == valor) {
            return 1;
        }
    }
    return 0;
}

static int vetor_pares_contem(const par *vetor, int n, int codEstacao, int codProxEstacao) {
    int i;
    for (i = 0; i < n; i++) {
        if (vetor[i].codEstacao == codEstacao && vetor[i].codProxEstacao == codProxEstacao) {
            return 1;
        }
    }
    return 0;
}

static int recalcular_estatisticas(FILE *arquivo, cabecalho *c) {
    int *estacoes = NULL;
    par *pares = NULL;
    int capEstacoes = 0;
    int capPares = 0;
    int nEstacoes = 0;
    int nPares = 0;
    int rrn;

    for (rrn = 0; rrn < c->proxRRN; rrn++) {
        reg_dados r;
        if (!seek_rrn(arquivo, rrn) || !ler_registro(arquivo, &r)) {
            free(estacoes);
            free(pares);
            return 0;
        }
        if (r.removido == '0') {
            continue;
        }

        if (r.codEstacao != -1 && !vetor_int_contem(estacoes, nEstacoes, r.codEstacao)) {
            if (nEstacoes == capEstacoes) {
                int novoCap = capEstacoes == 0 ? 16 : capEstacoes * 2;
                int *novo = (int *)realloc(estacoes, (size_t)novoCap * sizeof(int));
                if (novo == NULL) {
                    free(estacoes);
                    free(pares);
                    return 0;
                }
                estacoes = novo;
                capEstacoes = novoCap;
            }
            estacoes[nEstacoes++] = r.codEstacao;
        }

        if (r.codEstacao != -1 && r.codProxEstacao != -1 &&
            !vetor_pares_contem(pares, nPares, r.codEstacao, r.codProxEstacao)) {
            if (nPares == capPares) {
                int novoCap = capPares == 0 ? 16 : capPares * 2;
                par *novo = (par *)realloc(pares, (size_t)novoCap * sizeof(par));
                if (novo == NULL) {
                    free(estacoes);
                    free(pares);
                    return 0;
                }
                pares = novo;
                capPares = novoCap;
            }
            pares[nPares].codEstacao = r.codEstacao;
            pares[nPares].codProxEstacao = r.codProxEstacao;
            nPares++;
        }
    }

    c->nroEstacoes = nEstacoes;
    c->nroParesEstacao = nPares;
    free(estacoes);
    free(pares);
    return 1;
}

static int finalizar_escrita(FILE *arquivo, cabecalho *c) {
    if (!recalcular_estatisticas(arquivo, c)) {
        return 0;
    }
    c->status = '0';
    if (!escrever_cabecalho(arquivo, c)) {
        return 0;
    }
    return fclose(arquivo) == 0;
}

static void fechar_em_falha(FILE *arquivo, cabecalho *c) {
    if (arquivo == NULL || c == NULL) {
        return;
    }
    c->status = '0';
    escrever_cabecalho(arquivo, c);
    fclose(arquivo);
}

static int inserir_registro(FILE *arquivo, cabecalho *c, const reg_dados *r) {
    if (c->topo != -1) {
        reg_dados removido;
        int rrnLivre = c->topo;
        if (!seek_rrn(arquivo, rrnLivre) || !ler_registro(arquivo, &removido)) {
            return 0;
        }
        c->topo = removido.proximo;
        if (!seek_rrn(arquivo, rrnLivre) || !escrever_registro(arquivo, r)) {
            return 0;
        }
        return 1;
    }

    if (!seek_rrn(arquivo, c->proxRRN)) {
        return 0;
    }
    if (!escrever_registro(arquivo, r)) {
        return 0;
    }
    c->proxRRN++;
    return 1;
}

static int remover_registro(FILE *arquivo, cabecalho *c, int rrn) {
    reg_dados r;
    if (!seek_rrn(arquivo, rrn) || !ler_registro(arquivo, &r)) {
        return 0;
    }
    if (r.removido == '0') {
        return 1;
    }

    r.removido = '0';
    r.proximo = c->topo;
    c->topo = rrn;
    if (!seek_rrn(arquivo, rrn) || !escrever_registro(arquivo, &r)) {
        return 0;
    }
    return 1;
}

static void aplicar_campo_update(registro_campos *c, const char *campo, const char *valor) {
    if (strcmp(campo, "codEstacao") == 0 || strcmp(campo, "codestacao") == 0) {
        c->codEstacao = parse_inteiro(valor);
    } else if (strcmp(campo, "nomeEstacao") == 0 || strcmp(campo, "nomeestacao") == 0) {
        parse_string(c->nomeEstacao, sizeof(c->nomeEstacao), valor);
    } else if (strcmp(campo, "codLinha") == 0 || strcmp(campo, "codlinha") == 0) {
        c->codLinha = parse_inteiro(valor);
    } else if (strcmp(campo, "nomeLinha") == 0 || strcmp(campo, "nomelinha") == 0) {
        parse_string(c->nomeLinha, sizeof(c->nomeLinha), valor);
    } else if (strcmp(campo, "codProxEstacao") == 0 ||
               strcmp(campo, "codproxestacao") == 0) {
        c->codProxEstacao = parse_inteiro(valor);
    } else if (strcmp(campo, "distProxEstacao") == 0 ||
               strcmp(campo, "distproxestacao") == 0) {
        c->distProxEstacao = parse_inteiro(valor);
    } else if (strcmp(campo, "codLinhaIntegra") == 0 ||
               strcmp(campo, "codlinhaintegra") == 0) {
        c->codLinhaIntegra = parse_inteiro(valor);
    } else if (strcmp(campo, "codEstIntegra") == 0 || strcmp(campo, "codestintegra") == 0 ||
               strcmp(campo, "codestacaointegra") == 0) {
        c->codEstIntegra = parse_inteiro(valor);
    }
}

int funcionalidade_1(const char *csv_entrada, const char *bin_saida) {
    FILE *csv = fopen(csv_entrada, "rb");
    FILE *bin = NULL;
    cabecalho c = criar_cabecalho();
    char linha[512];
    int primeiraLinha = 1;

    if (csv == NULL) {
        printf(FALHA_MSG "\n");
        return 0;
    }

    bin = fopen(bin_saida, "w+b");
    if (bin == NULL) {
        fclose(csv);
        printf(FALHA_MSG "\n");
        return 0;
    }

    c.status = '1';
    if (!escrever_cabecalho(bin, &c)) {
        fclose(csv);
        fclose(bin);
        printf(FALHA_MSG "\n");
        return 0;
    }

    while (ler_linha(csv, linha, sizeof(linha))) {
        registro_campos campos;
        reg_dados r;
        char linhaCopia[512];

        if (linha[0] == '\0') {
            continue;
        }

        copiar_limpo(linhaCopia, sizeof(linhaCopia), linha);
        {
            int statusLinha = linha_csv_para_campos(linhaCopia, &campos);
            if (statusLinha == 2 && primeiraLinha) {
                primeiraLinha = 0;
                continue;
            }
            if (statusLinha != 1) {
                fclose(csv);
                fclose(bin);
                printf(FALHA_MSG "\n");
                return 0;
            }
        }
        primeiraLinha = 0;

        r = criar_registro_nulo();
        preencher_registro(&r, &campos);
        if (!inserir_registro(bin, &c, &r)) {
            fclose(csv);
            fclose(bin);
            printf(FALHA_MSG "\n");
            return 0;
        }
    }

    fclose(csv);
    if (!finalizar_escrita(bin, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }
    BinarioNaTela((char *)bin_saida);
    return 1;
}

int funcionalidade_2(const char *bin) {
    FILE *arquivo = NULL;
    cabecalho c;
    int rrn;

    if (!abrir_binario_leitura(bin, &arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }

    for (rrn = 0; rrn < c.proxRRN; rrn++) {
        reg_dados r;
        if (!seek_rrn(arquivo, rrn) || !ler_registro(arquivo, &r)) {
            fclose(arquivo);
            printf(FALHA_MSG "\n");
            return 0;
        }
        if (r.removido == '0') {
            continue;
        }
        imprimir_registro_saida(&r);
    }

    fclose(arquivo);
    return 1;
}

int funcionalidade_3(const char *bin, int n) {
    FILE *arquivo = NULL;
    cabecalho c;
    int i;

    if (!abrir_binario_leitura(bin, &arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        int m;
        int rrn;
        int encontrou = 0;
        criterio_busca criterios[MAX_CRITERIOS];

        if (scanf("%d", &m) != 1 || m <= 0 || m > MAX_CRITERIOS) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        if (!ler_criterios_da_entrada(m, criterios)) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }

        for (rrn = 0; rrn < c.proxRRN; rrn++) {
            reg_dados r;
            if (!seek_rrn(arquivo, rrn) || !ler_registro(arquivo, &r)) {
                fechar_em_falha(arquivo, &c);
                printf(FALHA_MSG "\n");
                return 0;
            }
            if (r.removido == '0') {
                continue;
            }
            if (registro_casa_criterios_and(&r, criterios, m)) {
                imprimir_registro_saida(&r);
                encontrou = 1;
            }
        }
        if (!encontrou) {
            printf("Registro inexistente.\n");
        }
    }

    fclose(arquivo);
    return 1;
}

int funcionalidade_4(const char *bin, int n) {
    FILE *arquivo = NULL;
    cabecalho c;
    int i;

    if (!abrir_binario_escrita(bin, &arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        int m;
        int rrn;
        criterio_busca criterios[MAX_CRITERIOS];

        if (scanf("%d", &m) != 1 || m <= 0 || m > MAX_CRITERIOS) {
            fclose(arquivo);
            printf(FALHA_MSG "\n");
            return 0;
        }
        if (!ler_criterios_da_entrada(m, criterios)) {
            fclose(arquivo);
            printf(FALHA_MSG "\n");
            return 0;
        }

        for (rrn = 0; rrn < c.proxRRN; rrn++) {
            reg_dados r;
            if (!seek_rrn(arquivo, rrn) || !ler_registro(arquivo, &r)) {
                fclose(arquivo);
                printf(FALHA_MSG "\n");
                return 0;
            }
            if (r.removido == '0') {
                continue;
            }
            if (registro_casa_criterios_and(&r, criterios, m)) {
                if (!remover_registro(arquivo, &c, rrn)) {
                    fechar_em_falha(arquivo, &c);
                    printf(FALHA_MSG "\n");
                    return 0;
                }
            }
        }
    }

    if (!finalizar_escrita(arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }
    BinarioNaTela((char *)bin);
    return 1;
}

int funcionalidade_5(const char *bin, int n) {
    FILE *arquivo = NULL;
    cabecalho c;
    int i;

    if (!abrir_binario_escrita(bin, &arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        registro_campos campos;
        reg_dados r;
        char tok[128];

        memset(&campos, 0, sizeof(campos));

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        campos.codEstacao = parse_inteiro(tok);

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        parse_string(campos.nomeEstacao, sizeof(campos.nomeEstacao), tok);

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        campos.codLinha = parse_inteiro(tok);

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        parse_string(campos.nomeLinha, sizeof(campos.nomeLinha), tok);

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        campos.codProxEstacao = parse_inteiro(tok);

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        campos.distProxEstacao = parse_inteiro(tok);

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        campos.codLinhaIntegra = parse_inteiro(tok);

        if (!ler_token_custom(tok, sizeof(tok))) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        campos.codEstIntegra = parse_inteiro(tok);

        r = criar_registro_nulo();
        preencher_registro(&r, &campos);
        if (!inserir_registro(arquivo, &c, &r)) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
    }

    if (!finalizar_escrita(arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }
    BinarioNaTela((char *)bin);
    return 1;
}

int funcionalidade_6(const char *bin, int n) {
    FILE *arquivo = NULL;
    cabecalho c;
    int i;

    if (!abrir_binario_escrita(bin, &arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }

    for (i = 0; i < n; i++) {
        int m;
        int p;
        int rrn;
        criterio_busca criterios[MAX_CRITERIOS];
        criterio_busca updates[MAX_CRITERIOS];

        if (scanf("%d", &m) != 1 || m <= 0 || m > MAX_CRITERIOS) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        if (!ler_criterios_da_entrada(m, criterios)) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        if (scanf("%d", &p) != 1 || p <= 0 || p > MAX_CRITERIOS) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }
        if (!ler_criterios_da_entrada(p, updates)) {
            fechar_em_falha(arquivo, &c);
            printf(FALHA_MSG "\n");
            return 0;
        }

        for (rrn = 0; rrn < c.proxRRN; rrn++) {
            reg_dados r;
            registro_campos campos;
            int j;

            if (!seek_rrn(arquivo, rrn) || !ler_registro(arquivo, &r)) {
                fechar_em_falha(arquivo, &c);
                printf(FALHA_MSG "\n");
                return 0;
            }
            if (r.removido == '0' || !registro_casa_criterios_and(&r, criterios, m)) {
                continue;
            }

            extrair_campos_registro(&r, &campos);
            for (j = 0; j < p; j++) {
                aplicar_campo_update(&campos, updates[j].campo, updates[j].valor);
            }
            preencher_registro(&r, &campos);
            if (!atualizar_registro_inplace(arquivo, rrn, &r)) {
                fechar_em_falha(arquivo, &c);
                printf(FALHA_MSG "\n");
                return 0;
            }
        }
    }

    if (!finalizar_escrita(arquivo, &c)) {
        printf(FALHA_MSG "\n");
        return 0;
    }
    BinarioNaTela((char *)bin);
    return 1;
}