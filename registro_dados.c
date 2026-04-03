#include <stdio.h>
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

void imprimir_registro(reg_dados *rg)
{   
    printf("\n%d",rg->codEstacao);
    if(rg->codLinha!=-1)
        printf(" %d",rg->codLinha);
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

    printf(" %s", rg->nomeEstacao);
    if(rg->tamNomeLinha != 0)
        printf(" %s",rg->nomeLinha);
    else 
        printf(" NULO");
    

    
}