/*
 * Trabalho desenvolvido por Nicolas José Mota e Gustavo Vieira Gomes
 * nusp: 16990096 e 16907251
 */

#include <stdio.h>
#include <stdlib.h>

#include "funcionalidades.h"

int main(void) {
    int func;
    char arq1[256];
    char arq2[256];
    int n;

    if (scanf("%d", &func) != 1) {
        printf("Falha no processamento do arquivo\n");
        return 1;
    }

    switch (func) {
    case 1:
        if (scanf("%255s %255s", arq1, arq2) != 2) {
            printf("Falha no processamento do arquivo\n");
            return 1;
        }
        return funcionalidade_1(arq1, arq2) ? 0 : 1;
    case 2:
        if (scanf("%255s", arq1) != 1) {
            printf("Falha no processamento do arquivo\n");
            return 1;
        }
        return funcionalidade_2(arq1) ? 0 : 1;
    case 3:
        if (scanf("%255s %d", arq1, &n) != 2) {
            printf("Falha no processamento do arquivo\n");
            return 1;
        }
        return funcionalidade_3(arq1, n) ? 0 : 1;
    case 4:
        if (scanf("%255s %d", arq1, &n) != 2) {
            printf("Falha no processamento do arquivo\n");
            return 1;
        }
        return funcionalidade_4(arq1, n) ? 0 : 1;
    case 5:
        if (scanf("%255s %d", arq1, &n) != 2) {
            printf("Falha no processamento do arquivo\n");
            return 1;
        }
        return funcionalidade_5(arq1, n) ? 0 : 1;
    case 6:
        if (scanf("%255s %d", arq1, &n) != 2) {
            printf("Falha no processamento do arquivo\n");
            return 1;
        }
        return funcionalidade_6(arq1, n) ? 0 : 1;
    default:
        printf("Falha no processamento do arquivo\n");
        return 1;
    }
}