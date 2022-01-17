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
#define MAX_UTILIZADORES 200
#define MAX_ESCOLAS 5
#define MAX_MOVIMENTOS 1000 //Array doesn't work with array[5000]

//Funções Gerais

bool validacaoBinaria(char[]);
char validacaoCharacter(char [], char []);
void esperarEnter(void);
const char* getfield(char*, int);
void obterString(char[], char *);
float obterFloat(char[]);
int obterInt(char text[]);
int charParaInt(char);
void obterMayusculas(char [], char *);

//Funções específicas

//void carregarFicheiro(char[]);
void importFicheiroCSVUtilizadores(char[], Utilizador [], int *);
void importFicheiroCSVEscolas(char[], Escola [], int *);
void importFicheiroCSVTransacoes(char[], Transacao [], int *);
void guardarDadosUtilizador(char[], Utilizador*, int);
void guardarDadosEscola(char[], Escola*, int);
void guardarDadosTransacao(char [], Transacao*, int);
void crearUtilizador(Utilizador *, int);
void escolherTipoUtilizador(char *);
void crearEscola(Escola *);

int main()
{
    // Let print special caracters
    setlocale(LC_ALL, "Portuguese");

    // Ficheiros
    char caminhoCSVEscolas[] = /*"files/BIN/escolas.bin";*/"files/CSV/escolas.csv";
    char caminhoCSVUtilizadores[] = /*"files/BIN/utilizadores.bin";*/"files/CSV/utilizadores.csv";
    char caminhoCSVMovimentos[] = /*"files/BIN/movimentos.bin";*/"files/CSV/movimentos.csv";

    char caminhoBinEscolas[] = "files/BIN/escolas.bin";
    char caminhoBinUtilizadores[] = "files/BIN/utilizadores.bin";
    char caminhoBinMovimentos[] = "files/BIN/movimentos.bin";

    // Variveis
    char resposta;
    int index, index_novo_dado;
    int counter_utilizadores = 0, counter_escolas = 0, counter_movimentos = 0;
    // Dados
    Utilizador utilizadores[MAX_UTILIZADORES];
    Escola escolas[MAX_ESCOLAS];
    Transacao movimentos[MAX_MOVIMENTOS];

    system("cls");
    if(validacaoBinaria("Quere importar dados dos ficheiros ao sistema?"))
    {
        system("cls");
        importFicheiroBINUtilizadores(caminhoBinUtilizadores, utilizadores, &counter_utilizadores);
        //importFicheiroCSVUtilizadores(caminhoCSVUtilizadores, utilizadores, &counter_utilizadores);
        importFicheiroCSVEscolas(caminhoCSVEscolas, escolas, &counter_escolas);
        importFicheiroCSVTransacoes(caminhoCSVMovimentos, movimentos, &counter_movimentos);
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

    //Depuração de String
    //Verificar se existem \n na string e sustitui
    for(int index = 0;index < sizeof(input); index++)
        if(input[index] == '\n')
            input[index] = '\0';

    //Copia a string inserida para a variavel pasada a função
    strcpy(data,input);
}

// Importa os dados dos Utilizadores para o programa apartir de ficheiros CSV
void importFicheiroCSVUtilizadores(char filePath[], Utilizador lista_utilizadores[], int *contador_utilizadores)
{
    //Utilizador lista_utilizadores[200];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        //printf("Carregando Ficheiro %s", filePath);
        char buffer[1024];
        int linha = 0, coluna = 0, index = 0;
        contador_utilizadores = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            coluna = 0;
            linha++;

            index = linha - 1;
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                guardarDadosUtilizador(value, &lista_utilizadores[index],coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
            //ImprimeUtilizador(lista_utilizadores[index]);
        }
        *contador_utilizadores = index;
        // Close the file
        fclose(fileStream);
        printf("\nFinish reading file %s\n", filePath);

    }
    else
    {
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
}

// Importa os dados dos Utilizadores para o programa apartir de ficheiros Bin
void importFicheiroBINUtilizadores(char filePath[], Utilizador lista_utilizadores[], int *contador_utilizadores)
{
    FILE* ficheiro = fopen(filePath, "rb");
    if (ficheiro)
    {
        // Leer ficheiros Metodo 3
        fseek(ficheiro,0L, SEEK_END);
        *contador_utilizadores = ftell(ficheiro)/sizeof(Utilizador);
        fseek(ficheiro,0L, SEEK_SET);
        fread(lista_utilizadores, sizeof(Utilizador), *contador_utilizadores, ficheiro);

        printf("\nDados carregados\n");
    } else {
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// TODO Exporta os dados dos Utilizadores para o programa apartir de ficheiros Bin
void exportFicheiroBINUtilizadores(char filePath[], Utilizador lista_utilizadores[], int *contador_utilizadores)
{
    FILE* ficheiro = fopen(filePath, "rb");
    if (ficheiro)
    {
        // Leer ficheiros Metodo 3
        fseek(ficheiro,0L, SEEK_END);
        *contador_utilizadores = ftell(ficheiro)/sizeof(Utilizador);
        fseek(ficheiro,0L, SEEK_SET);
        fread(lista_utilizadores, sizeof(Utilizador), *contador_utilizadores, ficheiro);

        printf("\nDados carregados\n");
    } else {
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
    esperarEnter();
}

// Importa os dados das Escolas para o programa
void importFicheiroCSVEscolas(char filePath[], Escola lista_escolas[], int *contador_escolas)
{
    //Escola lista_escolas[5];
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        //printf("Carregando Ficheiro %s", filePath);
        char buffer[1024];
        int linha = 0, index = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            int coluna = 0;
            linha++;

            index = linha - 1;
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                //Check with it brakes reading Abreviatura
                //printf("%s\t", value);
                guardarDadosEscola(value, &lista_escolas[index],coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
            //ImprimeEscola(lista_escolas[index]);
            fclose(fileStream);
        }
        *contador_escolas = index;
        // Close the file
        printf("\nFinish reading file %s\n", filePath);
    }
    else
    {
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
}

// Importa os dados das Transacoes para o programa
void importFicheiroCSVTransacoes(char filePath[], Transacao lista_movimentos[], int *contador_movimentos)
{
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream)
    {
        //printf("Carregando Ficheiro %s", filePath);
        char buffer[1024];
        int linha = 0, index = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            int coluna = 0;
            linha++;

            index = linha - 1;
            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value)
            {
                guardarDadosTransacao(value, &lista_movimentos[index],coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
            //ImprimeTransacao(lista_movimentos[index]);
        }
        *contador_movimentos = index;
        // Close the file
        fclose(fileStream);
        printf("\nFinish reading file %s\n", filePath);
    }
    else
    {
        printf("\nError opening file: %s\n", filePath);
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

// Cria um utilizador
void crearUtilizador(Utilizador *utilizador_actual, int id_novo_utilizador)
{
    Utilizador novo_utilizador = *utilizador_actual;
    system("cls");
    do
    {
        novo_utilizador.ID = id_novo_utilizador;
        //TODO Processo de seleção de escola dinámico com os dados no programa
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
    strcpy(inserir_tipo,TIPO_UTILIZADOR[charParaInt(validacaoCharacter(texto, valoresValidos) - 1)]);
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

// Obtem todas as letras mayusculas de uma string
void obterMayusculas(char texto[], char *string_destino)
{
    char letras_mayusculas[10] = "";
    for(int index = 0;index < strlen(texto) ; index++)
        if ('A' <= texto[index] && texto[index] <= 'Z')
            strncat(letras_mayusculas, &texto[index], 1);
    strcpy(string_destino, letras_mayusculas);
}

