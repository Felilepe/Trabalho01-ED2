#ifndef PARSE_PM_H
#define PARSE_PM_H
 
#include "hte.h"
 
/**
 * @brief Lê o arquivo .pm e popula os dois hashfiles.
 * Para cada linha do arquivo:
 * p cpf nome sobrenome sexo dd/mm/aaaa  →  insere habitante em h_hab.
 * m cpf cep face num complemento        →  insere morador   em h_mor.
 * Os dados são serializados diretamente nos buckets do hashfile,
 * sem arquivo auxiliar. A chave em ambos os hashfiles é o CPF.
 * @param caminho  Caminho completo do arquivo .pm.
 * @param h_hab    Hashfile de habitantes.
 * @param h_mor    Hashfile de moradores.
 **/
void parsePm(const char *caminho, Hash h_hab, Hash h_mor);
 
/**
 * @brief Recupera os dados de um habitante do hashfile.
 * @param h_hab      Hashfile de habitantes.
 * @param cpf        CPF do habitante a buscar.
 * @param nome       Buffer de saída para o nome       (mín. 50 bytes).
 * @param sobrenome  Buffer de saída para o sobrenome  (mín. 50 bytes).
 * @param sexo       Ponteiro de saída para o sexo ('M' ou 'F').
 * @param nascimento Buffer de saída para a data       (mín. 12 bytes).
 * @return true se encontrado, false caso contrário.
 **/
bool pmGetHabitante(Hash h_hab, char *cpf,char *nome, char *sobrenome, char *sexo, char *nascimento);
 
/**
 * @brief Recupera os dados de um morador do hashfile.
 * @param h_mor       Hashfile de moradores.
 * @param cpf         CPF do morador a buscar.
 * @param cep         Buffer de saída para o CEP        (mín. 20 bytes).
 * @param face        Ponteiro de saída para a face ('N','S','L','O').
 * @param num         Ponteiro de saída para o número.
 * @param complemento Buffer de saída para o complemento (mín. 50 bytes).
 * @return true se encontrado, false caso contrário.
 **/
bool pmGetMorador(Hash h_mor, char *cpf, char *cep, char *face, int *num, char *complemento);
 
#endif
