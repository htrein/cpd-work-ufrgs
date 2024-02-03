/* BIBLIOTECAS -------------------------------------------------------*/

#include "app_jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* CONSTANTES ---------------------------------------------------------*/

#define ORDER 2

// ESTRUTURAS
typedef struct
{
    char nome[1000];
    char descricao[MAX_NOME_LENGTH];
    int id_cargo;
    long id_est_loc;
    long id_est_crit;
} CARGO;

typedef struct
{
    char nome[1000];
    int id_ind;
} INDUSTRIA;

typedef struct
{
    char nome[1000];
    int id_local;
} LOCALIZACAO;

typedef struct
{
    char qualif[100];
    char especializacao[100];
    char yoe[100];
    char habilidades[MAX_NOME_LENGTH/2];
    int id_crit;
} CRITERIOS;

typedef struct
{
    int id_est_emp;
    int id_est_cargo;
    char data_criacao[100];
} OFERTA;

typedef struct
{
    char nome[1000];
    char website[1000];
    int id_emp;
    long id_est_ind;
} EMPRESA;

typedef struct
{
    char nome_empresa[1000];
    char nome_industria[1000];
    char nome_cargo[1000];
    char nome_localizacao[1000];
    char descricao[10000];
    char website[100];
    char especializacao[1000];//checar se os tamanhos são os mesmos usados na criacao dos binários!
    char qualificacao[1000];
    char habilidades[5000];
    char yoe[100];
    char data_criacao[100];
} REGISTRO;

/* MAIN FUNCTION -------------------------------------------------------*/

// MAIN PARA TESTAR SE A ARVORE B ESTÁ DE FATO FUNCIONANDO, SIM ELA ESTÁ.
/*
int main() {
    Node* root = NULL;
    KEYPOS reg;

    reg.key = 10;
    reg.pos = 10;

    insert(&root, reg);
    insert(&root, reg);
    insert(&root, reg);
    insert(&root, reg);
    insert(&root, reg);
    insert(&root, reg);
    insert(&root, reg);
    insert(&root, reg);

    printf("Traversal of the constructed tree: \n");
    traverse(root);
    printf("\n");

    return 0;
}

*/

