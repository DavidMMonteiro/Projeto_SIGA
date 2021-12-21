/*
    INFO
    Projeto para recriar o sistema SIGA das cantinas das escolas que fazem parte do IPL.
    Este sistema ir� carregar ficheiros csv com a informa��o de escolas, utilizadores e as transa��es de cada utilizador.
    Ir� permitir recriar o funcionamento do sistema SIGA como os correspondentes movimentos dos utilizadores nas diferentes institui��es

    OWNER INFO
    Class: 4515 Curso T�cnico Superior Profissional de Programa��o de Sistemas de informa��o
    Student(s) number: 2211849
    Creator(s): David Machado Monteiro
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include <errno.h>
#include "utilizador.c"
#include "escola.c"
#include "transacao.c"

#define TIPO_UTILIZADOR (char[3][255]) {"Estudante","Docente","Funcion�rio"}
#define MAX_UTILIZADORES 200
#define MAX_ESCOLAS 5
#define MAX_MOVIMENTOS 5000


bool validacaoBinaria(char[]);
char validacaoCharacter(char [], char []);
void esperarEnter(void);
const char* getfield(char*, int);
void carregarFicheiro(char[]);
void importFicheiroUtilizadores(char [], Utilizador[]);
void guardarDadosUtilizador(char[], Utilizador*, int);
void importFicheiroEscolas(char [], Escola []);
void guardarDadosEscola(char[], Escola*, int);
void importFicheiroTransacoes(char[], Transacao []);
void guardarDadosTransacao(char [], Transacao*, int);
void crearUtilizador(void);
const char* obterString(char[]);
float obterNumero(char[]);
int charParaInt(char);
const char* escolherTipoUtilizador(void);

int main()
{
    // Let print special caracters
    setlocale(LC_ALL, "Portuguese");

    // Variaveis
    char resposta;
    char caminhoEscolas[] = /*"files/BIN/escolas.bin";*/"files/CSV/escolas.csv";
    char caminhoUtilizadores[] = /*"files/BIN/utilizadores.bin";*/"files/CSV/utilizadores.csv";
    char caminhoMovimentos[] = /*"files/BIN/movimentos.bin";*/"files/CSV/movimentos.csv";

    //Dados
    Utilizador utilizador[MAX_UTILIZADORES];
    Escola escolas[MAX_ESCOLAS];
    Transacao transacoes[MAX_MOVIMENTOS];

    do{
        system("cls");
        resposta = validacaoCharacter("1 - Carregar Ficheiros CSV\n2 - Importar CSV Files\n3 - Inserir Dados\nX - Sair", "123X");
        switch(resposta){
            case '1':
                system("cls");
                carregarFicheiro(caminhoEscolas);
                carregarFicheiro(caminhoUtilizadores);
                carregarFicheiro(caminhoMovimentos);
                esperarEnter();
                break;
            case '2':
                system("cls");
                importFicheiroUtilizadores(caminhoUtilizadores, utilizador);
                importFicheiroEscolas(caminhoEscolas, escolas);
                ImprimeUtilizador(utilizador[0],false);
                //TODO Check why it doesn't get inside function to read moviments
                importFicheiroTransacoes(caminhoMovimentos, transacoes);
                esperarEnter();
                break;
            case '3':
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
                break;
        }
    }while(resposta != 'X');
}

// Valida��o para quest�es de Sim ou N�o

bool validacaoBinaria(char texto[])
{
    char resposta;
    do
    {
        //system("cls");
        printf("%s", texto);
        printf("\nY - Sim\nN - N�o\n\n-->");
        fflush(stdin);
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    }
    while(resposta != 'Y' && resposta != 'N');
    return resposta == 'Y' ? true : false;
}

