
/* CONSTANTES -------------------------------------------------------------------*/
#define ORDER 2

/* BIBLIOTECAS ------------------------------------------------------------------*/
#include <stdbool.h>

/* TIPOS DE DADOS -------------------------------------------------------------- */

typedef struct{
    char nome[100];
    int id_ind;
}INDUSTRIA;

typedef struct{
    char nome[100];
    char website[100];
    int id_emp;
    INDUSTRIA *chave_estrangeira1;
}EMPRESA;

typedef struct{
    char nome[100];
    int id_local;
}LOCALIZACAO;

typedef struct{
    char nome[100];
    char descricao[1000];
    int id_cargo;
    LOCALIZACAO *chave_estrangeira1;
}CARGO;

typedef struct{
    CARGO *chave_estrangeira1;
    EMPRESA *chave_estrangeira2;
    char data_criacao[100];
}OFERTA;

typedef struct{
    char nome[150];
    char tipo[100];
    int id_crit;
}CRITERIOS;

typedef struct{
    CRITERIOS *chave_estrangeira1;
    CARGO *chave_estrangeira2;
}REQUISITA;

// chave-posicao, armazena chave e posicao do registro que queremos
typedef struct keyPos{
    int key;
    int pos:
}KEYPOS;

/* tipo NODO */
// Definindo a estrutura de um nó da árvore B
typedef struct node {
    //int keys[2 * ORDER - 1];
    KEYPOS registros[2 * ORDER - 1];    // registros com chaves-posicoes dos arquivos
    struct node *children[2 * ORDER];
    int num_keys;
    bool is_leaf;                       // é folha.
} Node;

/* PROTÓTIPOS DE FUNCOES -------------------------------------------------------------- */


Node* createNode();
void splitChild(Node* parent, int index, Node* child);
void insert(Node** root, int key);
void traverse(Node* root);

