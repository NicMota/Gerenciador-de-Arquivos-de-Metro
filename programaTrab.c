/*
    Trabalho desenvolvido por NIcolas José Mota
    nusp: 16990096

    para mais informações do projeto acessar https://github.com/NicMota/<nome-do-projeto>

    

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecidas.c"

struct header{
    char status;
    int topo;
    int proxRRN;
    int nEstações;
    int nParesEstação; 

};

//campos de tamanho fixo com valores nulos devem ser representados pelo valor -1
//campos de tamanho variavel com valores nulos tem tamanho 0 no indicador de tamanho


struct data_reg{ // 80 bytes
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
   
    
    
};


void criarRegistros()
{
    FILE* arquivo = fopen("estacoes.csv","r");
    
    if(arquivo == NULL)
        return; //adicionar msg de erro

    char linha[1024];

    fgets(linha,sizeof(linha),arquivo);


    while(fgets(linha,sizeof(linha),arquivo))
    {   


        char* campos = strtok(linha,",");
        while(campos != 0){
            printf("campo %s\n",campos);
            campos = strtok(0,",");
        }
        
    }

    FILE* out = fopen("estacoes.bin","w");

    
    fclose(arquivo);
    
}



int main()
{

    int option;
    
    scanf("%d",&option);

    switch(option)
    {
        case 1:
            
            char nome_arquivo_in[256];
            char nome_arquivo_out[256];

            scanf("%s",nome_arquivo_in);
            scanf("%s",nome_arquivo_out);
            
            criarRegistros();

        break;
        case 2:

        break;
    }
    

}