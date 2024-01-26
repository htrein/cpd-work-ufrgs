
/* BIBLIOTECAS -------------------------------------------------------*/

#include "app_jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* CONSTANTES ---------------------------------------------------------*/

#define ORDER 2

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
        newChild->registro[i].key = child->registro[i + ORDER].key; //NAO SEI SE FUNCIONA
        newChild->registro[i].pos = child->registro[i + ORDER].pos; //NAO SEI SE FUNCIONA
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
        parent->registro[i + 1].key = parent->registro[i].key; //NAO SEI SE FUNCIONA
        parent->registro[i + 1].pos = parent->registro[i].pos; //NAO SEI SE FUNCIONA
    }
    // O MESMO ALI DE CIMA!
    //parent->keys[index] = child->keys[ORDER - 1];
    parent->registro[index].key = child->registro[ORDER - 1].key; //NAO SEI SE FUNCIONA
    parent->registro[index].pos = child->registro[ORDER - 1].pos; //NAO SEI SE FUNCIONA
    parent->num_keys++;
}

// Função para inserir uma chave em uma árvore B
/* AO INVES DE INT KEY COLOCAMOS KEYPOS registro */
void insert(Node** root, KEYPOS reg) {
    Node* temp = *root;

    /* CASO RAIZ VAZIA */
    if (temp == NULL) {
        *root = createNode();
        /* NAO ADICIONAMOS MAIS SOMENTE KEY, E SIM, KEY E POS*/
        //(*root)->keys[0] = key;
        (*root)->registro[0].key = reg.key; //NAO TENHO CERTEZA SE VA FUNCIONAR
        (*root)->registro[0].pos = reg.pos; //NAO TENHO CERTEZA SE VA FUNCIONAR
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
            if (newRoot->keys[0] < key)
                i++;
            insert(&newRoot->children[i], key);
            *root = newRoot;
        } else {
            while (!temp->is_leaf) {
                int i = temp->num_keys - 1;
                while (i >= 0 && temp->keys[i] > key)
                    i--;
                i++;
                if (temp->children[i]->num_keys == 2 * ORDER - 1) {
                    splitChild(temp, i, temp->children[i]);
                    if (temp->keys[i] < key)
                        i++;
                }
                temp = temp->children[i];
            }
            int i = temp->num_keys - 1;
            while (i >= 0 && temp->keys[i] > key) {
                temp->keys[i + 1] = temp->keys[i];
                i--;
            }
            temp->keys[i + 1] = key;
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
            printf("%d ", root->keys[i]);
        }
        traverse(root->children[i]);
    }
}
