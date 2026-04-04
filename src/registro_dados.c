#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

reg_dados* criar_registro
( 
    char removido,
    int proximo,
    int codEstacao, //not null
    int codLinha,
    int codProxEstacao,
    int distProxEstacao,
    int codLinhaIntegra,
    int codEstIntegra,
    int tamNomeEstacao,
    char* nomeEstacao, //not null
    int tamNomeLinha,   
    char* nomeLinha
){

    reg_dados *rg = malloc(sizeof(reg_dados));

    rg->removido = removido;;
    rg->proximo = proximo;
    rg->codEstacao = codEstacao;
    rg->codLinha = codLinha;
    rg->codProxEstacao = codProxEstacao;
    rg->distProxEstacao = distProxEstacao;
    rg->codLinhaIntegra = codLinhaIntegra;
    rg->codEstIntegra = codEstIntegra;
    rg->tamNomeEstacao = tamNomeEstacao;
    rg->nomeEstacao = nomeEstacao;
    rg->tamNomeLinha = tamNomeLinha;
    rg->nomeLinha = nomeLinha;

}



reg_dados* ler_dados(char* linha)
{       
        reg_dados *rg = criar_registro('0',-1,-1,-1,-1,-1,-1,-1,0,NULL,0,NULL);
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