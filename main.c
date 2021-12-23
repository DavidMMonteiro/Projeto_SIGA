/*
    INFO
    Projeto para recriar o sistema SIGA das cantinas das escolas que fazem parte do IPL.
    Este sistema ira carregar ficheiros csv com a informacao de escolas, utilizadores e as transacoes de cada utilizador.
    Ira permitir recriar o funcionamento do sistema SIGA como os correspondentes movimentos dos utilizadores nas diferentes instituicoes

    OWNER INFO
    Class: 4515 Curso Tecnico Superior Profissional de Programacao de Sistemas de informacao
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

#define TIPO_UTILIZADOR (char*[3]) {"Estudante","Docente","Funcionario"}

//Funções Gerais

bool validacaoBinaria(char[]);
char validacaoCharacter(char [], char []);
void esperarEnter(void);
const char* getfield(char*, int);
void obterString(char[], char *);
float obterFloat(char[]);
int obterInt(char text[]);
int charParaInt(char);

//Funções específicas

void carregarFicheiro(char[]);
void importFicheiroUtilizadores(char[], Utilizador []);
void importFicheiroEscolas(char[], Escola []);
void importFicheiroTransacoes(char[], Transacao []);
void guardarDadosUtilizador(char[], Utilizador*, int);
void guardarDadosEscola(char[], Escola*, int);
void guardarDadosTransacao(char [], Transacao*, int);
void crearUtilizador(Utilizador *, int);
void escolherTipoUtilizador(char *);
void crearEscola(Escola *);
void obterMayusculas(char [], char *); 

int main()
{
    // Let print special caracters
    setlocale(LC_ALL, "Portuguese");

    // Ficheiros
    char caminhoEscolas[] = /*"files/BIN/escolas.bin";*/"files/CSV/escolas.csv";
    char caminhoUtilizadores[] = /*"files/BIN/utilizadores.bin";*/"files/CSV/utilizadores.csv";
    char caminhoMovimentos[] = /*"files/BIN/movimentos.bin";*/"files/CSV/movimentos.csv";
    // Variveis
    char resposta;
    int index, index_novo_dado;
    // Dados
    Utilizador utilizadores[200];
    Escola escolas[5];
    Transacao movimentos[1000];//This shit doesn't work if you use array[5000]

    if(validacaoBinaria("Quere carregar os ficheiros?"))
    {
        system("cls");
        carregarFicheiro(caminhoEscolas);
        carregarFicheiro(caminhoUtilizadores);
        carregarFicheiro(caminhoMovimentos);
        esperarEnter();
    }

    system("cls");
    if(validacaoBinaria("Quere importar dados dos ficheiros ao sistema?"))
    {
        system("cls");
        importFicheiroUtilizadores(caminhoUtilizadores, utilizadores);
        importFicheiroEscolas(caminhoEscolas, escolas);
        importFicheiroTransacoes(caminhoMovimentos, movimentos);
        esperarEnter();
    }

    do{
        system("cls");
        resposta = validacaoCharacter("1 - Ver dados no sistema\n2 - Inserir Dados\nX - Sair", "12X");
        switch(resposta){
            case '1':
                switch (validacaoCharacter("U - Dados Utilizadores\nE - Dados Escolas\nM - Movimentos\nX - Sair", "UEMX"))
                {
                case 'U':
                    for(index = 0;index < sizeof(utilizadores)/sizeof(utilizadores[0]);index++)
                        if(strlen(utilizadores[index].Nome) != 0) 
                            ImprimeUtilizador(utilizadores[index]);
                    esperarEnter();
                    break;
                case 'E':
                    for(index = 0;index < sizeof(escolas)/sizeof(escolas[0]);index++)
                        if(strlen(escolas[index].Nome) != 0) 
                            ImprimeEscola(escolas[index]);
                    esperarEnter();
                    break;
                case 'M':
                    for(index = 0;index < sizeof(movimentos)/sizeof(movimentos[0]);index++)
                        if(strlen(movimentos[index].Tipo) != 0) 
                            ImprimeTransacao(movimentos[index]);
                    esperarEnter();
                    break;
                default:
                    break;
                }
                break; 
            case '2':
                system("cls");
                switch(validacaoCharacter("Escolha o tipo de da dados que quere inserir.\nU - Utilizador\nE - Escola\nX - Cancelar", "UEX"))
                {
                case 'U':
                    index = 0;
                    index_novo_dado = -1;
                    while(index_novo_dado < 0 && index < sizeof(utilizadores)/sizeof(utilizadores[0]))
                    {
                        if(strlen(utilizadores[index].Nome) == 0)
                            index_novo_dado = index;
                        index++;
                    }
                    if(index_novo_dado < 0) printf("Sem espaço para utilizador!");
                    else crearUtilizador(&utilizadores[index_novo_dado], index_novo_dado);
                    break;
                case 'E':
                    index = 0;
                    index_novo_dado = -1;
                    while(index_novo_dado < 0 && index < sizeof(escolas)/sizeof(escolas[0]))
                    {
                        if(strlen(escolas[index].Nome) == 0)
                            index_novo_dado = index;
                        index++;
                    }
                    if(index_novo_dado < 0) printf("Sem espaço para nova Escola!");
                    else crearEscola(&escolas[index_novo_dado]);

                    break;
                }
                break;
            case 'X':
                break;

            default:
                system("cls");
                printf("Opção não encontrada");
                esperarEnter();
                break;
        }
    }while(resposta != 'X');
}

