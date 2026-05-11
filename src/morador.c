#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "morador.h"

typedef struct
{
    char* cpf, *cep, *comp;
    char face;
    int num;
}morador;



Morador moradorCreate(char* cpf, char* cep, char face, int num, char* complemento)
{
    morador *m = malloc(sizeof(morador));
    if(m == NULL){
        printf("Erro: falha ao alocar memoria para ponteiro em moradorCreate\n");
        return NULL;
    }

    m -> cpf = (char*)malloc(strlen(cpf) + 1);
    if(m -> cpf == NULL){
        printf("Erro: falha ao alocar memoria para cpf em moradorCreate\n");
        free(m);
        return NULL;
    }
    strcpy(m -> cpf, cpf);

    m -> comp = (char*)malloc(strlen(complemento) + 1);
    if(m -> comp == NULL){
        printf("Erro: falha ao alocar memoria para complemento em moradorCreate\n");
        free(m -> cpf);
        free(m);
        return NULL;
    }
    strcpy(m -> comp, complemento);

    m -> cep = (char*)malloc(strlen(cep) + 1);
    if(m -> cep == NULL){
        printf("Erro: falha ao alocar memoria para cep em moradorCreate\n");
        free(m -> cpf);
        free(m -> comp);
        free(m);
        return NULL;
    }
    strcpy(m -> cep, cep);

    m -> face = face;
    m -> num = num;


    return m;
}



char* moradorGetCpf(Morador m){return ((morador*)m) -> cpf;}

char* moradorGetCep(Morador m){return ((morador*)m) -> cep;}

char* moradorGetComplemento(Morador m){return ((morador*)m) -> comp;}

char moradorGetFace(Morador m){return ((morador*)m) -> face;}

int moradorGetNumero(Morador m){return ((morador*)m) -> num;}



void moradorSetCpf(Morador m, char* cpf)
{
    if (m == NULL || cpf == NULL) return;
    morador *hab = (morador*)m;
    free(hab -> cpf);
    hab -> cpf = (char*)malloc(strlen(cpf) + 1);
    if(hab -> cpf == NULL){
        printf("Erro: falha ao alocar memoria para cpf em moradorSetCpf\n");
        return;
    }
    strcpy(hab -> cpf, cpf);
}

void moradorSetCep(Morador m, char* cep)
{
    if (m == NULL || cep == NULL) return;
    morador *hab = (morador*)m;
    free(hab -> cep);
    hab -> cep = (char*)malloc(strlen(cep) + 1);
    if(hab -> cep == NULL){
        printf("Erro: falha ao alocar memoria para cep em moradorSetCep\n");
        return;
    }
    strcpy(hab -> cep, cep);
}

void moradorSetComplemento(Morador m, char* complemento)
{
    if (m == NULL || complemento == NULL) return;
    morador *hab = (morador*)m;
    free(hab -> comp);
    hab -> comp = (char*)malloc(strlen(complemento) + 1);
    if(hab -> comp == NULL){
        printf("Erro: falha ao alocar memoria para complemento em moradorSetComplemento\n");
        return;
    }
    strcpy(hab -> comp, complemento);
}

void moradorSetFace(Morador m, char face)
{
    morador *hab = (morador*)m;
    hab -> face = face;
}

void moradorSetNumero(Morador m, int num)
{
    morador *hab = (morador*)m;
    hab -> num = num;
}



void moradorDestroy(Morador m)
{
    if (m == NULL) return;
    morador *hab = (morador*)m;
    free(hab -> cep);
    free(hab -> cpf);
    free(hab -> comp);
    free(hab);
}