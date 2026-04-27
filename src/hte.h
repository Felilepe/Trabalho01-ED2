#ifndef HTE_H
#define HTE_H

/**
 * @def MAX_KEY_LENGTH
 * @brief Tamanho máximo permitido para as chaves (strings) inseridas na tabela Hash.
 * -Inclui o caractere terminador nulo ('\0').
 **/
#define MAX_KEY_LENGTH 32

#include <stdbool.h>

/**
 * @typedef Hash
 * @brief Tipo opaco que representa a Tabela Hash Extensível.
 * -Esconde a implementação interna (o hte_directory) do utilizador da biblioteca.
 **/
typedef void* Hash;

/**
 * @brief Abre ou cria um ficheiro de banco de dados Hash.
 * -Se o ficheiro já existir, carrega a estrutura do diretório para a memória.
 * -Se não existir, cria um novo ficheiro e inicializa a estrutura básica.
 * @param filename Nome ou caminho do ficheiro binário principal (ex: "banco.bin").
 * @return Retorna um ponteiro válido para a estrutura Hash, ou NULL em caso de erro.
 **/
Hash hash_openFile(const char* filename);

/**
 * @brief Insere um novo registo (chave-valor) na tabela Hash.
 * @param h Ponteiro para a estrutura Hash aberta.
 * @param key String representando a chave a ser inserida (máx. MAX_KEY_LENGTH).
 * @param value Valor inteiro associado à chave.
 * @return Retorna true se a inserção for bem-sucedida, ou false se houver erro (ex: ponteiro nulo).
 **/
bool hash_insertReg(Hash h, char* key, int value);

/**
 * @brief Remove um registo da tabela Hash com base na sua chave.
 * @param h Ponteiro para a estrutura Hash aberta.
 * @param key String representando a chave a ser removida.
 * @return Retorna true se o registo foi encontrado e removido, ou false caso contrário.
 **/
bool hash_removeReg(Hash h, char* key);

/**
 * @brief Obtém o tamanho atual do diretório do Hashing Extensível.
 * @param h Ponteiro para a estrutura Hash.
 * @return Retorna o número de entradas no diretório (directory_size), ou -1 em caso de erro.
 **/
int hash_getSize(Hash h);

/**
 * @brief Verifica se uma chave específica já existe na tabela Hash.
 * @param h Ponteiro para a estrutura Hash.
 * @param key String representando a chave a ser procurada.
 * @return Retorna true se a chave existe, ou false se não for encontrada ou houver erro.
 **/
bool hash_exists(Hash h, char* key);

/**
 * @brief Recupera o valor associado a uma determinada chave.
 * @param h Ponteiro para a estrutura Hash.
 * @param key String representando a chave a ser procurada.
 * @return Retorna o valor (int) associado à chave, ou -1 se a chave não for encontrada.
 **/
int hash_getRegistry(Hash h, char* key);

/**
 * @brief Fecha a tabela Hash de forma segura, guardando o estado no disco.
 * -Grava as informações vitais (profundidade global, tamanho do diretório e 
 * offsets) num ficheiro auxiliar ("diretorio.bin") e liberta a memória RAM.
 * @param h Ponteiro para a estrutura Hash a ser fechada.
 **/
void hash_closeFile(Hash h);

#endif