// Validacao para questoes de Sim ou Nao
bool validacaoBinaria(char texto[])
{
    char resposta;
    do
    {
        //system("cls");
        printf("%s", texto);
        printf("\nY - Sim\nN - Nao\n\n-->");
        fflush(stdin);
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    }
    while(resposta != 'Y' && resposta != 'N');
    return resposta == 'Y' ? true : false;
}

// Validacao de leitura de char's
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

// Obtem um valor numerico float
float obterFloat(char text[])
{
    float valor;
    system("cls");
    printf(text);
    fflush(stdin);
    scanf(" %f", &valor);
    return valor;
}

//Obter um valor numerico int
int obterInt(char text[])
{
    int valor;
    system("cls");
    printf(text);
    fflush(stdin);
    scanf(" %d", &valor);
    return valor;
}

//Conversao de numeros em char para Int
int charParaInt(char input) {
    int output = 0;
	output = input - '0';
	return output;
}

// Para o programa ate o utilizador insira enter
void esperarEnter(void)
{
    fflush(stdin);
    printf("\nEnter para continuar...");
    getchar();
}

// Obtem uma string do utilizador
void obterString(char text[], char *data)
{
    char input[255];
    do{
        system("cls");
        fflush(stdin);
        printf(text);
        fgets(input, sizeof(input), stdin);
    }while(input == "");
    input[sizeof(input)-1] = '\0';
    strcpy(data,input);
}

// Lee informacao dos ficheiros CSV que forem pasados
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
void importFicheiroUtilizadores(char filePath[], Utilizador lista_utilizadores[])
{
    //Utilizador lista_utilizadores[200];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        printf("Carregando Ficheiro %s", filePath);
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
                guardarDadosUtilizador(value, &lista_utilizadores[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeUtilizador(lista_utilizadores[index]);
        }
        // Close the file
        fclose(fileStream);
        printf("\nFinish reading file %s\n\n", filePath);

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
    //Escola lista_escolas[5];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {

        printf("Carregando Ficheiro %s", filePath);
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
            ImprimeEscola(lista_escolas[index]);
        }
        // Close the file
        fclose(fileStream);
        printf("\nFinish reading file %s\n\n", filePath);
    }
    else
    {
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Importa os dados das Transacoes para o programa
void importFicheiroTransacoes(char filePath[], Transacao lista_movimentos[])
{
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        printf("Carregando Ficheiro %s", filePath);
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
                guardarDadosTransacao(value, &lista_movimentos[index],column);
                value = strtok(NULL, ";");
                column++;
            }
            ImprimeTransacao(lista_movimentos[index]);
        }
        // Close the file
        fclose(fileStream);
        printf("\nFinish reading file %s\n\n", filePath);
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

/* Devolve a informacao de uma string no campo especificado
const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
        {
        if (!--num)
            return tok;
    }
    return NULL;
}*/

// Cria um utilizador
void crearUtilizador(Utilizador *utilizador_actual, int id_novo_utilizador)
{
    Utilizador novo_utilizador = *utilizador_actual;
    system("cls");
    do
    {
        novo_utilizador.ID = id_novo_utilizador;
        novo_utilizador.ID_Escola = charParaInt(validacaoCharacter("Escolha a sua escola\n2 - Escola Superior de Tecnologia e Gestão(ESTG)\n3 - Escola Superior de Turismo e Tecnologia do Mar(ESTM)", "23"));
        obterString("Insira o seu nome: ", novo_utilizador.Nome);
        novo_utilizador.NIF = obterInt("Insira o seu NIF:");
        escolherTipoUtilizador(&novo_utilizador.Tipo);
        obterString("Insira o seu Email: ", novo_utilizador.Email);
        novo_utilizador.Valor_Conta = obterFloat("Insira o seu valor inicial:");
        system("cls");
        printf("Dados inseridos:");
        ImprimeUtilizador(novo_utilizador);
        esperarEnter();
    }while(!validacaoBinaria("\nTem a sarteca que quere inserir este dados?"));
    *utilizador_actual = novo_utilizador;
}

// Pede ao utilizador insira un tipo de utilizador
void escolherTipoUtilizador(char *inserir_tipo)
{
    char texto[] = "Escolha o tipo de utilizador\n1 - Estudante\n2 - Docente\n3 - Funcionario", valoresValidos[] = "123";
    strcpy(inserir_tipo,TIPO_UTILIZADOR[validacaoCharacter(texto, valoresValidos) - '0' - 1]);
}

// Cria uma nova escola
void crearEscola(Escola *escola_actual) 
{
    Escola nova_escola = *escola_actual;
    system("cls");
    do
    {
        nova_escola.ID = obterInt("Insira Id da nova escola: ");
        obterString("Insira o nome da nova escola: ", &nova_escola.Nome);
        obterMayusculas(nova_escola.Nome, nova_escola.Abreviatura);
        obterString("Insira o Campus da nova escola: ", &nova_escola.Campus);
        obterString("Insira a Localidade da nova escola: ", &nova_escola.Localidade);
        system("cls");
        printf("Dados inseridos:");
        ImprimeEscola(nova_escola);
        esperarEnter();
    }while(!validacaoBinaria("\nTem a sarteca que quere inserir este dados?"));
    *escola_actual = nova_escola;
}

// Obtem todas as letras mayusculas da string inserida
void obterMayusculas(char nome[], char *abreviatura_escola) 
{
    char abrevitura[10] = "";
    for(int index = 0;index < strlen(nome) ; index++) 
        if ('A' <= nome[index] && nome[index] <= 'Z')
            strncat(abrevitura, &nome[index], 1);
    strcpy(abreviatura_escola, abrevitura);
}

