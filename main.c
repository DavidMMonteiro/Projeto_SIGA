/*
    INFO
    Projeto para recriar o sistema SIGA das cantinas das escolas que fazem parte do IPL.
    Este sistema ira carregar ficheiros binarios ou csv com a informacao de escolas, utilizadores e as transacoes de cada utilizador.
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
#include <windows.h>

typedef struct // Estrutura Tempo
{
    int Ano;
    int Mes;
    int Dia;
    int Hora;
    int Minuto;
    int Segundo;
} Tempo;

typedef struct // Estrutura Utilizador
{
    int ID;
    int ID_Escola;
    char Nome[1024];
    int NIF;
    char Tipo[255];
    char Email[1024];
    float Valor_Conta;
} Utilizador;

typedef struct // Estrutura Escola
{
    int ID;
    char Nome[1024];
    char Abreviatura[10];
    char Campus[1024];
    char Localidade[1024];
} Escola;

typedef struct // Estrutura de movimentos
{
    int ID;
    int ID_Utilizador;
    char Tipo[255];
    float Valor;
    Tempo Data_Hora;
} Transacao;

#define TIPO_UTILIZADOR \
    (char *[3]) { "Estudante", "Docente", "Funcionario" }
#define TIPO_MOVIMENTO \
    (char *[2]) { "Pagamento", "Carregamento" }
#define MAX_UTILIZADORES 200
#define MAX_ESCOLAS 5
#define MAX_MOVIMENTOS 1000 // Array doesn't work with array[5000]

//---------Declaração Funções Structs---------//

void ImprimeUtilizador(Utilizador);
void ImprimeEscola(Escola);
void ImprimeTransacao(Transacao);
void ImprimeTempo(Tempo);

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
void obterData_Hora(Tempo *);
float calculoPercentagem(int, int);
int pedirTipoMovimento();
int pedirTipoUtilizador();
bool validarDataMovimento(Tempo, Tempo, Tempo);
void inicializarHora(Tempo *);
void obterAno(Tempo *, char[]);
void obterMes(Tempo *, char[]);
void obterDia(Tempo *, char[]);
Tempo obterData(char[]);

//---------Declaração Funções Especificas---------//

//---------Funções Ver dados---------//

void ver_utilizadores(Utilizador[], int);
void ver_escolas(Escola[], int);
void ver_movimentos(Transacao[], int);

//---------Funções Importe de dados---------//

void importFicheiroBinUtilizadores(char[], Utilizador[], int *);
void importFicheiroBinEscolas(char[], Escola[], int *);
void importFicheiroBinTransacoes(char[], Transacao[], int *);
void importFicheiroCSVUtilizadores(char[], Utilizador[], int *);
void importFicheiroCSVEscolas(char[], Escola[], int *);
void importFicheiroCSVTransacoes(char[], Transacao[], int *);

//---------Funções Exporte de dados---------//

void exportFicheiroBinUtilizadores(char[], Utilizador[], int, bool);
void exportFicheiroBinEscolas(char[], Escola[], int, bool);
void exportFicheiroBinTransacoes(char[], Transacao[], int, bool);
void exportFicheiroCSVUtilizadores(char[], Utilizador[], int, bool);
void exportFicheiroCSVEscolas(char[], Escola[], int, bool);
void exportFicheiroCSVTransacoes(char[], Transacao[], int, bool);

//---------Funções Leitura de dados CSV---------//

void guardarDadosUtilizador(char[], Utilizador *, int);
void guardarDadosEscola(char[], Escola *, int);
void guardarDadosTransacao(char[], Transacao *, int);

void crearUtilizador(Utilizador *, int, int *, Escola[], int);
void crearEscola(Escola *, int *);
void escolherTipoMovimento(char *);
int obter_index_utilizador(Utilizador[], int);
void crearMovimento(Utilizador *, Transacao *, int *);
void calculoTotalFaturacao(Utilizador[], Escola[], Transacao[], int, int, int);
void calculoPercentagemMovimentos(Utilizador[], Escola[], Transacao[], int, int, int);
void calculoTotalUtilizadorEntreDatas(Utilizador[], Escola[], Transacao[], int, int, int);
bool validarDataInicialDataFinal(Tempo, Tempo);

//---------------------------------------------//

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
    int index, index_novo_dado, index_utilizador;
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
        resposta = validacaoCharacter("1 - Ver dados no sistema\n2 - Estadisticas\n3 - Inserir Dados\n4 - Guardar dados\n5 - Importar dados\nX - Sair", "12345X");
        switch (resposta)
        {
        case '1': // Mostrar dados no sistema
            system("cls");
            switch (validacaoCharacter("U - Dados Utilizadores\nE - Dados Escolas\nM - Movimentos\nX - Sair", "UEMX"))
            {
            case 'U': // Mostrar utilizadores
                ver_utilizadores(utilizadores, counter_utilizadores);
                esperarEnter();
                break;
            case 'E': // Mostrar escolas
                ver_escolas(escolas, counter_escolas);
                esperarEnter();
                break;
            case 'M': // Mostrar transações
                ver_movimentos(movimentos, counter_movimentos);
                esperarEnter();
                break;
            default:
                break;
            }
            break;
        case '2': // Ver as estadísticas do sistema
            system("cls");
            switch (validacaoCharacter("Insira o tipo de dados que quere visualizar:\n1 - Total faturado por escola\n2 - Percentegam de transacoes por escola\n3 - Total de transacoes\nX - Sair", "123X"))
            {
            case '1': // Vai mostrar a faturação por escola
                calculoTotalFaturacao(utilizadores, escolas, movimentos, counter_utilizadores, counter_escolas, counter_movimentos);
                break;
            case '2': // Vai mostrar a percentagem de movimentos por escola
                calculoPercentagemMovimentos(utilizadores, escolas, movimentos, counter_utilizadores, counter_escolas, counter_movimentos);
                break;
            case '3': // Vai mostrar o total de movimentos, entre duas datas, em função do tipo de utilizador
                calculoTotalUtilizadorEntreDatas(utilizadores, escolas, movimentos, counter_utilizadores, counter_escolas, counter_movimentos);
                break;

            default:
                break;
            }
            break;
        case '3': // Inserir um novo registo
            system("cls");
            switch (validacaoCharacter("Escolha o tipo de da dados que quere inserir.\nU - Utilizador\nE - Escola\nM - Movimento\nX - Cancelar", "UEMX"))
            {
            case 'U': // Inserir um novo utilizador
                if (counter_escolas <= 0)
                {
                    printf("Nao existem escolas registadas.\nRegistre primeiro uma escola para poder ter utilizadores.");
                    esperarEnter();
                }
                else
                {
                    index = 0;
                    index_novo_dado = -1;
                    // Procura o primeiro campo nos utilizadores vacio

                    while (index_novo_dado < 0 && index < MAX_UTILIZADORES)
                    {
                        if (strlen(utilizadores[index].Nome) == 0)
                            index_novo_dado = index;
                        index++;
                    }
                    // Verifica se existe espaço no array
                    if (index_novo_dado < 0)
                        printf("Sem espaco para utilizador!");
                    else
                        crearUtilizador(&utilizadores[index_novo_dado], index_novo_dado, &counter_utilizadores, escolas, counter_escolas);
                }
                break;
            case 'E': // Inserir uma nova escola
                index = 0;
                index_novo_dado = -1;
                // Procura o primeiro campo nas escolas vacio
                while (index_novo_dado < 0 && index < MAX_ESCOLAS)
                {
                    if (strlen(escolas[index].Nome) == 0)
                        index_novo_dado = index;
                    index++;
                }
                // Verifica se existe espaço no array
                if (index_novo_dado < 0)
                    printf("Sem espaco para nova Escola!");
                else
                    crearEscola(&escolas[index_novo_dado], &counter_escolas);

                break;
            case 'M': // Insire um novo movimento
                if (counter_escolas <= 0)
                {
                    printf("Nao existem escolas registadas.\nRegistre primeiro uma escola para poder ter movimentos.");
                    esperarEnter();
                }
                else if (counter_utilizadores <= 0)
                {
                    printf("Nao existem utilizadores registadas.\nRegistre primeiro um utilizador para poder ter movimentos.");
                    esperarEnter();
                }
                else
                {
                    system("cls");
                    index_utilizador = obter_index_utilizador(utilizadores, counter_utilizadores);
                    if (index_utilizador < 0)
                    {
                        printf("Nao foi encontrado nenhum utilizador");
                        esperarEnter();
                    }
                    else
                    {
                        index = 0;
                        index_novo_dado = -1;
                        // Procura o primeiro campo nos movimentos vacio
                        while (index_novo_dado < 0 && index < MAX_MOVIMENTOS)
                        {
                            if (strlen(movimentos[index].Tipo) == 0)
                                index_novo_dado = index;
                            index++;
                        }

                        // Verifica se existe espaço no array
                        if (index_novo_dado < 0)
                        {
                            printf("Sem espaco para novo movimento!");
                            esperarEnter();
                        }
                        else
                            crearMovimento(&utilizadores[index_utilizador], &movimentos[index_novo_dado], &counter_movimentos);
                    }
                }
                break;
            }
            break;
        case '4': // Exportação de dados
            system("cls");
            switch (validacaoCharacter("\nExportar como\n1 - CSV\n2 - Bin\nX - Sair", "12X"))
            {
            case '1': // Exportação para ficheiro CSV
                resposta_rewrite = validacaoBinaria("Quere re-escrever os dados?");
                exportFicheiroCSVUtilizadores(caminhoCSVUtilizadores, utilizadores, counter_utilizadores, resposta_rewrite);
                exportFicheiroCSVEscolas(caminhoCSVEscolas, escolas, counter_escolas, resposta_rewrite);
                exportFicheiroCSVTransacoes(caminhoCSVMovimentos, movimentos, counter_movimentos, resposta_rewrite);
                esperarEnter();
                break;

            case '2': // Exportação para ficheiro Binario
                system("cls");
                resposta_rewrite = validacaoBinaria("Quere re-escrever os dados?");
                exportFicheiroBinUtilizadores(caminhoBinUtilizadores, utilizadores, counter_utilizadores, resposta_rewrite);
                exportFicheiroBinEscolas(caminhoBinEscolas, escolas, counter_escolas, resposta_rewrite);
                exportFicheiroBinTransacoes(caminhoBinMovimentos, movimentos, counter_movimentos, resposta_rewrite);
                esperarEnter();
                break;
            }
            break;
        case '5': // Importação de ficheiros
            system("cls");
            switch (validacaoCharacter("\nImportar ficheiro\n1 - CSV\n2 - Bin\nX - Sair", "12X"))
            {
            case '1': // Importar ficheiro CSV
                counter_utilizadores = 0;
                counter_escolas = 0;
                counter_movimentos = 0;
                importFicheiroCSVUtilizadores(caminhoCSVUtilizadores, utilizadores, &counter_utilizadores);
                importFicheiroCSVEscolas(caminhoCSVEscolas, escolas, &counter_escolas);
                importFicheiroCSVTransacoes(caminhoCSVMovimentos, movimentos, &counter_movimentos);
                esperarEnter();
                break;

            case '2': // Importar ficheiro CSV
                counter_utilizadores = 0;
                counter_escolas = 0;
                counter_movimentos = 0;
                importFicheiroBinUtilizadores(caminhoBinUtilizadores, utilizadores, &counter_utilizadores);
                importFicheiroBinEscolas(caminhoBinEscolas, escolas, &counter_escolas);
                importFicheiroBinTransacoes(caminhoBinMovimentos, movimentos, &counter_movimentos);
                esperarEnter();
                break;
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
            printf("Opçãonaoencontrada");
            esperarEnter();
            break;
        }
    } while (resposta != 'X');
}

//---------Funções Structs---------//

// Imprime os dados do utilizador inserido
void ImprimeUtilizador(Utilizador util)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "\nId.%d \nId Escola.%d \nNome: %s \nNIF:%d \nTipo Utilizador:%s \nEmail:%s \nValor na Conta: %.2f", util.ID, util.ID_Escola, util.Nome, util.NIF, util.Tipo, util.Email, util.Valor_Conta);

    printf("%s\n", informacao);
}

// Imprime os dados da escola inserida
void ImprimeEscola(Escola escola)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "\nId.%d \nNome: %s \nAbreviatura: %s \nCampus: %s \nLocalidade: %s", escola.ID, escola.Nome, escola.Abreviatura, escola.Campus, escola.Localidade);

    printf("%s\n", informacao);
}

// Imprime os dados do movimento inserido
void ImprimeTransacao(Transacao transacao)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "\nId.%d \nId Util.%d \nTipo: %s \nValor: %.2f \nData: %d-%d-%d \nHora: %d:%d:%d", transacao.ID, transacao.ID_Utilizador, transacao.Tipo, transacao.Valor, transacao.Data_Hora.Dia, transacao.Data_Hora.Mes, transacao.Data_Hora.Ano, transacao.Data_Hora.Hora, transacao.Data_Hora.Minuto, transacao.Data_Hora.Segundo);

    printf("%s\n", informacao);
}

// Imprime os dados do tempo inserido
void ImprimeTempo(Tempo time)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "\n%d/%d/%d %d:%d:%d", time.Dia, time.Mes, time.Ano, time.Hora, time.Minuto, time.Minuto);

    printf("%s\n", informacao);
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

/* Validacao de leitura de char's
 Parametros:
 - Texto a mostrar
 - Opções*/
