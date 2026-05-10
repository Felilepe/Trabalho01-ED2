/* ============================================================
 * Test Runner - Arquivo principal para executar todos os testes
 * ============================================================
 * 
 * Este arquivo agrega todos os testes do projeto e permite 
 * compilar e executar os testes de forma organizada usando Unity.
 * 
 * Para compilar e executar:
 *   gcc -I. *.c tst/t_*.c -o test_runner
 *   ./test_runner
 */

#include "../unity.h"
#include <stdio.h>

/* Protótipos dos testes - Habitante */
void test_habitanteCreate_successful(void);
void test_habitanteCreate_female(void);
void test_habitanteSetCpf(void);
void test_habitanteSetNome(void);
void test_habitanteSetSobrenome(void);
void test_habitanteSetSexo(void);
void test_habitanteSetNascimento(void);
void test_habitanteMultiple_updates(void);
void test_habitanteSetCpf_null_pointer(void);
void test_habitanteCreate_multiple_instances(void);

/* Protótipos dos testes - Morador */
void test_moradorCreate_successful(void);
void test_moradorCreate_different_faces(void);
void test_moradorSetCpf(void);
void test_moradorSetCep(void);
void test_moradorSetFace(void);
void test_moradorSetNumero(void);
void test_moradorSetComplemento(void);
void test_moradorMultiple_updates(void);
void test_moradorSetCpf_null_pointer(void);
void test_moradorNumero_negative(void);
void test_moradorNumero_edge_cases(void);
void test_moradorCreate_multiple_instances(void);

/* Protótipos dos testes - Quadra */
void test_quadraCreate_successful(void);
void test_quadraCalcArea(void);
void test_quadraCalcArea_various_sizes(void);
void test_quadraSetCoordX(void);
void test_quadraSetCoordY(void);
void test_quadraSetHeight(void);
void test_quadraSetWidth(void);
void test_quadraSetBordaWidth(void);
void test_quadraSetCorBorda(void);
void test_quadraSetCorPreench(void);
void test_quadraGetType(void);
void test_quadraMultiple_updates(void);
void test_quadraSetCorBorda_null_pointer(void);
void test_quadraDestroy_null_pointer(void);
void test_quadraCoordinates_negative(void);
void test_quadraCreate_multiple_instances(void);
void test_quadraSmall_values(void);
void test_quadraLarge_values(void);

/* Protótipos dos testes - Lista */
void test_lista_create(void);
void test_lista_insertHead_single(void);
void test_lista_insertTail_single(void);
void test_lista_insertHead_multiple(void);
void test_lista_insertTail_multiple(void);
void test_lista_removeHead(void);
void test_lista_removeTail(void);
void test_lista_removeNode_by_index(void);
void test_lista_getItem_by_index(void);
void test_lista_passthrough(void);
void test_lista_removeAll(void);
void test_lista_removeTail_until_empty(void);
void test_lista_mixed_operations(void);
void test_lista_getItem_empty_list(void);
void test_lista_getItem_out_of_bounds(void);
void test_lista_removeHead_single_element(void);
void test_lista_refill_after_empty(void);

/* Protótipos dos testes - Hash (HTE) */
void test_hash_create(void);
void test_hashInsertReg_and_getRegistry(void);
void test_hashGetRegistry_nonexistent_key(void);
void test_hash_multiple_insertions(void);
void test_hashInsertReg_update_existing(void);
void test_hashExists(void);
void test_hash_various_data_types(void);
void test_hash_many_registrations(void);
void test_hashGetSize(void);
void test_hashRemoveReg(void);
void test_hash_binary_data(void);
void test_hash_long_keys(void);
void test_hash_large_data(void);
void test_hash_fixed_size_data(void);

/* Protótipos dos testes - Parse GEO */
void test_geoGetQuadra_nonexistent_cep(void);
void test_geoGetQuadra_null_parameters(void);
void test_geoGetQuadra_string_buffer_safety(void);
void test_parseGeo_null_hash(void);
void test_parseGeo_nonexistent_file(void);
void test_parseGeo_valid_file(void);
void test_parseGeo_and_retrieve_quadra(void);
void test_geoGetQuadra_long_cep(void);
void test_parseGeo_malformed_lines(void);
void test_parseGeo_multiple_calls(void);
void test_geoGetQuadra_optional_null_params(void);
void test_geoGetQuadra_color_strings(void);

