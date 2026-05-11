#ifndef MORADOR_H
#define MORADOR_H

/**
 * @typedef Morador
 * @brief Tipo opaco que representa a associação de uma pessoa a um endereço (moradia).
 **/
typedef void* Morador;

/**
 * @brief Cria e aloca memória para um novo morador.
 * @param cpf         String contendo o CPF do morador.
 * @param cep         String contendo o CEP da quadra onde reside.
 * @param face        Caractere indicando a face da quadra ('N', 'S', 'L', 'O').
 * @param num         Número do endereço na face.
 * @param complemento String com informações adicionais do endereço.
 * @return Ponteiro válido para o tipo Morador recém-criado, ou NULL em caso de erro.
 **/
Morador moradorCreate(char* cpf, char* cep, char face, int num, char* complemento);

/**
 * @brief Libera a memória alocada para o morador.
 * @param m Ponteiro para o morador a ser destruído.
 **/
void moradorDestroy(Morador m);

/**
 * @brief Obtém o CPF do morador.
 * @param m Ponteiro para o morador.
 * @return String contendo o CPF.
 **/
char* moradorGetCpf(Morador m);

/**
 * @brief Obtém o CEP onde o morador reside.
 * @param m Ponteiro para o morador.
 * @return String contendo o CEP da quadra.
 **/
char* moradorGetCep(Morador m);

/**
 * @brief Obtém a face da quadra onde o morador reside.
 * @param m Ponteiro para o morador.
 * @return Caractere representando a face.
 **/
char moradorGetFace(Morador m);

/**
 * @brief Obtém o número do endereço do morador.
 * @param m Ponteiro para o morador.
 * @return Valor inteiro representando o número.
 **/
int moradorGetNumero(Morador m);

/**
 * @brief Obtém o complemento do endereço do morador.
 * @param m Ponteiro para o morador.
 * @return String contendo o complemento.
 **/
char* moradorGetComplemento(Morador m);

/**
 * @brief Altera o CPF de um morador.
 * @param m   Ponteiro para o morador.
 * @param cpf Nova string de CPF.
 **/
void moradorSetCpf(Morador m, char* cpf);

/**
 * @brief Altera o CEP de residência do morador.
 * @param m   Ponteiro para o morador.
 * @param cep Nova string de CEP.
 **/
void moradorSetCep(Morador m, char* cep);

/**
 * @brief Altera a face de residência do morador.
 * @param m    Ponteiro para o morador.
 * @param face Nova face da quadra.
 **/
void moradorSetFace(Morador m, char face);

/**
 * @brief Altera o número do endereço do morador.
 * @param m   Ponteiro para o morador.
 * @param num Novo número.
 **/
void moradorSetNumero(Morador m, int num);

/**
 * @brief Altera o complemento do endereço do morador.
 * @param m    Ponteiro para o morador.
 * @param comp Nova string de complemento.
 **/
void moradorSetComplemento(Morador m, char* comp);

#endif