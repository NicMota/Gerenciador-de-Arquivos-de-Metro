#ifndef FUNCIONALIDADES
#define FUNCIONALIDADES


/*
    FUNCIONALIDADE CRIAR REGISTROS

    permite a leitura de varios registros obtidos através de um arquivo csv de entrada
    e os grava em um arquivo de dados de saida binario.

    (arquivo_in):
    nome do arquivo csv de entrada que contem os valores dos campos de registros a serem escritos
    no arquivo de saida

    (arquivo_out): 
    nome do arquivo bin de saida gerado na execução da funcao
*/
void criarRegistros(char *arquivo_in, char* arquivo_out);


/*
    FUNCIONALIDADE RECUPERAR REGISTROS

    permite a recuperação dos dados de um arquivo binario segundo as especificações do projeto, 

    (nome_arquivo): 
    nome do arquivo binario a ser lido pela função 
*/
void recuperar_registros(char *nome_arquivo);

/* 
    FUNCIONALIDADE RECUPERAR REGISTROS CONDICIONAL

    permite a recuperação de dados de um arquivo binário seguindo condições de busca.

    (nome_arquivo): 
    nome do arquivo binario a ser lido pela função 

    (n):
    numero de buscas a serem feitas

*/
void recuperar_registros_onde(char *nome_arquivo, int n);


#endif