#ifndef AUXILIARES
#define AUXILIARES

#include "registro_dados.h"
#include "cabecalho.h"
#include <stdio.h>

/*
    funcoes auxiliares :p
*/

/*
    procura o nome de uma estação dentro de um array de nomes

    nome: nome a ser procurado
    nomes: array de nomes
    tam: tamanho do array
*/
int procurar_nome_estacao(char* nome, char** nomes,int tam);

/*
    procura um par dentro de um array de pares

    par: par a ser procurado
    pares:array de pares
    tam: tamanho do array
*/
int procurar_par(par *p, par** pares, int tam);




/*
    busca dentro de um arquivo registros que cumpram as condicoes de busca da funcionalidade de
    retornar_registros_onde

    campos: nome dos campos 
    valores: valores dos campos
    m: numero de campos
    arquivo: arquivo onde a busca sera efetuada

    retorna n de resultados da busca
*/
int busca(char campos[][50], char valores[][50],int m, FILE *arquivo);

#endif