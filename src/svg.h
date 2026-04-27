#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include "retangulo.h"
#include "circulo.h"
#include "lista.h"
#include "linha.h"
#include "texto.h"
#include "poligono.h"
#include "anteparo.h"
#include "formas.h"

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
*@brief Insere a tag SVG para um Círculo no arquivo.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param c O Círculo a ser desenhado.
******************************************************************/
void svg_insertCirculo(FILE *file_name, Circulo c);

/******************************************************************
*@brief Insere a tag SVG para um Retângulo no arquivo.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param r O Retângulo a ser desenhado.
******************************************************************/
void svg_insertRetangulo(FILE *file_name, Retangulo r);

/******************************************************************
*@brief Insere a tag SVG para uma Linha no arquivo.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param l A Linha a ser desenhada.
******************************************************************/
void svg_insertLinha(FILE *file_name, Linha l);

/******************************************************************
*@brief Insere a tag SVG para um Texto no arquivo.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param t O Texto a ser desenhado.
******************************************************************/
void svg_insertTexto(FILE *file_name, Texto t);

/******************************************************************
*@brief Insere a representação visual de um Polígono de Visibilidade.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param p O Polígono a ser desenhado.
******************************************************************/
void svg_insertPoligonoVis(FILE *file_name, Poligono p); 

/******************************************************************
*@brief Insere a representação visual de um Anteparo (obstáculo).
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param a O Anteparo a ser desenhado.
******************************************************************/
void svg_insertAnteparo(FILE *file_name, Anteparo a);

/******************************************************************
*@brief Desenha a Bounding Box (caixa envolvente) de um polígono.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param p O polígono do qual a Bounding Box será desenhada.
******************************************************************/
void svg_insertBoundingBox(FILE *file_name, Poligono p);

/******************************************************************
*@brief Insere uma marcação visual (ex: um "X" ou círculo) representando uma "Bomba" ou ponto de impacto.
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param x A coordenada X do ponto.
*@param y A coordenada Y do ponto.
******************************************************************/
void svg_insertBomb(FILE *file_name, double x, double y); 


/******************************************************************
*@brief Insere uma forma genérica no SVG (chamada polimórfica).
*@param file_name O ponteiro (FILE*) para o arquivo SVG.
*@param f A forma a ser desenhada.
******************************************************************/
void svg_insertForma(FILE *file_name, forma f);


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