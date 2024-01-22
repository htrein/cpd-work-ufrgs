#include "app_jobs.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int option;
    int option_analytics;
    char nome[101];
    int numero;

    do
    {
        system("cls");
        printf("--JOBS SEARCH ENGINE--\n");
        printf("(0) Atualizar arquivo fonte\n");
        printf("(1) Busca livre\n");
        printf("(2) Busca por perfil\n");
        printf("(3) Analise estatistica\n");
        scanf("%d", &option);
    }
    while(option!=0&&option!=1&&option!=2&&option!=3);

    switch(option)
    {
    case 0:
        printf("atualizando arquivo fonte...\n");
        FILE *arquivo_p = fopen("dataset.csv", "r");
        if (arquivo_p == NULL) {
            printf("Erro ao abrir o arquivo");
        return 1;
         while (fscanf(arquivo_p, "%100[^,],%d", nome, &numero) == 2) {
        // Processa os dados lidos
        printf("Nome: %s, Número: %d\n", nome, numero);
        }
    }
    case 1:
        printf("--BUSCA LIVRE--");
        break;
    case 2:
        printf("--BUSCA POR PERFIL--");
        break;
    case 3:
        do
        {
            system("cls");
            printf("--ANALISE ESTATISTICA--\n");
            printf("(1) Tabela de medias\n");
            printf("(2) Tabela de frequencia\n");
            scanf("%d", &option_analytics);
        }
        while(option_analytics!=1&&option_analytics!=2);

        break;
    default:
        printf("default");
        break;
    }


    return 0;
}
