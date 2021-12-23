#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct //Estrutura Escola
{
    int ID;
    char Nome[1024];
    char Abreviatura[10];
    char Campus[1024];
    char Localidade[1024];
} Escola;

void ImprimeEscola(Escola escola)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "\n\nId.%d \nNome: %s \nAbreviatura: %s \nCampus: %s \nLocalidade: %s",
         escola.ID,escola.Nome,escola.Abreviatura,escola.Campus,escola.Localidade);

    printf("%s",informacao);
}