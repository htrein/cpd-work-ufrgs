//--------------------------------------------------------------------
// BIBLIOTECAS
//--------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//--------------------------------------------------------------------
// CONSTANTES
//--------------------------------------------------------------------

#define MAX_NOME_LENGTH 10000
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
    char qualif[180];
    char especializacao[180];
    char yoe[50];
    char habilidades[500];
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
    char especializacao[1000];
    char qualificacao[1000];
    char habilidades[5000];
    char yoe[100];
    char data_criacao[100];
} REGISTRO;

// chave-posicao, armazena chave e posicao(indice) do registro que queremos
typedef struct keyPos
{
    int key;
    long pos;
    int encontrado;
} KEYPOS;

// Definindo a estrutura de um nodo da arvore B
typedef struct node
{
    //int keys[2 * ORDER - 1];
    KEYPOS registros[2 * ORDER - 1];
    struct node *children[2 * ORDER];
    int num_keys;
    bool is_leaf;
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
int str_to_inteiro(char str[]);
int compara_industrias(const void *a, const void *b);
int compara_industrias_contra(const void *a, const void *b);
int compara_locs(const void *a, const void *b);
int compara_locs_contra(const void *a, const void *b);

//--------------------------------------------------------------------
// MAIN FUNCTION
//--------------------------------------------------------------------

int main()
{
    //variaveis
    char option;
    char op_busca;
    char op_clas;
    int op_pag = 0;


    //entrada do usuario
    char key_busca[1000];

    //posicoes de memoria
    long pos_emp;
    long pos_carg;
    long pos_oft;

    //estruturas
    INDUSTRIA ind;
    EMPRESA emp;
    OFERTA oft;
    CARGO carg;
    LOCALIZACAO loc;
    CRITERIOS crit;
    REGISTRO reg;
    KEYPOS *resultado;

//--------  ARVORES B --------------------------------------------------------------------------------------------------------

    // MONTAR ARVORE NO INICIO DO PROGRAMA.
    Node* root_emp = NULL;
    Node* root_cargos = NULL;
    Node* root_ofertas_emp = NULL;
    Node* root_ofertas_cargos = NULL;
    FILE* arqemp2;
    FILE* arqcargos2;
    FILE* arq_ofertas_emp;
    FILE* arq_ofertas_cargos;
    KEYPOS ch;

    // INICIO DO PROCESSO DE MONTAGEM DA ARVORE B de empresa.
    arqemp2 = fopen("empresas.bin", "rb");

    if (arqemp2 == NULL)
    {
        printf("Erro ao abrir o arquivo de empresas.\n");
        return 1;
    }

    while (fread(&emp, sizeof(EMPRESA), 1, arqemp2) == 1)
    {
        ch.key = str_to_inteiro(emp.nome);
        ch.pos = emp.id_emp;
        ch.encontrado = 0;
        insert(&root_emp, ch);
    }

    fclose(arqemp2);

    /* A VARIAVEL root_emp É UM PONTEIRO PARA O NODO RAIZ DA ARVORE B DE EMPRESAS */

    // INICIO DO PROCESSO DE MONTAGEM DA ARVORE B de cargos.
    arqcargos2 = fopen("cargos.bin", "rb");

    if (arqcargos2 == NULL)
    {
        printf("Erro ao abrir o arquivo de cargos.\n");
        return 1;
    }

    while (fread(&carg, sizeof(CARGO), 1, arqcargos2) == 1)
    {
        ch.key = str_to_inteiro(carg.nome);
        ch.pos = carg.id_cargo;
        ch.encontrado = 0;
        insert(&root_cargos, ch);
    }

    fclose(arqcargos2);

    /* A VARIAVEL root_cargos É UM PONTEIRO PARA O NODO RAIZ DA ARVORE B DE CARGOS */

    // INICIO DO PROCESSO DE MONTAGEM DA ARVORE B de ofertas emp.
    arq_ofertas_emp = fopen("ofertas.bin", "rb");

    if (arq_ofertas_emp == NULL)
    {
        printf("Erro ao abrir o arquivo de ofertas.\n");
        return 1;
    }

    int i = 0;
    while (fread(&oft, sizeof(OFERTA), 1, arq_ofertas_emp) == 1)
    {
        ch.key = oft.id_est_emp;
        ch.pos = i;             // posicao, indice no arquivo ofertas.bin
        ch.encontrado = 0;
        insert(&root_ofertas_emp, ch);
        i++;
    }

    fclose(arq_ofertas_emp);

    /* A VARIAVEL root_ofertas_emp É UM PONTEIRO PARA O NODO RAIZ DA ARVORE B DE ofertas-empresas */

    // INICIO DO PROCESSO DE MONTAGEM DA ARVORE B de ofertas emp.
    arq_ofertas_cargos = fopen("ofertas.bin", "rb");

    if (arq_ofertas_cargos == NULL)
    {
        printf("Erro ao abrir o arquivo de ofertas.\n");
        return 1;
    }

    i = 0;
    while (fread(&oft, sizeof(OFERTA), 1, arq_ofertas_cargos) == 1)
    {
        ch.key = oft.id_est_cargo;
        ch.pos = i;             // posicao, indice no arquivo ofertas.bin
        ch.encontrado = 0;
        insert(&root_ofertas_cargos, ch);
        i++;
    }

    fclose(arq_ofertas_cargos);

    /* A VARIAVEL root_ofertas_cargos É UM PONTEIRO PARA O NODO RAIZ DA ARVORE B DE ofertas-cargos */


    //////////////////TESTAR ISSO DAQUI PRA VER QUE ALGUNS ENCONTRADOS NÃO ESTÃO EM 0////////////
    //printTree(root_ofertas_emp);

    //MENU DE OPCOES
    do
    {
        system("cls");
        printf("\t\t\t\t\t\t--JOBS SEARCH ENGINE--\n\n");
        printf("(1) Busca Rapida\n\n");
        printf("(2) Classificacao\n\n");
        printf("=>");
        scanf("%c", &option);
    }
    while(option!='1'&&option!='2');

    switch(option)
    {
    case '1':
        do
        {
            system("cls");
            printf("--BUSCA RAPIDA--");
            printf("\nEscolha a categoria pela qual gostaria de buscar!\n");
            printf("(1) Empresas\n");
            printf("(2) Cargos\n");
            scanf("%c", &op_busca);
        }
        while(op_busca!='1' && op_busca!='2');
        getchar();
        system("cls");

        int key_busca_convertida; // PRA USAR COMO CHAVE NA FUNCAO DE SEARCH, POIS SO RECEBE INT E NAO STRING.

        if(op_busca=='1')
        {
            printf("Digite o nome da empresa:\n**exemplos: east india securities ltd., pearl global solutions, AxisTechnolabs**\n\n");
            fgets(key_busca, sizeof(key_busca), stdin);
            // Remover o caractere de nova linha presente na entrada do usuário
            int len = strlen(key_busca);
            if (len > 0 && key_busca[len - 1] == '\n')
            {
                key_busca[len - 1] = '\0';
            }

            FILE* arqemp = fopen("empresas.bin", "rb");
            FILE* arqind = fopen("industrias.bin", "rb");
            FILE* arqof = fopen("ofertas.bin", "rb");
            FILE* arqcarg = fopen("cargos.bin", "rb");
            FILE* arqloc = fopen("localizacoes.bin", "rb");
            FILE* arqcrit = fopen("criterios.bin", "rb");

            key_busca_convertida = str_to_inteiro(key_busca);   // funcao converte a entrada
            resultado = search(root_emp, key_busca_convertida);
            if (resultado == NULL)
            {
                printf("Empresa nao registrada!\n");
                break;
            }
            else
            {
                pos_emp = resultado->pos * sizeof(EMPRESA);

                if (fseek(arqemp, pos_emp, SEEK_SET) != 0)
                {
                    printf("Erro ao posicionar o ponteiro de arquivo no arquivo de empresas!\n");
                    break;
                }
                else
                {
                    fread(&emp, sizeof(EMPRESA), 1, arqemp);
                    strcpy(reg.nome_empresa, emp.nome);
                    strcpy(reg.website, emp.website);
                }

                if(fseek(arqind, emp.id_est_ind, SEEK_SET) != 0)
                {
                    printf("Erro ao posicionar o ponteiro de arquivo no arquivo de industrias!\n");
                    break;
                }
                else
                {
                    fread(&ind, sizeof(INDUSTRIA), 1, arqind);
                    strcpy(reg.nome_industria, ind.nome);
                }

                char op_pag = '1';
                while (op_pag == '1') //como existem mais de uma vaga sendo ofertada pela mesma empresa, aqui é um loop até todas as vagas sejam encontradas
                {                   //esse encadeamento de arquivos eh explicado no relatório do trabalho
                    int paginada = 0;
                    do
                    {
                        resultado = search(root_ofertas_emp, pos_emp);
                        if (resultado == NULL)
                        {
                            printf("Sem mais resultados!\n");
                            break;
                        }

                        pos_oft = resultado->pos * sizeof(OFERTA);

                        if (fseek(arqof, pos_oft, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de ofertas!\n");
                            break;
                        }
                        else
                        {
                            fread(&oft, sizeof(OFERTA), 1, arqof);
                            strcpy(reg.data_criacao, oft.data_criacao);
                            pos_carg = oft.id_est_cargo;
                        }

                        if (fseek(arqcarg, pos_carg, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de cargos!\n");
                            break;
                        }
                        else
                        {
                            fread(&carg, sizeof(CARGO), 1, arqcarg);
                            strcpy(reg.nome_cargo, carg.nome);
                            strcpy(reg.descricao, carg.descricao);
                        }

                        if (fseek(arqloc, carg.id_est_loc, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de localizacao!\n");
                            break;
                        }
                        else
                        {
                            fread(&loc, sizeof(LOCALIZACAO), 1, arqloc);
                            strcpy(reg.nome_localizacao, loc.nome);
                        }

                        if (fseek(arqcrit, carg.id_est_crit, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de criterios!\n");
                            break;
                        }
                        else
                        {
                            fread(&crit, sizeof(CRITERIOS), 1, arqcrit);
                            strcpy(reg.habilidades, crit.habilidades);
                            strcpy(reg.qualificacao, crit.qualif);
                            strcpy(reg.especializacao, crit.especializacao);
                            strcpy(reg.yoe, crit.yoe);
                        }

                        paginada++;
                        printf("Empresa: %s\nWebsite: %s\nIndustria: %s\nCargo: %s\nData de oferta: %s\nDescricao: %s\nLocalizacao: %s\n--Requisitos--\nHabilidades: %s\nEspecializacao: %s\nQualificacao: %s\nAnos de experiencia: %s\n\n\n",
                               reg.nome_empresa, reg.website, reg.nome_industria, reg.nome_cargo, reg.data_criacao, reg.descricao, reg.nome_localizacao, reg.habilidades, reg.especializacao, reg.qualificacao, reg.yoe);

                    }
                    while (resultado != NULL && paginada < 6);

                    // Paginacao de resultados
                    if (resultado == NULL)
                    {
                        break;
                    }
                    else
                    {
                        printf("\n\nVoce gostaria de ver mais resultados?\n");
                        printf("(1) Sim \t\t (2) Nao\n\n");
                        char input_result;
                        do
                        {
                            scanf("%c", &input_result);
                        }
                        while (input_result != '1' && input_result != '2');
                        op_pag = input_result;
                    }
                }
                fclose(arqemp);
                fclose(arqind);
                fclose(arqof);
                fclose(arqcarg);
                fclose(arqloc);
                fclose(arqcrit);

            }
        }
        else
        {
            printf("Digite o nome do cargo:\n**exemplos: Python Lead, Java Developer, UI Developer**\n\n");
            fgets(key_busca, sizeof(key_busca), stdin);
            // Remover o caractere de nova linha presente na entrada do usuário
            int len = strlen(key_busca);
            if (len > 0 && key_busca[len - 1] == '\n')
            {
                key_busca[len - 1] = '\0'; // Substitui o caractere de nova linha por null terminator
            }
            FILE* arqemp = fopen("empresas.bin", "rb");
            FILE* arqind = fopen("industrias.bin", "rb");
            FILE* arqof = fopen("ofertas.bin", "rb");
            FILE* arqcarg = fopen("cargos.bin", "rb");
            FILE* arqloc = fopen("localizacoes.bin", "rb");
            FILE* arqcrit = fopen("criterios.bin", "rb");

            // conversao para int eh necessaria, pois os valores chave das arvores sao inteiros.
            key_busca_convertida = str_to_inteiro(key_busca);
            resultado = search(root_cargos, key_busca_convertida);

            if (resultado == NULL)
            {
                printf("Cargo nao encontrado!\n");
                  break;
            }
            else
            {
                pos_carg = resultado->pos*sizeof(CARGO);

                if (fseek(arqcarg, pos_carg, SEEK_SET) != 0)
                {
                    printf("Erro ao posicionar o ponteiro de arquivo no arquivo de cargos!\n");
                      break;
                }
                else
                {
                    fread(&carg, sizeof(CARGO), 1, arqcarg);
                    strcpy(reg.nome_cargo, carg.nome);
                    strcpy(reg.descricao, carg.descricao);
                }

                char op_pag = '1';
                while (op_pag == '1')//como existem mais de uma vaga sendo ofertada pela mesma empresa, aqui é um loop até todas as vagas sejam encontradas
                {                   //esse encadeamento de arquivos eh explicado no relatório do trabalho
                    int paginada = 0;
                    do
                    {
                        resultado = search(root_ofertas_cargos, pos_carg);
                        if (resultado == NULL)
                        {
                            printf("Sem mais resultados!\n");
                            break;
                        }
                        else
                        {
                            pos_oft = resultado->pos * sizeof(OFERTA);
                        }
                        if (fseek(arqof, pos_oft, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de ofertas!\n");
                              break;
                        }
                        else
                        {
                            fread(&oft, sizeof(OFERTA), 1, arqof);
                            strcpy(reg.data_criacao, oft.data_criacao);
                            pos_emp = oft.id_est_emp;
                        }

                        //le no arquivo de empresas e printa as informacoes
                        if (fseek(arqemp, pos_emp, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de empresas!\n");
                              break;
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
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de industrias!\n");
                              break;
                        }
                        else
                        {
                            fread(&ind, sizeof(INDUSTRIA), 1, arqind);
                            strcpy(reg.nome_industria, ind.nome);
                        }

                        //le no arquivo de criterios na posicao cargos.crit
                        if (fseek(arqcrit, carg.id_est_crit, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de criterios!\n");
                              break;
                        }
                        else
                        {

                            fread(&crit, sizeof(CRITERIOS), 1, arqcrit);
                            strcpy(reg.habilidades, crit.habilidades);
                            strcpy(reg.especializacao, crit.especializacao);
                            strcpy(reg.qualificacao, crit.qualif);
                            strcpy(reg.yoe, crit.yoe);
                        }

                        //le no arquivo de localizacoes na posicao cargos.id_est_loc
                        if (fseek(arqloc, carg.id_est_loc, SEEK_SET) != 0)
                        {
                            printf("Erro ao posicionar o ponteiro de arquivo no arquivo de localizacao!\n");
                              break;
                        }
                        else
                        {
                            fread(&loc, sizeof(LOCALIZACAO), 1, arqloc);
                            strcpy(reg.nome_localizacao, loc.nome);
                        }
                        paginada++;
                        printf("Empresa: %s\nWebsite: %s\nIndustria: %s\nCargo: %s\nData de oferta: %s\nDescricao: %s\nLocalizacao: %s\n--Requisitos--\nHabilidades: %s\nEspecializacao: %s\nQualificacao: %s\nAnos de experiencia: %s\n\n\n", reg.nome_empresa, reg.website, reg.nome_industria, reg.nome_cargo, reg.data_criacao, reg.descricao, reg.nome_localizacao, reg.habilidades, reg.especializacao, reg.qualificacao, reg.yoe);
                    }
                    while (resultado != NULL && paginada < 6);

                    // Paginacao de resultados
                    if (resultado == NULL)
                    {
                        break;
                    }
                    else
                    {
                        printf("\n\nVoce gostaria de ver mais resultados?\n");
                        printf("(1) Sim \t\t (2) Nao\n\n");
                        char input_result;
                        do
                        {
                            scanf("%c", &input_result);
                        }
                        while (input_result != '1' && input_result != '2');
                        op_pag = input_result;
                    }
                }

            }
            fclose(arqemp);
            fclose(arqind);
            fclose(arqof);
            fclose(arqcarg);
            fclose(arqloc);
            fclose(arqcrit);
        }

        break;


    case '2':

        do
        {
            system("cls");
            printf("--CLASSIFICACAO--");
            printf("Escolha a categoria pela qual gostaria de classificar!\n");
            printf("(1) Industrias\n");
            printf("(2) Localizacao\n");
            scanf("%c", &op_clas);
        }
        while(op_clas!='1' &&op_clas!='2');
        system("cls");
        getchar();

        if(op_clas=='1')
        {
            FILE* arqemp = fopen("empresas.bin", "rb");
            FILE* arqind = fopen("industria.bin", "rb");
            //FILE* arqof = fopen("ofertas.bin", "rb");
            //FILE* arqcarg = fopen("cargos.bin", "rb");
            //FILE* arqloc = fopen("localizacao.bin", "rb");
            //FILE* arqcrit = fopen("criterios.bin", "rb");

            fseek(arqind, 0, SEEK_END);
            int num_industrias = ftell(arqind) / sizeof(INDUSTRIA); // numero de industrias no arquivo

            fseek(arqind, 0, SEEK_SET); // move o ponteiro para o inicio do arquivo

            INDUSTRIA* industria_e_chave = malloc(num_industrias * sizeof(INDUSTRIA));

            for (int i = 0; i < num_industrias; i++) { // preenche o array com nome da industria e chave a partir do arquivo binario
                {
                    fread(industria_e_chave[i].nome, sizeof(industria_e_chave[i].nome), 1, arqind);
                    fread(&industria_e_chave[i].id_ind, sizeof(industria_e_chave[i].id_ind), 1, arqind);
                }
            }

            qsort(industria_e_chave, num_industrias, sizeof(INDUSTRIA), compara_industrias); // ordena (nesse caso, ordem alfabetica)

            EMPRESA emp;

            for (int i = 0; i < num_industrias; i++) { // imprime cada vaga
                while(fread(&emp, sizeof(EMPRESA), 1, arqemp) == 1) {
                    if (emp.id_est_ind == industria_e_chave[i].id_ind) { // se a empresa for da industria em questao
                        printf("Empresa: %s\nWebsite: %s\nIndustria: %s\n\n", emp.nome, emp.website, industria_e_chave[i].nome);
                    }
                }
            }

            fclose(arqemp);
            fclose(arqind);
            //fclose(arqof);
            //fclose(arqcarg);
            //fclose(arqloc);
            //fclose(arqcrit);

            free(industria_e_chave);
        }
        else
        {
            FILE* arqemp = fopen("empresas.bin", "rb");
            FILE* arqind = fopen("industria.bin", "rb");
            FILE* arqof = fopen("ofertas.bin", "rb");
            FILE* arqcarg = fopen("cargos.bin", "rb");
            FILE* arqloc = fopen("localizacao.bin", "rb");
            FILE* arqcrit = fopen("criterios.bin", "rb");

            fseek(arqloc, 0, SEEK_END);
            int num_locs = ftell(arqloc) / sizeof(LOCALIZACAO); // numero de industrias no arquivo

            fseek(arqloc, 0, SEEK_SET); // move o ponteiro para o inicio do arquivo

            LOCALIZACAO* loc_e_chave = malloc(num_locs * sizeof(LOCALIZACAO));

            for (int i = 0; i < num_locs; i++) { // preenche o array com nome da industria e chave a partir do arquivo binario
                {
                    fread(loc_e_chave[i].nome, sizeof(loc_e_chave[i].nome), 1, arqloc);
                    fread(&loc_e_chave[i].id_local, sizeof(loc_e_chave[i].id_local), 1, arqloc);
                }
            }

            qsort(loc_e_chave, num_locs, sizeof(LOCALIZACAO), compara_locs); // ordena em ordem alfabetica

            CARGO cargo;
            OFERTA oferta;
            int chave_cargo;

            for (int i = 0; i < num_locs; i++) { // imprime cada vaga
                while(fread(&cargo, sizeof(LOCALIZACAO), 1, arqloc) == 1) {
                    if (cargo.id_est_loc == loc_e_chave[i].id_local) { // se o cargo for do local em questao
                        printf("Cargo: %s\nDescricao: %s\nLocalizacao: %s\n", cargo.nome, cargo.descricao, loc_e_chave[i].nome);
                        chave_cargo = cargo.id_cargo;

                        while(fread(&oferta, sizeof(oferta), 1, arqof) == 1) { // usa a chave do cargo obtida para encontrar a data de criacao no arqof
                            if (oferta.id_est_cargo == chave_cargo) { // procura a chave estrangeira do cargo
                                printf("Data de criacao: %s\n", oferta.data_criacao);
                            }
                        }
                    }
                }
            }

            fclose(arqemp);
            fclose(arqind);
            fclose(arqof);
            fclose(arqcarg);
            fclose(arqloc);
            fclose(arqcrit);

            free(loc_e_chave);
        }
        break;

    default:
        break;


    }
    return 0;
}

//--------------------------------------------------------------------
// FUNCOES
//--------------------------------------------------------------------

// Funcao para criar um novo nodo
Node* createNode()
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        perror("Erro ao alocar memoria para o novo nodo!\n");
        exit(EXIT_FAILURE);
    }

    newNode->num_keys = 0;
    newNode->is_leaf = true;

    for (int i = 0; i < 2 * ORDER; i++)
    {
        newNode->children[i] = NULL;
        //newNode->keys[i] = 0;
        newNode->registros[i].key = 0;
        newNode->registros[i].pos = 0;
    }

    return newNode;
}

// Funcao para dividir um nodo filho do pai
void splitChild(Node* parent, int index, Node* child)
{
    /* NODO FILHO A DIREITA CRIADO */
    Node* newChild = createNode();
    newChild->is_leaf = child->is_leaf;
    newChild->num_keys = ORDER - 1;

    // DIVIDE AS CHAVES ENTRE NODOS FILHOS
    for (int i = 0; i < ORDER - 1; i++)
    {
        //newChild->keys[i] = child->keys[i + ORDER];
        newChild->registros[i].key = child->registros[i + ORDER].key;
        newChild->registros[i].pos = child->registros[i + ORDER].pos;
    }

    if (!child->is_leaf)
    {
        for (int i = 0; i < ORDER; i++)
            newChild->children[i] = child->children[i + ORDER];
    }

    child->num_keys = ORDER - 1;

    for (int i = parent->num_keys; i >= index + 1; i--)
        parent->children[i + 1] = parent->children[i];

    parent->children[index + 1] = newChild;

    for (int i = parent->num_keys - 1; i >= index; i--)
    {
        //parent->keys[i + 1] = parent->keys[i];
        parent->registros[i + 1].key = parent->registros[i].key;
        parent->registros[i + 1].pos = parent->registros[i].pos;
    }

    //parent->keys[index] = child->keys[ORDER - 1];
    parent->registros[index].key = child->registros[ORDER - 1].key;
    parent->registros[index].pos = child->registros[ORDER - 1].pos;
    parent->num_keys++;
}

// Funcao para inserir uma chave em uma arvore B
/* AO INVES DE INT key COLOCAMOS KEYPOS registro */
void insert(Node** root, KEYPOS reg)
{
    Node* temp = *root;

    /* CASO RAIZ VAZIA */
    if (temp == NULL)
    {

        *root = createNode();
        //(*root)->keys[0] = key;
        (*root)->registros[0].key = reg.key;
        (*root)->registros[0].pos = reg.pos;
        (*root)->num_keys = 1;

    }
    else
    {
        /* CASO NODO ESTIVER CHEIO */
        if (temp->num_keys == 2 * ORDER - 1)
        {

            Node* newRoot = createNode();
            newRoot->is_leaf = false;
            newRoot->children[0] = *root;

            /* FUNCAO QUE DIVIDE O NODO CHEIO */
            splitChild(newRoot, 0, *root);
            int i = 0;

            /* O REGISTRO A SER INSERIDO NAO SERA INSERIDO NA NOVA RAIZ E SIM EM UM DOS FILHOS
               POR ISSO A CHAMADA RECURSIVA ABAIXO*/

            /* POR QUE I = 0 OU I = 1? POIS UMA NOVA RAIZ SEMPRE TEM 2 FILHOS */

            /*if (newRoot->keys[0] < key)
                i++;
            */

            if (newRoot->registros[0].key < reg.key)
                i++;

            // CHAMADA RECURSIVA
            insert(&newRoot->children[i], reg);
            *root = newRoot;

        }
        else
        {

            /* CASO O NODO NAO ESTIVER CHEIO */
            // FACA ENQUANTO NAO EH FOLHA
            while (!temp->is_leaf)
            {

                int i = temp->num_keys - 1; // i, vira um indice do nodo que contem as chaves

                // procurando posicao do filho da chave(registrador) i na qual verificar se eh folha para inserir.
                while (i >= 0 && temp->registros[i].key > reg.key) // temp->keys[i] vira temp->registro[i].key, key vira reg.key
                    i--;
                i++;

                if (temp->children[i]->num_keys == 2 * ORDER - 1)
                {
                    splitChild(temp, i, temp->children[i]);
                    if (temp->registros[i].key < reg.key)          // temp->keys[i] < key vira temp->registro[i].key < reg.key
                        i++;
                }
                temp = temp->children[i];
            }

            // AQUI SABEMOS QUE O NODO EH FOLHA, ENTAO SOMENTE RESTA A INSERCAO.
            int i = temp->num_keys - 1;
            while (i >= 0 && temp->registros[i].key > reg.key)                  // temp->keys[i] > key VIRA temp->registro[i].key > reg.key
            {
                temp->registros[i + 1].key = temp->registros[i].key;            // temp->keys[i + 1] = temp->keys[i] vira temp->registro[i + 1].key = temp->registro[i].key;

                temp->registros[i + 1].pos = temp->registros[i].pos;
                i--;
            }
            //temp->keys[i + 1] = key;
            temp->registros[i + 1].key = reg.key;

            temp->registros[i + 1].pos = reg.pos;
            temp->num_keys++;
        }
    }
}

// Funcao para percorrer a arvore B
void traverse(Node* root)
{
    if (root != NULL)
    {
        int i;
        for (i = 0; i < root->num_keys; i++)
        {
            traverse(root->children[i]);
            //printf("%d ", root->keys[i]);

            printf("ch:%d, pos:%d, enc:%d\n", root->registros[i].key, root->registros[i].pos, root->registros[i].encontrado );

        }
        traverse(root->children[i]);
    }
}

// Funcao para buscar um registro na B-Tree
KEYPOS* search(Node* root, int key)
{
    // Encontra a primeira chave maior ou igual à chave de entrada
    int i = 0;
    while (i < root->num_keys && key > root->registros[i].key)
    {
        i++;
    }

    // Procura por todos os registros com a mesma chave no mesmo nodo, mesmo que já tenham sido marcados como encontrados
    while (i < root->num_keys && key == root->registros[i].key)
    {
        if (root->registros[i].encontrado == 0)
        {
            root->registros[i].encontrado = 1; // Marca como encontrado
            return &root->registros[i];
        }
        i++;
    }

    // Se a chave não for encontrada e este for um nodo folha, retorna NULL
    if (root->is_leaf)
    {
        return NULL;
    }

    // Vai para o filho apropriado
    return search(root->children[i], key);
}




// ESSA FUNCAO GERARÁ CHAVES A PARTIR DE STRINGS
// ESSAS CHAVES SERAO USADAS NA ARVORE E TAMBEM NO MOMENTO APOS O USUARIO
// DAR SUA ENTRADA.

int str_to_inteiro(char str[])
{
    int soma_ascii = 0;

    // Convertendo a string para inteiros com base nos valores ASCII dos caracteres
    for (int i = 0; str[i] != '\0'; i++)
    {
        // Convertendo o caractere para seu valor ASCII
        soma_ascii = soma_ascii + (int)str[i];
    }

    return soma_ascii;
}

int compara_industrias(const void *a, const void *b) {
    INDUSTRIA* indA = (INDUSTRIA*)a;
    INDUSTRIA* indB = (INDUSTRIA*)b;
    return strcmp(indA->nome, indB->nome); // compara em ordem alfabetica
}

int compara_industrias_contra(const void *a, const void *b) {
    INDUSTRIA* indA = (INDUSTRIA*)a;
    INDUSTRIA* indB = (INDUSTRIA*)b;
    return strcmp(indB->nome, indA->nome); // compara em ordem contra alfabetica
}

int compara_locs(const void *a, const void *b) {
    LOCALIZACAO* locA = (LOCALIZACAO*)a;
    LOCALIZACAO* locB = (LOCALIZACAO*)b;
    return strcmp(locA->nome, locB->nome); // ordem alfabetica
}

int compara_locs_contra(const void *a, const void *b) {
    LOCALIZACAO* locA = (LOCALIZACAO*)a;
    LOCALIZACAO* locB = (LOCALIZACAO*)b;
    return strcmp(locB->nome, locA->nome); // ordem contra alfabetica
}