/* Protótipos dos testes - Parse PM */
void test_pmGetHabitante_nonexistent_cpf(void);
void test_pmGetMorador_nonexistent_cpf(void);
void test_pmGetHabitante_string_safety(void);
void test_pmGetHabitante_null_parameters(void);
void test_pmGetMorador_null_parameters(void);
void test_parsePm_valid_file(void);
void test_parsePm_nonexistent_file(void);
void test_parsePm_null_hashes(void);
void test_parsePm_malformed_lines(void);
void test_parsePm_retrieve_after_parsing(void);
void test_pmGetHabitante_long_cpf(void);
void test_pmGetMorador_long_cpf(void);
void test_parsePm_multiple_calls(void);
void test_parsePm_comments_and_empty_lines(void);
void test_parsePm_various_faces(void);

/* Protótipos dos testes - SVG */
void test_startSVG_creates_file(void);
void test_startSVG_has_structure(void);
void test_svgInsertQuadra(void);
void test_svgMarcaRedCross(void);
void test_svgMarcaBlackCircle(void);
void test_svgMarcaRedSquare(void);
void test_svgMarcaCep(void);
void test_svgMarcaContagemFace(void);
void test_svgMarcaContagemTotal(void);
void test_createSVG(void);
void test_createSVG_null_list(void);
void test_svg_multiple_insertions(void);
void test_stopSVG_closes_file(void);
void test_stopSVG_closes_svg_tag(void);

/* Protótipos dos testes - Parse QRY */
void test_query_file_creation(void);
void test_parse_qry_empty_file(void);
void test_parse_qry_simple_lines(void);
void test_parse_qry_malformed_lines(void);
void test_parse_qry_various_commands(void);
void test_parse_qry_multiple_ceps(void);
void test_parse_qry_long_lines(void);
void test_parse_qry_extra_spaces(void);
void test_parse_qry_case_sensitive(void);
void test_parse_qry_special_chars_in_cep(void);
void test_parse_qry_different_faces(void);
void test_parse_qry_corrupted_file(void);
void test_parse_qry_large_file(void);

