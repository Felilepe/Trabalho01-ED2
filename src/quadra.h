#ifndef QUADRA_H
#define QUADRA_H


typedef void* Quadra;

/****************************************************************************************
* @brief Cria e aloca a memoria para um novo retângulo com os parâmetros fornecidos.    
* @param cep Identificador único para o retângulo.                                       
* @param x Coordenada X do âncora do retângulo.                                         
* @param y Coordenada Y do âncora do retângulo.                                         
* @param w Largura do retângulo.                                                        
*@param h Altura do retângulo                                                           
*@param corborda String que representa a cor da borda.                                  
* @param corpreench String que representa a cor de preenchimento.                       
* @return Retorna um ponteiro do tipo Quadra para o retângulo recém-criado.          
****************************************************************************************/
Quadra quadraCreate(char* cep, double x, double  y, double w, double h);

/****************************************************************************************
* @brief Obtém a área de um retângulo.                                                  
* @param r O retângulo do qual a área será obtida.                                      
* @return Retorna o valor da área (um double).                                          
****************************************************************************************/
double quadraCalcArea(Quadra r);

/****************************************************************************************
* @brief Libera a memoria alocada para o retângulo.
* @param r O retângulo a ser destruído.
****************************************************************************************/
void quadraDestroy(Quadra r);


// ... (Getters) ...

/****************************************************************************************
* @brief Obtém a coordenada X da âncora de um retângulo.                                
* @param r O retângulo do qual a coordenada será obtida.                                
* @return Retorna o valor da coordenada X (um double).                                  
****************************************************************************************/
double quadraGetCoordX(Quadra r);

/****************************************************************************************
* @brief Obtém a coordenada Y do âncora de um retângulo.                                
* @param r O retângulo do qual a coordenada será obtida.                                
* @return Retorna o valor da coordenada Y (um double).                                  
****************************************************************************************/
double quadraGetCoordY(Quadra r);

/****************************************************************************************
* @brief Obtém a cor de borda de um retângulo.                                
* @param r O retângulo do qual a cor será obtida.                                
* @return Retorna o valor da Cor de Borda (uma string).                                  
****************************************************************************************/
char* quadraGetCorBorda(Quadra r);

/****************************************************************************************
* @brief Obtém a Cor de Preenchimento de um retângulo.                                
* @param r O retângulo do qual a cor será obtida.                                
* @return Retorna o valor da Cor de Preenchimento (uma string).                                  
****************************************************************************************/
char* quadraGetCorPreench(Quadra r);

/****************************************************************************************
 * @brief Obtém a altura de um retângulo.                                                
 * @param r O retângulo do qual a altura será obtida.                                    
 * @return Retorna o valor da altura (um double).                                        
 ****************************************************************************************/
double quadraGetHeight(Quadra r);

/****************************************************************************************
 * @brief Obtém a largura de um retângulo.                                               
 * @param r O retângulo do qual a largura será obtida.                                   
 * @return Retorna o valor da largura (um double).                                       
 ****************************************************************************************/
double quadraGetWidth(Quadra r);

/****************************************************************************************
 * @brief Obtém a espessura da borda de um retângulo.                                               
 * @param r O retângulo do qual a largura será obtida.                                   
 * @return Retorna o valor da espessura (um double).                                       
 ****************************************************************************************/
double quadraGetBordaWidth(Quadra r);

/****************************************************************************************
 * @brief Obtém o ID de um retângulo.                                                    
 * @param r O retângulo do qual o ID será obtido.                                        
 * @return Retorna o valor do ID (um int).                                               
 ****************************************************************************************/
char* quadraGetCep(Quadra r);

/****************************************************************************************
 * @brief Obtém o Tipo de um retângulo.                                                    
 * @param r O retângulo do qual o Tipo será obtido.                                        
 * @return Retorna o valor do Tipo (um int).                                               
 ****************************************************************************************/
int quadraGetType(Quadra r);


// ... (Setters) ...

/****************************************************************************************
* @brief Define a coordenada X da âncora de um retângulo.
* @param r O Retângulo.
* @param x A nova coordenada X.
****************************************************************************************/
void quadraSetCoordX(Quadra r, double x);

/****************************************************************************************
* @brief Define a coordenada Y da âncora de um retângulo.
* @param r O Retângulo.
* @param y A nova coordenada Y.
****************************************************************************************/
void quadraSetCoordY(Quadra r, double y);

/****************************************************************************************
* @brief Define a Altura da âncora de um retângulo.
* @param r O Retângulo.
* @param h A nova Altura.
****************************************************************************************/
void quadraSetHeight(Quadra r, double h);

/****************************************************************************************
* @brief Define a Largura da âncora de um retângulo.
* @param r O Retângulo.
* @param w A nova Largura.
****************************************************************************************/
void quadraSetWidth(Quadra r, double w);

/****************************************************************************************
* @brief Define a espessura da borda de um retângulo.
* @param r O Retângulo.
* @param w A nova Largura.
****************************************************************************************/
void quadraSetBordaWidth(Quadra r, double sw);

/****************************************************************************************
* @brief Define a Cor de Borda de um retângulo
* @param r O Retângulo.
* @param corborda A nova Cor de Borda.
****************************************************************************************/
void quadraSetCorBorda(Quadra r, char *corborda);

/****************************************************************************************
* @brief Define a Cor de Preenchimento de um retângulo
* @param r O Retângulo.
* @param corpreench A nova Cor de Preenchimento.
****************************************************************************************/
void quadraSetCorPreench(Quadra r, char *corpreench);


#endif
