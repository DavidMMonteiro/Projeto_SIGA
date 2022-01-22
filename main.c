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

#define TIPO_UTILIZADOR \
    (char *[3]) { "Estudante", "Docente", "Funcionario" }
#define MAX_UTILIZADORES 200
#define MAX_ESCOLAS 5
#define MAX_MOVIMENTOS 1000 // Array doesn't work with array[5000]

//---------Declaração Funções Gerais---------//

bool validacaoBinaria(char[]);
char validacaoCharacter(char[], char[]);
void esperarEnter(void);
const char *getfield(char *, int);
void obterString(char[], char *);
float obterFloat(char[]);
int obterInt(char text[]);
int charParaInt(char);
void obterMayusculas(char[], char *);

//---------Declaração Funções Especificas---------//

void importFicheiroBinUtilizadores(char[], Utilizador[], int *);
void importFicheiroBinEscolas(char[], Escola[], int *);
void importFicheiroBinTransacoes(char[], Transacao[], int *);
void importFicheiroCSVUtilizadores(char[], Utilizador[], int *);
void importFicheiroCSVEscolas(char[], Escola[], int *);
void importFicheiroCSVTransacoes(char[], Transacao[], int *);

void exportFicheiroBinUtilizadores(char [], Utilizador [], int, bool);
void exportFicheiroBinEscolas(char [], Escola [], int, bool);
void exportFicheiroBinTransacoes(char [], Transacao [], int, bool);
void exportFicheiroCSVUtilizadores(char [], Utilizador[], int, bool);
void exportFicheiroCSVEscolas(char [], Escola[], int, bool);
void exportFicheiroCSVTransacoes(char [], Transacao[], int, bool);

void guardarDadosUtilizador(char[], Utilizador *, int);
void guardarDadosEscola(char[], Escola *, int);
void guardarDadosTransacao(char[], Transacao *, int);

void crearUtilizador(Utilizador *, int, int *);
void escolherTipoUtilizador(char *);
void crearEscola(Escola *, int *);

