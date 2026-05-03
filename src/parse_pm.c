#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse_pm.h"
#include "hte.h"
 
#define MAX_LINHA 256
 
 
typedef struct {
    char cpf[20];
    char nome[50];
    char sobrenome[50];
    char sexo;
    char nascimento[12];
} HabitanteReg;
 
typedef struct {
    char cpf[20];
    char cep[20];
    char face;
    int  num;
    char complemento[50];
} MoradorReg;
 
 
static void processar_p(const char *linha, Hash h_hab)
{
    HabitanteReg reg;
    char sexo_str[4];
 
    int lidos = sscanf(linha,
                       "%*s %19s %49s %49s %3s %11s",
                       reg.cpf,
                       reg.nome,
                       reg.sobrenome,
                       sexo_str,
                       reg.nascimento);
 
    if (lidos != 5) {
        fprintf(stderr, "AVISO: linha 'p' malformada: %s", linha);
        return;
    }
 
    reg.sexo = sexo_str[0];
 
    hashInsertReg(h_hab, reg.cpf, &reg, sizeof(HabitanteReg));
}
 
static void processar_m(const char *linha, Hash h_mor)
{
    MoradorReg reg;
    char face_str[16];
 
    int lidos = sscanf(linha,
                       "%*s %19s %19s %15s %d %49s",
                       reg.cpf,
                       reg.cep,
                       face_str,
                       &reg.num,
                       reg.complemento);
 
    if (lidos != 5) {
        fprintf(stderr, "AVISO: linha 'm' malformada: %s", linha);
        return;
    }
 
    /* "Face.W" -> 'W', "N" -> 'N' */
    char *ponto = strchr(face_str, '.');
    reg.face = ponto ? *(ponto + 1) : face_str[0];
 
    hashInsertReg(h_mor, reg.cpf, &reg, sizeof(MoradorReg));
}
 


void parsePm(const char *caminho, Hash h_hab, Hash h_mor)
{
    if (caminho == NULL || h_hab == NULL || h_mor == NULL) {
        fprintf(stderr, "ERRO: parametro nulo em parsePm\n");
        return;
    }
 
    FILE *pm = fopen(caminho, "r");
    if (pm == NULL) {
        fprintf(stderr, "ERRO: nao foi possivel abrir '%s'\n", caminho);
        return;
    }
 
    char linha[MAX_LINHA];
 
    while (fgets(linha, sizeof(linha), pm) != NULL) {
        if (linha[0] == '\n' || linha[0] == '#') continue;
 
        char cmd[8];
        if (sscanf(linha, "%7s", cmd) != 1) continue;
 
        if      (strcmp(cmd, "p") == 0) processar_p(linha, h_hab);
        else if (strcmp(cmd, "m") == 0) processar_m(linha, h_mor);
        else
            fprintf(stderr, "AVISO: comando desconhecido em .pm: '%s'\n", cmd);
    }
 
    fclose(pm);
}
 
bool pmGetHabitante(Hash h_hab, char *cpf,
                    char *nome, char *sobrenome,
                    char *sexo, char *nascimento)
{
    HabitanteReg reg;
 
    if (!hashGetRegistry(h_hab, cpf, &reg, sizeof(HabitanteReg)))
        return false;
 
    if (nome) {
        strncpy(nome, reg.nome, 49);
        nome[49] = '\0';
    }
    if (sobrenome) {
        strncpy(sobrenome, reg.sobrenome, 49);
        sobrenome[49] = '\0';
    }
    if (sexo)       *sexo = reg.sexo;
    if (nascimento) {
        strncpy(nascimento, reg.nascimento, 11);
        nascimento[11] = '\0';
    }
 
    return true;
}
 
bool pmGetMorador(Hash h_mor, char *cpf,
                  char *cep, char *face,
                  int *num, char *complemento)
{
    MoradorReg reg;
 
    if (!hashGetRegistry(h_mor, cpf, &reg, sizeof(MoradorReg)))
        return false;
 
    if (cep) {
        strncpy(cep, reg.cep, 19);
        cep[19] = '\0';
    }
    if (face)        *face = reg.face;
    if (num)         *num  = reg.num;
    if (complemento) {
        strncpy(complemento, reg.complemento, 49);
        complemento[49] = '\0';
    }
 
    return true;
}
