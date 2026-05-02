#ifndef MORADOR_H
#define MORADOR_H

typedef void* Morador;

Morador moradorCreate(char* cpf, char* cep, char face, int num, char* complemento);

void moradorDestroy(Morador m);



char* moradorGetCpf(Morador m);

char* moradorGetCep(Morador m);

char moradorGetFace(Morador m);

int moradorGetNumero(Morador m);

char* moradorGetComplemento(Morador m);



void moradorSetCpf(Morador m, char* cpf);

void moradorSetCep(Morador m, char* cep);

void moradorSetFace(Morador m, char face);

void moradorSetNumero(Morador m, int num);

void moradorSetComplemento(Morador m, char* comp);

#endif