char validacaoCharacter(char texto[], char valores_validos[])
{
    char resposta;
    do
    {
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
void esperarEnter()
{
    fflush(stdin);
    printf("\nEnter para continuar...");
    getchar();
}

/* Obtem uma string do utilizador
 Parametros;
 - Texto da informação a pedir
 - String destino onde guardar a informção obtida*/
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

/* Obtem todas as letras mayusculas de uma string
 Parametros:
 - String a analizar
 - String destino para guardar a informação procesada*/
void obterMayusculas(char texto[], char *string_destino)
{
    char letras_mayusculas[10] = "";
    for (int index = 0; index < strlen(texto); index++)
        if ('A' <= texto[index] && texto[index] <= 'Z')
            strncat(letras_mayusculas, &texto[index], 1);
    strcpy(string_destino, letras_mayusculas);
}

/* Obtem a data e hora no systema
 Parametros:
 - Variavel onde vai ser guarda a informação*/
void obterData_Hora(Tempo *datos_novos)
{
    SYSTEMTIME time;

    GetLocalTime(&time);

    datos_novos->Ano = time.wYear;
    datos_novos->Mes = time.wMonth;
    datos_novos->Dia = time.wDay;
    datos_novos->Hora = time.wHour;
    datos_novos->Minuto = time.wMinute;
    datos_novos->Segundo = time.wSecond;
}

// Calcula a porcentagem de um valor em função do numero de elementos
float calculoPercentagem(int valor_original, int numero_elementos)
{
    return numero_elementos > 0 && valor_original > 0 ? (valor_original * 100) / numero_elementos : 0.00;
}

// Vai pedir au utilizador escolher um dos tipos de movimento da variavel global TIPO_MOVIMENTO
int pedirTipoMovimento()
{
    char texto[1024] = "Tipo de movimento:", tmp_texto[1024];
    char opcoes[255], tmp_opcao[15] = "";
    int index = 0;
    system("cls");
    for (index = 0; index < (int)(sizeof(TIPO_MOVIMENTO) / sizeof(TIPO_MOVIMENTO[0])); index++)
    {
        itoa((index + 1), tmp_opcao, 10);
        strcat(opcoes, tmp_opcao);
        snprintf(tmp_texto, sizeof(tmp_texto), "\n\t%d - %s", index + 1, TIPO_MOVIMENTO[index]);
        strcat(texto, tmp_texto);
    }
    return charParaInt(validacaoCharacter(texto, opcoes)) - 1;
}

// Vai pedir au utilizador escolher um dos tipos de utilizdor da variavel global TIPO_UTILIZADOR
int pedirTipoUtilizador()
{
    char texto[1024] = "Tipo de utilizador:", tmp_texto[1024];
    char opcoes[255], tmp_opcao[15] = "";
    int index = 0;
    system("cls");
    for (index = 0; index < (int)(sizeof(TIPO_UTILIZADOR) / sizeof(TIPO_UTILIZADOR[0])); index++)
    {
        itoa((index + 1), tmp_opcao, 10);
        strcat(opcoes, tmp_opcao);
        snprintf(tmp_texto, sizeof(tmp_texto), "\n\t%d - %s", index + 1, TIPO_UTILIZADOR[index]);
        strcat(texto, tmp_texto);
    }
    return charParaInt(validacaoCharacter(texto, opcoes)) - 1;
}

/* Vai permitir au utilizador escolher uma escola
 Parametros:
 - Lista de escolas
 - Numero de escolas registadas*/
Escola escolherEscola(Escola lista_escolas[], int contador)
{
    int index = 0, tmp_id_escola, index_escola = -1;
    do
    {
        system("cls");
        printf("Escolha uma escola:");
        for (index = 0; index < contador; index++)
        {
            printf("\nId.%d - %s", lista_escolas[index].ID, lista_escolas[index].Nome);
        }
        tmp_id_escola = obterInt("\n->");

        for (index = 0; index < contador; index++)
            if (lista_escolas[index].ID == tmp_id_escola)
                index_escola = index;
        if (index_escola < 0)
        {
            printf("Escola nao encontrado");
            esperarEnter();
        }
    } while (index_escola < 0);
    return lista_escolas[index_escola];
}

/* Inicializa a hora de uma data a 0
 Parametro:
 - Variavel Tempo para guarda a informação*/
void inicializarHora(Tempo *data)
{
    data->Hora = 0;
    data->Minuto = 0;
    data->Segundo = 0;
}

/* Obter e filtrar a data.
 Parametro:
 - Variavel Tempo para guardar a informação
 - Texto com o tipo de data a pesquisar*/
void obterAno(Tempo *data, char texto[])
{
    SYSTEMTIME time;

    // Obtem a data atual
    GetLocalTime(&time);
    // Temporal data
    Tempo tmp_data;
    //
    bool validator = false;
    // Pede o ano
    do
    {

        system("cls");
        printf(texto);
        //
        tmp_data.Ano = obterInt("Insira o ano:");
        // Validação dos dados
        validator = (tmp_data.Ano > time.wYear);
        if (validator)
        {
            printf("Ano nao pode ser superior ao ano atual!");
            esperarEnter();
        }
    } while (validator);

    data->Ano = tmp_data.Ano;
}

/* Obter e filtrar o mes.
 Parametros:
 - Variavel Tempo para guardar a informação
 - Texto com o tipo de data a pesquisar*/
void obterMes(Tempo *data, char texto[])
{
    SYSTEMTIME time;
    // Obtem a data atual
    GetLocalTime(&time);
    // Temporal data
    Tempo tmp_data;
    //
    bool validator = false;
    // Pede o mes
    do
    {

        system("cls");
        //
        printf(texto);
        printf("Data: --/--/%d\n", data->Ano);
        tmp_data.Mes = obterInt("Insira um mes:");
        // Validação dos dados
        validator = ((tmp_data.Mes > time.wMonth && data->Ano == time.wYear) || tmp_data.Mes > 12);
        if (validator)
        {
            printf("Insira um mes valido");
            esperarEnter();
        }
    } while (validator);

    data->Mes = tmp_data.Mes;
}

/* Obter e filtrar o dia.
 Parametro:
 - Variavel Tempo para guardar a informação
 - Texto com o tipo de data a pesquisar*/
void obterDia(Tempo *data, char texto[])
{
    SYSTEMTIME time;
    // Temporal data
    Tempo tmp_data;
    // Obtem a data atual
    GetLocalTime(&time);
    //
    bool validator = false;
    // Pede o dia
    do
    {

        system("cls");
        //
        printf(texto);
        printf("Data: --/%d/%d\n", data->Mes, data->Ano);
        tmp_data.Dia = obterInt("Insira um dia:");
        // Validação dos dados
        validator = ((tmp_data.Dia > time.wDay && data->Mes == time.wMonth && data->Ano == time.wYear) || tmp_data.Dia > 30);
        if (validator)
        {
            printf("Dia ao valido.");
            esperarEnter();
        }
    } while (validator);

    data->Dia = tmp_data.Dia;
}

/* Pede os valores de uma data ao utilizador.
 Parametro:
 - Texto com o tipo de data a pesquisar*/
Tempo obterData(char texto[])
{
    // Data temporal
    Tempo tmp_data;

    do
    {
        system("cls");
        // Obtem informação da data do utilizador
        printf(texto);
        obterAno(&tmp_data, texto);
        printf(texto);
        obterMes(&tmp_data, texto);
        printf(texto);
        obterDia(&tmp_data, texto);
        //
        inicializarHora(&tmp_data);

        // Mostra a informação ao utilizador
        system("cls");
        printf("Data inserida:");
        ImprimeTempo(tmp_data);
        // Valido se o utilizador quere proseguir com a data inserida ou não
    } while (!validacaoBinaria("Quere proseguir com esta data?"));

    // Retorna a data inserida
    return tmp_data;
}

// TODO Validação de NIF

// TODO Validação de Mail

// TODO Fazer ID's dinâmicas

//---------Funções especificas---------//

/* Permite ver a lista de utilizadores
 Parametros:
 - Lista de utilizadores
 - Número de registos*/
void ver_utilizadores(Utilizador lista_utilizadores[], int contador)
{
    // Filtra se existem utilizadores ou não
    if (contador > 0)
        for (int index = 0; index < contador; index++)
            ImprimeUtilizador(lista_utilizadores[index]);
    else
        printf("Sem registo de utilizadores");
}

/* Permite ver a lista de escolas
 Parametros:
 - Lista de escolas
 - Número de registos*/
void ver_escolas(Escola lista_escolas[], int contador)
{
    // Filtra se existem escolas ou não
    if (contador > 0)
        for (int index = 0; index < contador; index++)
            ImprimeEscola(lista_escolas[index]);
    else
        printf("Sem registo de escolas");
}

/* Permite ver a lista de movimentos
 Parametros:
 - Lista de movimentos
 - Número de registos*/
void ver_movimentos(Transacao lista_movimentos[], int contador)
{
    if (contador > 0)
        for (int index = 0; index < contador; index++)
            ImprimeTransacao(lista_movimentos[index]);
    else
        printf("Sem registo de movimentos");
}

/* Importa os dados dos Utilizadores para o programa apartir de ficheiros CSV
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void importFicheiroCSVUtilizadores(char filePath[], Utilizador lista_utilizadores[], int *contador_utilizadores)
{
    FILE *fileStream = fopen(filePath, "r");
    char buffer[1024];
    int coluna, index;
    if (fileStream)
    {
        index = 0;
        *contador_utilizadores = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            coluna = 0;
            // Dividir os dados du ficheiro CSV
            char *value = strtok(buffer, ";");
            while (value)
            {
                // Procesamento de dados
                guardarDadosUtilizador(value, &lista_utilizadores[index], coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
            index++;
        }
        *contador_utilizadores = index;
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

/* Exporta os dados dos Utilizadores para ficheiros CSV
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void exportFicheiroCSVUtilizadores(char filePath[], Utilizador lista_utilizadores[], int counter_utilizadores, bool rescreber)
{
    // Vai validar se existem informação a ser guardada
    if (counter_utilizadores <= 0)
        return;
    FILE *ficheiro = fopen(filePath, rescreber ? "w" : "a");
    int index;
    if (ficheiro)
    {
        // Estruturação dos dados das structs numa string para inserir no ficheiro CSV
        for (index = 0; index < counter_utilizadores; index++)
            fprintf(ficheiro, "%d;%d;%s;%d;%s;%s;%.2f\n",
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

/* Importa os dados dos Utilizadores para o programa apartir de ficheiros Bin
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
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

/* Exporta os dados dos Utilizadores para o programa apartir de ficheiros Bin
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void exportFicheiroBinUtilizadores(char filePath[], Utilizador lista_utilizadores[], int contador_utilizadores, bool rescreber)
{
    // Vai validar se existem informação a ser guardada
    if (contador_utilizadores <= 0)
        return;
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

/* Importa os dados das Escolas para o programa
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void importFicheiroCSVEscolas(char filePath[], Escola lista_escolas[], int *contador_escolas)
{
    FILE *fileStream = fopen(filePath, "r");
    int index, coluna;
    // Informação do ficheiro
    char buffer[1024];
    if (fileStream)
    {
        index = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            coluna = 0;
            // Dividir os dados do CSV
            char *value = strtok(buffer, ";");
            while (value)
            {
                // Procesamento dos dados
                guardarDadosEscola(value, &lista_escolas[index], coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
            index++;
        }
        // Fechar ficheiro
        fclose(fileStream);
        // Atribuir contagem de escolas
        *contador_escolas = index;
        printf("\nFinish reading file %s\n", filePath);
    }
    else
    {
        // Caso a abertura do ficheiro de erro
        printf("\nError opening file: %s\n", filePath);
        perror("Error");
    }
}

/* Exporta os dados das Escolas ficheiros CSV
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void exportFicheiroCSVEscolas(char filePath[], Escola lista_escolas[], int counter_escolas, bool rescreber)
{
    // Vai validar se existem informação a ser guardada
    if (counter_escolas <= 0)
        return;
    FILE *ficheiro = fopen(filePath, rescreber ? "w" : "a");
    int index;
    if (ficheiro)
    {
        // Estruturação dos dados das structs numa string para inserir no ficheiro CSV
        for (index = 0; index < counter_escolas; index++)
            fprintf(ficheiro, "%d;%s;%s;%s;%s",
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

/* Importa os dados das Escolas para o programa apartir de ficheiros Bin
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
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

/* Exporta os dados das Escolas para o programa apartir de ficheiros Bin
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void exportFicheiroBinEscolas(char filePath[], Escola lista_escolas[], int contador_escolas, bool rescreber)
{
    // Vai validar se existem informação a ser guardada
    if (contador_escolas <= 0)
        return;
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

/* Importa os dados das Transacoes para o programa apartir de ficheiros CSV
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void importFicheiroCSVTransacoes(char filePath[], Transacao lista_movimentos[], int *contador_movimentos)
{
    FILE *fileStream = fopen(filePath, "r");
    char buffer[1024];
    int index, coluna = 0;
    if (fileStream)
    {
        char buffer[1024];
        index = 0;
        while (fgets(buffer, sizeof(buffer), fileStream))
        {
            coluna = 0;
            // Dividir dados
            char *value = strtok(buffer, ";");
            while (value)
            {
                // TODO Ver porque não lee coluna Horas
                // Procesamento de dados
                guardarDadosTransacao(value, &lista_movimentos[index], coluna);
                value = strtok(NULL, ";");
                coluna++;
            }
            index++;
            // ImprimeTransacao(lista_movimentos[index]);
        }
        *contador_movimentos = index;
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

/* Exporta os dados das transações ficheiros CSV
 Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void exportFicheiroCSVTransacoes(char filePath[], Transacao lista_movimentos[], int counter_movimentos, bool rescreber)
{
    // Vai validar se existem informação a ser guardada
    if (counter_movimentos <= 0)
        return;
    FILE *ficheiro = fopen(filePath, rescreber ? "w" : "a");
    int index;
    if (ficheiro)
    {
        // Estruturação dos dados das structs numa string para inserir no ficheiro CSV
        for (index = 0; index < counter_movimentos; index++)
            fprintf(ficheiro, "%d;%d;%s;%.2f;%d/%d/%d;%d:%d:%d\n",
                    lista_movimentos[index].ID, lista_movimentos[index].ID_Utilizador, lista_movimentos[index].Tipo, lista_movimentos[index].Valor, lista_movimentos[index].Data_Hora.Dia, lista_movimentos[index].Data_Hora.Mes, lista_movimentos[index].Data_Hora.Ano, lista_movimentos[index].Data_Hora.Hora, lista_movimentos[index].Data_Hora.Minuto, lista_movimentos[index].Data_Hora.Segundo);
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

/* Importa os dados das Transacoes para o programa apartir de ficheiros Bin
Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
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

/* Exporta os dados das Transacoes para o programa apartir de ficheiros Bin
Parametros:
 - Caminho do ficheiro
 - Lista
 - Número de registo*/
void exportFicheiroBinTransacoes(char filePath[], Transacao lista_movimentos[], int contador_movimentos, bool rescreber)
{
    // Vai validar se existem informação a ser guardada
    if (contador_movimentos <= 0)
        return;
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

/* Insire os dados do novo utilizador, obtidos dos CSV, na struct pasada por referencia
 Parametros:
 - Informação obtida
 - Struct onde guardar informação
 - Coluna a ser analizada
 */
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

/* Insire os dados da nova escola, obtidos dos CSV, na struct pasada por referencia
 Parametros:
 - Informação obtida
 - Struct onde guardar informação
 - Coluna a ser analizada
 */
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

/* Insire os dados da nova escola, obtidos dos CSV, na struct pasada por referencia
 Parametros:
 - Informação obtida
 - Struct onde guardar informação
 - Coluna a ser analizada
 */
void guardarDadosTransacao(char novo_dado[], Transacao *dados_antigos, int coluna)
{
    int index_data = 0;
    char *dados_data;
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
        // Divide a string da data
        dados_data = strtok(novo_dado, "/");
        // loop pelos dados da data
        while (dados_data)
        {
            switch (index_data)
            {
            case 0:
                dados_antigos->Data_Hora.Dia = atoi(dados_data);
                break;
            case 1:
                dados_antigos->Data_Hora.Mes = atoi(dados_data);
                break;
            case 2:
                dados_antigos->Data_Hora.Ano = atoi(dados_data);
                break;
            }
            index_data++;
            dados_data = strtok(NULL, "/");
        }
        break;
    case 5: // Save Tempo do movimento
        // Divide a string do tempo
        dados_data = strtok(novo_dado, ":");
        // loop pelos dados do tempo
        while (dados_data)
        {
            switch (index_data)
            {
            case 0:
                dados_antigos->Data_Hora.Hora = atoi(dados_data);
                break;
            case 1:
                dados_antigos->Data_Hora.Minuto = atoi(dados_data);
                break;
            case 2:
                dados_antigos->Data_Hora.Segundo = atoi(dados_data);
                break;
            }
            index_data++;
            dados_data = strtok(NULL, ":");
        }
        break;
    }
}

/* Função Cria um utilizador
 Parametros:
 - Novo utilizador no array
 - Id do novo utilizador //Dinamisar
 - Contador de registos de utilizadores
 - Lista de Escolas
 - Contador de registos de escolas */
void crearUtilizador(Utilizador *utilizador_actual, int id_novo_utilizador, int *contador_utilizadores, Escola lista_escolas[], int contador_Escolas)
{
    // Faz uma copia do utilizador pasado por referencia
    Utilizador novo_utilizador = *utilizador_actual;
    char resposta;
    system("cls");
    do
    {
        // Atribui uma id
        // TODO criar uma id mais dinâmica
        novo_utilizador.ID = id_novo_utilizador;
        // Atribui a escola escolhida pelo utilizador
        novo_utilizador.ID_Escola = escolherEscola(lista_escolas, contador_Escolas).ID;
        // Atribui o nome inserido pelo utilizador
        obterString("Insira o seu nome utilizador: ", novo_utilizador.Nome);
        // Atribui o NIF inserido pelo utilizador
        novo_utilizador.NIF = obterInt("Insira o seu NIF:");
        // Atribui o tipo de utilizador inserido pelo utilizador
        strcpy(novo_utilizador.Tipo, TIPO_UTILIZADOR[pedirTipoUtilizador()]);
        // Atribui o email inserido pelo utilizador
        obterString("Insira o seu Email: ", novo_utilizador.Email);
        // Atribui o valor inicial na conta inserido pelo utilizador
        novo_utilizador.Valor_Conta = obterFloat("Insira o seu valor inicial:");
        system("cls");
        // Mostra o dados o utilizador antes de guardar
        ImprimeUtilizador(novo_utilizador);
        // Permite ver o utilizador se os dados que quere inserir estão corretos
        resposta = validacaoCharacter("\nTem a certeza que quere inserir estes dados?\nY-Sim\nN-Nao\nX-Cancelar novo registo", "YNX");
    } while (resposta == 'N');
    if (resposta == 'Y')
    {
        // Guarda os dados no sistema
        *utilizador_actual = novo_utilizador;
        (*contador_utilizadores)++;
    }
    else
    {
        system("cls");
        printf("Novo movimento cancelado");
        esperarEnter();
    }
}

/* Função Cria uma nova escola
 Parametros:
 - Nova escola no array
 - Contador de registos de escolas */
void crearEscola(Escola *escola_actual, int *contador_escolas)
{
    // Faz uma copia da escola pasado por referencia
    Escola nova_escola = *escola_actual;
    char resposta;
    system("cls");
    do
    {
        // Atribui uma nova id
        if (validacaoBinaria("Quere inserir a id manualmente?"))
            nova_escola.ID = obterInt("Insira Id da nova escola: ");
        else
            nova_escola.ID = contador_escolas + 1;
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
        // Permite ver ao utilizador se os dados que quere inserir estão corretos
        resposta = validacaoCharacter("\nTem a certeza que quere inserir estes dados?\nY-Sim\nN-Nao\nX-Cancelar novo registo", "YNX");
    } while (resposta == 'N');

    if (resposta == 'Y')
    {
        // Guarda os dados no sistema
        *escola_actual = nova_escola;
        (*contador_escolas)++;
    }
    else
    {
        system("cls");
        printf("Nova escola cancelado");
        esperarEnter();
    }
}

/* Função criar Transação
 Parametros:
 - Utilizar do movimento
 - Novo movimento
 - Contador de movimentos*/
void crearMovimento(Utilizador *user, Transacao *movimento, int *contador_movimentos)
{
    Transacao novo_movimento = *movimento;
    int valor_movimento = 0;
    bool valor_validator = FALSE;
    char resposta;
    char texto[255];
    system("cls");
    do
    {
        // Atribui uma id
        novo_movimento.ID = *contador_movimentos + 1;
        // Atribui id user
        novo_movimento.ID_Utilizador = user->ID;
        // Atribui o tipo de movimento
        strcpy(novo_movimento.Tipo, TIPO_MOVIMENTO[pedirTipoMovimento()]);
        snprintf(texto, sizeof(texto), "Utilizador %d.%s\nTipo de movimento: %s\nInsira o valor do movimento:", user->ID, user->Nome, novo_movimento.Tipo);
        // Atribui o valor do movimento
        if (!strcmp(novo_movimento.Tipo, TIPO_MOVIMENTO[0]))
        {
            do
            {
                valor_movimento = obterFloat(texto);
                valor_validator = valor_movimento > user->Valor_Conta;
                if (valor_validator)
                {
                    printf("Não tem dinheiro suficiente na conta.");
                }
            } while (valor_validator);
            novo_movimento.Valor = valor_movimento;
        }
        else
            novo_movimento.Valor = obterFloat(texto);
        obterData_Hora(&novo_movimento.Data_Hora);
        system("cls");
        // Mostra o dados do movimento antes de guardar
        ImprimeTransacao(novo_movimento);
        // Permite ver o utilizador se os dados que quere inserir estão corretos
        resposta = validacaoCharacter("\nTem a certeza que quere inserir estes dados?\nY-Sim\nN-Nao\nX-Cancelar novo registo", "YNX");
    } while (resposta == 'N');

    if (resposta == 'Y')
    {
        if (!strcmp(novo_movimento.Tipo, TIPO_MOVIMENTO[0]))
            user->Valor_Conta -= novo_movimento.Valor;
        else
            user->Valor_Conta += novo_movimento.Valor;
        // Guarda os dados no sistema
        *movimento = novo_movimento;
        (*contador_movimentos)++;
    }
    else
    {
        system("cls");
        printf("Novo movimento cancelado");
        esperarEnter();
    }
}

/* Função obter index do Utilizador
 Parametros:
 - Lista de utilizadores
 - Contador de utilizadores*/
int obter_index_utilizador(Utilizador lista_utilizadores[], int contador_utilizadores)
{
    int index = 0, index_utilizador = -1;
    int user_numb = obterInt("Insira o numero do utilizador:");
    do
    {
        if (user_numb == lista_utilizadores[index].ID)
            index_utilizador = index;
        index++;
    } while (index < contador_utilizadores && index_utilizador == -1);

    return index_utilizador;
}

/* Função total de faturação por escola
 Parametros:
 - Lista de utilizadores
 - Lista de escolas
 - Lista de movimentos
 - Contadores das listas*/
void calculoTotalFaturacao(Utilizador lista_utilizadores[], Escola lista_escolas[], Transacao lista_movimentos[], int contador_utilizadores, int contador_escolas, int contador_movimentos)
{
    float valor_faturacao;
    int index_escola, index_utilizador, index_movimentos;
    system("cls");
    printf("Faturacao das escolas");
    // Vai procesar cada escola registada
    for (index_escola = 0; index_escola < contador_escolas; index_escola++)
    {
        // Reinicia o valor de faturação da escola
        valor_faturacao = 0;
        // Vai procesar os movimentos registados
        for (index_movimentos = 0; index_movimentos < contador_movimentos; index_movimentos++)
        {
            // Vair procesar os utilizadores resgistados
            for (index_utilizador = 0; index_utilizador < contador_utilizadores; index_utilizador++)
            {
                // Verifica a escola do utilizador, os moviemntos do utilizador e tipo de movimento
                if (lista_utilizadores[index_utilizador].ID_Escola == lista_escolas[index_escola].ID && lista_movimentos[index_movimentos].ID_Utilizador == lista_utilizadores[index_utilizador].ID && !strcmp(lista_movimentos[index_movimentos].Tipo, TIPO_MOVIMENTO[0]))
                    valor_faturacao += lista_movimentos[index_movimentos].Valor;
            }
        }
        // Mostra informação da escola
        printf("\nTotal faturado pela escola %s: %.2f euros", lista_escolas[index_escola].Nome, valor_faturacao);
    }
    esperarEnter();
}

/* Função calcular percentagem de tipo de movimentos
 Parametros:
 - Lista de utilizadores
 - Lista de escolas
 - Lista de movimentos
 - Contadores das listas*/
void calculoPercentagemMovimentos(Utilizador lista_utilizadores[], Escola lista_escolas[], Transacao lista_movimentos[], int contador_utilizadores, int contador_escolas, int contador_movimentos)
{
    int contador_pagamento = 0, contador_carregamento = 0, contador_movimentos_escola = 0;
    int index_escola, index_utilizador, index_movimentos;
    system("cls");
    printf("Percentagem de movimentos das escolas");
    // Vai procesar cada escola registada
    for (index_escola = 0; index_escola < contador_escolas; index_escola++)
    {
        contador_movimentos_escola = 0;
        contador_pagamento = 0;
        contador_carregamento = 0;
        // Vai procesar os movimentos registados
        for (index_movimentos = 0; index_movimentos < contador_movimentos; index_movimentos++)
        {
            // Vair procesar os utilizadores resgistados
            for (index_utilizador = 0; index_utilizador < contador_utilizadores; index_utilizador++)
            {
                // Verifica a escola do utilizador, os moviemntos do utilizador e tipo de movimento
                if (lista_utilizadores[index_utilizador].ID_Escola == lista_escolas[index_escola].ID && lista_movimentos[index_movimentos].ID_Utilizador == lista_utilizadores[index_utilizador].ID)
                {
                    contador_movimentos_escola += 1;
                    if (!strcmp(lista_movimentos[index_movimentos].Tipo, TIPO_MOVIMENTO[0]))
                        contador_pagamento += 1;
                    else
                        contador_carregamento += 1;
                }
            }
        }
        // Mostra informação da escola
        printf("\n\nPercentagem movimentos escola %s", lista_escolas[index_escola].Nome);
        printf("\n\tPercentagem pagamentos: %.2f %%", calculoPercentagem(contador_pagamento, contador_movimentos_escola));
        printf("\n\tPercentagem carregamento: %.2f %%", calculoPercentagem(contador_carregamento, contador_movimentos_escola));
    }
    esperarEnter();
}

/* Função para calcular total de pagamentos entre duas datas por tipo de utilizador
 Parametros:
 - Lista de utilizadores
 - Lista de escolas
 - Lista de movimentos
 - Contadores das listas*/
void calculoTotalUtilizadorEntreDatas(Utilizador lista_utilizadores[], Escola lista_escolas[], Transacao lista_movimentos[], int contador_utilizadores, int contador_escolas, int contador_movimentos)
{
    // Valor total a calcular
    float valor_total = 0;
    // Index's das listas
    int index_escola, index_utilizador, index_movimentos;
    // Tipos de movimento e utilizador a pesquisar
    int tipo_movimento, tipo_utilizador;
    //
    bool validation;
    // Datas
    Tempo data_inicial, data_final;
    //
    tipo_movimento = pedirTipoMovimento();
    tipo_utilizador = pedirTipoUtilizador();
    data_inicial = obterData("Insira os dados da data inicial.\n");
    do
    {
        data_final = obterData("Insira os dados da data final.\n");
        validation = !validarDataInicialDataFinal(data_inicial, data_final);
        if (validation)
        {
            printf("Insira uma data final [%d/%d/%d] superior a inicial [%d/%d/%d]", data_inicial.Dia, data_inicial.Mes, data_inicial.Ano, data_final.Dia, data_final.Mes, data_final.Ano);
            esperarEnter();
        }
    } while (validation);

    system("cls");
    printf("Data inicial: %d/%d/%d\nData Final: %d/%d/%d\n", data_inicial.Dia, data_inicial.Mes, data_inicial.Ano, data_final.Dia, data_final.Mes, data_final.Ano);

    // Vair procesar os utilizadores resgistados
    for (index_utilizador = 0; index_utilizador < contador_utilizadores; index_utilizador++)
    {
        if (!strcmp(lista_utilizadores[index_utilizador].Tipo, TIPO_UTILIZADOR[tipo_utilizador]))
            // Vai procesar os movimentos registados
            for (index_movimentos = 0; index_movimentos < contador_movimentos; index_movimentos++)
            {
                // Verifica a escola do utilizador, os moviemntos do utilizador e tipo de movimento
                if (lista_movimentos[index_movimentos].ID_Utilizador == lista_utilizadores[index_utilizador].ID &&
                    !strcmp(lista_movimentos[index_movimentos].Tipo, TIPO_MOVIMENTO[tipo_movimento]) &&
                    validarDataMovimento(lista_movimentos[index_movimentos].Data_Hora, data_inicial, data_final))
                    valor_total += lista_movimentos[index_movimentos].Valor;
            }
    }
    printf("Valor total dos %s dos %s: %.2f", TIPO_MOVIMENTO[tipo_movimento], TIPO_UTILIZADOR[tipo_utilizador], valor_total);
    esperarEnter();
}

/* Validação da data inicial e data final
 Parametros:
 - Data inicial
 - Data final*/
bool validarDataInicialDataFinal(Tempo data_inicial, Tempo data_final)
{

    // Verifica se o ano da data menor e maior que o ano da data maior
    bool validation = (data_inicial.Ano <= data_final.Ano);

    if (validation)
        if (data_inicial.Ano == data_final.Ano)
        {
            validation = (data_inicial.Mes < data_final.Mes);
            if (data_inicial.Mes == data_final.Mes)
                validation = (data_inicial.Dia < data_final.Dia);
        }

    return validation;
}

// Valição da uma data dentro de uns parametros
bool validarDataMovimento(Tempo data_verificar, Tempo start, Tempo end)
{
    // Validação dentro dos parametros
    bool validation = (start.Ano <= data_verificar.Ano && data_verificar.Ano <= end.Ano);

    // Se estiver dentro dos parametros
    if (validation)
    {
        // Verifica se tem o mesmo ano que a inicial
        if (start.Ano == data_verificar.Ano)
        {
            // Verifica o mes
            validation = (start.Mes <= data_verificar.Mes);
            // Verifica se esta no mesmo mes que a data inicial
            if (start.Mes == data_verificar.Mes)
                validation = (start.Dia <= data_verificar.Dia);
        }
        // Verifica se tem o mesmo ano que a final
        else if (end.Ano == data_verificar.Ano)
        {
            // Verifica o mes
            validation = (end.Mes >= data_verificar.Mes);
            // Verifica se esta no mesmo mes que a data final
            if (end.Mes == data_verificar.Mes)
                validation = (end.Dia >= data_verificar.Dia);
        }
    }
    return validation;
}