#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_LENGTH 10000

typedef struct
{
    long id_est_emp;
    long id_est_cargo;
    char data_criacao[100];
} OFERTA;

typedef struct
{
    char empresa[500];
    char cargo[500];
    char data[100];
} TEMP;

typedef struct
{
    char nome[1000];
    char descricao[MAX_NOME_LENGTH];
    int id_cargo;
    long id_est_loc;
} CARGO;

typedef struct
{
    char nome[1000];
    char website[1000];
    int id_emp;
    long id_est_ind;
} EMPRESA;

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

    OFERTA oft;

    while (fread(&oft, sizeof(OFERTA), 1, arquivoBinario) == 1)
    {
        printf("Cargo: %ld\nEmpresa: %ld\nData de criacao: %s\n\n", oft.id_est_cargo, oft.id_est_emp, oft.data_criacao);
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
    OFERTA oft;
    EMPRESA emp;
    TEMP temp;

    char buffer[MAX_NOME_LENGTH];
    char linhaConcatenada[MAX_NOME_LENGTH];
    char chave[1000]; // Variável externa para armazenar a chave
    int i,t;
    char proximaLinha[MAX_NOME_LENGTH];
    int flag = 0;

    // Inicializa os buffers
    buffer[0] = '\0';
    linhaConcatenada[0] = '\0';

    // Leitura linha por linha
    while (fgets(buffer, sizeof(buffer), arquivoTexto) != NULL)
    {
        removeQuebraLinha(buffer);
        sscanf(buffer, "%999[^,]", temp.cargo);
        int t = strlen(temp.cargo)+1;
        if(temp.cargo[0]=='"')
        {
            for(i = 0; buffer[i+1]!='\"'||buffer[i+2]!=','; i++)
            {
                temp.cargo[i] = buffer[i+1];
            }
            temp.cargo[i] = '\0';
            t = strlen(temp.cargo)+3;
        }

        if(buffer[t]=='\"')
        {
            char aux[MAX_NOME_LENGTH];
            for(i = 0; i < MAX_NOME_LENGTH; i++,t++)
                aux[i] = buffer[t+1];

            if(strstr(aux,"\",")!= NULL)
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(aux[i]=='\"'&&aux[i+1]==',')
                        break;
                    if (((unsigned char)aux[i] > 32 && (unsigned char)aux[i] < 127)||(unsigned char)aux[i]== 10)
                    {
                        temp.empresa[i] = aux[i];
                    }
                    else
                        temp.empresa[i] = ' ';
                }
            }
            else
            {
                flag = 1;
                fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
                snprintf(linhaConcatenada, sizeof(linhaConcatenada), "%s\n %s", aux, proximaLinha);

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
                        temp.empresa[i] = linhaConcatenada[i];
                    else
                        temp.empresa[i] = ' ';
                        //printf("%c", linhaConcatenada[i]);

                }
            }
            temp.empresa[i] = '\0';
            int j = i+2;
            if(flag==1)
            {
                strcpy(aux, linhaConcatenada);
                j = i+2;
                flag = 0;
            }

            if(aux[j]=='\"')//e a localizacao tem
            {
                fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
                fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
                snprintf(linhaConcatenada, sizeof(linhaConcatenada), "%s\n %s", aux, proximaLinha);

                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(linhaConcatenada[j+1]=='\"')
                        break;
                    temp.data[i] = linhaConcatenada[j+1];
                    j++;

                }
                temp.data[i] = '\0';
            }
            else//e a localizacao nao
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(aux[j]=='\n')
                        break;
                    temp.data[i] = aux[j];
                    j++;

                }
                temp.data[i] = '\0';

            }
        }
        else//se a descricao nao tem aspas
        {
            for(i = 0; i < MAX_NOME_LENGTH; i++,t++)
            {
                if(buffer[t]==',')
                    break;
                if (((unsigned char)buffer[t] > 32 && (unsigned char)buffer[t] < 127)||(unsigned char)buffer[t]== 10)
                    temp.empresa[i] = buffer[t];
                else
                    temp.empresa[i] = ' ';
            }
            temp.empresa[i] = '\0';
            int j = t+1;
            if(buffer[j]=='\"')//e a localizacao tem
            {
                if(strstr(buffer, "\",")==NULL)
                {
                    fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
                    fgets(proximaLinha, sizeof(proximaLinha), arquivoTexto);
                    snprintf(linhaConcatenada, sizeof(linhaConcatenada), "%s %s", buffer, proximaLinha);
                    for(i = 0; i < MAX_NOME_LENGTH; i++){
                        if(linhaConcatenada[j+1]=='\"')
                            break;
                        temp.data[i] = linhaConcatenada[j+1];
                        j++;
                    }
                }
                else
                {
                    for(i = 0; i < MAX_NOME_LENGTH; i++)
                    {
                        if(buffer[j+1]=='\"')
                            break;
                        temp.data[i] = buffer[j+1];
                        j++;

                    }
                }
                temp.data[i] = '\0';
            }
            else//e a localizacao nao
            {
                for(i = 0; i < MAX_NOME_LENGTH; i++)
                {
                    if(buffer[j]=='\n')
                        break;
                    temp.data[i] = buffer[j];
                    j++;

                }
                temp.data[i] = '\0';

            }
        }
        /*if(buffer[0] == '"')
        {

            for(i = 0; buffer[i+1]!='\"'||buffer[i+2]!=','; i++)
            {
                temp.empresa[i] = buffer[i+1];
            }
            temp.empresa[i] = '\0';
            t = strlen(temp.empresa)+3;

            if(buffer[t] == '"')
            {
                for(i = 0; buffer[t+1]!='\"'||buffer[t+2]!=','; i++,t++)
                {
                    temp.cargo[i] = buffer[t+1];
                }
                temp.cargo[i] = '\0';
                k = t +3;
            }
            else
            {
                for(i = 0; buffer[t]!=','; i++,t++)
                {
                    temp.cargo[i] = buffer[t];
                }
                temp.cargo[i] = '\0';
                k = t + 1;
            }
            for(i = 0; buffer[k]!=','; i++,k++)
            {
                temp.data[i] = buffer[k];
            }
            temp.data[i] = '\0';

        }
        else
        {
            sscanf(buffer, "%999[^,]", temp.empresa);
            t = sizeof(temp.empresa)+1;
            if(buffer[t]=='\"')
            {
                for(i = 0; buffer[t]!='\"'||buffer[t+1]!=','; i++,t++)
                {
                    temp.cargo[i] = buffer[t];
                }
                temp.cargo[i] = '\0';
            }
            else
            {
                sscanf(buffer, "%999[^,],%999[^,],%999[^,]", temp.empresa, temp.cargo, temp.data);
            }
        }
        */
          //acha a chave estrangeira
          FILE *arqemp = fopen("empresas2.bin", "rb");
          FILE *arqcarg = fopen("cargos2.bin", "rb");
          int encontrado = 0;
          if (arqemp == NULL)
          {
              printf("Erro ao abrir o arquivo\n");
              return 1; // Ou outra ação apropriada em caso de erro
          }
          rewind(arqemp);

          while (!feof(arqemp) && encontrado == 0)
              if (fread(&emp, sizeof(EMPRESA), 1, arqemp) == 1)
              {
                  //printf("%s\n%s\n\n\n", temp.empresa, emp.nome);
                  if (strcmp(temp.empresa, emp.nome) == 0)
                  {
                      encontrado = 1;
                      fseek(arqemp, -sizeof(EMPRESA), SEEK_CUR);
                      oft.id_est_emp = ftell(arqemp);
                  }
              }
              else
                  oft.id_est_emp = -1;
          fclose(arqemp);

          encontrado = 0;
           while (!feof(arqcarg) && encontrado == 0)
              if (fread(&cargos, sizeof(CARGO), 1, arqcarg) == 1)
              {

                  if (strcmp(temp.cargo, cargos.nome) == 0)
                  {
                      encontrado = 1;
                      fseek(arqcarg, -sizeof(CARGO), SEEK_CUR);
                      oft.id_est_cargo = ftell(arqcarg);
                  }
              }
              else
                  oft.id_est_cargo = -1;
          fclose(arqcarg);
        // Escreve no arquivo binário
        strcpy(oft.data_criacao, temp.data);
        fwrite(&oft, sizeof(OFERTA), 1, arquivoBinario);
        // Reinicia as strings para a próxima iteração
        temp.data[0] = '\0';
        temp.cargo[0] = '\0';
        temp.empresa[0] = '\0';
    }

    fclose(arquivoTexto);
    fclose(arquivoBinario);
}

int main()
{
    const char *arquivoTexto = "ofertass.txt";
    const char *arquivoBinario = "ofertas.bin";

    // Escreve o arquivo binário a partir do arquivo de texto
    escreverBinario(arquivoTexto, arquivoBinario);

    // Lê e imprime os dados do arquivo binário
    lerBinario(arquivoBinario);

    return 0;
}
