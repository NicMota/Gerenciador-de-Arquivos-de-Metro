#ifndef REGISTRO
#define REGISTRO

#include <stdio.h>

#define REGISTRO_SZ 80

/*
    Estrutura logica do registro de dados.
    O formato em disco eh variavel ate 80 bytes e segue:
    removido, proximo, codEstacao, codLinha, codProxEstacao,
    distProxEstacao, codLinhaIntegra, codEstIntegra,
    tamNomeEstacao, nomeEstacao, tamNomeLinha, nomeLinha.
*/
typedef struct reg_dados {
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    char nomeEstacao[80];
    int tamNomeLinha;
    char nomeLinha[80];
} reg_dados;

/*
    Estrutura logica com os campos separados para leitura/impressao.
*/
typedef struct registro_campos {
    int codEstacao;
    char nomeEstacao[80];
    int codLinha;
    char nomeLinha[80];
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
} registro_campos;

/*
    Cria um registro base com valores nulos e lixo '$'.
*/
reg_dados criar_registro_nulo(void);

/*
    Preenche o registro fisico a partir dos campos logicos.
*/
void preencher_registro(reg_dados *r, const registro_campos *campos);

/*
    Extrai os campos logicos de um registro fisico.
*/
void extrair_campos_registro(const reg_dados *r, registro_campos *campos);

/*
    Escreve um registro no arquivo binario.
*/
int escrever_registro(FILE *arquivo, const reg_dados *r);

/*
    Le um registro do arquivo binario.
*/
int ler_registro(FILE *arquivo, reg_dados *r);

/*
    Imprime um registro no formato de saida da especificacao.
*/
void imprimir_registro_saida(const reg_dados *r);

/*
    Atualiza um registro no RRN informado.
*/
int atualizar_registro_inplace(FILE *arquivo, int rrn, const reg_dados *r);

#endif