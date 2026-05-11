#ifndef HABITANTE_H
#define HABITANTE_H

/**
 * @typedef habitante
 * @brief Tipo opaco que representa os dados de um habitante.
 **/
typedef void* habitante;

/**
 * @brief Cria e aloca memória para um novo habitante.
 * @param cpf        String contendo o CPF único do habitante.
 * @param nome       String com o nome do habitante.
 * @param sobrenome  String com o sobrenome do habitante.
 * @param sexo       Caractere representando o sexo (ex: 'M' ou 'F').
 * @param nascimento String contendo a data de nascimento (ex: "dd/mm/aaaa").
 * @return Ponteiro válido para o tipo habitante recém-criado, ou NULL em caso de erro.
 **/
habitante habitanteCreate(char* cpf, char* nome, char* sobrenome, char sexo, char* nascimento);

/**
 * @brief Obtém o CPF do habitante.
 * @param h Ponteiro para o habitante.
 * @return String contendo o CPF.
 **/
char* habitanteGetCpf(habitante h);

/**
 * @brief Obtém o nome do habitante.
 * @param h Ponteiro para o habitante.
 * @return String contendo o nome.
 **/
char* habitanteGetNome(habitante h);

/**
 * @brief Obtém o sobrenome do habitante.
 * @param h Ponteiro para o habitante.
 * @return String contendo o sobrenome.
 **/
char* habitanteGetSobrenome(habitante h);

/**
 * @brief Obtém o sexo do habitante.
 * @param h Ponteiro para o habitante.
 * @return Caractere representando o sexo.
 **/
char habitanteGetSexo(habitante h);

/**
 * @brief Obtém a data de nascimento do habitante.
 * @param h Ponteiro para o habitante.
 * @return String contendo a data de nascimento.
 **/
char* habitanteGetNascimento(habitante h);

/**
 * @brief Altera o CPF de um habitante existente.
 * @param h   Ponteiro para o habitante.
 * @param cpf Nova string de CPF.
 **/
void habitanteSetCpf(habitante h, char* cpf);

/**
 * @brief Altera o nome de um habitante.
 * @param h    Ponteiro para o habitante.
 * @param nome Novo nome.
 **/
void habitanteSetNome(habitante h, char* nome);

/**
 * @brief Altera o sobrenome de um habitante.
 * @param h         Ponteiro para o habitante.
 * @param sobrenome Novo sobrenome.
 **/
void habitanteSetSobrenome(habitante h, char* sobrenome);

/**
 * @brief Altera o sexo de um habitante.
 * @param h    Ponteiro para o habitante.
 * @param sexo Novo caractere de sexo.
 **/
void habitanteSetSexo(habitante h, char sexo);

/**
 * @brief Altera a data de nascimento de um habitante.
 * @param h          Ponteiro para o habitante.
 * @param nascimento Nova string de data de nascimento.
 **/
void habitanteSetNascimento(habitante h, char* nascimento);

/**
 * @brief Libera a memória alocada para o habitante.
 * @param h Ponteiro para o habitante a ser destruído.
 **/
void habitanteDestroy(habitante h);

#endif