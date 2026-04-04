#include "registro_dados.h"
#include "cabecalho.h"
#include <stdio.h>


int procurar_nome_estacao(char* nome, char** nomes,int tam)
{
    for(int i = 0;i<tam;i++)
    {
        if(strcmp(nome,nomes[i])==0)
            return 1;
    }
    return 0;
}

int procurar_par(par *p, par** pares, int tam)
{
    for(int i = 0;i<tam;i++)
    {
        if(p->codEstacao == pares[i]->codEstacao && p->codProxEstacao == pares[i]->codProxEstacao)
            return 1;
    }
    return 0;
}
reg_dados* ler_dados(char* linha)
{       
        reg_dados *rg = malloc(sizeof(reg_dados));

        rg->removido = '0';
        rg->proximo = -1;
        rg->codEstacao = -1;
        rg->codLinha = -1;
        rg->codProxEstacao = -1;
        rg->distProxEstacao = -1;
        rg->codLinhaIntegra = -1;
        rg->codEstIntegra = -1;
        rg->tamNomeEstacao = 0;
        rg->nomeEstacao = NULL; 
        rg->tamNomeLinha = 0;
        rg->nomeLinha = NULL;


        int i = 0;
       
        char *campos;
        while( (campos = strsep(&linha,",") )!= NULL && i<8){
           
            
            campos[strcspn(campos, "\r\n")] = '\0';
            
            //tratar campos com valores nulos
            switch (i)
            {
            case 0:
                if(strlen(campos)==0)
                {
                    printf("COD ESTAÇÃO NÃO PODE SER NULO");
                    free(rg);
                    return NULL;
                }
                    
                rg->codEstacao = atoi(campos);
                break;
            case 1:
                if(strlen(campos)>0){
                    rg->tamNomeEstacao = strlen(campos);
                    rg->nomeEstacao = malloc(rg->tamNomeEstacao+1);
                    strcpy(rg->nomeEstacao,campos);
                    break;
                } else {
                    free(rg);
                    printf("NOME ESTACAO NÃO PODE SER NULO");
                    return NULL;
                }
                    
            case 2:

                rg->codLinha = atoi(campos);
                break;

            case 3:
                if(strlen(campos)>0){
                    rg->tamNomeLinha = strlen(campos);
                    rg->nomeLinha = malloc(rg->tamNomeLinha+1);

                    strcpy(rg->nomeLinha,campos);
                    break;  
                }else{
                    rg->tamNomeLinha = 0;
                }
            case 4:
                
                if(strlen(campos) > 0) rg->codProxEstacao = atoi(campos);
                break;
            case 5:
                if(strlen(campos) > 0) rg->distProxEstacao = atoi(campos);
                break;

            case 6:

                if(strlen(campos) > 0) rg->codLinhaIntegra = atoi(campos);
                break;

            case 7:
                
                if(strlen(campos) > 0) rg->codEstIntegra =  atoi(campos);
                break;
            
            default:
                break;
            }
            
            
            i++;
            
           
        }


       

      
        return rg;
}

