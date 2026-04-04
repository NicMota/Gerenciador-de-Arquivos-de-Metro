#include "registro_dados.h"
#include "cabecalho.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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


int busca(char campos[][50], char valores[][50],int m, FILE *arquivo)
{   

    //pula cabecalho
    fseek(arquivo,17,SEEK_SET);

    reg_dados **resultados;
    int n_resultados = 0;
    reg_dados *rg;

    while(1)
    {   
        rg = ler_registro_bin(arquivo);

        if(rg==NULL)
            break;

        if(rg->removido=='1')
        {
            free(rg->nomeEstacao);
            free(rg->nomeLinha);
            free(rg);
            continue;
        }

        int i;
        for(i = 0;i<m;i++)
        {   
            
            if(strcmp(campos[i],"codEstacao")==0)
            {   
                
                int valor;
                if(strcmp(valores[i],"NULO") == 0)
                {
                    valor = -1;
                }else
                {
                    valor = atoi(valores[i]);   
                }
                
                if(valor !=rg->codEstacao)  break;
                
            }else   
            if(strcmp(campos[i],"codLinha")==0)
            {
                int valor;
                if(strcmp(valores[i],"NULO") == 0)
                {
                    valor = -1;
                }else
                {
                    valor = atoi(valores[i]);   
                }
                if(valor !=rg->codLinha)  break;
            }else
            if(strcmp(campos[i],"codProxEstacao")==0)
            {
                int valor;
                if(strcmp(valores[i],"NULO") == 0)
                {
                    valor = -1;
                }else
                {
                    valor = atoi(valores[i]);   
                }
                if(valor !=rg->codProxEstacao)  break;
            }else
            if(strcmp(campos[i],"distProxEstacao")==0)
            {
                int valor;
                if(strcmp(valores[i],"NULO") == 0)
                {
                    valor = -1;
                }else
                {
                    valor = atoi(valores[i]);   
                }
                if(valor !=rg->distProxEstacao)  break;
            }else
            if(strcmp(campos[i],"codLinhaIntegra")==0)
            {
                int valor;
                if(strcmp(valores[i],"NULO") == 0)
                {
                    valor = -1;
                }else
                {
                    valor = atoi(valores[i]);   
                }
                if(valor !=rg->codLinhaIntegra)  break;
            }else
            if(strcmp(campos[i],"codEstIntegra")==0)
            {
                int valor;
                if(strcmp(valores[i],"NULO") == 0)
                {
                    valor = -1;
                }else
                {
                    valor = atoi(valores[i]);   
                }
                if(valor !=rg->codEstIntegra)  break;
            }else
            if(strcmp(campos[i],"nomeEstacao")==0)
            {
                if(strcmp(valores[i],rg->nomeEstacao) != 0) break;
            }else
            if(strcmp(campos[i],"nomeLinha")==0)
            {   
                if(strcmp(valores[i],"NULO")==0)
                {
                    if(rg->tamNomeLinha != 0) break;
                }else{
                    
                    if(strcmp(valores[i],rg->nomeLinha) != 0) break;
               
                }
            }else
            {   
                printf("nome campo invalido");
                return n_resultados;
            }

        }
        if(i==m)
        {   
            
            imprimir_registro(rg);
            n_resultados++;
        }

        return n_resultados;
      
    }
    if(n_resultados==0)
    {
        printf("Registro inexistente.\n");
        return n_resultados;
    }
    
}