#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro_dados.h"
#include "cabecalho.h"
#include "auxiliares.h"
#include "fornecidas.h"





void criarRegistros(char *arquivo_in, char* arquivo_out)
{
    FILE* arquivo = fopen(arquivo_in,"r");
    FILE* out = fopen(arquivo_out,"wb");
    if(arquivo == NULL || out == NULL)
    {   
        printf("Falha no processamento do arquivo.");
        return; //adicionar msg de erro
    }
        

    char linha[1024];

    fgets(linha,sizeof(linha),arquivo);

    cabecalho* c = criar_cabecalho('0',-1,0,0,0);

    escrever_cabecalho(c,out);

    char **nomes_estacao = malloc(200*sizeof(char*));
    int qtdNomes = 0;

    par **pares = malloc(200*sizeof(par*));
    int qtdPares = 0; 

    int qtdRegDados = 0;

    while(fgets(linha,sizeof(linha),arquivo))
    {   
       
        reg_dados *rg = ler_dados(linha);


        if(!procurar_nome_estacao(rg->nomeEstacao,nomes_estacao,qtdNomes))
        {   
            nomes_estacao[qtdNomes] = malloc(strlen(rg->nomeEstacao)+1);
            strcpy(nomes_estacao[qtdNomes],rg->nomeEstacao);
            qtdNomes++;
        }



        par *p = malloc(sizeof(par));

        p->codEstacao= rg->codEstacao;
        p->codProxEstacao= rg->codProxEstacao;



        if(!procurar_par(p,pares,qtdPares) && p->codProxEstacao!=-1)
        {
            pares[qtdPares] = p;
            qtdPares++;
        }else
            free(p);
        
        
        
      
        escrever_dados(rg,out);

        

        int bytes = 37 + rg->tamNomeEstacao + rg->tamNomeLinha;
        char lixo = '$';
        while(bytes<80)
        {   

            fwrite(&lixo,sizeof(char),1,out);
            bytes++;
        }
        
        qtdRegDados++;
        
        free(rg->nomeEstacao);
        free(rg->nomeLinha);
        free(rg);
       
        
    }

    c->status='1';
    c->proxRRN = qtdRegDados;
    c->nEstacoes = qtdNomes;
    c->nParesEstacao = qtdPares;
    
    fseek(out,0,SEEK_SET);
    escrever_cabecalho(c,out);
    
    
    

    for(int i = 0; i < qtdNomes; i++) {
        free(nomes_estacao[i]);
    }
    free(nomes_estacao);


    for(int i = 0; i < qtdPares; i++) {
        free(pares[i]);
    }
    free(pares);
    free(c); 

    fclose(arquivo);
    fclose(out);
    BinarioNaTela(arquivo_out);

    return;
}



void recuperar_registros(char *nome_arquivo)
{
    FILE *arq = fopen(nome_arquivo,"rb");
    if(arq == NULL)
    {
        printf("Falha no processamento do arquivo.");
        return;
    }
    //pula cabecalho
    fseek(arq,17,SEEK_SET);

    ;
    reg_dados *rg = malloc(sizeof(reg_dados));
    while(1)
    {   
        rg = ler_registro_bin(arq);
        if(rg == NULL)
            break;
        if(rg->removido=='1')
        {   
            free(rg->nomeEstacao);
            free(rg->nomeLinha);
            free(rg);
            continue;
        }
      
        imprimir_registro(rg);
        
    }
    
   
    
    fclose(arq);

}

void recuperar_registros_onde(char *nome_arquivo, int n)
{       

    FILE *arq = fopen(nome_arquivo,"rb");
    if(arq == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    for(int i = 0;i<n;i++)
    {
        int m; scanf("%d",&m);

        char campos[m][50]; 
        char valores[m][50];

        for(int i = 0;i<m;i++)
        {   
            scanf("%s",campos[i]);
            if(strcmp(campos[i],"nomeLinha") == 0 || strcmp(campos[i],"nomeEstacao") == 0)
            {
                ScanQuoteString(valores[i]);
            }else
            {
                scanf("%s",valores[i]);
            }

            

        }

        int n = busca(campos,valores,m,arq);
        printf("\n");

        
    }
    fclose(arq);


}