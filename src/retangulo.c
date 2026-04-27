#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "retangulo.h"

#define TIPO_R 2

typedef struct retangulo 
{
    int id, type;
    double x, y, w, h;
    char *corborda, *corpreench;

}retangulo;



Retangulo retangulo_create(int id, double x, double y, double w, double h, char *corborda, char*corpreench)
{
    retangulo *ret= malloc(sizeof(retangulo));
    if(ret == NULL){
        printf("Erro na alocação de memória ao criar retangulo");
        exit(1);
    }
    
    ret -> id = id;
    ret -> type = TIPO_R;
    ret -> x = x;
    ret -> y = y;
    ret -> w = w;
    ret -> h = h;
    
    ret -> corborda = (char*)malloc(strlen(corborda) + 1);
    if(ret -> corborda == NULL){
        printf("Erro na alocação da memorio ao atribuir cor da borda");
        exit(1);
    }
    strcpy(ret -> corborda, corborda);

    ret -> corpreench = (char*)malloc(strlen(corpreench) + 1);
    if(ret -> corpreench == NULL){
        printf("Erro na alocacao de memoria ao atribuir cor de preenchimento");
        exit(1);
    }
    strcpy(ret -> corpreench, corpreench);

    return ret;
}


double retangulo_getCoordX(Retangulo r) {return ((retangulo*)r) -> x;}

double retangulo_getCoordY(Retangulo r) {return ((retangulo*)r) -> y;}

int retangulo_getID(Retangulo r) {return((retangulo*)r) -> id;}

double retangulo_getHeight(Retangulo r) {return ((retangulo*)r) -> h;}

double retangulo_getWidth(Retangulo r) {return ((retangulo*)r) -> w;}

char* retangulo_getCorBorda(Retangulo r) {return ((retangulo*)r) -> corborda;}

char* retangulo_getCorPreench(Retangulo r) {return ((retangulo*)r) -> corpreench;}

int retangulo_getType(Retangulo r) {return ((retangulo*)r) -> type;}

double retangulo_calcArea(Retangulo r)
{
    double altura = ((retangulo*)r) -> h;
    double largura = ((retangulo*)r) -> w;
    
    return altura * largura;
}


void retangulo_setCoordX(Retangulo r, double x) {((retangulo*)r) -> x = x;}

void retangulo_setCoordY(Retangulo r, double y) {((retangulo*)r) -> y = y;}

void retangulo_setHeight(Retangulo r, double h) {((retangulo*)r) -> h = h;}

void retangulo_setWidth(Retangulo r, double w) {((retangulo*)r) -> w = w;}

void retangulo_setCorBorda(Retangulo r, char* corborda)
{
    free(((retangulo*)r) -> corborda);
    ((retangulo*)r) -> corborda = (char*)malloc(strlen(corborda) + 1);
    strcpy(((retangulo*)r) -> corborda, corborda);
}

void retangulo_setCorPreench(Retangulo r, char* corpreench)
{
    free(((retangulo*)r) -> corpreench);
    ((retangulo*)r) -> corpreench = (char*)malloc(strlen(corpreench) + 1);
    strcpy(((retangulo*)r) -> corpreench, corpreench);
}


void retangulo_destroy(Retangulo r)
{
    free(((retangulo*)r) -> corborda);
    free(((retangulo*)r) -> corpreench);
    free(r);
}



Lista *retangulo_toAnteparo(Retangulo r, int *id_next)
{
    Lista *anteparos = lista_create();

    if(anteparos == NULL){
        printf("Erro: lista_create retornou pointer nulo em retangulo_toAnteparo.");
        exit(1);
    }

    double x = retangulo_getCoordX(r);
    double y = retangulo_getCoordY(r);
    double h = retangulo_getHeight(r);
    double w = retangulo_getWidth(r);
    char *cor = retangulo_getCorBorda(r);

    Anteparo topo = anteparo_create(++(*id_next), x,(y + h), (x + w), (y + h), cor);
    Anteparo esq = anteparo_create(++(*id_next), x, y, x, (y + h), cor);
    Anteparo dir = anteparo_create(++(*id_next), (x + w), y, (x + w), (y + h), cor);
    Anteparo chao = anteparo_create(++(*id_next), x, y, (x + w), y, cor);

    lista_insertTail(anteparos, topo);      
    lista_insertTail(anteparos, esq);
    lista_insertTail(anteparos, dir);
    lista_insertTail(anteparos, chao);

    return anteparos;
}