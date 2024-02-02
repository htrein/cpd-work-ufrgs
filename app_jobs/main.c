//--------------------------------------------------------------------
// BIBLIOTECAS
//--------------------------------------------------------------------

#include "app_jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//--------------------------------------------------------------------
// CONSTANTES
//--------------------------------------------------------------------

#define ORDER 2

//--------------------------------------------------------------------
// ESTRUTURAS
//--------------------------------------------------------------------

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
    char especializacao[1000]; //checar se os tamanhos s�o os mesmos usados na criacao dos bin�rios!
    char qualificacao[1000];
    char habilidades[5000];
    char yoe[100];
    char data_criacao[100];
} REGISTRO;

// chave-posicao, armazena chave e posicao do registro que queremos
typedef struct keyPos{
    int key;
    int pos;
}KEYPOS;

// Definindo a estrutura de um n� da �rvore B
typedef struct node {
    //int keys[2 * ORDER - 1];
    KEYPOS registros[2 * ORDER - 1];    // registros com chaves-posicoes dos arquivos
    struct node *children[2 * ORDER];
    int num_keys;
    bool is_leaf;                       // � folha
} Node;

//--------------------------------------------------------------------
// PROTOTIPOS
//--------------------------------------------------------------------

Node* createNode();
void splitChild(Node* parent, int index, Node* child);
//void insert(Node** root, int key);
void insert(Node** root, KEYPOS reg);
void traverse(Node* root);
KEYPOS* search(Node* root, int key);

//--------------------------------------------------------------------
// MAIN FUNCTION
//--------------------------------------------------------------------

// MAIN PARA TESTAR SE A ARVORE B EST� DE FATO FUNCIONANDO, SIM ELA EST�.
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

        // ...

        fclose("dataset.csv");

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
            KEYPOS resultado; 

            resultado.pos = search(arqemp, key_busca); // nao sei se isso funciona

            if (resultado.pos == NULL)  // se a busca retornou NULL
            {
                printf("Empresa nao encontrada\n");
                break;
            }
            else
            {
                pos_emp = resultado.pos; // se a busca retornou uma posicao, pos_emp recebe essa posicao
            }

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
            //faz um "SearchTree" para a tree de ofertas e procura pelo empresa.id e retorna o cargo.id, � preciso tambem retornar a data de criacao!!! //REPETIR ESSE PROCESSO AT� QUE TODOS OS CARGOS TENHAM SIDO LIDOS
            //a partir daqui tudo � dentro de um do{} while todos nodos com o empresa.id tenham sido encontrados e tratados
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
            //le o arquivo de localiza��o na posicao cargos.id_est_loc e printa os campos
            if(fseek(arqloc, carg.id_est_loc, SEEK_SET) != 0)
            {
                printf("Erro ao posicionar o ponteiro de arquivo no arquivo de localizacao\n");
            }
            else
            {
                fread(&loc, sizeof(LOCALIZACAO), 1, arqloc);
                strcpy(reg.nome_localizacao, loc.nome);
            }
            //le o arquivo de crit�rios da posicao cargos.id_est_crit e printa os campos
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
                printf("\n\nVoc� gostaria de ver mais resultados?\n");
                printf("(1) Sim \t\t (2) Nao\n\n");
                scanf("%d", &op_pag);
            }
            if(op_pag == 1)
            {
                system("cls");
                paginada = 0;
            }

            //while(op_pag!=2 || Xxxxx);

            //aqui � o final do do - while e � onde � preciso printar, para que a struct possa ser sobrescrita com o que � relacionado a cargo
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
            //faz um searchtree na arvore de ofertas e descobre o empresa.id e a data de cria��o!!!
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
            //le no arquivo de crit�rios na posicao cargos.crit
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
            //le no arquivo de localiza��es na posicao cargos.id_est_loc
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
                printf("\n\nVoc� gostaria de ver mais resultados?\n");
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

//--------------------------------------------------------------------
// FUNCOES
//--------------------------------------------------------------------

// Fun��o para criar um novo n�
Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro ao alocar mem�ria para o novo n�");
        exit(EXIT_FAILURE);
    }

    newNode->num_keys = 0;
    newNode->is_leaf = true;

    for (int i = 0; i < 2 * ORDER; i++) {
        newNode->children[i] = NULL;
        //newNode->keys[i] = 0;
        newNode->registros[i].key = 0;  // NAO SEI SE VAI FUNCIONAR!
        newNode->registros[i].pos = 0;  // NAO SEI SE VAI FUNCIONAR!
    }

    return newNode;
}

// Fun��o para dividir um n� filho do pai
void splitChild(Node* parent, int index, Node* child) {
    /* NODO FILHO A DIREITA CRIADO */
    Node* newChild = createNode();
    newChild->is_leaf = child->is_leaf;
    newChild->num_keys = ORDER - 1;

    // DIVIDE AS CHAVES ENTRE NODOS FILHOS
    for (int i = 0; i < ORDER - 1; i++){
        //newChild->keys[i] = child->keys[i + ORDER];
        newChild->registros[i].key = child->registros[i + ORDER].key; //NAO SEI SE FUNCIONA
        newChild->registros[i].pos = child->registros[i + ORDER].pos; //NAO SEI SE FUNCIONA
    }

    if (!child->is_leaf) {
        for (int i = 0; i < ORDER; i++)
            newChild->children[i] = child->children[i + ORDER];
    }

    child->num_keys = ORDER - 1;

    for (int i = parent->num_keys; i >= index + 1; i--)
        parent->children[i + 1] = parent->children[i];

    parent->children[index + 1] = newChild;

    // O QUE FAZ? NAO SEI, POREM VOU TENTAR SOMENTE SUBSTITUIR PELO NOVO TIPO
    for (int i = parent->num_keys - 1; i >= index; i--){
        //parent->keys[i + 1] = parent->keys[i];
        parent->registros[i + 1].key = parent->registros[i].key; //NAO SEI SE FUNCIONA
        parent->registros[i + 1].pos = parent->registros[i].pos; //NAO SEI SE FUNCIONA
    }
    // O MESMO ALI DE CIMA!
    //parent->keys[index] = child->keys[ORDER - 1];
    parent->registros[index].key = child->registros[ORDER - 1].key; //NAO SEI SE FUNCIONA
    parent->registros[index].pos = child->registros[ORDER - 1].pos; //NAO SEI SE FUNCIONA
    parent->num_keys++;
}

