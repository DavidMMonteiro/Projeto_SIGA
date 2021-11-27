/*
    INFO
    Projeto para recriar o sistema SIGA das cantinas das escolas que fazem parte do IPL.
    Este sistema irï¿½ carregar ficheiros csv com a informaï¿½ï¿½o de escolas, utilizadores e as transaï¿½ï¿½es de cada utilizador.
    Irï¿½ permitir recriar o funcionamento do sistema SIGA como os correspondentes movimentos dos utilizadores nas diferentes instituiï¿½ï¿½es

    OWNER INFO
    Class: 4515 Curso Tï¿½cnico Superior Profissional de Programaï¿½ï¿½o de Sistemas de informaï¿½ï¿½o
    Student(s) number: 2211849
    Creator(s): David Machado Monteiro
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <errno.h>

#define TIPO_UTILIZADOR (char*[3]) {"Estudante","Docente","Funcionário"}


typedef struct
{
    int ID;
    int ID_Escola;
    char *Nome;
    int NIF;
    char *Tipo;
    char *Email;
    float Valor_Conta;
} Utilizador;

Utilizador instanciarUtilizador(void)
{
    Utilizador util;
    util.ID = 0;
    util.ID_Escola = 0;
    util.Nome = "";
    util.NIF = 0;
    util.Tipo = "";
    util.Email = "";
    util.Valor_Conta = 0.0;
}

void ImprimeUtilizador(Utilizador util)
{
  printf("Id.%d Id Escola.%d Nome: %s NIF:%d Tipo Utilizador:%s Email:%s Valor na Conta: %f",
         util.ID,util.ID_Escola,util.Nome,util.NIF,util.Tipo,util.Email,util.Valor_Conta);
}

typedef struct
{
    int ID;
    char *Nome;
    char *Abreviatura;
    char *Campus;
    char *Localidade;
} Escola;
typedef struct
{
    int ID;
    char ID_Utilizador;
    char *Tipo;
    float valor;
    char *Data;
    char *Hora;
} Transacao;

bool validacaoBinaria(char*);
char validacaoCharacter(char *, char *);
void esperarEnter(void);
void carregarCSV(char*);
Utilizador *carregarCSVUtilizadores(char*);
const char* getfield(char*, int);
Utilizador crearUtilizador(void);
char *obterString(char*);
char *escolherTipoUtilizador(void);

int main()
{
    // Let print special caracters
    setlocale(LC_ALL, "Portuguese");

    // Variaveis
    char resposta;
    char caminhoEscolas[] = "CSV/escolas.csv";
    char caminhoUtilizadores[] = "CSV/utilizadores.csv";
    char caminhoMovimentos[] = "CSV/movimentos.csv";

    // Ask the user load CSV files
    if(validacaoBinaria("Quere ver os dados apartir de ficheiros csv?"))
    {
        system("cls");
        printf("Carregando Ficheiros CSV\n\n");
        printf("Carregando Ficheiro %s\n\n", caminhoEscolas);
        carregarCSV(caminhoEscolas);
        printf("Finish reading file %s\n\n\nCarregando Ficheiro %s\n\n",caminhoEscolas,caminhoUtilizadores);
        carregarCSV(caminhoUtilizadores);
        printf("Finish reading file %s\n\n\nCarregando Ficheiro %s\n\n",caminhoUtilizadores,caminhoMovimentos);
        carregarCSV(caminhoMovimentos);
        printf("Finish reading file %s\n\n\n",caminhoMovimentos);
        esperarEnter();
    }

    if(validacaoBinaria("Quere importar os dados apartir de ficheiros csv?"))
    {
        system("cls");
        printf("Carregando Ficheiros CSV\n\n");
        printf("Carregando Ficheiro %s\n\n", caminhoUtilizadores);
        carregarCSVUtilizadores(caminhoUtilizadores);
        printf("Finish reading file %s", caminhoUtilizadores);
        /*printf("Finish reading file %s\n\n\nCarregando Ficheiro %s\n\n",caminhoEscolas,caminhoUtilizadores);
        carregarCSV(caminhoUtilizadores);
        printf("Finish reading file %s\n\n\nCarregando Ficheiro %s\n\n",caminhoUtilizadores,caminhoMovimentos);
        carregarCSV(caminhoMovimentos);
        printf("Finish reading file %s\n\n\n",caminhoMovimentos);*/
        esperarEnter();
    }

    if(validacaoBinaria("Quere inserir dados manualmente?"))
    {
        system("cls");
        resposta = validacaoCharacter("Escolha o tipo de da dados que quere inserir.\nU - Utilizador\nE - Escola\nX - Cancelar", "UEX");
        switch(resposta)
        {
        case 'U':
            crearUtilizador();
            break;
        case 'E':
            //TODO Crear Escola
            break;
        }
    }
}

