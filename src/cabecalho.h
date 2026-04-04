#ifndef CABECALHO
#define CABECALHO

#include <stdio.h>


typedef struct cabecalho{
    char status;
    int topo;
    int proxRRN;
    int nEstacoes;
    int nParesEstacao; 

}cabecalho;

typedef struct par{
    int codEstacao;
    int codProxEstacao;
}par;

/*
    cria um cabecalho e o retorna
*/
cabecalho *criar_cabecalho(char status, int topo, int proxRRN, int nEstacoes, int nParesEstacao);

/*
    escreve um cabecalho no começo do arquivo de saida

    c: cabecalho a ser escrito
    out: arquivo de saida
*/
void escrever_cabecalho(cabecalho *c, FILE *out);

#endif