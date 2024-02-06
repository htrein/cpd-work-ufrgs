#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_LENGTH 1000

typedef struct
{
    char nome[1000];
    char website[1000];
    int id_emp;
    long id_est_ind;
} EMPRESA;

typedef struct
{
    char nome[1000];
    int id_ind; //1-2000
} INDUSTRIA;

char *removerAspasDuplas(const char *str)
{
    // Calcula o comprimento da string
    size_t comprimento = strlen(str);

    // Aloca espaço para a nova string (pode ser no máximo o mesmo comprimento)
    char *novaString = (char *)malloc(comprimento + 1);

    if (novaString == NULL)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    // Índice para a nova string
    size_t i = 0;

    // Itera sobre a string original
    for (size_t j = 0; j < comprimento; j++)
    {
        // Copia os caracteres que não são aspas duplas
        if (str[j] != '\"')
        {
            novaString[i++] = str[j];
        }
    }

    // Adiciona o terminador nulo
    novaString[i] = '\0';

    return novaString;
}

void removeQuebraLinha(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0'; // Remove a quebra de linha
    }
}

void escreverBinario(const char *nomeArquivoTexto, const char *nomeArquivoBinario)
{
    FILE *arquivoTexto = fopen(nomeArquivoTexto, "r");
    FILE *arquivoBinario = fopen(nomeArquivoBinario, "wb");
    int k = 0;

    if (arquivoTexto == NULL || arquivoBinario == NULL)
    {
        perror("Erro ao abrir arquivos");
        exit(EXIT_FAILURE);
    }

    EMPRESA empresa;
    INDUSTRIA indust;
    char buffer[MAX_NOME_LENGTH];
    char linhaConcatenada[MAX_NOME_LENGTH];
    char chave[1000]; // Variável externa para armazenar a chave

    // Inicializa os buffers
    buffer[0] = '\0';
    linhaConcatenada[0] = '\0';

    // Leitura linha por linha
    while (fgets(buffer, sizeof(buffer), arquivoTexto) != NULL)
    {
        if (buffer[0] == '\"')
        {
            // Remove a quebra de linha do final da próxima linha
            char proximaLinha[MAX_NOME_LENGTH];
            fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
            removeQuebraLinha(proximaLinha);

            // Concatena a próxima linha à atual
            snprintf(linhaConcatenada, sizeof(linhaConcatenada), "%s %s", buffer, proximaLinha);

            if (strstr(linhaConcatenada, ",,") != NULL)
            {
                if (linhaConcatenada[strlen(linhaConcatenada) - 1] == '\"')
                    sscanf(linhaConcatenada, "%999[^,],,\"%999[^\"]\"", empresa.nome, chave);
                else
                    sscanf(linhaConcatenada, "%999[^,],,%999[^\n]", empresa.nome, chave);
                strcpy(empresa.website, "indisponivel");
            }
            else
            {

                if (buffer[strlen(buffer) - 1] == '\"')
                    // Processa a linha completa
                    sscanf(linhaConcatenada, " \"%999[^\"]\",%999[^,],\"%999[^\"]\"", empresa.nome, empresa.website, chave);
                else
                    sscanf(linhaConcatenada, " \"%999[^\"]\",%999[^,],%999[^\n]", empresa.nome, empresa.website, chave);
            }
        }
        else
        {
            if (strstr(buffer, ",,") != NULL)
            {
                if (buffer[strlen(buffer) - 1] == '\"')
                    sscanf(buffer, "%999[^,],,\"%999[^\n]\"", empresa.nome, chave);
                else
                    sscanf(buffer, "%999[^,],,%999[^\n]", empresa.nome, chave);
                strcpy(empresa.website, "indisponivel");
            }
            else
            {

                if (buffer[strlen(buffer) - 1] == '\"')
                    // Processa a linha normalmente
                    sscanf(buffer, "%999[^,],%999[^,],\"%999[^\"]\"", empresa.nome, empresa.website, chave);
                else
                    sscanf(buffer, "%999[^,],%999[^,],%999[^\n]", empresa.nome, empresa.website, chave);
            }
        }
        char *novaChave = removerAspasDuplas(chave);
        char *novoNome = removerAspasDuplas(empresa.nome);
        strcpy(chave, novaChave);
        strcpy(empresa.nome, novoNome);

        // Libera a memória alocada pela função removerAspasDuplas
        free(novaChave);
        free(novoNome);
        empresa.id_emp = k;
        //acha a chave estrangeira
        FILE *arquivo = fopen("industria2.bin", "rb");
        int encontrado = 0;

        rewind(arquivo);
        while (!feof(arquivo) && encontrado == 0)
            if (fread(&indust, sizeof(INDUSTRIA), 1, arquivo) == 1)
              //  printf("%s %s\n", indust.nome, chave);
                if (!(strcmp(indust.nome, chave)))
                {

                    encontrado = 1;
                    fseek(arquivo, -sizeof(INDUSTRIA), SEEK_CUR);
                    empresa.id_est_ind = ftell(arquivo);
                }

        fclose(arquivo);
        // Escreve no arquivo binário
        fwrite(&empresa, sizeof(EMPRESA), 1, arquivoBinario);

        // Reinicia as strings para a próxima iteração
        empresa.nome[0] = '\0';
        empresa.website[0] = '\0';
        chave[0] = '\0';
        k++;
    }

    fclose(arquivoTexto);
    fclose(arquivoBinario);
}

void lerBinario(const char *nomeArquivoBinario)
{
    FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb");

    if (arquivoBinario == NULL)
    {
        perror("Erro ao abrir arquivo binário");
        exit(EXIT_FAILURE);
    }

    EMPRESA empresa;

    while (fread(&empresa, sizeof(EMPRESA), 1, arquivoBinario) == 1)
    {
        printf("Nome:%s\nWebsite:%s\nID Emp:%d\nID Ind:%ld\n\n", empresa.nome, empresa.website, empresa.id_emp, empresa.id_est_ind);
    }

    fclose(arquivoBinario);
}

int main()
{
    const char *arquivoTexto = "empresas_texto.txt";
    const char *arquivoBinario = "empresas.bin";

    // Escreve o arquivo binário a partir do arquivo de texto
    //escreverBinario(arquivoTexto, arquivoBinario);

    // Lê e imprime os dados do arquivo binário
    lerBinario(arquivoBinario);

    return EXIT_SUCCESS;
}
