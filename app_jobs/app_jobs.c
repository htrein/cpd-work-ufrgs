
/* BIBLIOTECAS -------------------------------------------------------*/

#include "app_jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* CONSTANTES ---------------------------------------------------------*/

#define ORDER 2 //

/* FUNCOES --------------------------------------------------------------*/

// Função para criar um novo nó
Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro ao alocar memória para o novo nó");
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

// Função para dividir um nó filho do pai
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

// Função para inserir uma chave em uma árvore B
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

            /* O REGISTRO A SER INSERIDO NAO SERÁ INSERIDO NA NOVA RAIZ E SIM EM UM DOS FILHOS
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
            // FAÇA ENQUANTO NAO EH FOLHA
            while (!temp->is_leaf) {

                int i = temp->num_keys - 1; // i, vira um índice do nodo que contem as chaves

                // procurando posicao do filho da chave(registrador) i na qual verificar se é folha para inserir.
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

            // AQUI SABEMOS QUE O NODO É FOLHA, ENTAO SOMENTE RESTA A INSERCAO.
            int i = temp->num_keys - 1;
            while (i >= 0 && temp->registros[i].key > reg.key) {                // temp->keys[i] > key VIRA temp->registro[i].key > reg.key
                temp->registros[i + 1].key = temp->registros[i].key;            // temp->keys[i + 1] = temp->keys[i] vira temp->registro[i + 1].key = temp->registro[i].key;
                // ACREDITO SER NECESSARIO DESLOCAR A POSICAO TAMBEM.
                // ISSO ABAIXO É UM PALPITE, POIS NAO ENTENDI ESSA PARTE DO CODIGO
                temp->registros[i + 1].pos = temp->registros[i].pos;
                i--;
            }
            //temp->keys[i + 1] = key;
            temp->registros[i + 1].key = reg.key;
            // ISSO ABAIXO É UM PALPITE, POIS NAO ENTENDI ESSA PARTE DO CODIGO
            temp->registros[i + 1].pos = reg.pos;
            temp->num_keys++;
        }
    }
}

// Função para percorrer a árvore B
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
