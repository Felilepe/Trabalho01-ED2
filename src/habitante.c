#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "habitante.h"

typedef struct st_habitante
{
    char* nome, *sobrenome, *cpf;
    char sexo;
    char* nascimento;
}Habitante;

habitante habitanteCreate(char* cpf, char* nome, char* sobrenome, char sexo, char* nascimento)
{
    Habitante *h = malloc(sizeof(Habitante));
    if(h == NULL){
        printf("Erro: falha ao alocar memoria para ponteiro em habitanteCreate\n");
        return NULL;
    }

    h -> cpf = malloc(strlen(cpf) + 1);
    if(h -> cpf == NULL){
        printf("Erro: falha ao alocar memoria para cpf em habitanteCreate\n");
        return NULL;
    }
    strcpy(h -> cpf, cpf);

    h -> nome = malloc(strlen(nome) + 1);
    if(h -> nome == NULL){
        printf("Erro: falha ao alocar memoria para nome em habitanteCreate\n");
        return NULL;
    }
    strcpy(h -> nome, nome);

    h -> sobrenome = malloc(strlen(sobrenome) + 1);
    if(h -> sobrenome == NULL){
        printf("Erro: falha ao alocar memoria para sobrenome em habitanteCreate\n");
        return NULL;
    }
    strcpy(h -> sobrenome, sobrenome);
    
    h -> sexo = sexo;

    h -> nascimento = malloc(strlen(nascimento) + 1);
    if(h -> nascimento == NULL){
        printf("Erro: falha ao alocar memoria para data de nascimento em habitanteCreate\n");
        return NULL;
    }
    strcpy(h -> nascimento, nascimento);
    

    return h;
}



char* habitanteGetCpf(habitante h){return ((Habitante*)h) -> cpf;}

char* habitanteGetNome(habitante h){return ((Habitante*)h) -> nome;}

char* habitanteGetSobrenome(habitante h){return ((Habitante*)h) -> sobrenome;}

char habitanteGetSexo(habitante h){return ((Habitante*)h) -> sexo;}

char* habitanteGetNascimento(habitante h){return ((Habitante*)h) -> nascimento;}



void habitanteSetCpf(habitante h, char* cpf)
{
    Habitante *hab = (Habitante*)h;
    free(hab -> cpf);
    hab -> cpf = malloc(strlen(cpf) + 1);
    if(hab -> cpf == NULL){
        printf("Erro: falha ao alocar memoria para cpf em habitanteSetCpf\n");
        return;
    }
    strcpy(hab -> cpf, cpf);
}

void habitanteSetNome(habitante h, char* nome)
{
    Habitante *hab = (Habitante*)h;
    free(hab -> nome);
    hab -> nome = malloc(strlen(nome) + 1);
    if(hab -> nome == NULL){
        printf("Erro: falha ao alocar memoria para nome em habitanteSetNome\n");
        return;
    }
    strcpy(hab -> nome, nome);
}

void habitanteSetSobrenome(habitante h, char* sobrenome)
{
    Habitante *hab = (Habitante*)h;
    free(hab -> sobrenome);
    hab -> sobrenome = malloc(strlen(sobrenome) + 1);
    if(hab -> sobrenome == NULL){
        printf("Erro: falha ao alocar memoria para sobrenome em habitanteSetSobrenome\n");
        return;
    }
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
    if(hab -> nascimento == NULL){
        printf("Erro: falha ao alocar memoria para data de nascimento em habitanteSetNascimento\n");
        return;
    }
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