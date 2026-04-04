#include <stdio.h>
#include <stdlib.h>
#include "registro_dados.h"

void escrever_dados(reg_dados *rg,FILE* out)
{
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
}

reg_dados *ler_registro_bin(FILE *bin)
{       
    reg_dados *rg = malloc(sizeof(reg_dados));

    if(fread(&rg->removido,sizeof(char),1,bin)!=1)
        return NULL; 

    
    fread(&rg->proximo,sizeof(int),1,bin);
    fread(&rg->codEstacao,sizeof(int),1,bin);
    fread(&rg->codLinha,sizeof(int),1,bin);
    fread(&rg->codProxEstacao,sizeof(int),1,bin);
    fread(&rg->distProxEstacao,sizeof(int),1,bin);
    fread(&rg->codLinhaIntegra,sizeof(int),1,bin);
    fread(&rg->codEstIntegra,sizeof(int),1,bin);
    fread(&rg->tamNomeEstacao,sizeof(int),1,bin);

    if(rg->tamNomeEstacao>0){
        rg->nomeEstacao = malloc(rg->tamNomeEstacao+1);
        fread(rg->nomeEstacao,1,rg->tamNomeEstacao,bin);
        rg->nomeEstacao[rg->tamNomeEstacao] = '\0';
    }
    fread(&rg->tamNomeLinha,sizeof(int),1,bin);

    if(rg->tamNomeLinha){
        rg->nomeLinha = malloc(rg->tamNomeLinha);
        fread(rg->nomeLinha,1,rg->tamNomeLinha,bin);
        rg->nomeLinha[rg->tamNomeLinha] = '\0';
    }
    int bytes = 37 + rg->tamNomeEstacao + rg->tamNomeLinha;
    fseek(bin, 80 - bytes, SEEK_CUR); // pula o lixo
    return rg;
}
void imprimir_registro(reg_dados *rg)
{   


    printf("%d",rg->codEstacao);
    printf(" %s", rg->nomeEstacao);
    if(rg->codLinha!=-1)
        printf(" %d",rg->codLinha);
    else
        printf(" NULO");
    
    if(rg->tamNomeLinha != 0)
        printf(" %s",rg->nomeLinha);
    else 
        printf(" NULO");


    if(rg->codProxEstacao!=-1)
        printf(" %d",rg->codProxEstacao);
    else
        printf(" NULO");
    if(rg->distProxEstacao!=-1)
        printf(" %d",rg->distProxEstacao);
    else
        printf(" NULO");
    if(rg->codLinhaIntegra!=-1)
        printf(" %d",rg->codLinhaIntegra);
    else
        printf(" NULO");
    if(rg->codEstIntegra!=-1)
        printf(" %d",rg->codEstIntegra);
    else
        printf(" NULO");

    printf("\n");
   
    

    
}