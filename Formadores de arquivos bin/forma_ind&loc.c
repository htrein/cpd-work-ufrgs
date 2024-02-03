#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_LENGTH 1000

typedef struct{
    char nome[1000];
    int id_local;
}LOCALIZACAO;

typedef struct{
    char nome[1000];
    int id_ind;
}INDUSTRIA;

void escreverBinario(const char *nomeArquivoTexto, const char *nomeArquivoBinario) {
    FILE *arquivoTexto = fopen(nomeArquivoTexto, "r");
    FILE *arquivoBinario = fopen(nomeArquivoBinario, "wb");
    int k = 0;

    if (arquivoTexto == NULL || arquivoBinario == NULL) {
        perror("Erro ao abrir arquivos");
        exit(EXIT_FAILURE);
    }

    INDUSTRIA industria;
    char buffer[MAX_NOME_LENGTH];

    while (fgets(buffer, MAX_NOME_LENGTH, arquivoTexto) != NULL) {
        // Remove a quebra de linha do final da string lida
        buffer[strcspn(buffer, "\n")] = '\0';
        if (buffer[0] == '\"') {
            // Se a linha começa com aspas
            int parada;
            for (parada = 1; buffer[parada] != '\"'; parada++) {
                industria.nome[parada - 1] = buffer[parada];
            }
            char *posicao = &buffer[parada+2];
            industria.id_ind = atoi(posicao);
            industria.nome[parada - 1] = '\0';
        } else {
            // Se a linha não começa com aspas
            sscanf(buffer, "%99[^,]", industria.nome);
        }
        industria.id_ind = k;
        k++;
        // Escreve no arquivo binário
        fwrite(&industria, sizeof(INDUSTRIA), 1, arquivoBinario);
    }

    fclose(arquivoTexto);
    fclose(arquivoBinario);
}

void lerBinario(const char *nomeArquivoBinario) {
    FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb");

    if (arquivoBinario == NULL) {
        perror("Erro ao abrir arquivo binário");
        exit(EXIT_FAILURE);
    }

    INDUSTRIA industria;

    while (fread(&industria, sizeof(INDUSTRIA), 1, arquivoBinario) == 1) {
        printf("Nome:%s\nid_ind:%d\n\n", industria.nome, industria.id_ind);
    }

    fclose(arquivoBinario);
}

int main() {
    const char *arquivoTexto = "input.txt";
    const char *arquivoBinario = "industria2.bin";

    // Escreve o arquivo binário a partir do arquivo de texto
    escreverBinario(arquivoTexto, arquivoBinario);

    // Lê e imprime os dados do arquivo binário
    lerBinario(arquivoBinario);

    return EXIT_SUCCESS;
}


