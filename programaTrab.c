/*
    Trabalho desenvolvido por NIcolas José Mota
    nusp: 16990096

    para mais informações do projeto acessar https://github.com/NicMota/<nome-do-projeto>

    

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct header{
    char status;
    int topo;
    int proxRRN;
    int nEstações;
    int nParesEstação; 

};
struct data_reg{
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    char* nomeEstacao;
    char* nomeLinha;
    char removido;
    int proximo;
    int tamNomeEstacao;
    int tamNomeLinha;
};


void criarRegistros()
{

    FILE* arquivo = fopen("estacoes.csv","rw+");    

}



int main()
{

    int option;
    
    scanf("%d",&option);

    switch(option)
    {
        case 1:
            
            criarRegistros();

        break;
        case 2:

        break;
    }
    

}