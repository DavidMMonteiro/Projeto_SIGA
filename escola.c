#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct //Estrutura Escola
{
    int ID;
    char Nome[1024];
    char Abreviatura[5];
    char Campus[1024];
    char Localidade[1024];
} Escola;

void ImprimeEscola(Escola escola, bool conseguirString)
{
    char informacao[1024];

    snprintf(informacao, sizeof(informacao), "Id.%d \nNome: %s \nAbreviatura: %s \nCampus: %s \nLocalidade: %s",
         escola.ID,escola.Nome,escola.Abreviatura,escola.Campus,escola.Localidade);

    if(!conseguirString)printf(informacao);
    //else return *informacao;
}