// Valida��o de leitura de char's
char validacaoCharacter(char texto[], char valores_validos[])
{
    char resposta;
    do
    {
        system("cls");
        printf(texto);
        printf("\n-->");
        fflush(stdin);
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

// Lee informa��o dos ficheiros CSV que forem pasados
void carregarFicheiro(char filePath[])
{
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        printf("\nCarregando Ficheiro %s\n\n", filePath);

        char buffer[1024];
        int row = 0, column = 0;

        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            column = 0;
            row++;

            //Optional to ignore the header tables
            if (row == 1) continue;
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
        printf("Finish reading file %s\n\n", filePath);
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Importa os dados dos Utilizadores para o programa
void importFicheiroUtilizadores(char filePath[], Utilizador lista_utilizador[])
{
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        printf("Carregando Ficheiro %s\n\n", filePath);
        char buffer[1024];
        int row = 0, column = 0;

        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            column = 0;
            row++;

            //Optional to ignore the header tables
            if (row == 1) continue;
            int index = row - 2;
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                guardarDadosUtilizador(value, &lista_utilizador[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeUtilizador(lista_utilizador[index],false);
            printf("\n\n");
        }
        // Close the file
        fclose(fileStream);
        printf("Finish reading file %s\n\n", filePath);
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Importa os dados das Escolas para o programa
void importFicheiroEscolas(char filePath[], Escola lista_escolas[])
{
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {

        printf("Carregando Ficheiro %s\n\n", filePath);
        char buffer[1024];
        int row = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            int column = 0;
            row++;
            //Optional to ignore the header tables
            if (row == 1) continue;
            int index = row - 2;
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                //Check with it brakes reading Abreviatura
                //printf("%s\t", value);
                guardarDadosEscola(value, &lista_escolas[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeEscola(lista_escolas[index], false);
            printf("\n");
        }
        // Close the file
        fclose(fileStream);
        printf("Finish reading file %s\n\n", filePath);
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Importa os dados das Transa��es para o programa
void importFicheiroTransacoes(char filePath[], Transacao lista_transacoes[])
{
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        printf("Carregando Ficheiro %s\n\n",filePath);
        esperarEnter();
        char buffer[1024];
        int row = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            printf("Reading line %d",row);
            int column = 0;
            row++;
            //Optional to ignore the header tables
            if (row == 1) continue;
            int index = row - 2;
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                printf("%s\t", value);
                guardarDadosTransacao(value, &lista_transacoes[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeTransacao(lista_transacoes[index], false);
            printf("\n\n");
        }
        // Close the file
        fclose(fileStream);
        printf("Finish reading file %s\n\n\n", filePath);
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Insire os dados do novo utilizador
void guardarDadosUtilizador(char novo_dado[], Utilizador *dados_antigos, int coluna)
{
    switch(coluna){
        case 0: //Save ID Utilizador
            dados_antigos->ID = atoi(novo_dado);
            break;
        case 1: //Save ID Escola
            dados_antigos->ID_Escola = atoi(novo_dado);
            break;
        case 2: //Save Nome Utilizador
            strcpy(dados_antigos->Nome,novo_dado);
            break;
        case 3: //Save NIF
            dados_antigos->NIF = atoi(novo_dado);
            break;
        case 4: //Save Tipo de Utilizador
            strcpy(dados_antigos->Tipo,novo_dado);
            break;
        case 5: //Save Email
            strcpy(dados_antigos->Email,novo_dado);
            break;
        case 6: //Save Valor na conta
            dados_antigos->Valor_Conta = strtof(novo_dado,NULL);
            break;
    }
}

// Insire os dados da nova escola
void guardarDadosEscola(char novo_dado[], Escola *dados_antigos, int coluna)
{
    switch(coluna){
        case 0: //Save Id
            dados_antigos->ID = atoi(novo_dado);
            break;
        case 1: //Save Nome Escola
            strcpy(dados_antigos->Nome,novo_dado);
            break;
        case 2: //Save Abreviatura Escola
            strcpy(dados_antigos->Abreviatura,novo_dado);
            break;
        case 3: //Save Campus
            strcpy(dados_antigos->Campus,novo_dado);
            break;
        case 4: //Save Localidade
            strcpy(dados_antigos->Localidade,novo_dado);
            break;
    }
}

// Insire os dados da nova escola
void guardarDadosTransacao(char novo_dado[], Transacao *dados_antigos, int coluna)
{
    switch(coluna){
        case 0:
            //Save ID Transa��o
            dados_antigos->ID = atoi(novo_dado);
            break;
        case 1: //Save ID Utilizador
            dados_antigos->ID_Utilizador = atoi(novo_dado);
            break;
        case 2: //Save Tipo de Movimento
            strcpy(dados_antigos->Tipo,novo_dado);
            break;
        case 3: //Save o valor do movimento
            dados_antigos->Valor = strtof(novo_dado,NULL);
            break;
        case 4: //Save Data do movimento
            strcpy(dados_antigos->Data,novo_dado);
            break;
        case 5: //Save Hora do movimento
            strcpy(dados_antigos->Hora,novo_dado);
            break;
    }
}

// Devolve a informa��o de uma string no campo especificado
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
void crearUtilizador(void)
{
    //TODO
    Utilizador novo_utilizador;
    char texto_verificacao[1024];
    system("cls");
    do
    {
        novo_utilizador.ID_Escola = charParaInt(validacaoCharacter("Escolha a sua escola\n2 - Escola Superior de Tecnologia e Gest�o(ESTG)\n3 - Escola Superior de Turismo e Tecnologia do Mar(ESTM)", "23"));
        //TODO Check why can't copy string
        strcpy(novo_utilizador.Nome,obterString("Insira o seu nome:"));
        printf("Show Name: %s",novo_utilizador.Nome);
        esperarEnter();
        novo_utilizador.NIF = (int) obterNumero("Insira o seu NIF:");
        strcpy(novo_utilizador.Tipo, escolherTipoUtilizador());
        strcpy(novo_utilizador.Email, obterString("Insira o seu Email:"));
        novo_utilizador.Valor_Conta = obterNumero("Insira o seu valor inicial:");
        //printf(ImprimeUtilizador(novo_utilizador,true));
        system("cls");
        //TODO Check why ImprimeUtilizador retorna null
        //snprintf(texto_verificacao,sizeof(texto_verificacao),"Tem a sarte�a que quere inserir este dados?\n%s\n",ImprimeUtilizador(novo_utilizador,true));
    }while(!validacaoBinaria(texto_verificacao));
}

// Obtem uma string do utilizador
const char* obterString(char text[])
{
    char data[255];
    system("cls");
    printf(text);
    fflush(stdin);
    fgets(data, sizeof(data), stdin);
    return &data;
}

// Obtem ym valor num�rico
float obterNumero(char text[])
{
    float valor;
    system("cls");
    printf(text);
    scanf(" %f", &valor);
    return valor;
}

//Convers�o de n�meros em char para Int
int charParaInt(char input) {
    int output = 0;
	output = input - '0';
	return output;
}

// Pete ao utilizador insira un tipo de utilizador
const char *escolherTipoUtilizador(void)
{
    char texto[] = "Escolha o tipo de utilizador\n1 - Estudante\n2 - Docente\n3 - Funcion�ria", valoresValidos[] = "123";

    // TODO Check how to make it dynamic
    /*system("cls");
    for(int index = 0; index < sizeof(TIPO_UTILIZADOR)/sizeof(TIPO_UTILIZADOR[0]); index++)
    {
        texto = texto + "\n" + index + 1 + " - " + TIPO_UTILIZADOR[index];
        strcat(valoresValidos, index + 1);
    }
    strcat(texto,printf("\n\n-->"));*/
    return TIPO_UTILIZADOR[validacaoCharacter(texto, valoresValidos) - '0' - 1];
}

