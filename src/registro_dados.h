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

/*
    cria um registro 

    n vou escrever todos os parametros dsclp

    retorna o registro criado
*/
reg_dados *criar_registro
( 
    char removido,
    int proximo,
    int codEstacao, //not null
    int codLinha,
    int codProxEstacao,
    int distProxEstacao,
    int codLinhaIntegra,
    int codEstIntegra,
    int tamNomeEstacao,
    char* nomeEstacao, //not null
    int tamNomeLinha,   
    char* nomeLinha
);

/*
    escreve um registro em um arquivo de saida

    rg: registro a ser escrito
    out: arquivo de saida
*/
void escrever_dados(reg_dados *rg, FILE *out);


/*
    imprime um registro

    rg:registro a ser imprimido 
*/
void imprimir_registro(reg_dados *rg);

/*
    le 80 bytes do arquivo bin e recupera um registro de dados

    bin: arquivo binario

    retorna um ponteiro de reg_dados;
*/
reg_dados *ler_registro_bin(FILE *bin);

/*
    recupera um registro de dados da linha, ou seja, le o buffer do strsep e
    retira os campos do registro

    linha: buffer do strsep

    retorna um ponteiro de reg_dados
*/
reg_dados *ler_dados(char* linha);
#endif