int main()
{
    // Let print special caracters
    setlocale(LC_ALL, "Portuguese");

    // Ficheiros
    char caminhoCSVEscolas[] = "files/CSV/escolas.csv";
    char caminhoCSVUtilizadores[] = "files/CSV/utilizadores.csv";
    char caminhoCSVMovimentos[] = "files/CSV/movimentos.csv";

    char caminhoBinEscolas[] = "files/Bin/escolas.Bin";
    char caminhoBinUtilizadores[] = "files/Bin/utilizadores.Bin";
    char caminhoBinMovimentos[] = "files/Bin/movimentos.Bin";

    // Variveis
    char resposta;
    bool resposta_rewrite;
    int index, index_novo_dado;
    int counter_utilizadores = 0, counter_escolas = 0, counter_movimentos = 0;
    // Dados
    Utilizador utilizadores[MAX_UTILIZADORES];
    Escola escolas[MAX_ESCOLAS];
    Transacao movimentos[MAX_MOVIMENTOS];

    system("cls");
    if (validacaoBinaria("Quere importar dados dos ficheiros ao sistema?"))
    {
        system("cls");
        importFicheiroBinUtilizadores(caminhoBinUtilizadores, utilizadores, &counter_utilizadores);
        // importFicheiroCSVUtilizadores(caminhoCSVUtilizadores, utilizadores, &counter_utilizadores);
        importFicheiroBinEscolas(caminhoBinEscolas, escolas, &counter_escolas);
        // importFicheiroCSVEscolas(caminhoCSVEscolas, escolas, &counter_escolas);
        importFicheiroBinTransacoes(caminhoBinMovimentos, movimentos, &counter_movimentos);
        // importFicheiroCSVTransacoes(caminhoCSVMovimentos, movimentos, &counter_movimentos);
        esperarEnter();
    }

    do
    {
        system("cls");
        resposta = validacaoCharacter("1 - Ver dados no sistema\n2 - Inserir Dados\n3 - Guardar dados\nX - Sair", "123X");
        switch (resposta)
        {
        case '1': // Mostrar dados no sistema
            switch (validacaoCharacter("U - Dados Utilizadores\nE - Dados Escolas\nM - Movimentos\nX - Sair", "UEMX"))
            {
            case 'U': // Mostrar utilizadores
                for (index = 0; index < counter_utilizadores; index++)
                    ImprimeUtilizador(utilizadores[index]);
                esperarEnter();
                break;
            case 'E': // Mostrar escolas
                for (index = 0; index < counter_escolas; index++)
                    ImprimeEscola(escolas[index]);
                esperarEnter();
                break;
            case 'M': // Mostrar transações
                for (index = 0; index < counter_movimentos; index++)
                    ImprimeTransacao(movimentos[index]);
                esperarEnter();
                break;
            default:
                break;
            }
            break;
        case '2': // Inserir um novo registo
            system("cls");
            switch (validacaoCharacter("Escolha o tipo de da dados que quere inserir.\nU - Utilizador\nE - Escola\nX - Cancelar", "UEX"))
            {
            case 'U': // Inserir um novo utilizador
                index = 0;
                index_novo_dado = -1;
                while (index_novo_dado < 0 && index < counter_utilizadores)
                {
                    if (strlen(utilizadores[index].Nome) == 0)
                        index_novo_dado = index;
                    index++;
                }
                if (index_novo_dado < 0)
                    printf("Sem espaço para utilizador!");
                else
                    crearUtilizador(&utilizadores[index_novo_dado], index_novo_dado, &counter_utilizadores);
                break;
            case 'E': // Inserir uma nova escola
                index = 0;
                index_novo_dado = -1;
                while (index_novo_dado < 0 && index < sizeof(escolas) / sizeof(escolas[0]))
                {
                    if (strlen(escolas[index].Nome) == 0)
                        index_novo_dado = index;
                    index++;
                }
                if (index_novo_dado < 0)
                    printf("Sem espaço para nova Escola!");
                else
                    crearEscola(&escolas[index_novo_dado], &counter_escolas);

                break;
            }
            break;
        case '3': // Exportação de dados
            resposta = validacaoCharacter("\nExportar como\n1 - CSV\n2 - Bin", "12");
            if (resposta == '1') // Exportação para ficheiro CSV
            {
                resposta_rewrite = validacaoBinaria("Quere re-escrever os dados?");
                exportFicheiroCSVUtilizadores(caminhoCSVUtilizadores, utilizadores, counter_utilizadores,resposta_rewrite);
                exportFicheiroCSVEscolas(caminhoCSVEscolas, escolas, counter_escolas, resposta_rewrite);
                exportFicheiroCSVTransacoes(caminhoCSVMovimentos, movimentos, counter_movimentos, resposta_rewrite);
                esperarEnter();
            }
            else if (resposta == '2') // Exportação para ficheiro Binario
            {
                system("cls");
                resposta_rewrite = validacaoBinaria("Quere re-escrever os dados?");
                exportFicheiroBinUtilizadores(caminhoBinUtilizadores, utilizadores, counter_utilizadores, resposta_rewrite);
                exportFicheiroBinEscolas(caminhoBinEscolas, escolas, counter_escolas, resposta_rewrite);
                exportFicheiroBinTransacoes(caminhoBinMovimentos, movimentos, counter_movimentos, resposta_rewrite);
                esperarEnter();
            }
            break;
        case 'X': // Opção de saida do programa
            system("cls");
            // Verificação para guardar os dados do sistema
            if (validacaoBinaria("Quere guardar os dados em sistema?"))
            {
                system("cls");
                // Verificação para rescrever ou acrechentar os dados
                resposta_rewrite = validacaoBinaria("Quere re-escrever os dados?");
                exportFicheiroBinUtilizadores(caminhoBinUtilizadores, utilizadores, counter_utilizadores, resposta_rewrite);
                exportFicheiroBinEscolas(caminhoBinEscolas, escolas, counter_escolas, resposta_rewrite);
                exportFicheiroBinTransacoes(caminhoBinMovimentos, movimentos, counter_movimentos, resposta_rewrite);
                esperarEnter();
            }
            break;

        default: // Caso o sistema deixe pasar uma opção não valida
            system("cls");
            printf("Opção não encontrada");
            esperarEnter();
            break;
        }
    } while (resposta != 'X');
}

//---------Funções genéricas---------//

// Validacao para questoes de Sim ou Nao
bool validacaoBinaria(char texto[])
{
    char resposta;
    do
    {
        // system("cls");
        printf("%s", texto);
        printf("\nY - Sim\nN - Nao\n\n-->");
        fflush(stdin);
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    } while (resposta != 'Y' && resposta != 'N');
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
    } while (!strchr(valores_validos, resposta));
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

