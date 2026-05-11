#ifndef HTE_H
#define HTE_H
 
#include <stdbool.h>
#include <stddef.h>
 
 
/**
 * @typedef Hash
 * @brief Tipo opaco que representa o Hashfile Dinâmico em disco.
 **/
typedef void *Hash;
 
/**
 * @brief Abre ou cria um hashfile.
 * Se o arquivo já existir (ex: "habitantes.hf"), carrega o diretório
 * a partir do arquivo auxiliar correspondente ("habitantes.hfc").
 * Se não existir, cria um novo arquivo e inicializa a estrutura.
 * @param filename Caminho do arquivo principal (ex: "habitantes.hf").
 * @return Ponteiro válido para a estrutura Hash, ou NULL em caso de erro.
 **/
Hash hashOpenFile(const char *filename);
 
/**
 * @brief Insere ou atualiza um registro no hashfile.
 * Se a chave já existir, os dados são sobrescritos.
 * Se o bucket estiver cheio, um split é realizado automaticamente.
 * @param h         Ponteiro para a estrutura Hash.
 * @param key       Chave string (máx. 31 caracteres úteis + '\0').
 * @param data      Ponteiro para os dados a armazenar.
 * @param data_size Tamanho em bytes dos dados (máx. MAX_DATA_SIZE).
 * @return true se bem-sucedido, false em caso de erro.
 **/
bool hashInsertReg(Hash h, char *key, void *data, size_t data_size);
 
/**
 * @brief Remove um registro do hashfile com base na chave.
 * @param h   Ponteiro para a estrutura Hash.
 * @param key Chave do registro a remover.
 * @return true se encontrado e removido, false caso contrário.
 **/
bool hashRemoveReg(Hash h, char *key);
 
/**
 * @brief Verifica se uma chave existe no hashfile.
 * @param h   Ponteiro para a estrutura Hash.
 * @param key Chave a procurar.
 * @return true se existe, false caso contrário.
 **/
bool hashExists(Hash h, char *key);
 
/**
 * @brief Recupera os dados associados a uma chave.
 * Copia os dados armazenados para o buffer apontado por 'out'.
 * O buffer deve ter pelo menos MAX_DATA_SIZE bytes.
 * @param h        Ponteiro para a estrutura Hash.
 * @param key      Chave a procurar.
 * @param out      Buffer de saída onde os dados serão copiados.
 * @param out_size Tamanho do buffer de saída.
 * @return true se a chave foi encontrada e os dados copiados,
 *         false caso contrário.
 **/
bool hashGetRegistry(Hash h, char *key, void *out, size_t out_size);
 
/**
 * @brief Retorna o número de entradas no diretório (potência de 2).
 * @param h Ponteiro para a estrutura Hash.
 * @return Tamanho do diretório, ou -1 em caso de erro.
 **/
int hashGetSize(Hash h);
 
/**
 * @brief Gera um arquivo-texto legível (.hfd) com o estado do hashfile.
 * @param h        Ponteiro para a estrutura Hash.
 * @param filename Caminho do arquivo .hfd a ser gerado.
 **/
void hashDumpFile(Hash h, const char *filename);

/**
 * @brief Itera sobre todos os registros do hashfile, chamando o callback
 *        para cada registro ocupado.
 * @param h   Ponteiro para a estrutura Hash.
 * @param cb  Função chamada para cada registro: recebe a chave, o ponteiro
 *            para os dados, o tamanho dos dados e o ponteiro auxiliar.
 * @param aux Ponteiro auxiliar repassado a cada chamada do callback.
 **/
void hashForEach(Hash h,
                  void (*cb)(char *key, void *data, size_t data_size, void *aux),
                  void *aux);
 
/**
 * @brief Fecha o hashfile, persistindo o diretório em disco.
 * @param h Ponteiro para a estrutura Hash a ser fechada.
 **/
void hashCloseFile(Hash h);
 
#endif
