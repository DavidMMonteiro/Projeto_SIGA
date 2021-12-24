#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct //Estrutura de movimentos
{
    int ID;
    int ID_Utilizador;
    char Tipo[1024];
    float Valor;
    char Data[11];
    char Hora[10];
} Transacao;

void ImprimeTransacao(Transacao transacao)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "\nId.%d \nId Util.%d \nTipo: %s \nValor: %.2f \nData: %s \nHora: %s",
         transacao.ID,transacao.ID_Utilizador,transacao.Tipo,transacao.Valor,transacao.Data,transacao.Hora);

    printf("%s\n",informacao);
}