void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, char *argv[])
{
    UNITY_BEGIN();

    /* Testes de Habitante */
    RUN_TEST(test_habitanteCreate_successful);
    RUN_TEST(test_habitanteCreate_female);
    RUN_TEST(test_habitanteSetCpf);
    RUN_TEST(test_habitanteSetNome);
    RUN_TEST(test_habitanteSetSobrenome);
    RUN_TEST(test_habitanteSetSexo);
    RUN_TEST(test_habitanteSetNascimento);
    RUN_TEST(test_habitanteMultiple_updates);
    RUN_TEST(test_habitanteSetCpf_null_pointer);
    RUN_TEST(test_habitanteCreate_multiple_instances);

    /* Testes de Morador */
    RUN_TEST(test_moradorCreate_successful);
    RUN_TEST(test_moradorCreate_different_faces);
    RUN_TEST(test_moradorSetCpf);
    RUN_TEST(test_moradorSetCep);
    RUN_TEST(test_moradorSetFace);
    RUN_TEST(test_moradorSetNumero);
    RUN_TEST(test_moradorSetComplemento);
    RUN_TEST(test_moradorMultiple_updates);
    RUN_TEST(test_moradorSetCpf_null_pointer);
    RUN_TEST(test_moradorNumero_negative);
    RUN_TEST(test_moradorNumero_edge_cases);
    RUN_TEST(test_moradorCreate_multiple_instances);

    /* Testes de Quadra */
    RUN_TEST(test_quadraCreate_successful);
    RUN_TEST(test_quadraCalcArea);
    RUN_TEST(test_quadraCalcArea_various_sizes);
    RUN_TEST(test_quadraSetCoordX);
    RUN_TEST(test_quadraSetCoordY);
    RUN_TEST(test_quadraSetHeight);
    RUN_TEST(test_quadraSetWidth);
    RUN_TEST(test_quadraSetBordaWidth);
    RUN_TEST(test_quadraSetCorBorda);
    RUN_TEST(test_quadraSetCorPreench);
    RUN_TEST(test_quadraGetType);
    RUN_TEST(test_quadraMultiple_updates);
    RUN_TEST(test_quadraSetCorBorda_null_pointer);
    RUN_TEST(test_quadraDestroy_null_pointer);
    RUN_TEST(test_quadraCoordinates_negative);
    RUN_TEST(test_quadraCreate_multiple_instances);
    RUN_TEST(test_quadraSmall_values);
    RUN_TEST(test_quadraLarge_values);

    /* Testes de Lista */
    RUN_TEST(test_lista_create);
    RUN_TEST(test_lista_insertHead_single);
    RUN_TEST(test_lista_insertTail_single);
    RUN_TEST(test_lista_insertHead_multiple);
    RUN_TEST(test_lista_insertTail_multiple);
    RUN_TEST(test_lista_removeHead);
    RUN_TEST(test_lista_removeTail);
    RUN_TEST(test_lista_removeNode_by_index);
    RUN_TEST(test_lista_getItem_by_index);
    RUN_TEST(test_lista_passthrough);
    RUN_TEST(test_lista_removeAll);
    RUN_TEST(test_lista_removeTail_until_empty);
    RUN_TEST(test_lista_mixed_operations);
    RUN_TEST(test_lista_getItem_empty_list);
    RUN_TEST(test_lista_getItem_out_of_bounds);
    RUN_TEST(test_lista_removeHead_single_element);
    RUN_TEST(test_lista_refill_after_empty);

    /* Testes de Hash (HTE) */
    RUN_TEST(test_hash_create);
    RUN_TEST(test_hashInsertReg_and_getRegistry);
    RUN_TEST(test_hashGetRegistry_nonexistent_key);
    RUN_TEST(test_hash_multiple_insertions);
    RUN_TEST(test_hashInsertReg_update_existing);
    RUN_TEST(test_hashExists);
    RUN_TEST(test_hash_various_data_types);
    RUN_TEST(test_hash_many_registrations);
    RUN_TEST(test_hashGetSize);
    RUN_TEST(test_hashRemoveReg);
    RUN_TEST(test_hash_binary_data);
    RUN_TEST(test_hash_long_keys);
    RUN_TEST(test_hash_large_data);
    RUN_TEST(test_hash_fixed_size_data);

    /* Testes de Parse GEO */
    RUN_TEST(test_geoGetQuadra_nonexistent_cep);
    RUN_TEST(test_geoGetQuadra_null_parameters);
    RUN_TEST(test_geoGetQuadra_string_buffer_safety);
    RUN_TEST(test_parseGeo_null_hash);
    RUN_TEST(test_parseGeo_nonexistent_file);
    RUN_TEST(test_parseGeo_valid_file);
    RUN_TEST(test_parseGeo_and_retrieve_quadra);
    RUN_TEST(test_geoGetQuadra_long_cep);
    RUN_TEST(test_parseGeo_malformed_lines);
    RUN_TEST(test_parseGeo_multiple_calls);
    RUN_TEST(test_geoGetQuadra_optional_null_params);
    RUN_TEST(test_geoGetQuadra_color_strings);

    /* Testes de Parse PM */
    RUN_TEST(test_pmGetHabitante_nonexistent_cpf);
    RUN_TEST(test_pmGetMorador_nonexistent_cpf);
    RUN_TEST(test_pmGetHabitante_string_safety);
    RUN_TEST(test_pmGetHabitante_null_parameters);
    RUN_TEST(test_pmGetMorador_null_parameters);
    RUN_TEST(test_parsePm_valid_file);
    RUN_TEST(test_parsePm_nonexistent_file);
    RUN_TEST(test_parsePm_null_hashes);
    RUN_TEST(test_parsePm_malformed_lines);
    RUN_TEST(test_parsePm_retrieve_after_parsing);
    RUN_TEST(test_pmGetHabitante_long_cpf);
    RUN_TEST(test_pmGetMorador_long_cpf);
    RUN_TEST(test_parsePm_multiple_calls);
    RUN_TEST(test_parsePm_comments_and_empty_lines);
    RUN_TEST(test_parsePm_various_faces);

    /* Testes de SVG */
    RUN_TEST(test_startSVG_creates_file);
    RUN_TEST(test_startSVG_has_structure);
    RUN_TEST(test_svgInsertQuadra);
    RUN_TEST(test_svgMarcaRedCross);
    RUN_TEST(test_svgMarcaBlackCircle);
    RUN_TEST(test_svgMarcaRedSquare);
    RUN_TEST(test_svgMarcaCep);
    RUN_TEST(test_svgMarcaContagemFace);
    RUN_TEST(test_svgMarcaContagemTotal);
    RUN_TEST(test_createSVG);
    RUN_TEST(test_createSVG_null_list);
    RUN_TEST(test_svg_multiple_insertions);
    RUN_TEST(test_stopSVG_closes_file);
    RUN_TEST(test_stopSVG_closes_svg_tag);

    /* Testes de Parse QRY */
    RUN_TEST(test_query_file_creation);
    RUN_TEST(test_parse_qry_empty_file);
    RUN_TEST(test_parse_qry_simple_lines);
    RUN_TEST(test_parse_qry_malformed_lines);
    RUN_TEST(test_parse_qry_various_commands);
    RUN_TEST(test_parse_qry_multiple_ceps);
    RUN_TEST(test_parse_qry_long_lines);
    RUN_TEST(test_parse_qry_extra_spaces);
    RUN_TEST(test_parse_qry_case_sensitive);
    RUN_TEST(test_parse_qry_special_chars_in_cep);
    RUN_TEST(test_parse_qry_different_faces);
    RUN_TEST(test_parse_qry_corrupted_file);
    RUN_TEST(test_parse_qry_large_file);

    return UNITY_END();
}
