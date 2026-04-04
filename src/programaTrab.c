/*
    Trabalho desenvolvido por Nicolas José Mota e Gustavo Vieira Gomes
    nusp: 16990096, 

    para mais informações do projeto acessar https://github.com/NicMota/Gerenciador-de-Arquivos-de-Metro


*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"



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
            
            criarRegistros(nome_arquivo_in,nome_arquivo_out);

        break;
        case 2:
            char nome_arquivo[256];
            scanf("%s",nome_arquivo);

            recuperar_registros(nome_arquivo);
        break;
        case 3:
            char nome_arquivo_busca[256];
            scanf("%s",nome_arquivo_busca);

            int n;  scanf("%d", &n);

            
            recuperar_registros_onde(nome_arquivo_busca, n);

            break;



    }
    
   
}