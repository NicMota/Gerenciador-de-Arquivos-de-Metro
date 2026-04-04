/*
    Trabalho desenvolvido por NIcolas José Mota
    nusp: 16990096

    para mais informações do projeto acessar https://github.com/NicMota/<nome-do-projeto>

    

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecidas.c"
#include "registro_dados.h"
#include "cabecalho.h"
#include "funcoes.c"










//função CRIAR TABELA
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

    cabecalho* c = malloc(sizeof(cabecalho));
    c->status = '0';
    c->topo = -1;
    c->proxRRN = 0;
    c->nEstações = 0;
    c->nParesEstação=0;
    
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
    c->nEstações = qtdNomes;
    c->nParesEstação = qtdPares;
    
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

            
      
        imprimir_registro(rg);
        free(rg->nomeEstacao);
        free(rg->nomeLinha);
        free(rg);
    }
    
   
    
    fclose(arq);

}
reg_dados **busca(char campos[][50], char valores[][50],int m, FILE *arquivo)
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
                return NULL;
            }

        }
        if(i==m)
        {   
            
            imprimir_registro(rg);
            n_resultados++;
        }

        free(rg->nomeEstacao);
        free(rg->nomeLinha);
        free(rg);
    }
    if(n_resultados==0)
    {
        printf("Registro inexistente.\n");
        return NULL;
    }
    
}
void recuperar_registros_onde(char *nome_arquivo, int n)
{       

    FILE *arq = fopen(nome_arquivo,"rb");
    if(arq == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO");
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
            valores[i][strcspn(valores[i], "\r\n")] = '\0';

        }

        reg_dados **resultados = busca(campos,valores,m,arq);
        printf("\n");

        
    }
    fclose(arq);


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