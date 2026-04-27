#ifndef RETANGULO_H
#define RETANGULO_H
#include "anteparo.h"
#include "lista.h"

typedef void* Retangulo;

/****************************************************************************************
* @brief Cria e aloca a memória para um novo retângulo com os parâmetros fornecidos.    
* @param id Identificador único para o retângulo.                                       
* @param x Coordenada X do âncora do retângulo.                                         
* @param y Coordenada Y do âncora do retângulo.                                         
* @param w Largura do retângulo.                                                        
*@param h Altura do retângulo                                                           
*@param corborda String que representa a cor da borda.                                  
* @param corpreench String que representa a cor de preenchimento.                       
* @return Retorna um ponteiro do tipo Retangulo para o retângulo recém-criado.          
****************************************************************************************/
Retangulo retangulo_create(int id, double x, double  y, double w, double h, char *corborda, char *corpreench);

/****************************************************************************************
* @brief Obtém a área de um retângulo.                                                  
* @param r O retângulo do qual a área será obtida.                                      
* @return Retorna o valor da área (um double).                                          
****************************************************************************************/
double retangulo_calcArea(Retangulo r);

/****************************************************************************************
* @brief Libera a memória alocada para o retângulo.
* @param r O retângulo a ser destruído.
****************************************************************************************/
void retangulo_destroy(Retangulo r);

/****************************************************************************************
* @brief Converte um retângulo em um anteparo.
* @param l O retângulo a ser convertido/utilizado.
* @param id_next Ponteiro para o próximo ID disponível (para geração de novos elementos).
* @return Retorna um objeto do tipo Anteparo.
****************************************************************************************/
Lista *retangulo_toAnteparo(Retangulo r, int *id_next);



// ... (Getters) ...

/****************************************************************************************
* @brief Obtém a coordenada X da âncora de um retângulo.                                
* @param r O retângulo do qual a coordenada será obtida.                                
* @return Retorna o valor da coordenada X (um double).                                  
****************************************************************************************/
double retangulo_getCoordX(Retangulo r);

/****************************************************************************************
* @brief Obtém a coordenada Y do âncora de um retângulo.                                
* @param r O retângulo do qual a coordenada será obtida.                                
* @return Retorna o valor da coordenada Y (um double).                                  
****************************************************************************************/
double retangulo_getCoordY(Retangulo r);

/****************************************************************************************
* @brief Obtém a cor de borda de um retângulo.                                
* @param r O retângulo do qual a cor será obtida.                                
* @return Retorna o valor da Cor de Borda (uma string).                                  
****************************************************************************************/
char* retangulo_getCorBorda(Retangulo r);

/****************************************************************************************
* @brief Obtém a Cor de Preenchimento de um retângulo.                                
* @param r O retângulo do qual a cor será obtida.                                
* @return Retorna o valor da Cor de Preenchimento (uma string).                                  
****************************************************************************************/
char* retangulo_getCorPreench(Retangulo r);

/****************************************************************************************
 * @brief Obtém a altura de um retângulo.                                                
 * @param r O retângulo do qual a altura será obtida.                                    
 * @return Retorna o valor da altura (um double).                                        
 ****************************************************************************************/
double retangulo_getHeight(Retangulo r);

/****************************************************************************************
 * @brief Obtém a largura de um retângulo.                                               
 * @param r O retângulo do qual a largura será obtida.                                   
 * @return Retorna o valor da largura (um double).                                       
 ****************************************************************************************/
double retangulo_getWidth(Retangulo r);

/****************************************************************************************
 * @brief Obtém o ID de um retângulo.                                                    
 * @param r O retângulo do qual o ID será obtido.                                        
 * @return Retorna o valor do ID (um int).                                               
 ****************************************************************************************/
int retangulo_getID(Retangulo r);

/****************************************************************************************
 * @brief Obtém o Tipo de um retângulo.                                                    
 * @param r O retângulo do qual o Tipo será obtido.                                        
 * @return Retorna o valor do Tipo (um int).                                               
 ****************************************************************************************/
int retangulo_getType(Retangulo r);


// ... (Setters) ...

/****************************************************************************************
* @brief Define a coordenada X da âncora de um retângulo.
* @param r O Retângulo.
* @param x A nova coordenada X.
****************************************************************************************/
void retangulo_setCoordX(Retangulo r, double x);

/****************************************************************************************
* @brief Define a coordenada Y da âncora de um retângulo.
* @param r O Retângulo.
* @param y A nova coordenada Y.
****************************************************************************************/
void retangulo_setCoordY(Retangulo r, double y);

/****************************************************************************************
* @brief Define a Altura da âncora de um retângulo.
* @param r O Retângulo.
* @param h A nova Altura.
****************************************************************************************/
void retangulo_setHeight(Retangulo r, double h);

/****************************************************************************************
* @brief Define a Largura da âncora de um retângulo.
* @param r O Retângulo.
* @param w A nova Largura.
****************************************************************************************/
void retangulo_setWidth(Retangulo r, double w);

/****************************************************************************************
* @brief Define a Cor de Borda de um retângulo
* @param r O Retângulo.
* @param corborda A nova Cor de Borda.
****************************************************************************************/
void retangulo_setCorBorda(Retangulo r, char *corborda);

/****************************************************************************************
* @brief Define a Cor de Preenchimento de um retângulo
* @param r O Retângulo.
* @param corpreench A nova Cor de Preenchimento.
****************************************************************************************/
void retangulo_setCorPreench(Retangulo r, char *corpreench);


#endif
