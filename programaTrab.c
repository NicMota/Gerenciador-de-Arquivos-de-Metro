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
        return; //adicionar msg de erro

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
        printf("ERRO AO ABRIR ARQUIVO");
        return;
    }
    //pula cabecalho
    fseek(arq,17,SEEK_SET);

    ;
    reg_dados *rg = malloc(sizeof(reg_dados));
    while(fread(&rg->removido,sizeof(char),1,arq) == 1)
    {
        fread(&rg->proximo,sizeof(int),1,arq);
        fread(&rg->codEstacao,sizeof(int),1,arq);
        fread(&rg->codLinha,sizeof(int),1,arq);
        fread(&rg->codProxEstacao,sizeof(int),1,arq);
        fread(&rg->distProxEstacao,sizeof(int),1,arq);
        fread(&rg->codLinhaIntegra,sizeof(int),1,arq);
        fread(&rg->codEstIntegra,sizeof(int),1,arq);
        fread(&rg->tamNomeEstacao,sizeof(int),1,arq);
        rg->nomeEstacao = malloc(rg->tamNomeEstacao);
        fread(rg->nomeEstacao,1,rg->tamNomeEstacao,arq);
        
        fread(&rg->tamNomeLinha,sizeof(int),1,arq);
        rg->nomeLinha = malloc(rg->tamNomeLinha);
        fread(rg->nomeLinha,1,rg->tamNomeLinha,arq);

        int bytes = 37 + rg->tamNomeEstacao + rg->tamNomeLinha;
        fseek(arq, 80 - bytes, SEEK_CUR); // pula o lixo
        imprimir_registro(rg);
    }
    
    free(rg->nomeEstacao);
    free(rg->nomeLinha);
    free(rg);
    
    fclose(arq);

}

void recuperar_registros_condicional(char *nome_arquivo)
{
    FILE *arq = fopen(arq,"rb");

    
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
            char nome_arquivo[256];
            scanf("%s",nome_arquivo);

            recuperar_arquivos_condicional(nome_arquivo);



    }
    
   
}