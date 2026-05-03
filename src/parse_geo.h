#ifndef PARSE_GEO_H
#define PARSE_GEO_H

#include <stdio.h>
#include <stdbool.h>
#include "hte.h"

/**
 * @brief Lê o arquivo .geo e popula o hashfile de quadras e o SVG base.
 * Para cada linha do arquivo:
 * q  cep x y w h      →  insere QuadraReg em h_quadras, desenha retângulo e CEP no SVG.
 * cq sw cfill cstrk   →  atualiza as cores e espessura padrão para as quadras seguintes.
 * A chave no hashfile é o CEP da quadra.
 * @param caminho   Caminho completo do arquivo .geo.
 * @param h_quadras Hashfile de quadras (cep -> QuadraReg).
 * @param svg       Arquivo SVG já aberto com startSVG().
 **/
void parseGeo(const char *caminho, Hash h_quadras, FILE *svg);

/**
 * @brief Recupera os dados de uma quadra do hashfile.
 * @param h_quadras Hashfile de quadras.
 * @param cep       CEP da quadra a buscar.
 * @param x         Ponteiro de saída para a coordenada X.
 * @param y         Ponteiro de saída para a coordenada Y.
 * @param w         Ponteiro de saída para a largura.
 * @param h         Ponteiro de saída para a altura.
 * @param sw        Ponteiro de saída para a espessura da borda.
 * @param cfill     Buffer de saída para a cor de preenchimento (mín. 30 bytes).
 * @param cstrk     Buffer de saída para a cor da borda         (mín. 30 bytes).
 * @return true se encontrada, false caso contrário.
 **/
bool geoGetQuadra(Hash h_quadras, char *cep, double *x, double *y, double *w, double *h, double *sw, char *cfill, char *cstrk);

#endif