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
#include <locale.h>
#include <stdbool.h>
#include <errno.h>

#define *TIPO_UTILIZADOR[] = {"Estudante","Docente","Funcion�rio"}

bool validacaoBinaria(char[]);
void carregarCSV(char []);

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
    if(validacaoBinaria("Quere carregar os dados apartir de ficheiros csv?")) {
        system("cls");
        printf("Carregando Ficheiros CSV\n\n");
        carregarCSV(caminhoEscolas);
        printf("Finish reading file %s\n\n\n",caminhoEscolas);
        carregarCSV(caminhoUtilizadores);
        printf("Finish reading file %s\n\n\n",caminhoUtilizadores);
        carregarCSV(caminhoMovimentos);
        printf("Finish reading file %s\n\n\n",caminhoMovimentos);
    }

    if(validacaoBinaria("Quere inserir dados manualmente?")) {
        system("cls");
        resposta = validacaoCharacter("Escolha o tipo de da dados que quere inserir.\nU - Utilizador\nE - Escola\X - Cancelar\n\n->", "UEX");
        switch(resposta) {
            case 'U':
                crearUtilizador();
                break;
            case 'E':
                //TODO Crear Escola
                break;
        }
    }
}

// Valida��o para quest�es de Sim ou N�o
bool validacaoBinaria(char texto[]) {
    printf("\nY - Sim\nN - N�o\n\n-->");
    char resposta;
    do {
        //system("cls");
        printf("%s", texto);
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    }while(resposta != 'Y' && resposta != 'N');
    return resposta == 'Y' ? true : false;
}

// Valida��o de leitura de char's
char validacaoCharacter(char text[], char valores_validos[]) {
    char resposta;
    do{
        system("cls");
        printf(text);
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    }while(!strchr(valores_validos, resposta));
    return resposta;
}

// Lee informa��o dos ficheiros CSV que forem pasados
void carregarCSV(char filePath[]) {
    FILE* fileStream = fopen(filePath, "r");
    if (fileStream) {
        char buffer[1024];

        int row = 0;
        int column = 0;

        while (fgets(buffer, 1024, fileStream)) {
            column = 0;
            row++;
            //if (row == 1) continue;

            // Splitting the data
            char* value = strtok(buffer, ";");
            while (value) {
                printf("%s\t", value);
                value = strtok(NULL, ";");
                column++;
            }

            printf("\n");
        }

        // Close the file
        fclose(fileStream);
    }else{
        printf("Error opening file: %s\n", filePath);
        perror("Error");
    }
}

// Devolve a informa��o de uma string no campo especificado
const char* getfield(char* line, int num) {
	const char* tok;
	for (tok = strtok(line, ";");
			tok && *tok;
			tok = strtok(NULL, ";\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}

// Cria um utilizador
void crearUtilizador(void) {
    //TODO
    char tipo_escola, nome[100], nif[9], *tipo_utilizador;
    system("cls");
    do{
        tipo_escola = validacaoCharacter("Escolha a sua escola\n2 - Escola Superior de Tecnologia e Gest�o(ESTG)\n3 - Escola Superior de Turismo e Tecnologia do Mar(ESTM)"), "23");
        nome = obterString("Insira o seu nome:");
        nif = obterString("Insira o seu NIF:");
        tipo_utilizador =
    }while(/*Condi��o utilizador aceitar*/);
}

// Obtem uma string do utilizador
char *obterString(char text[]) {
    char *nome;
    printf(text);
    scanf("%s", &nome);
    return nome;
}

//TODO guardarDadosCSV()
