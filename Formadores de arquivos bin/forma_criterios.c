#include <stdio.h>
#include <stdlib.h>
#define QTD_CRITERIOS 2250


typedef struct
{
    char qualif[180];
    char especializacao[180];
    char yoe[50];
    char habilidades[500];
    int id_crit;
} CRITERIOS;


int main(){

    FILE *arquivo;
    FILE *arquivoBin;
    CRITERIOS registros[QTD_CRITERIOS]; // 2250 eh numero de criterios
    char caractere;
    int i;
    int j;

    // Abre o arquivo no modo de leitura ("r")
    arquivo = fopen("criterios.txt", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1; // Retorna 1 para indicar erro
    }

    // Lê um caractere por vez até o final do arquivo
    i = 0;
    do{

        j = 0;
        // le skills, nao precisa tratar quebras de linha
        // se comeca com aspas terá virgula dentro.
        if((caractere = fgetc(arquivo)) != EOF && caractere == '"') {
            while ((caractere = fgetc(arquivo)) != EOF && caractere!= '"'){
                registros[i].habilidades[j] = caractere;
                j++;
            }
            caractere = fgetc(arquivo); // problema com virgula mais tarde
        }
        // sem aspas, o limitador eh virgula
        else{
            registros[i].habilidades[j] = caractere;
            j++;
            while ((caractere = fgetc(arquivo)) != EOF && caractere!= ','){
                registros[i].habilidades[j] = caractere;
                j++;
            }
        }
        registros[i].habilidades[j+1] = 0;

        j = 0;
        // le years experiencie, nao tem aspas
        while ((caractere = fgetc(arquivo)) != EOF && caractere!= ',') {
            registros[i].yoe[j] = caractere;
            j++;
        }
        registros[i].yoe[j+1] = 0;

        j = 0;
        // le especislizacao, nao precisa tratar quebra de linhas
        // se comeca com aspas terá virgula dentro.
        if((caractere = fgetc(arquivo)) != EOF && caractere == '"') {
            while ((caractere = fgetc(arquivo)) != EOF && caractere!='"'){
                registros[i].especializacao[j] = caractere;
                j++;
            }
            //caractere = fgetc(arquivo); // problema com virgula mais tarde
        }
        // sem aspas, o limitador eh virgula
        else{
            registros[i].especializacao[j] = caractere;
            j++;
            while ((caractere = fgetc(arquivo)) != EOF && caractere!= ','){
                registros[i].especializacao[j] = caractere;
                j++;
            }
        }
        registros[i].especializacao[j+1] = 0;


        /*
        while ((caractere = fgetc(arquivo)) != EOF && caractere!= ',') {
            if(caractere != '"'){
                registros[i].especializacao[j] = caractere;
                j++;
            }
        }
        registros[i].especializacao[j+1] = 0;
        */

        j = 0;
        // le qualificacao, precisa tratar quebra de linha
        // tratando caso com quebra de linha
        if((caractere = fgetc(arquivo)) != EOF && caractere == '"'){
            while ((caractere = fgetc(arquivo)) != EOF && caractere!= '"') {
                if(caractere != '\n'){
                    registros[i].qualif[j] = caractere;
                    j++;
                }
            }
            caractere = fgetc(arquivo); // evitar um \n no retorno.

        }
        else{ // tratando caso sem quebra de linha
                registros[i].qualif[j] = caractere; // CORRIGINDO PROBLEMA DE PERDER O A.
                j++;
            while ((caractere = fgetc(arquivo)) != EOF && caractere!= '\n') {
                registros[i].qualif[j] = caractere;
                j++;
            }
        }
        registros[i].qualif[j+1] = 0;
        registros[i].id_crit = i;
        i++;

    }while(i<2250);

    // Fecha o arquivo após a leitura
    fclose(arquivo);


    // Abrir o arquivo para escrita binária
    arquivoBin = fopen("criterios.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Escrever cada elemento do vetor no arquivo
    for (int i = 0; i < 2250; i++) {
        fwrite(&registros[i], sizeof(CRITERIOS), 1, arquivoBin);
    }

    // Fechar o arquivo
    fclose(arquivoBin);


    //for(i=0;i<QTD_CRITERIOS;i++)    printf("%s\n", registros[i].especializacao);

    return 0;
}