int main()
{
    //variaveis
    int option;
    int option_analytics;
    int op_busca = 0;
    int op_clas = 0;
    int op_pag = 0;
    int paginada = 0;

    //entrada do usuario
    char key_busca[1000];

    //posicoes de memoria
    long pos_emp;
    long pos_carg;

    //estruturas
    INDUSTRIA ind;
    EMPRESA emp;
    OFERTA oft;
    CARGO carg;
    LOCALIZACAO loc;
    CRITERIOS crit;
    REGISTRO reg;

    do
    {
        system("cls");
        printf("--JOBS SEARCH ENGINE--\n");
        printf("(0) Atualizar arquivo fonte\n");
        printf("(1) Busca Rapida\n");
        printf("(2) Classificacao\n");
        printf("(3) Analise estatistica\n");
        scanf("%d", &option);
    }
    while(option!=0&&option!=1&&option!=2&&option!=3);

    switch(option)
    {

    case 0:
        printf("atualizando arquivo fonte...\n");
        FILE *arquivo_p = fopen("dataset.csv", "r");
        if (arquivo_p == NULL)
        {
            printf("Erro ao abrir o arquivo");
            return 1;
        }

    case 1:
        do
        {
            system("cls");
            printf("--BUSCA RAPIDA--");
            printf("\nEscolha a categoria pela qual gostaria de buscar!\n");
            printf("(1) Empresas\n");
            printf("(2) Cargos\n");
            scanf("%d", &op_busca);
        }
        while(op_busca!=1 &&op_busca!=2);
        getchar();
        system("cls");

        if(op_busca==1)
        {
            paginada = 0;
            printf("Digite o nome da empresa:\n");
            scanf("%s", key_busca);
            FILE* arqemp = fopen("dataset.csv", "rb");
            FILE* arqind = fopen("dataset.csv", "rb");
            FILE* arqof = fopen("dataset.csv", "rb");
            FILE* arqcarg = fopen("dataset.csv", "rb");
            FILE* arqloc = fopen("dataset.csv", "rb");
            FILE* arqcrit = fopen("dataset.csv", "rb"); //TEM QUE COLOCAR OS NOMES DOS ARQUIVOS CORRETAMENTE

            //faz um "searchTree" para a tree de empresa e retorna a posicao de memoria dessa empresa
            XXXXXXXXXXXXXXXXXXXXX
            //le arquivo de empresas na posicao de memoria previamente encontrada e printa os campos
            if (fseek(arqemp, pos_emp, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de empresas\n");
            }
            else
            {
                fread(&emp, sizeof(EMPRESA), 1, arqemp);
                strcpy(reg.nome_empresa, emp.nome);
                strcpy(reg.website, emp.website);
            }
            //le arquivo de industrias na posicao empresa.id_est_ind e printa os campos
            if(fseek(arqind, emp.id_est_ind, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de industrias\n");
            }
            else
            {
                fread(&ind, sizeof(INDUSTRIA), 1, arqind);
                strcpy(reg.nome_industria, ind.nome);
            }
            //faz um "SearchTree" para a tree de ofertas e procura pelo empresa.id e retorna o cargo.id, é preciso tambem retornar a data de criacao (basta guardar a posição de memória do aquivo de ofertas em que foi encontrada o empresa.id)!!! //REPETIR ESSE PROCESSO ATÉ QUE TODOS OS CARGOS TENHAM SIDO LIDOS
            //a partir daqui tudo é dentro de um do{} while todos nodos com o empresa.id tenham sido encontrados e tratados
            XXXXXXXXXXXXXXXX
            //faz um "SearchTree" para a tree de cargos e procura pelo cargo.id e retorna a posicao de memoria do cargo
            XXXXXXXXXXXXXXXX
            //le no arquivo de cargos na posicao de memoria previamente encontrada e printa os campos
            if(fseek(arqcarg, pos_carg, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de cargos\n");
            }
            else
            {
                fread(&carg, sizeof(CARGO), 1, arqcarg);
                strcpy(reg.nome_cargo, carg.nome);
                strcpy(reg.descricao, carg.descricao);
            }
            //le o arquivo de localização na posicao cargos.id_est_loc e printa os campos
            if(fseek(arqloc, carg.id_est_loc, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de localizacao\n");
            }
            else
            {
                fread(&loc, sizeof(LOCALIZACAO), 1, arqloc);
                strcpy(reg.nome_localizacao, loc.nome);
            }
            //le o arquivo de critérios da posicao cargos.id_est_crit e printa os campos
            if(fseek(arqcrit, carg.id_est_crit, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de criterios\n");
            }
            else
            {
                fread(&crit, sizeof(CRITERIOS), 1, arqcrit);
                strcpy(reg.habilidades, crit.habilidades);
                strcpy(reg.qualificacao, crit.qualif);
                strcpy(reg.especializacao, crit.especializacao);
                strcpy(reg.yoe, crit.yoe);
            }
            if(paginada < 8)
                printf("Empresa: %s\nWebsite: %s\nIndustria: %s\nCargo: %s\nData de oferta: %s\nDescricao: %s\nLocalizacao: %s\n--Requisitos--\nHabilidades: %s\nEspecializacao: %s\nQualificacao: %s\nAnos de experiencia: %s\n\n", reg.nome_empresa, reg.website, reg.nome_industria, reg.nome_cargo, reg.data_criacao, reg.descricao, reg.nome_localizacao, reg.habilidades, reg.especializacao, reg.qualificacao, reg.yoe);
            else
            {
                printf("\n\nVocê gostaria de ver mais resultados?\n");
                printf("(1) Sim \t\t (2) Nao\n\n");
                scanf("%d", &op_pag);
            }
            if(op_pag == 1)
            {
                system("cls");
                paginada = 0;
            }

            //while(op_pag!=2 || Xxxxx);

            //aqui é o final do do - while e é onde é preciso printar, para que a struct possa ser sobrescrita com o que é relacionado a cargo
            fclose(arqemp);
            fclose(arqind);
            fclose(arqof);
            fclose(arqcarg);
            fclose(arqloc);
            fclose(arqcrit);

        }
        else
        {
            paginada = 0;
            printf("Digite o nome do cargo:\n");
            scanf("%s", key_busca);

            FILE* arqemp = fopen("dataset.csv", "rb");
            FILE* arqind = fopen("dataset.csv", "rb");
            FILE* arqof = fopen("dataset.csv", "rb");
            FILE* arqcarg = fopen("dataset.csv", "rb");
            FILE* arqloc = fopen("dataset.csv", "rb");
            FILE* arqcrit = fopen("dataset.csv", "rb"); //CORRIGIR O NOME DOS ARQUIVOS

            //faz um SearchTree na arvore de cargos e descobre a posicao de memoria
            XXXXX
            //le no arquivo de cargo nessa posicao e printa as informacoes
            if (fseek(arqcarg, pos_carg, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de cargos\n");
            }
            else
            {
                fread(&carg, sizeof(CARGO), 1, arqcarg);
                strcpy(reg.nome_cargo, carg.nome);
                strcpy(reg.descricao, carg.descricao);
            }
            //faz um searchtree na arvore de ofertas e descobre o empresa.id e a data de criação!!!
            XXXXX
            //faz um searchtree na arvore de empresas e descobre a posicao da memoria
            XXXXX
            //le no arquivo de empresas e printa as informacoes
            if (fseek(arqemp, pos_emp, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de empresas\n");
            }
            else
            {
                fread(&emp, sizeof(EMPRESA), 1, arqemp);
                strcpy(reg.nome_empresa, emp.nome);
                strcpy(reg.website, emp.website);
            }
            //le no arquivo de industrias na posicao empresas.id_est_ind
            if (fseek(arqind, emp.id_est_ind, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de industrias\n");
            }
            else
            {
                fread(&ind, sizeof(INDUSTRIA), 1, arqind);
                strcpy(reg.nome_industria, ind.nome);
            }
            //le no arquivo de critérios na posicao cargos.crit
            if (fseek(arqcrit, carg.id_est_crit, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de criterios\n");
            }
            else
            {
                fread(&crit, sizeof(CRITERIOS), 1, arqcrit);
                strcpy(reg.habilidades, crit.habilidades);
                strcpy(reg.especializacao, crit.especializacao);
                strcpy(reg.qualificacao, crit.qualif);
                strcpy(reg.yoe, crit.yoe);
            }
            //le no arquivo de localizações na posicao cargos.id_est_loc
            if (fseek(arqloc, carg.id_est_loc, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de localizacao\n");
            }
            else
            {
                fread(&loc, sizeof(LOCALIZACAO), 1, arqloc);
                strcpy(reg.nome_localizacao, loc.nome);
            }
            if(paginada < 8)
                printf("Empresa: %s\nWebsite: %s\nIndustria: %s\nCargo: %s\nData de oferta: %s\nDescricao: %s\nLocalizacao: %s\n--Requisitos--\nHabilidades: %s\nEspecializacao: %s\nQualificacao: %s\nAnos de experiencia: %s\n\n", reg.nome_empresa, reg.website, reg.nome_industria, reg.nome_cargo, reg.data_criacao, reg.descricao, reg.nome_localizacao, reg.habilidades, reg.especializacao, reg.qualificacao, reg.yoe);
            else
            {
                printf("\n\nVocê gostaria de ver mais resultados?\n");
                printf("(1) Sim \t\t (2) Nao\n\n");
                scanf("%d", &op_pag);
            }
            if(op_pag == 1)
            {
                system("cls");
                paginada = 0;
            }
        }
        while(op_pag!=2 || Xxxxx);
        fclose(arqemp);
        fclose(arqind);
        fclose(arqof);
        fclose(arqcarg);
        fclose(arqloc);
        fclose(arqcrit);

        break;

    case 2:

        do
        {
            system("cls");
            printf("--CLASSIFICACAO--");
            printf("Escolha a categoria pela qual gostaria de classificar!\n");
            printf("(1) Industrias\n");
            printf("(2) Localizacao\n");
            scanf("%d", &op_clas);
        }
        while(op_clas!=1 &&op_clas!=2);
        system("cls");
        getchar();

        if(op_clas==1)
        {

        }
        else
        {

        }
        break;

    case 3:

    default:
        printf("default");
        break;
    }
    return 0;
}
