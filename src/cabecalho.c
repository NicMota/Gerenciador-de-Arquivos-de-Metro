#include "cabecalho.h"

cabecalho criar_cabecalho(void) {
    cabecalho c;
    c.status = '0';
    c.topo = -1;
    c.proxRRN = 0;
    c.nroEstacoes = 0;
    c.nroParesEstacao = 0;
    return c;
}

int ler_cabecalho(FILE *arquivo, cabecalho *c) {
    if (arquivo == NULL || c == NULL) {
        return 0;
    }

    fseek(arquivo, 0, SEEK_SET);
    if (fread(&c->status, 1, 1, arquivo) != 1) return 0;
    if (fread(&c->topo, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&c->proxRRN, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&c->nroEstacoes, sizeof(int), 1, arquivo) != 1) return 0;
    if (fread(&c->nroParesEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    return 1;
}

int escrever_cabecalho(FILE *arquivo, const cabecalho *c) {
    if (arquivo == NULL || c == NULL) {
        return 0;
    }

    fseek(arquivo, 0, SEEK_SET);
    if (fwrite(&c->status, 1, 1, arquivo) != 1) return 0;
    if (fwrite(&c->topo, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&c->proxRRN, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&c->nroEstacoes, sizeof(int), 1, arquivo) != 1) return 0;
    if (fwrite(&c->nroParesEstacao, sizeof(int), 1, arquivo) != 1) return 0;
    return 1;
}
