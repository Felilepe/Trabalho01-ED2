#ifndef PARSE_QRY_H
#define PARSE_QRY_H

#include <stdio.h>
#include "hte.h"

/**
 * @brief Lê o arquivo .qry e processa cada comando, produzindo
 *        saídas no SVG e no TXT conforme especificado.
 *
 * Comandos suportados:
 *   rq  cep                          — remove quadra
 *   pq  cep                          — contagem de moradores por face
 *   censo                            — estatísticas gerais
 *   h?  cpf                          — dados de um habitante
 *   nasc cpf nome sobrenome sexo nasc — novo habitante nasce
 *   rip  cpf                         — habitante falece
 *   mud  cpf cep face num comp       — morador muda de endereço
 *   dspj cpf                         — morador é despejado
 *
 * @param caminho   Caminho completo do arquivo .qry.
 * @param h_quadras Hashfile de quadras.
 * @param h_hab     Hashfile de habitantes.
 * @param h_mor     Hashfile de moradores.
 * @param svg       Arquivo SVG já aberto (resultado após .qry).
 * @param txt       Arquivo TXT já aberto para saída textual.
 **/
void parseQry(const char *caminho,
              Hash h_quadras, Hash h_hab, Hash h_mor,
              FILE *svg, FILE *txt);

#endif