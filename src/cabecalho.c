#include "cabecalho.h"
#include <stdio.h>
#include <stdlib.h>

cabecalho *criar_cabecalho(char status, int topo, int proxRRN, int nEstacoes, int nParesEstacao)
{
    cabecalho* c = malloc(sizeof(cabecalho));
    c->status = status;
    c->topo = topo;
    c->proxRRN = proxRRN;
    c->nEstacoes = nEstacoes;
    c->nParesEstacao = nParesEstacao;
}
void escrever_cabecalho(cabecalho *c, FILE* out)
{
    fwrite(&c->status, 1, 1, out);
    fwrite(&c->topo, sizeof(int), 1, out);
    fwrite(&c->proxRRN, sizeof(int), 1, out);
    fwrite(&c->nEstacoes, sizeof(int), 1, out);
    fwrite(&c->nParesEstacao, sizeof(int), 1, out);
}
