#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct //Estrutura Utilizador
{
    int ID;
    int ID_Escola;
    char Nome[1024];
    int NIF;
    char Tipo[1024];
    char Email[1024];
    float Valor_Conta;
} Utilizador;

void ImprimeUtilizador(Utilizador util)
{
    char informacao[1024];
    
    snprintf(informacao, sizeof(informacao), "\nId.%d \nId Escola.%d \nNome: %s \nNIF:%d \nTipo Utilizador:%s \nEmail:%s \nValor na Conta: %.2f",
         util.ID,util.ID_Escola,util.Nome,util.NIF,util.Tipo,util.Email,util.Valor_Conta);
    
    printf("%s\n",informacao);
}