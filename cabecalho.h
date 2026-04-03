#ifndef CABECALHO
#define CABECALHO

#include <stdio.h>


typedef struct cabecalho{
    char status;
    int topo;
    int proxRRN;
    int nEstações;
    int nParesEstação; 

}cabecalho;

typedef struct par{
    int codEstacao;
    int codProxEstacao;
}par;


void escrever_cabecalho(cabecalho *c, FILE *out);

#endif