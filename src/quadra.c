#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "quadra.h"

#define TIPO_R 2
#define DEFAULT_WIDTH 1.5
#define DEFAULT_STROKE_COLOR "#E06F2D"
#define DEFAULT_FILL_COLOR "#E08E2D"

typedef struct quadra 
{
    int cpf, type;
    double x, y, w, h, sw;
    char *corborda, *corpreench;

}quadra;



Quadra quadraCreate(int cpf, double x, double y, double w, double h)
{
    quadra *ret= malloc(sizeof(quadra));
    if(ret == NULL){
        printf("Erro na alocação de memória ao criar quadra");
        exit(1);
    }
    
    ret -> cpf = cpf;
    ret -> type = TIPO_R;
    ret -> x = x;
    ret -> y = y;
    ret -> w = w;
    ret -> h = h;
    ret -> sw = DEFAULT_WIDTH;
    
    ret -> corborda = (char*)malloc(strlen(DEFAULT_STROKE_COLOR) + 1);
    if(ret -> corborda == NULL){
        printf("Erro na alocação da memorio ao atribuir cor da borda");
        return NULL;
    }
    strcpy(ret -> corborda, DEFAULT_STROKE_COLOR);

    ret -> corpreench = (char*)malloc(strlen(DEFAULT_FILL_COLOR) + 1);
    if(ret -> corpreench == NULL){
        printf("Erro na alocacao de memoria ao atribuir cor de preenchimento");
        return NULL;
    }
    strcpy(ret -> corpreench, DEFAULT_FILL_COLOR);

    return ret;
}


double quadraGetCoordX(Quadra r) {return ((quadra*)r) -> x;}

double quadraGetCoordY(Quadra r) {return ((quadra*)r) -> y;}

int quadraGetID(Quadra r) {return((quadra*)r) -> cpf;}

double quadraGetHeight(Quadra r) {return ((quadra*)r) -> h;}

double quadraGetWidth(Quadra r) {return ((quadra*)r) -> w;}

double quadraGetBordaWidth(Quadra r) {return ((quadra*)r) -> sw;}

char* quadraGetCorBorda(Quadra r) {return ((quadra*)r) -> corborda;}

char* quadraGetCorPreench(Quadra r) {return ((quadra*)r) -> corpreench;}

int quadraGetType(Quadra r) {return ((quadra*)r) -> type;}

double quadra_CalcArea(Quadra r)
{
    double altura = ((quadra*)r) -> h;
    double largura = ((quadra*)r) -> w;
    
    return altura * largura;
}


void quadraSetCoordX(Quadra r, double x) {((quadra*)r) -> x = x;}

void quadraSetCoordY(Quadra r, double y) {((quadra*)r) -> y = y;}

void quadraSetHeight(Quadra r, double h) {((quadra*)r) -> h = h;}

void quadraSetWidth(Quadra r, double w) {((quadra*)r) -> w = w;}

void quadraSetBordaWidth(Quadra r, double sw) {((quadra*)r) -> sw = sw;}

void quadraSetCorBorda(Quadra r, char* corborda)
{
    free(((quadra*)r) -> corborda);
    ((quadra*)r) -> corborda = (char*)malloc(strlen(corborda) + 1);
    strcpy(((quadra*)r) -> corborda, corborda);
}

void quadraSetCorPreench(Quadra r, char* corpreench)
{
    free(((quadra*)r) -> corpreench);
    ((quadra*)r) -> corpreench = (char*)malloc(strlen(corpreench) + 1);
    strcpy(((quadra*)r) -> corpreench, corpreench);
}


void quadraDestroy(Quadra r)
{
    free(((quadra*)r) -> corborda);
    free(((quadra*)r) -> corpreench);
    free(r);
}