#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include "quadra.h"
#include "lista.h"


/******************************************************************
*@brief Abre um arquivo .svg e escreve o cabeçalho SVG.
*@param path O caminho completo do arquivo a ser criado.
*@return Um ponteiro de arquivo (FILE*) para o arquivo SVG aberto.
******************************************************************/
FILE *startSVG(const char *path);

/******************************************************************
*@brief Escreve as tags de fechamento do SVG e fecha o arquivo.
*@param file_name O ponteiro (FILE*) para o arquivo SVG aberto.
******************************************************************/
void stopSVG(FILE *file_name);

/******************************************************************
*@brief Insere a tag SVG para um Retângulo no arquivo.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param r O Retângulo a ser desenhado.
******************************************************************/
void svgInsertQuadra(FILE *file_name, Quadra r);

void svgMarcaRedCross(FILE *file_name, double x, double y);

void svgMarcaBlackCircle(FILE *file_name, double x, double y);

void svgMarcaRedSquare(FILE *file_name, double x, double y);

/******************************************************************
*@brief Função de callback para `fila_passthrough` que desenha um item.
*@param i O item (forma) a ser desenhado.
*@param aux O ponteiro (FILE*) para o arquivo SVG (passado como aux_data).
******************************************************************/
void draw(item i, item aux);

/******************************************************************
*@brief Função principal que gera o arquivo SVG completo.
*@param file_name O caminho completo do arquivo .svg a ser criado.
*@param formas A Fila/Lista contendo todas as formas a serem desenhadas.
*@return Retorna o ponteiro para o arquivo criado (FILE*).
******************************************************************/
FILE *createSVG(const char *file_name, Lista *formas);

#endif