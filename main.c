/*
    INFO
    Projeto para recriar o sistema SIGA das cantinas das escolas que fazem parte do IPL.
    Este sistema irá carregar ficheiros csv com a informação de escolas, utilizadores e as transações de cada utilizador.
    Irá permitir recriar o funcionamento do sistema SIGA como os correspondentes movimentos dos utilizadores nas diferentes instituições

    OWNER INFO
    Class: 4515 Curso Técnico Superior Profissional de Programação de Sistemas de informação
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


typedef struct //Estrutura Utilizador
{
    int ID;
    int ID_Escola;
    char *Nome;
    int NIF;
    char *Tipo;
    char *Email;
    float Valor_Conta;
} Utilizador;

char *ImprimeUtilizador(Utilizador util, bool conseguirString)
{
  char informacao[1024];
  snprintf(informacao, sizeof(informacao), "Id.%d Id Escola.%d Nome: %s NIF:%d Tipo Utilizador:%s Email:%s Valor na Conta: %f",
         util.ID,util.ID_Escola,util.Nome,util.NIF,util.Tipo,util.Email,util.Valor_Conta);
  if(!conseguirString)printf(informacao);
  else return informacao;
}

typedef struct //Estrutura Escola
{
    int ID;
    char *Nome;
    char *Abreviatura;
    char *Campus;
    char *Localidade;
} Escola;

void ImprimeEscola(Escola escola)
{
  printf("Id.%d Nome:%s Abreviatura:%s Campus:%s Localidade:%s",
         escola.ID,escola.Nome,escola.Abreviatura,escola.Campus,escola.Localidade);
}

typedef struct //Estrutura Transação
{
    int ID;
    char ID_Utilizador;
    char *Tipo;
    float Valor;
    char *Data;
    char *Hora;
} Transacao;

void ImprimeTransacao(Transacao transacao)
{
  printf("Id.%d Id Util.%d Tipo:%s Valor:%f Data:%s Hora:%s",
         transacao.ID,transacao.ID_Utilizador,transacao.Tipo,transacao.Valor,transacao.Data,transacao.Hora);
}

bool validacaoBinaria(char[]);
char validacaoCharacter(char *, char *);
void esperarEnter(void);
const char* getfield(char*, int);
void carregarCSV(char*);
void importCSVUtilizadores(char*);
void guardarDadosUtilizador(char*, Utilizador*, int);
void importCSVEscolas(char*);
void guardarDadosEscola(char*, Escola*, int);
void importCSVTransacoes(char*);
void guardarDadosTransacao(char*, Transacao*, int);
Utilizador crearUtilizador(void);
const char* obterString(char*);
float obterNumero(char*);
int charParaInt(char);
const char* escolherTipoUtilizador(void);

int main()
{
    // Let print special caracters
    setlocale(LC_ALL, "Portuguese");

    // Variaveis
    char resposta;
    char caminhoEscolas[] = "CSV/escolas.csv";
    char caminhoUtilizadores[] = "CSV/utilizadores.csv";
    char caminhoMovimentos[] = "CSV/movimentos.csv";

    do{
        system("cls");
        resposta = validacaoCharacter("1 - Carregar Ficheiros CSV\n2 - Importar CSV Files\n3 - Inserir Dados\nX - Sair", "123X");
        switch(resposta){
            case '1':
                system("cls");
                printf("Carregando Ficheiro %s\n\n", caminhoEscolas);
                carregarCSV(caminhoEscolas);
                printf("Finish reading file %s\n\n\nCarregando Ficheiro %s\n\n",caminhoEscolas,caminhoUtilizadores);
                carregarCSV(caminhoUtilizadores);
                printf("Finish reading file %s\n\n\nCarregando Ficheiro %s\n\n",caminhoUtilizadores,caminhoMovimentos);
                carregarCSV(caminhoMovimentos);
                printf("Finish reading file %s\n\n\n",caminhoMovimentos);
                esperarEnter();
                break;
            case '2':
                system("cls");
                printf("Carregando Ficheiro %s\n\n", caminhoUtilizadores);
                importCSVUtilizadores(caminhoUtilizadores);
                printf("Finish reading file %s\n\n", caminhoUtilizadores);
                printf("Carregando Ficheiro %s\n\n", caminhoEscolas);
                importCSVEscolas(caminhoEscolas);
                printf("Finish reading file %s\n\n",caminhoEscolas);
                printf("Carregando Ficheiro %s\n\n",caminhoMovimentos);
                importCSVTransacoes(caminhoMovimentos);
                printf("Finish reading file %s\n\n\n",caminhoMovimentos);
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

// Validação para questões de Sim ou Não
bool validacaoBinaria(char texto[])
{
    char resposta;
    do
    {
        //system("cls");
        printf("%s", texto);
        printf("\nY - Sim\nN - Não\n\n-->");
        fflush(stdin);
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
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Importa os dados dos Utilizadores para o programa
void importCSVUtilizadores(char* filePath)
{
    Utilizador utilizador[200];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
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
                //TODO Check why it brakes with email
                printf("%s\t", value);
                guardarDadosUtilizador(value, &utilizador[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeUtilizador(utilizador[index],false);
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

// Importa os dados das Escolas para o programa
void importCSVEscolas(char* filePath)
{
    Escola escolas[5];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
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
                printf("%s\t", value);
                guardarDadosEscola(value, &escolas[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeEscola(escolas[index]);
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

// Importa os dados das Transações para o programa
void importCSVTransacoes(char* filePath)
{
    Transacao transacoes[5000];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
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
                printf("%s\t", value);
                guardarDadosTransacao(value, &transacoes[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeTransacao(transacoes[index]);
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

// Insire os dados do novo utilizador
void guardarDadosUtilizador(char* novo_dado, Utilizador *dados_antigos, int coluna)
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
void guardarDadosEscola(char* novo_dado, Escola *dados_antigos, int coluna)
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
void guardarDadosTransacao(char* novo_dado, Transacao *dados_antigos, int coluna)
{
    switch(coluna){
        case 0:
            //Save ID Transação
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
    char texto_verificacao[1024];
    system("cls");
    do
    {
        novo_utilizador.ID_Escola = charParaInt(validacaoCharacter("Escolha a sua escola\n2 - Escola Superior de Tecnologia e Gestão(ESTG)\n3 - Escola Superior de Turismo e Tecnologia do Mar(ESTM)", "23"));
        strcpy(novo_utilizador.Nome,obterString("Insira o seu nome:"));//Check how to storage strings in struct
        printf("Show Name: %s",novo_utilizador.Nome);
        esperarEnter();
        novo_utilizador.NIF = (int) obterNumero("Insira o seu NIF:");
        novo_utilizador.Tipo = escolherTipoUtilizador();
        novo_utilizador.Email = obterString("Insira o seu Email:");
        novo_utilizador.Valor_Conta = obterNumero("Insira o seu valor inicial:");
        printf(ImprimeUtilizador(novo_utilizador,true));
        system("cls");
        //TODO Check why ImprimeUtilizador retorna null
        snprintf(texto_verificacao,sizeof(texto_verificacao),"Tem a sarteça que quere inserir este dados?\n%s\n",ImprimeUtilizador(novo_utilizador,true));
    }while(!validacaoBinaria(texto_verificacao));
}

// Obtem uma string do utilizador
const char * obterString(char* text)
{
    char data[255];
    system("cls");
    printf(text);
    fflush(stdin);
    fgets(data, sizeof(data), stdin);
    return data;
}

float obterNumero(char* text)
{
    float valor;
    system("cls");
    printf(text);
    scanf(" %f", &valor);
    return valor;
}

//Conversão de números em char para Int
int charParaInt(char input) {
    int output = 0;
	output = input - '0';
	return output;
}

// Pete ao utilizador insira un tipo de utilizador
const char* escolherTipoUtilizador(void)
{
    char *texto = "Escolha o tipo de utilizador\n1 - Estudante\n2 - Docente\n3 - Funcionária", *valoresValidos = "123";

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

