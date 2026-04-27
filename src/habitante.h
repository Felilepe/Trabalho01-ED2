#ifndef HABITANTE_H
#define HABITANTE_H

typedef void* habitante;

habitante habitanteCreate(int cpf, char* nome, char* sobrenome, char sexo, char* nascimento);



int habitanteGetCpf(habitante h);

char* habitanteGetNome(habitante h);

char* habitanteGetSobrenome(habitante h);

char habitanteGetSexo(habitante h);

char* habitanteGetnascimento(habitante h);



void habitanteSetCpf(habitante h, int cpf);

void habitanteSetNome(habitante h, char* nome);

void habitanteSetSobrenome(habitante h, char* sobrenome);

void habitanteSetSexo(habitante h, char sexo);

void habitanteSetNascimento(habitante h, char* nascimento);



void habitanteDestroy(habitante h);

#endif