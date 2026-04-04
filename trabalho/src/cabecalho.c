#include "cabecalho.h"
#include <stdio.h>

void escrever_cabecalho(cabecalho *c, FILE* out)
{
     fwrite(&c->status, 1, 1, out);
    fwrite(&c->topo, sizeof(int), 1, out);
    fwrite(&c->proxRRN, sizeof(int), 1, out);
    fwrite(&c->nEstações, sizeof(int), 1, out);
    fwrite(&c->nParesEstação, sizeof(int), 1, out);
}