// Obter um valor numerico int
int obterInt(char text[])
{
    int valor;
    system("cls");
    printf(text);
    fflush(stdin);
    scanf(" %d", &valor);
    return valor;
}

// Conversao de numeros em char para Int
int charParaInt(char input)
{
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
    do
    {
        system("cls");
        fflush(stdin);
        printf(text);
        fgets(input, sizeof(input), stdin);
    } while (input == "");

    // Depuração de String
    // Verificar se existem \n na string e sustitui
    for (int index = 0; index < sizeof(input); index++)
        if (input[index] == '\n')
            input[index] = '\0';

    // Copia a string inserida para a variavel pasada a função
    strcpy(data, input);
}

// Obtem todas as letras mayusculas de uma string
void obterMayusculas(char texto[], char *string_destino)
{
    char letras_mayusculas[10] = "";
    for (int index = 0; index < strlen(texto); index++)
        if ('A' <= texto[index] && texto[index] <= 'Z')
            strncat(letras_mayusculas, &texto[index], 1);
    strcpy(string_destino, letras_mayusculas);
}

//---------Funções especificas---------//

// Importa os dados dos Utilizadores para o programa apartir de ficheiros CSV
void importFicheiroCSVUtilizadores(char filePath[], Utilizador lista_utilizadores[], int *contador_utilizadores)
{
    FILE *fileStream = fopen(filePath, "r");
    char buffer[1024];
    int linha = 0, coluna = 0, index = 0;
    if (fileStream)
    {
        linha = 0;
        coluna = 0;
        index = 0;
        *contador_utilizadores = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            coluna = 0;
            linha++;
            index = linha - 1;
            // Dividir os dados du ficheiro CSV
            char *value = strtok(buffer, ";");
            while (value)
            {
                // Procesamento de dados
                guardarDadosUtilizador(value, &lista_utilizadores[index], coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
        }
        *contador_utilizadores = index + 1;
        // Fechar o ficheiro
        fclose(fileStream);
        printf("\nFinish reading file %s\n", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro tiver algum erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
}

// Exporta os dados dos Utilizadores para ficheiros CSV
void exportFicheiroCSVUtilizadores(char filePath[], Utilizador lista_utilizadores[], int counter_utilizadores, bool rescreber)
{
    FILE *ficheiro = fopen(filePath, rescreber ? "w" : "a");
    int index;
    if (ficheiro)
    {
        // Estruturação dos dados das structs numa string para inserir no ficheiro CSV
        for (index = 0; index < counter_utilizadores; index++)
            fprintf(ficheiro, "%d;%d;%s;%d;%s;%s;%f\n",
                    lista_utilizadores[index].ID, lista_utilizadores[index].ID_Escola, lista_utilizadores[index].Nome,
                    lista_utilizadores[index].NIF, lista_utilizadores[index].Tipo, lista_utilizadores[index].Email,
                    lista_utilizadores[index].Valor_Conta);
        printf("\nDados exportados para %s", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro tiver algum erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// Importa os dados dos Utilizadores para o programa apartir de ficheiros Bin
void importFicheiroBinUtilizadores(char filePath[], Utilizador lista_utilizadores[], int *contador_utilizadores)
{
    FILE *ficheiro = fopen(filePath, "rb");
    if (ficheiro)
    {
        // Abertura do ficheiro no final
        fseek(ficheiro, 0L, SEEK_END);
        // Contador de dados no ficheiro
        *contador_utilizadores = ftell(ficheiro) / sizeof(Utilizador);
        // Abertura do ficheiro no inicio
        fseek(ficheiro, 0L, SEEK_SET);
        // Leitura dos dados do ficheiro para o sistema
        fread(lista_utilizadores, sizeof(Utilizador), *contador_utilizadores, ficheiro);
        // Out put para o utlizador
        printf("\nFicheiro %s carregado", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro tiver algum erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// Exporta os dados dos Utilizadores para o programa apartir de ficheiros Bin
void exportFicheiroBinUtilizadores(char filePath[], Utilizador lista_utilizadores[], int contador_utilizadores, bool rescreber)
{
    // Seleção de tipo de abertura de ficheiro feito pelo o utilizador
    // Ira permitir rescrever ou acrescentar os dados do ficheiro
    FILE *ficheiro = fopen(filePath, rescreber ? "wb" : "ab");
    if (ficheiro)
    {
        // Inserção dos dados no ficheiro
        fwrite(lista_utilizadores, sizeof(Utilizador), contador_utilizadores, ficheiro);
        printf("\nDados guardados.");
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// Importa os dados das Escolas para o programa
void importFicheiroCSVEscolas(char filePath[], Escola lista_escolas[], int *contador_escolas)
{
    FILE *fileStream = fopen(filePath, "r");
    int linha = 0, index = 0, coluna = 0;
    // Informação do ficheiro
    char buffer[1024];
    if (fileStream)
    {
        linha = 0;
        index = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            coluna = 0;
            linha++;
            index = linha - 1;
            // Dividir os dados do CSV
            char *value = strtok(buffer, ";");
            while (value)
            {
                // Procesamento dos dados
                guardarDadosEscola(value, &lista_escolas[index], coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
        }
        // Fechar ficheiro
        fclose(fileStream);
        // Atribuir contagem de escolas
        *contador_escolas = index + 1;
        printf("\nFinish reading file %s\n", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
}

// Exporta os dados das Escolas ficheiros CSV
void exportFicheiroCSVEscolas(char filePath[], Escola lista_escolas[], int counter_escolas, bool rescreber)
{
    FILE *ficheiro = fopen(filePath, rescreber ? "w" : "a");
    int index;
    if (ficheiro)
    {
        // Estruturação dos dados das structs numa string para inserir no ficheiro CSV
        for (index = 0; index < counter_escolas; index++)
            fprintf(ficheiro, "%d;%s;%s;%s;%s\n",
                    lista_escolas[index].ID, lista_escolas[index].Nome, lista_escolas[index].Abreviatura,
                    lista_escolas[index].Campus, lista_escolas[index].Localidade);
        printf("\nDados exportados para %s", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// Importa os dados das Escolas para o programa apartir de ficheiros Bin
void importFicheiroBinEscolas(char filePath[], Escola lista_escolas[], int *contador_escolas)
{
    FILE *ficheiro = fopen(filePath, "rb");
    if (ficheiro)
    {
        // Abertura do ficheiro no final
        fseek(ficheiro, 0L, SEEK_END);
        // Contador de dados no ficheiro
        *contador_escolas = ftell(ficheiro) / sizeof(Escola);
        // Abertura do ficheiro no inicio
        fseek(ficheiro, 0L, SEEK_SET);
        // Leitura dos dados do ficheiro para o sistema
        fread(lista_escolas, sizeof(Escola), *contador_escolas, ficheiro);
        // Out put para o utlizador
        printf("\nFicheiro %s carregado", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// Exporta os dados das Escolas para o programa apartir de ficheiros Bin
void exportFicheiroBinEscolas(char filePath[], Escola lista_escolas[], int contador_escolas, bool rescreber)
{
    // Seleção de tipo de abertura de ficheiro feito pelo o utilizador
    // Ira permitir rescrever ou acrescentar os dados do ficheiro
    FILE *ficheiro = fopen(filePath, rescreber ? "wb" : "ab");
    if (ficheiro)
    {
        // Inserção dos dados no ficheiro
        fwrite(lista_escolas, sizeof(Escola), contador_escolas, ficheiro);
        printf("\nDados guardados.");
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// Importa os dados das Transacoes para o programa apartir de ficheiros CSV
void importFicheiroCSVTransacoes(char filePath[], Transacao lista_movimentos[], int *contador_movimentos)
{
    FILE *fileStream = fopen(filePath, "r");
    char buffer[1024];
    int linha = 0, index = 0;
    int coluna = 0;
    if (fileStream)
    {
        char buffer[1024];
        linha = 0;
        index = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            coluna = 0;
            linha++;
            index = linha - 1;
            // Dividir dados
            char *value = strtok(buffer, ";");
            while (value)
            {
                // Procesamento de dados
                guardarDadosTransacao(value, &lista_movimentos[index], coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
            // ImprimeTransacao(lista_movimentos[index]);
        }
        *contador_movimentos = index + 1;
        // Fechar ficheiro
        fclose(fileStream);
        printf("\nFinish reading file %s\n", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
}

// Exporta os dados das transações ficheiros CSV
void exportFicheiroCSVTransacoes(char filePath[], Transacao lista_movimentos[], int counter_movimentos, bool rescreber){
    FILE *ficheiro = fopen(filePath, rescreber ? "w" : "a");
    int index;
    if (ficheiro)
    {
        // Estruturação dos dados das structs numa string para inserir no ficheiro CSV
        for (index = 0; index < counter_movimentos; index++)
            fprintf(ficheiro, "%d;%d;%s;%.2f;%s;%s\n",
                    lista_movimentos[index].ID, lista_movimentos[index].ID_Utilizador, lista_movimentos[index].Tipo, lista_movimentos[index].Valor, lista_movimentos[index].Data, lista_movimentos[index].Hora);
        printf("\nDados exportados para %s", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    fclose(ficheiro);
}

// Importa os dados das Transacoes para o programa apartir de ficheiros Bin
void importFicheiroBinTransacoes(char filePath[], Transacao lista_movimentos[], int *contador_movimentos)
{
    FILE *ficheiro = fopen(filePath, "rb");
    if (ficheiro)
    {
        // Abertura do ficheiro no final
        fseek(ficheiro, 0L, SEEK_END);
        // Contador de dados no ficheiro
        *contador_movimentos = ftell(ficheiro) / sizeof(Transacao);
        // Abertura do ficheiro no inicio
        fseek(ficheiro, 0L, SEEK_SET);
        // Leitura dos dados do ficheiro para o sistema
        fread(lista_movimentos, sizeof(Transacao), *contador_movimentos, ficheiro);
        // Out put para o utlizador
        printf("\nFicheiro %s carregado", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    // Fechar ficheiro
    fclose(ficheiro);
}

// Exporta os dados das Transacoes para o programa apartir de ficheiros Bin
void exportFicheiroBinTransacoes(char filePath[], Transacao lista_movimentos[], int contador_movimentos, bool rescreber)
{
    // Seleção de tipo de abertura de ficheiro feito pelo o utilizador
    // Ira permitir rescrever ou acrescentar os dados do ficheiro
    FILE *ficheiro = fopen(filePath, rescreber ? "wb" : "ab");
    if (ficheiro)
    {
        // Inserção dos dados no ficheiro
        fwrite(lista_movimentos, sizeof(Transacao), contador_movimentos, ficheiro);
        printf("\nDados guardados.");
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
    // Fechar ficheiro
    fclose(ficheiro);
}

// Insire os dados do novo utilizador na struct pasada por referencia
void guardarDadosUtilizador(char novo_dado[], Utilizador *dados_antigos, int coluna)
{
    switch (coluna)
    {
    case 0: // Save ID Utilizador
        dados_antigos->ID = atoi(novo_dado);
        break;
    case 1: // Save ID Escola
        dados_antigos->ID_Escola = atoi(novo_dado);
        break;
    case 2: // Save Nome Utilizador
        strcpy(dados_antigos->Nome, novo_dado);
        break;
    case 3: // Save NIF
        dados_antigos->NIF = atoi(novo_dado);
        break;
    case 4: // Save Tipo de Utilizador
        strcpy(dados_antigos->Tipo, novo_dado);
        break;
    case 5: // Save Email
        strcpy(dados_antigos->Email, novo_dado);
        break;
    case 6: // Save Valor na conta
        dados_antigos->Valor_Conta = strtof(novo_dado, NULL);
        break;
    }
}

// Insire os dados da nova escola na struct pasada por referencia
void guardarDadosEscola(char novo_dado[], Escola *dados_antigos, int coluna)
{
    switch (coluna)
    {
    case 0: // Save Id
        dados_antigos->ID = atoi(novo_dado);
        break;
    case 1: // Save Nome Escola
        strcpy(dados_antigos->Nome, novo_dado);
        break;
    case 2: // Save Abreviatura Escola
        strcpy(dados_antigos->Abreviatura, novo_dado);
        break;
    case 3: // Save Campus
        strcpy(dados_antigos->Campus, novo_dado);
        break;
    case 4: // Save Localidade
        strcpy(dados_antigos->Localidade, novo_dado);
        break;
    }
}

// Insire os dados da nova escola na struct pasada por referencia
void guardarDadosTransacao(char novo_dado[], Transacao *dados_antigos, int coluna)
{
    switch (coluna)
    {
    case 0:
        // Save ID Transações
        dados_antigos->ID = atoi(novo_dado);
        break;
    case 1: // Save ID Utilizador
        dados_antigos->ID_Utilizador = atoi(novo_dado);
        break;
    case 2: // Save Tipo de Movimento
        strcpy(dados_antigos->Tipo, novo_dado);
        break;
    case 3: // Save o valor do movimento
        dados_antigos->Valor = strtof(novo_dado, NULL);
        break;
    case 4: // Save Data do movimento
        strcpy(dados_antigos->Data, novo_dado);
        break;
    case 5: // Save Hora do movimento
        strcpy(dados_antigos->Hora, novo_dado);
        break;
    }
}

// Cria um utilizador
void crearUtilizador(Utilizador *utilizador_actual, int id_novo_utilizador, int *contador_utilizadores)
{
    // Faz uma copia do utilizador pasado por referencia
    Utilizador novo_utilizador = *utilizador_actual;
    system("cls");
    do
    {
        // Atribui uma id
        novo_utilizador.ID = id_novo_utilizador;
        // TODO Processo de seleção de escola dinámico com os dados no programa
        // Atribui a escola escolhida pelo utilizador
        novo_utilizador.ID_Escola = charParaInt(validacaoCharacter("Escolha a sua escola\n2 - Escola Superior de Tecnologia e Gestão(ESTG)\n3 - Escola Superior de Turismo e Tecnologia do Mar(ESTM)", "23"));
        // Atribui o nome inserido pelo utilizador
        obterString("Insira o seu nome: ", novo_utilizador.Nome);
        // Atribui o NIF inserido pelo utilizador
        novo_utilizador.NIF = obterInt("Insira o seu NIF:");
        // Atribui o tipo de utilizador inserido pelo utilizador
        escolherTipoUtilizador(&novo_utilizador.Tipo);
        // Atribui o email inserido pelo utilizador
        obterString("Insira o seu Email: ", novo_utilizador.Email);
        // Atribui o valor inicial na conta inserido pelo utilizador
        novo_utilizador.Valor_Conta = obterFloat("Insira o seu valor inicial:");
        system("cls");
        // Mostra o dados o utilizador antes de guardar
        ImprimeUtilizador(novo_utilizador);
        esperarEnter();
        // Permite ver o utilizador se os dados que quere inserir estão corretos
    } while (!validacaoBinaria("\nTem a sarteca que quere inserir este dados?"));
    // Guarda os dados no sistema
    *utilizador_actual = novo_utilizador;
    (*contador_utilizadores)++;
}

// Pede ao utilizador insira un tipo de utilizador
void escolherTipoUtilizador(char *inserir_tipo)
{
    // Pede o utilizador para escolher um numero atribuido o tipo de utilizador que quere inserir
    char texto[] = "Escolha o tipo de utilizador\n1 - Estudante\n2 - Docente\n3 - Funcionario", valoresValidos[] = "123";
    // Vai buscar o tipo de utilizador o variavel global apartir do numero obtido do utilizador
    strcpy(inserir_tipo, TIPO_UTILIZADOR[charParaInt(validacaoCharacter(texto, valoresValidos) - 1)]);
}

// Cria uma nova escola
void crearEscola(Escola *escola_actual, int *contador_escolas)
{
    // Faz uma copia da escola pasado por referencia
    Escola nova_escola = *escola_actual;
    system("cls");
    do
    {
        // Atribui uma nova id
        //TODO ID automática
        nova_escola.ID = obterInt("Insira Id da nova escola: ");
        // Atribui o nome da escola
        obterString("Insira o nome da nova escola: ", &nova_escola.Nome);
        // Atribui a abreviatura do nome da escola
        obterMayusculas(nova_escola.Nome, nova_escola.Abreviatura);
        // Atribui o campus da escola
        obterString("Insira o Campus da nova escola: ", &nova_escola.Campus);
        // Atribui a localida da escola
        obterString("Insira a Localidade da nova escola: ", &nova_escola.Localidade);
        system("cls");
        // Mostra os dados o utilizador
        ImprimeEscola(nova_escola);
        esperarEnter();
        // Permite ver ao utilizador se os dados que quere inserir estão corretos
    } while (!validacaoBinaria("\nTem a sarteca que quere inserir este dados?"));
    // Guarda os dados no sistema
    *escola_actual = nova_escola;
    (*contador_escolas)++;
}

//TODO Função para estadisticas

//TODO Função apagar utilizador

//TODO Função apagar Escola

//TODO Função apagar Transação