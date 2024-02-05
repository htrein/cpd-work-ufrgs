#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_LENGTH 10000

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

void lerBinario(const char *nomeArquivoBinario)
{
    FILE *arquivoBinario = fopen(nomeArquivoBinario, "rb");

    if (arquivoBinario == NULL)
    {
        perror("Erro ao abrir arquivo binário");
        exit(EXIT_FAILURE);
    }

    CARGO cargo;

    while (fread(&cargo, sizeof(CARGO), 1, arquivoBinario) == 1)
    {
        printf("Nome:%s\nDescricao:%s\nID Cargo:%d\nID loc:%ld\nID crit:%ld\n\n", cargo.nome, cargo.descricao, cargo.id_cargo, cargo.id_est_loc, cargo.id_est_crit);
    }

    fclose(arquivoBinario);
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

    CARGO cargos;
    LOCALIZACAO local;
    CRITERIOS crit;
    char indice[15];
    int num;
    char buffer[MAX_NOME_LENGTH];
    char linhaConcatenada[MAX_NOME_LENGTH];
    char chave[1000]; // Variável externa para armazenar a chave
    int i;
    char proximaLinha[MAX_NOME_LENGTH];
    int flag = 0;

    // Inicializa os buffers
    buffer[0] = '\0';
    linhaConcatenada[0] = '\0';

    // Leitura linha por linha
    while (fgets(buffer, sizeof(buffer), arquivoTexto) != NULL)
    {
        removeQuebraLinha(buffer);
        sscanf(buffer, "%999[^,]", cargos.nome);
        int t = strlen(cargos.nome)+1;
        if(cargos.nome[0]=='"')
        {
            for(i = 0; buffer[i+1]!='\"'||buffer[i+2]!=','; i++)
            {
                cargos.nome[i] = buffer[i+1];
            }
            cargos.nome[i] = '\0';
            t = strlen(cargos.nome)+3;
        }

        if(buffer[t]=='\"')//quando a descrição tem aspas
        {
            char temp[MAX_NOME_LENGTH];
            for(i = 0; i < MAX_NOME_LENGTH; i++,t++)
                temp[i] = buffer[t+1];

            if(strstr(temp,"\",")!= NULL)
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(temp[i]=='\"'&&temp[i+1]==',')
                        break;
                    if (((unsigned char)temp[i] > 32 && (unsigned char)temp[i] < 127)||(unsigned char)temp[i]== 10)
                    {
                        cargos.descricao[i] = temp[i];
                    }else
                        cargos.descricao[i] = ' ';
                }
            }
            else
            {
                flag = 1;
                fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
                snprintf(linhaConcatenada, sizeof(linhaConcatenada), "%s %s", temp, proximaLinha);

                while (strstr(linhaConcatenada, "\",") == NULL && !feof(arquivoTexto))
                {
                    fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
                    size_t comprimentoConcatenado = strlen(linhaConcatenada);
                    size_t comprimentoProximaLinha = strlen(proximaLinha);

                    if (comprimentoConcatenado + comprimentoProximaLinha < sizeof(linhaConcatenada))
                    {
                        snprintf(linhaConcatenada, sizeof(linhaConcatenada), "%s %s", linhaConcatenada, proximaLinha);
                    }
                    else
                    {
                        // Lidere com a situação de buffer insuficiente
                        printf("Erro: Buffer insuficiente para concatenar linhas.\n");
                        break;
                    }
                }


                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(linhaConcatenada[i]=='\"'&&linhaConcatenada[i+1]==',')
                        break;
                    if (((unsigned char)linhaConcatenada[i] > 32 && (unsigned char)linhaConcatenada[i] < 127)||(unsigned char)linhaConcatenada[i]== 10)
                        cargos.descricao[i] = linhaConcatenada[i];
                    else
                        cargos.descricao[i] = ' ';

                }
            }
            cargos.descricao[i] = '\0';
            int j = i+2;
            if(flag==1)
            {
                strcpy(temp, linhaConcatenada);
                j = i+2;
                flag = 0;
            }

            if(temp[j]=='\"')//e a localizacao tem
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(temp[j+1]=='\"')
                        break;
                    chave[i] = temp[j+1];
                    j++;

                }
                chave[i] = '\0';
                indice[0] = temp[j+3];
                indice[1] = '\0';
                if(temp[j+4]!='\n'){
                indice[1] = temp[j+4];
                if(temp[j+5]!='\n'){
                indice[2] = temp[j+5];
                if(temp[j+6]!='\n'){
                indice[3] = temp[j+6];
                }
                else indice[3] = '\0';
                }else indice[2] = '\0';
                }else indice[1] = '\0';
                indice[4] = '\0';
            }
            else//e a localizacao nao
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(temp[j]==',')
                        break;
                    chave[i] = temp[j];
                    j++;

                }
                chave[i] = '\0';
                indice[0] = temp[j+1];
                indice[1] = '\0';
                if(temp[j+2]!='\n'){
                indice[1] = temp[j+2];
                if(temp[j+3]!='\n'){
                indice[2] = temp[j+3];
                if(temp[j+4]!='\n'){
                indice[3] = temp[j+4];
                }
                else indice[3] = '\0';
                }else indice[2] = '\0';
                }else indice[1] = '\0';
                indice[4] = '\0';
            }
        }
        else//se a descricao nao tem aspas
        {
            for(i = 0; i < MAX_NOME_LENGTH; i++,t++)
            {
                if(buffer[t]==',')
                    break;
                if (((unsigned char)buffer[t] > 32 && (unsigned char)buffer[t] < 127)||(unsigned char)buffer[t]== 10)
                    cargos.descricao[i] = buffer[t];
                else
                    cargos.descricao[i] = ' ';
            }
            cargos.descricao[i] = '\0';
            int j = t+1;
            if(buffer[j]=='\"')//e a localizacao tem
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(buffer[j+1]=='\"')
                        break;
                    chave[i] = buffer[j+1];
                    j++;

                }
                chave[i] = '\0';
                indice[0] = buffer[j+3];
                indice[1] = '\0';
                if(buffer[j+4]!='\n'){
                indice[1] = buffer[j+4];
                if(buffer[j+5]!='\n'){
                indice[2] = buffer[j+5];
                if(buffer[j+6]!='\n'){
                indice[3] = buffer[j+6];
                }
                else indice[3] = '\0';
                }else indice[2] = '\0';
                }else indice[1] = '\0';
                indice[4] = '\0';
            }
            else//e a localizacao nao
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(buffer[j]==',')
                        break;
                    chave[i] = buffer[j];
                    j++;

                }
                chave[i] = '\0';
                indice[0] = buffer[j+1];
                indice[1] = '\0';
                if(buffer[j+2]!='\n'){
                indice[1] = buffer[j+2];
                if(buffer[j+3]!='\n'){
                indice[2] = buffer[j+3];
                if(buffer[j+4]!='\n'){
                indice[3] = buffer[j+4];
                }
                else indice[3] = '\0';
                }else indice[2] = '\0';
                }else indice[3] = '\0';
                indice[4] = '\0';
            }
        }
        num = atoi(indice);
        if(num == 2139){
            cargos.id_est_crit = -1;
            num--;
        }
        if(num == 2032){
            cargos.id_est_crit = -1;
            num--;
        }
        cargos.id_est_crit = num * sizeof(CRITERIOS);
        cargos.id_cargo = k;

        //acha a chave estrangeira
        FILE *arquivo = fopen("localizacao2.bin", "rb");
        FILE *arq = fopen("criterios.bin", "rb");

        int encontrado = 0;
        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo 'localizacao.bin'\n");
            return 1; // Ou outra ação apropriada em caso de erro
        }
        rewind(arquivo);
        while (!feof(arquivo) && encontrado == 0)
            if (fread(&local, sizeof(LOCALIZACAO), 1, arquivo) == 1)
            {
                if (strcmp(local.nome, chave)==0)
                {
                    encontrado = 1;
                    fseek(arquivo, -sizeof(LOCALIZACAO), SEEK_CUR);
                    cargos.id_est_loc = ftell(arquivo);
                }
            }
            else
                cargos.id_est_loc = -1;

        fclose(arquivo);
        encontrado = 0;



        // Escreve no arquivo binário
        fwrite(&cargos, sizeof(CARGO), 1, arquivoBinario);

        // Reinicia as strings para a próxima iteração
        cargos.nome[0] = '\0';
        cargos.descricao[0] = '\0';
        chave[0] = '\0';
        k++;
    }

    fclose(arquivoTexto);
    fclose(arquivoBinario);
}

int main()
{
    const char *arquivoTexto = "cargos_new.txt";
    const char *arquivoBinario = "cargos.bin";

    // Escreve o arquivo binário a partir do arquivo de texto
    escreverBinario(arquivoTexto, arquivoBinario);

    // Lê e imprime os dados do arquivo binário
    lerBinario(arquivoBinario);

    return EXIT_SUCCESS;
}
