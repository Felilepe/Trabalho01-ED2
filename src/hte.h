#ifndef HTE_H
#define HTE_H
 
#include <stdbool.h>
 
/**
 * @typedef Hash
 * @brief Tipo opaco que representa o Hashfile Dinâmico em disco.
 **/
typedef void* Hash;
 
/**
 * @brief Abre ou cria um arquivo de banco de dados Hash.
 * Se o arquivo já existir (ex: "quadras.hf"), carrega o diretorio a partir
 * do arquivo auxiliar correspondente (ex: "quadras.hfc").
 * Se não existir, cria um novo arquivo e inicializa a estrutura.
 * @param filename Caminho do arquivo binário principal (ex: "quadras.hf").
 * @return Ponteiro válido para a estrutura Hash, ou NULL em caso de erro.
 **/
Hash hashOpenFile(const char* filename);
 
/**
 * @brief Insere ou atualiza um registro (chave -> valor inteiro) no hashfile.
 * Se a chave já existir, o valor é atualizado. Se o bucket estiver cheio,
 * um split é realizado automaticamente.
 * @param h   Ponteiro para a estrutura Hash.
 * @param key Chave string (máx. 31 caracteres úteis + '\0').
 * @param value Valor inteiro a associar à chave.
 * @return true se bem-sucedido, false em caso de erro (ponteiro nulo).
 **/
bool hashInsertReg(Hash h, char* key, int value);
 
/**
 * @brief Remove um registro do hashfile com base na chave.
 * @param h   Ponteiro para a estrutura Hash.
 * @param key Chave do registro a remover.
 * @return true se encontrado e removido, false caso contrário.
 **/
bool hashRemoveReg(Hash h, char* key);
 
/**
 * @brief Verifica se uma chave existe no hashfile.
 * @param h   Ponteiro para a estrutura Hash.
 * @param key Chave a procurar.
 * @return true se existe, false caso contrário.
 **/
bool hashExists(Hash h, char* key);
 
/**
 * @brief Recupera o valor inteiro associado a uma chave.
 * @param h   Ponteiro para a estrutura Hash.
 * @param key Chave a procurar.
 * @return O valor associado, ou -1 se a chave não existir ou houver erro.
 **/
int hashGetRegistry(Hash h, char* key);
 
/**
 * @brief Retorna o número de entradas no diretorio (potência de 2).
 * @param h Ponteiro para a estrutura Hash.
 * @return Tamanho do diretorio (int), ou -1 em caso de erro.
 **/
int hashGetSize(Hash h);
 
/**
 * @brief Gera um arquivo-texto legível (.hfd) com o estado atual do hashfile.
 * Mostra: profundidade global, cada bucket com profundidade local,
 * número de registros e as chaves armazenadas.
 * Também registra quantas expansões ocorreram até o momento.
 * @param h        Ponteiro para a estrutura Hash.
 * @param filename Caminho do arquivo .hfd a ser gerado.
 **/
void hashDumpFile(Hash h, const char* filename);
 
/**
 * @brief Fecha o hashfile de forma segura, persistindo o diretorio em disco.
 * Salva profundidade global, tamanho e offsets no arquivo .hfc correspondente,
 * libera toda a memoria alocada e fecha os arquivos.
 * @param h Ponteiro para a estrutura Hash a ser fechada.
 **/
void hashCloseFile(Hash h);
 
#endif