// Fun��o para inserir uma chave em uma �rvore B
/* AO INVES DE INT key COLOCAMOS KEYPOS registro */
void insert(Node** root, KEYPOS reg) {
    Node* temp = *root;

    /* CASO RAIZ VAZIA */
    if (temp == NULL) {

        *root = createNode();
        /* NAO ADICIONAMOS MAIS SOMENTE KEY, E SIM, KEY E POS*/
        //(*root)->keys[0] = key;
        (*root)->registros[0].key = reg.key; //NAO TENHO CERTEZA SE VA FUNCIONAR
        (*root)->registros[0].pos = reg.pos; //NAO TENHO CERTEZA SE VA FUNCIONAR
        (*root)->num_keys = 1;

    } else {
        /* CASO NODO ESTIVER CHEIO */
        if (temp->num_keys == 2 * ORDER - 1) {

            Node* newRoot = createNode();
            newRoot->is_leaf = false;
            newRoot->children[0] = *root;

            /* FUNCAO QUE DIVIDE O NODO CHEIO */
            splitChild(newRoot, 0, *root);
            int i = 0;

            /* O REGISTRO A SER INSERIDO NAO SER� INSERIDO NA NOVA RAIZ E SIM EM UM DOS FILHOS
               POR ISSO A CHAMADA RECURSIVA ABAIXO*/

            /* POR QUE I = 0 OU I = 1? POIS UMA NOVA RAIZ SEMPRE TEM 2 FILHOS */

            /*if (newRoot->keys[0] < key)
                i++;
            */

            if (newRoot->registros[0].key < reg.key) // NAO SEI SE FUNCIONA
                i++;

            // CHAMADA RECURSIVA
            insert(&newRoot->children[i], reg); // SOMENTE TROQUEI key -> reg
            *root = newRoot;

        } else {

            /* CASO O NODO NAO ESTIVER CHEIO */
            // FA�A ENQUANTO NAO EH FOLHA
            while (!temp->is_leaf) {

                int i = temp->num_keys - 1; // i, vira um �ndice do nodo que contem as chaves

                // procurando posicao do filho da chave(registrador) i na qual verificar se � folha para inserir.
                while (i >= 0 && temp->registros[i].key > reg.key) // temp->keys[i] vira temp->registro[i].key, key vira reg.key
                    i--;
                i++;

                if (temp->children[i]->num_keys == 2 * ORDER - 1) {
                    splitChild(temp, i, temp->children[i]);
                    if (temp->registros[i].key < reg.key)          // temp->keys[i] < key vira temp->registro[i].key < reg.key
                        i++;
                }
                temp = temp->children[i];
            }

            // AQUI SABEMOS QUE O NODO � FOLHA, ENTAO SOMENTE RESTA A INSERCAO.
            int i = temp->num_keys - 1;
            while (i >= 0 && temp->registros[i].key > reg.key) {                // temp->keys[i] > key VIRA temp->registro[i].key > reg.key
                temp->registros[i + 1].key = temp->registros[i].key;            // temp->keys[i + 1] = temp->keys[i] vira temp->registro[i + 1].key = temp->registro[i].key;
                // ACREDITO SER NECESSARIO DESLOCAR A POSICAO TAMBEM.
                // ISSO ABAIXO � UM PALPITE, POIS NAO ENTENDI ESSA PARTE DO CODIGO
                temp->registros[i + 1].pos = temp->registros[i].pos;
                i--;
            }
            //temp->keys[i + 1] = key;
            temp->registros[i + 1].key = reg.key;
            // ISSO ABAIXO � UM PALPITE, POIS NAO ENTENDI ESSA PARTE DO CODIGO
            temp->registros[i + 1].pos = reg.pos;
            temp->num_keys++;
        }
    }
}

// Fun��o para percorrer a �rvore B
void traverse(Node* root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->num_keys; i++) {
            traverse(root->children[i]);
            //printf("%d ", root->keys[i]);
            printf("ch:%d, pos:%d\n", root->registros[i].key, root->registros[i].pos ); //TESTANDO
        }
        traverse(root->children[i]);
    }
}

// Funcao para buscar um registro na B-Tree
KEYPOS* search(Node* root, int key) {
    // Encontra a primeira chave maior ou igual a chave entrada
    int i = 0;
    while (i < root->num_keys && key > root->registros[i].key) {
        i++;
    }

    // Se a chave for encontrada neste nodo, retorna o registro
    if (i < root->num_keys && key == root->registros[i].key) {
        return &root->registros[i];
    }

    // Se a chave nao for encontrada e este for um nodo folha, retorna NULL
    if (root->is_leaf) {
        return NULL;
    }

    // Vai para o filho apropriado
    return search(root->children[i], key);
}