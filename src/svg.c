#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quadra.h"
#include "svg.h"

#include "lista.h"


#define OPACITY 0.5
#define DEFAULT_WIDTH 1.5
#define DEFAULT_FONT "Arial"
#define DEFAULT_WEIGHT 400


static void insert_text(FILE *file_name, double x, double y, char* texto, char *corborda, char*corpreench, int tamanho, char* anchor)
{
    fprintf(file_name, "\t<text x=\"%lf\" y=\"%lf\" stroke=\"%s\" fill=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%spt\" text-anchor=\"%s\">%s</text>\n",
       x, y, corborda, corpreench, DEFAULT_FONT, DEFAULT_WEIGHT, tamanho, anchor, texto);
}

static void insert_square(FILE *file_name, double x, double y, double w, double h, char* corpreench, char* corborda)
{
    fprintf(file_name, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
	    x, y, w, h, corpreench, corborda, OPACITY, DEFAULT_WIDTH);
}

static void insert_circle(FILE *file_name, double x, double y, double r, char* corpreench, char* corborda)
{
    fprintf(file_name, " <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
    x, y, r, corpreench, corborda, OPACITY, DEFAULT_WIDTH);
}

static void callback_inserir_quadra(void* item, void* aux) 
{
    Quadra r = (Quadra)item;
    FILE* arquivo = (FILE*)aux; 
    
    svgInsertQuadra(arquivo, r);
}



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
	    quadraGetCep(r), quadraGetCoordX(r), quadraGetCoordY(r), quadraGetWidth(r), quadraGetHeight(r), quadraGetCorBorda(r), 
        quadraGetCorPreench(r), OPACITY, quadraGetBordaWidth(r));
}

void svgMarcaRedCross(FILE *file_name, double x, double y)
{
    insert_text(file_name, x, y, "†", "red", "red", 12, "middle");
}

void svgMarcaBlackCircle(FILE *file_name, double x, double y)
{
    insert_circle(file_name, x, y, 5, "black", "black");
}

void svgMarcaRedSquare(FILE *file_name, double x, double y)
{
    insert_square(file_name, x, y, 5, 5, "red", "red");
}



void draw(item i, item aux)
{
    FILE* svg = (FILE*)aux;
    svgInsertQuadra(svg, (Quadra)i);
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