// Validação para questões de Sim ou Não
bool validacaoBinaria(char *texto)
{
    char resposta;
    do
    {
        //system("cls");
        printf("%s", texto);
        printf("\nY - Sim\nN - Não\n\n-->");
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    }
    while(resposta != 'Y' && resposta != 'N');
    return resposta == 'Y' ? true : false;
}

// Validação de leitura de char's
char validacaoCharacter(char *texto, char *valores_validos)
{
    char resposta;
    do
    {
        system("cls");
        printf(texto);
        printf("\n-->");
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    }
    while(!strchr(valores_validos, resposta));
    return resposta;
}

// Para o programa ate o utilizador insira enter
void esperarEnter(void)
{
    fflush(stdin);
    printf("Enter para continuar...");
    getchar();
    system("cls");
}

// Lee informação dos ficheiros CSV que forem pasados
void carregarCSV(char* filePath)
{
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        char buffer[1024];
        int row = 0, column = 0;

        while (fgets(buffer, 1024, fileStream))
        {
            column = 0;
            row++;

            //Optional to ignore the header tables
            //if (row == 1) continue;
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                printf("%s\t", value);
                value = strtok(NULL, ";");
                column++;
            }
            printf("\n");
        }
        // Close the file
        fclose(fileStream);
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

Utilizador *carregarCSVUtilizadores(char* filePath)
{
    Utilizador utilizadores[200];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        char buffer[1024];
        int row = 0;

        while (fgets(buffer, 1024, fileStream))
        {
            int column = 0;
            row++;
            //Optional to ignore the header tables
            if (row == 1) continue;
            int index = row - 2;
            utilizadores[index] = instanciarUtilizador();
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                utilizadores[index] = guardarDadosUtilizador(value, utilizadores[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeUtilizador(utilizadores[index]);
            printf("\n");
        }
        // Close the file
        fclose(fileStream);
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Insire os dados do novo utilizador no correspondente variavel
Utilizador guardarDadosUtilizador(char* novo_dado, Utilizador dados_antigos, int coluna)
{
    switch(coluna){
        case 0:
            dados_antigos.ID = (int) novo_dado;
            break;
        case 1:
            dados_antigos.ID_Escola = (int) novo_dado;
            break;
        case 2:
            strcpy(dados_antigos.Nome,novo_dado);
            break;
        case 3:
            dados_antigos.NIF = (int) novo_dado;
            break;
        case 4:
            strcpy(dados_antigos.Tipo,novo_dado);
            break;
        case 5:
            strcpy(dados_antigos.Email,novo_dado);
            break;
        case 6:
            dados_antigos.Valor_Conta = (float) novo_dado;
            break;
    }
    return dados_antigos;
}

// Devolve a informação de uma string no campo especificado
const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
        {
        if (!--num)
            return tok;
    }
    return NULL;
}

// Cria um utilizador
Utilizador crearUtilizador(void)
{
    //TODO
    Utilizador novo_utilizador;
    system("cls");
    do
    {
        novo_utilizador.ID_Escola = validacaoCharacter("Escolha a sua escola\n1 - \n2 - Escola Superior de Tecnologia e Gestão(ESTG)\n3 - Escola Superior de Turismo e Tecnologia do Mar(ESTM)", "23");
        novo_utilizador.Nome = obterString("Insira o seu nome:");
        novo_utilizador.NIF= obterString("Insira o seu NIF:");
        novo_utilizador.Tipo = escolherTipoUtilizador();
        novo_utilizador.Email = obterString("Insira o seu Email:");
        novo_utilizador.Valor_Conta = (float) obterString("Insira o seu valor inicial:");
    }while(!validacaoBinaria("Tem a sarteça que quere inserir este dados?\n" + ImprimeUtilizador(novo_utilizador)));
}

// Obtem uma string do utilizador
char *obterString(char* text)
{
    char *nome;
    printf(text);
    scanf("%s", &nome);
    return nome;
}

// Pete ao utilizador insira un tipo de utilizador
char *escolherTipoUtilizador(void)
{
    char input;
    char texto[1024] = "Escolha o tipo de utilizador\n1 - Estudante\n2 - Docente\n3 - Funcionária\n\n-->", *valoresValidos = "123";

    // TODO Check how to make it dynamic
    /*system("cls");
    for(int index = 0; index < sizeof(TIPO_UTILIZADOR)/sizeof(TIPO_UTILIZADOR[0]); index++)
    {
        texto = texto + "\n" + index + 1 + " - " + TIPO_UTILIZADOR[index];
        strcat(valoresValidos, index + 1);
    }
    strcat(texto,printf("\n\n-->"));*/
    return TIPO_UTILIZADOR[(int) validacaoCharacter(texto, valoresValidos)];
}

//TODO guardarDadosCSV()
