/* BIBLIOTECAS -------------------------------------------------------*/

#include "app_jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* CONSTANTES ---------------------------------------------------------*/

#define ORDER 2

/* MAIN FUNCTION -------------------------------------------------------*/


int main() {
    Node* root = NULL;

    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 5);
    insert(&root, 6);
    insert(&root, 12);
    insert(&root, 30);
    insert(&root, 7);
    insert(&root, 17);

    printf("Traversal of the constructed tree: \n");
    traverse(root);
    printf("\n");

    return 0;
}


/*
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
        // O nome na verdade é busca rápida, é uma busca
        //direta, sem uso de filtros sugeridos pelo programa.
        printf("(1) Busca Rapida\n");
        // Acredito que o melhor nome seria busca com filtros,
        // onde sugerimos determinados filtros para classicaar
        // as vagas de emprego
        printf("(2) Busca por Filtro\n");
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
    // Análise estatística só faz sentido depois de realizar uma Busca!
    default:
        printf("default");
        break;
    }

    return 0;
}
*/
