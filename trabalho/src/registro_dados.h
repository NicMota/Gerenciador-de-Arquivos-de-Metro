#ifndef REGISTRO
#define REGISTRO

#include <stdio.h>



//campos de tamanho fixo com valores nulos devem ser representados pelo valor -1
//campos de tamanho variavel com valores nulos tem tamanho 0 no indicador de tamanho
typedef struct reg_dados{ // 80 bytes
    char removido;
    int proximo;
    int codEstacao; //not null
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    char* nomeEstacao; //not null
    int tamNomeLinha;   
    char* nomeLinha;
   
}reg_dados;

void escrever_dados(reg_dados *rg, FILE *out);
void imprimir_registro(reg_dados *rg);
reg_dados *ler_registro_bin(FILE *bin);
#endif