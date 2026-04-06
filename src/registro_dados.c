#include "registro_dados.h"

#include "cabecalho.h"
#include <string.h>

reg_dados criar_registro_nulo(void) {
    reg_dados r;
    memset(&r, 0, sizeof(reg_dados));
    r.removido = '1';
    r.proximo = -1;
    r.codEstacao = -1;
    r.codLinha = -1;
    r.codProxEstacao = -1;
    r.distProxEstacao = -1;
    r.codLinhaIntegra = -1;
    r.codEstIntegra = -1;
    r.tamNomeEstacao = 0;
    r.tamNomeLinha = 0;
    r.nomeEstacao[0] = '\0';
    r.nomeLinha[0] = '\0';
    return r;
}

void preencher_registro(reg_dados *r, const registro_campos *campos) {
    if (r == NULL || campos == NULL) {
        return;
    }

    *r = criar_registro_nulo();
    r->codEstacao = campos->codEstacao;
    r->codLinha = campos->codLinha;
    r->codProxEstacao = campos->codProxEstacao;
    r->distProxEstacao = campos->distProxEstacao;
    r->codLinhaIntegra = campos->codLinhaIntegra;
    r->codEstIntegra = campos->codEstIntegra;
    if (campos->nomeEstacao[0] != '\0') {
        strncpy(r->nomeEstacao, campos->nomeEstacao, sizeof(r->nomeEstacao) - 1);
        r->nomeEstacao[sizeof(r->nomeEstacao) - 1] = '\0';
        r->tamNomeEstacao = (int)strlen(r->nomeEstacao);
    }
    if (campos->nomeLinha[0] != '\0') {
        strncpy(r->nomeLinha, campos->nomeLinha, sizeof(r->nomeLinha) - 1);
        r->nomeLinha[sizeof(r->nomeLinha) - 1] = '\0';
        r->tamNomeLinha = (int)strlen(r->nomeLinha);
    }
}

void extrair_campos_registro(const reg_dados *r, registro_campos *campos) {
    if (r == NULL || campos == NULL) {
        return;
    }

    memset(campos, 0, sizeof(registro_campos));
    campos->codEstacao = r->codEstacao;
    campos->codLinha = r->codLinha;
    campos->codProxEstacao = r->codProxEstacao;
    campos->distProxEstacao = r->distProxEstacao;
    campos->codLinhaIntegra = r->codLinhaIntegra;
    campos->codEstIntegra = r->codEstIntegra;
    if (r->tamNomeEstacao > 0) {
        strncpy(campos->nomeEstacao, r->nomeEstacao, sizeof(campos->nomeEstacao) - 1);
        campos->nomeEstacao[sizeof(campos->nomeEstacao) - 1] = '\0';
    }
    if (r->tamNomeLinha > 0) {
        strncpy(campos->nomeLinha, r->nomeLinha, sizeof(campos->nomeLinha) - 1);
        campos->nomeLinha[sizeof(campos->nomeLinha) - 1] = '\0';
    }
}

int escrever_registro(FILE *arquivo, const reg_dados *r) {
    int bytes;
    char lixo = '$';

    if (arquivo == NULL || r == NULL) {
        return 0;
    }

    if (fwrite(&r->removido, sizeof(char), 1, arquivo) != 1) return 0;
    if (fwrite(&r->proximo, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&r->codEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&r->codLinha, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&r->codProxEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&r->distProxEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&r->codLinhaIntegra, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&r->codEstIntegra, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&r->tamNomeEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (r->tamNomeEstacao > 0) {
        if (fwrite(r->nomeEstacao, sizeof(char), (size_t)r->tamNomeEstacao, arquivo) !=
            (size_t)r->tamNomeEstacao)
            return 0;
    }
    if (fwrite(&r->tamNomeLinha, sizeof(int), 1, arquivo) != 1) return 0;
    if (r->tamNomeLinha > 0) {
        if (fwrite(r->nomeLinha, sizeof(char), (size_t)r->tamNomeLinha, arquivo) !=
            (size_t)r->tamNomeLinha)
            return 0;
    }

    bytes = 37 + r->tamNomeEstacao + r->tamNomeLinha;
    while (bytes < REGISTRO_SZ) {
        if (fwrite(&lixo, sizeof(char), 1, arquivo) != 1) return 0;
        bytes++;
    }
    return 1;
}

int ler_registro(FILE *arquivo, reg_dados *r) {
    int bytes;

    if (arquivo == NULL || r == NULL) {
        return 0;
    }

    *r = criar_registro_nulo();
    if (fread(&r->removido, sizeof(char), 1, arquivo) != 1) return 0;
    if (fread(&r->proximo, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&r->codEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&r->codLinha, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&r->codProxEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&r->distProxEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&r->codLinhaIntegra, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&r->codEstIntegra, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&r->tamNomeEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    if (r->tamNomeEstacao > 0) {
        if (r->tamNomeEstacao >= (int)sizeof(r->nomeEstacao)) return 0;
        if (fread(r->nomeEstacao, sizeof(char), (size_t)r->tamNomeEstacao, arquivo) !=
            (size_t)r->tamNomeEstacao)
            return 0;
        r->nomeEstacao[r->tamNomeEstacao] = '\0';
    }
    if (fread(&r->tamNomeLinha, sizeof(int), 1, arquivo) != 1) return 0;
    if (r->tamNomeLinha > 0) {
        if (r->tamNomeLinha >= (int)sizeof(r->nomeLinha)) return 0;
        if (fread(r->nomeLinha, sizeof(char), (size_t)r->tamNomeLinha, arquivo) !=
            (size_t)r->tamNomeLinha)
            return 0;
        r->nomeLinha[r->tamNomeLinha] = '\0';
    }

    bytes = 37 + r->tamNomeEstacao + r->tamNomeLinha;
    if (bytes < REGISTRO_SZ) {
        fseek(arquivo, REGISTRO_SZ - bytes, SEEK_CUR);
    }
    return 1;
}

void imprimir_registro_saida(const reg_dados *r) {
    registro_campos c;

    extrair_campos_registro(r, &c);

    if (c.codEstacao == -1) printf("NULO ");
    else printf("%d ", c.codEstacao);
    if (c.nomeEstacao[0] == '\0') printf("NULO ");
    else printf("%s ", c.nomeEstacao);
    if (c.codLinha == -1) printf("NULO ");
    else printf("%d ", c.codLinha);
    if (c.nomeLinha[0] == '\0') printf("NULO ");
    else printf("%s ", c.nomeLinha);
    if (c.codProxEstacao == -1) printf("NULO ");
    else printf("%d ", c.codProxEstacao);
    if (c.distProxEstacao == -1) printf("NULO ");
    else printf("%d ", c.distProxEstacao);
    if (c.codLinhaIntegra == -1) printf("NULO ");
    else printf("%d ", c.codLinhaIntegra);
    if (c.codEstIntegra == -1) printf("NULO\n");
    else printf("%d\n", c.codEstIntegra);
}

int atualizar_registro_inplace(FILE *arquivo, int rrn, const reg_dados *r) {
    long long offset = (long long)CABECALHO_SZ + (long long)rrn * REGISTRO_SZ;
    if (arquivo == NULL || r == NULL || rrn < 0) {
        return 0;
    }
    fseek(arquivo, (long)offset, SEEK_SET);
    return escrever_registro(arquivo, r);
}