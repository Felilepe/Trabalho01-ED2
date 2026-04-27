#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quadra.h"

#include "lista.h"


#define OPACITY 0.5


FILE* startSVG(const char* file_path) {
	FILE* svg = fopen(file_path, "w");
	if (svg == NULL) {
		perror("Erro: Arquivo NULL.");
		exit(1);
	}

     fprintf(svg, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
     fprintf(svg, "<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"1000\" height=\"1000\">\n");

 

	return svg;
}

void stopSVG(FILE *file_name)
{
    if (file_name == NULL) return;


    fprintf(file_name, "</svg>\n");

    fclose(file_name);
}



void svgInsertQuadra(FILE *file_name, Quadra r)
{
    fprintf(file_name, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
	    quadraGetID(r), quadraGetCoordX(r), quadraGetCoordY(r), quadraGetWidth(r), quadraGetHeight(r), quadraGetCorBorda(r), 
        quadraGetCorPreench(r), OPACITY, quadraGetBordaWidth);
}




static void callback_inserir_quadra(void* item, void* aux) 
{
    Quadra r = (Quadra)item;
    FILE* arquivo = (FILE*)aux; 
    
    svgInsertQuadra(arquivo, r);
}

FILE* createSVG(const char *svg_path, Lista *formas)
{
    if (formas == NULL) {
        fprintf(stderr, "Aviso: lista NULL em createSVG (%s)\n", svg_path);
        return NULL;
    }

    FILE *arquivo_svg = startSVG(svg_path);
    if (arquivo_svg == NULL) {
        printf("Erro ao abrir arquivo SVG.\n");
        return NULL;
    }

    lista_passthrough(formas, callback_inserir_quadra, arquivo_svg);

    return arquivo_svg;
}
