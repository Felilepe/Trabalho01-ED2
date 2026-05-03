#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse_geo.h"
#include "hte.h"
#include "quadra.h"
#include "svg.h"

#define MAX_LINHA 256


typedef struct {
    char   cep[20];
    double x, y, w, h, sw;
    char   cfill[30];
    char   cstrk[30];
} QuadraReg;


static double default_sw    = 1.5;
static char   default_fill[30] = "#E08E2D";
static char   default_strk[30] = "#E06F2D";


static void processar_q(const char *linha, Hash h_quadras, FILE *svg)
{
    QuadraReg reg;

    int lidos = sscanf(linha,
                       "%*s %19s %lf %lf %lf %lf",
                       reg.cep,
                       &reg.x, &reg.y,
                       &reg.w, &reg.h);

    if (lidos != 5) {
        fprintf(stderr, "AVISO: linha 'q' malformada: %s", linha);
        return;
    }

    /* Aplica os defaults atuais */
    reg.sw = default_sw;
    strncpy(reg.cfill, default_fill, sizeof(reg.cfill) - 1);
    reg.cfill[sizeof(reg.cfill) - 1] = '\0';
    strncpy(reg.cstrk, default_strk, sizeof(reg.cstrk) - 1);
    reg.cstrk[sizeof(reg.cstrk) - 1] = '\0';

    /* Armazena no hashfile */
    hashInsertReg(h_quadras, reg.cep, &reg, sizeof(QuadraReg));

    /* Cria Quadra temporária apenas para desenhar no SVG */
    Quadra q = quadraCreate(reg.cep, reg.x, reg.y, reg.w, reg.h);
    if (q == NULL) {
        fprintf(stderr, "AVISO: falha ao criar Quadra temporaria para '%s'\n",
                reg.cep);
        return;
    }

    svgInsertQuadra(svg, q);
    svgMarcaCep(svg, q);

    quadraDestroy(q);
}

static void processar_cq(const char *linha)
{
    char cfill[30], cstrk[30];
    double sw;

    int lidos = sscanf(linha,
                       "%*s %lf %29s %29s",
                       &sw, cfill, cstrk);

    if (lidos != 3) {
        fprintf(stderr, "AVISO: linha 'cq' malformada: %s", linha);
        return;
    }

    default_sw = sw;
    strncpy(default_fill, cfill, sizeof(default_fill) - 1);
    default_fill[sizeof(default_fill) - 1] = '\0';
    strncpy(default_strk, cstrk, sizeof(default_strk) - 1);
    default_strk[sizeof(default_strk) - 1] = '\0';
}



void parseGeo(const char *caminho, Hash h_quadras, FILE *svg)
{
    if (caminho == NULL || h_quadras == NULL || svg == NULL) {
        fprintf(stderr, "ERRO: parametro nulo em parseGeo\n");
        return;
    }

    /* Reinicia os defaults a cada chamada */
    default_sw = 1.5;
    strncpy(default_fill, "#E08E2D", sizeof(default_fill) - 1);
    default_fill[sizeof(default_fill) - 1] = '\0';
    strncpy(default_strk, "#E06F2D", sizeof(default_strk) - 1);
    default_strk[sizeof(default_strk) - 1] = '\0';

    FILE *geo = fopen(caminho, "r");
    if (geo == NULL) {
        fprintf(stderr, "ERRO: nao foi possivel abrir '%s'\n", caminho);
        return;
    }

    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), geo) != NULL) {
        if (linha[0] == '\n' || linha[0] == '#') continue;

        char cmd[8];
        if (sscanf(linha, "%7s", cmd) != 1) continue;

        if      (strcmp(cmd, "q")  == 0) processar_q(linha, h_quadras, svg);
        else if (strcmp(cmd, "cq") == 0) processar_cq(linha);
        else
            fprintf(stderr, "AVISO: comando desconhecido em .geo: '%s'\n", cmd);
    }

    fclose(geo);
}

bool geoGetQuadra(Hash h_quadras, char *cep, double *x, double *y, double *w, double *h, double *sw, char *cfill, char *cstrk)
{
    QuadraReg reg;

    if (!hashGetRegistry(h_quadras, cep, &reg, sizeof(QuadraReg)))
        return false;

    if (x)     *x  = reg.x;
    if (y)     *y  = reg.y;
    if (w)     *w  = reg.w;
    if (h)     *h  = reg.h;
    if (sw)    *sw = reg.sw;
    if (cfill) {
        strncpy(cfill, reg.cfill, 29);
        cfill[29] = '\0';
    }
    if (cstrk) {
        strncpy(cstrk, reg.cstrk, 29);
        cstrk[29] = '\0';
    }

    return true;
}