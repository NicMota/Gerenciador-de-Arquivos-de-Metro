#ifndef FUNCIONALIDADES
#define FUNCIONALIDADES

/*
    FUNCIONALIDADE [1] CREATE TABLE

    Le o arquivo CSV de entrada e grava os dados em um arquivo binario.
*/
int funcionalidade_1(const char *csv_entrada, const char *bin_saida);

/*
    FUNCIONALIDADE [2] SELECT FROM

    Recupera e imprime todos os registros ativos do arquivo.
*/
int funcionalidade_2(const char *bin);

/*
    FUNCIONALIDADE [3] SELECT FROM WHERE

    Recupera registros ativos que satisfazem todos os criterios de busca.
*/
int funcionalidade_3(const char *bin, int n);

/*
    FUNCIONALIDADE [4] DELETE FROM WHERE

    Remove logicamente registros ativos que satisfazem todos os criterios.
*/
int funcionalidade_4(const char *bin, int n);

/*
    FUNCIONALIDADE [5] INSERT INTO

    Insere novos registros reaproveitando slots removidos (pilha).
*/
int funcionalidade_5(const char *bin, int n);

/*
    FUNCIONALIDADE [6] UPDATE WHERE

    Atualiza registros ativos que satisfazem os criterios de busca.
*/
int funcionalidade_6(const char *bin, int n);

#endif