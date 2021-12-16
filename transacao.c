#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct //Estrutura Transa��o
{
    int ID;
    int ID_Utilizador;
    char Tipo[1024];
    float Valor;
    char Data[11];
    char Hora[10];
} Transacao;

char *ImprimeTransacao(Transacao transacao, bool conseguirString)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "Id.%d \nId Util.%d \nTipo: %s \nValor: %f \nData: %s \nHora: %s",
         transacao.ID,transacao.ID_Utilizador,transacao.Tipo,transacao.Valor,transacao.Data,transacao.Hora);

    if(!conseguirString)printf(informacao);
    else return *informacao;
}