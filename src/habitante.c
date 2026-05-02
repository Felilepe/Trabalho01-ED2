#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "habitante.h"

typedef struct st_habitante
{
    int cpf;
    char* nome, *sobrenome;
    char sexo;
    char* nascimento;
}Habitante;

habitante habitanteCreate(int cpf, char* nome, char* sobrenome, char sexo, char* nascimento)
{
    Habitante *h = malloc(sizeof(Habitante));
    if(h == NULL){
        printf("ERRO: falha ao alocar memória para ponteiro em habitanteCreate");
        return NULL;
    }

    h -> cpf = cpf;

    h -> nome = malloc(strlen(nome) + 1);
    if(h -> nome == NULL){
        printf("Erro: falha ao alocar memória para nome em habitanteCreate");
        return NULL;
    }
    strcpy(h -> nome, nome);

    h -> sobrenome = malloc(strlen(sobrenome) + 1);
    if(h -> sobrenome == NULL){
        printf("Erro: falha ao alocar memória para sobrenome em habitanteCreate");
        return NULL;
    }
    strcpy(h -> sobrenome, sobrenome);
    
    h -> sexo = sexo;

    h -> nascimento = malloc(strlen(nascimento) + 1);
    if(h -> nascimento == NULL){
        printf("Erro: falha ao alocar memória para data de nascimento em habitanteCreate");
        return NULL;
    }
    strcpy(h -> nascimento, nascimento);
    

    return h;
}



int habitanteGetCpf(habitante h){return ((Habitante*)h) -> cpf;}

char* habitanteGetNome(habitante h){return ((Habitante*)h) -> nome;}

char* habitanteGetSobrenome(habitante h){return ((Habitante*)h) -> sobrenome;}

char habitanteGetSexo(habitante h){return ((Habitante*)h) -> sexo;}

char* habitanteGetNascimento(habitante h){return ((Habitante*)h) -> nascimento;}



void habitanteSetCpf(habitante h, int cpf)
{
    ((Habitante*)h) -> cpf = cpf;
}

void habitanteSetNome(habitante h, char* nome)
{
    Habitante *hab = (Habitante*)h;
    free(hab -> nome);
    hab -> nome = malloc(strlen(nome) + 1);
    strcpy(hab -> nome, nome);
}

void habitanteSetSobrenome(habitante h, char* sobrenome)
{
    Habitante *hab = (Habitante*)h;
    free(hab -> sobrenome);
    hab -> sobrenome = malloc(strlen(sobrenome) + 1);
    strcpy(hab -> sobrenome, sobrenome);
}

void habitanteSetSexo(habitante h, char sexo)
{
((Habitante*)h) -> sexo = sexo;
}

void habitanteSetNascimento(habitante h, char* nascimento)
{
    Habitante *hab = (Habitante*)h;
    free(hab -> nascimento);
    hab -> nascimento = malloc(strlen(nascimento) + 1);
    strcpy(hab -> nascimento, nascimento);
}



void habitanteDestroy(habitante h)
{
    Habitante *hab = (Habitante*)h;
    free(hab -> nome);
    free(hab -> sobrenome);
    free(hab -> nascimento);
    free(hab);

}