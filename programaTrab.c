/*
    Trabalho desenvolvido por NIcolas José Mota
    nusp: 16990096

    para mais informações do projeto acessar https://github.com/NicMota/<nome-do-projeto>

    

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecidas.c"
#include "RegistroDados.h"
#include "Cabecalho.h"
#include "funcoes.c"





typedef struct par{
    int codEstacao;
    int codProxEstacao;
}par;


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


void criarRegistros(char *arquivo_in, char* arquivo_out)
{
    FILE* arquivo = fopen(arquivo_in,"r");
    
    if(arquivo == NULL)
        return; //adicionar msg de erro

    char linha[1024];

    fgets(linha,sizeof(linha),arquivo);

    cabecalho* c = malloc(sizeof(cabecalho));
    c->status = '0';
    c->topo = -1;
    c->proxRRN = 0;
    c->nEstações = 0;
    c->nParesEstação=0;
    
  

    FILE* out = fopen(arquivo_out,"wb");
    fwrite(&c->status, 1, 1, out);
    fwrite(&c->topo, sizeof(int), 1, out);
    fwrite(&c->proxRRN, sizeof(int), 1, out);
    fwrite(&c->nEstações, sizeof(int), 1, out);
    fwrite(&c->nParesEstação, sizeof(int), 1, out);

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
        
        
        
      


        fwrite(&rg->removido,1,1,out);
        fwrite(&rg->proximo,sizeof(int),1,out);
        fwrite(&rg->codEstacao,sizeof(int),1,out);
        fwrite(&rg->codLinha,sizeof(int),1,out);
        fwrite(&rg->codProxEstacao,sizeof(int),1,out);
        fwrite(&rg->distProxEstacao,sizeof(int),1,out);
        fwrite(&rg->codLinhaIntegra,sizeof(int),1,out);
        fwrite(&rg->codEstIntegra,sizeof(int),1,out);
        fwrite(&rg->tamNomeEstacao,sizeof(int),1,out);
        fwrite(rg->nomeEstacao,1,rg->tamNomeEstacao,out);
        fwrite(&rg->tamNomeLinha,sizeof(int),1,out);
        fwrite(rg->nomeLinha,1,rg->tamNomeLinha,out);

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
    fwrite(&c->status, 1, 1, out);
    fwrite(&c->topo, sizeof(int), 1, out);
    fwrite(&c->proxRRN, sizeof(int), 1, out);
    fwrite(&c->nEstações, sizeof(int), 1, out);
    fwrite(&c->nParesEstação, sizeof(int), 1, out);
    
    
    

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

        break;
    }
    
   
}