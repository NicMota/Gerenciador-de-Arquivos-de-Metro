#ifndef CABECALHO
#define CABECALHO


//campos de tamanho fixo com valores nulos devem ser representados pelo valor -1
//campos de tamanho variavel com valores nulos tem tamanho 0 no indicador de tamanho
typedef struct cabecalho{
    char status;
    int topo;
    int proxRRN;
    int nEstações;
    int nParesEstação; 

}cabecalho;
#endif