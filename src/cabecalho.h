#ifndef CABECALHO
#define CABECALHO

#include <stdio.h>

#define CABECALHO_SZ 17

/*
    Registro de cabecalho do arquivo binario (17 bytes).
*/
typedef struct cabecalho {
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacao;
} cabecalho;

/*
    Par (codEstacao, codProxEstacao), usado para contagem de pares distintos.
*/
typedef struct par {
    int codEstacao;
    int codProxEstacao;
} par;

/*
    Cria um cabecalho inicial consistente com valores padrao.
*/
cabecalho criar_cabecalho(void);

/*
    Le o cabecalho do arquivo.
*/
int ler_cabecalho(FILE *arquivo, cabecalho *c);

/*
    Escreve o cabecalho no inicio do arquivo.
*/
int escrever_cabecalho(FILE *arquivo, const cabecalho *c);

#endif