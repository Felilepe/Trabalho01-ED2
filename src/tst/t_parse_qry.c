#include "../unity.h"
#include "../parse_qry.h"
#include "../parse_geo.h"
#include "../parse_pm.h"
#include "../hte.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ── Arquivos temporários ── */
#define TMP_QRY  "tmp_test.qry"
#define TMP_SVG  "tmp_test.svg"
#define TMP_TXT  "tmp_test.txt"
#define TMP_GEO  "tmp_test.geo"
#define TMP_PM   "tmp_test.pm"
#define TMP_HF_Q "tmp_quadras.hf"
#define TMP_HF_H "tmp_hab.hf"
#define TMP_HF_M "tmp_mor.hf"

static Hash  h_quadras, h_hab, h_mor;
static FILE *svg_out, *txt_out;

/* Lê conteúdo de arquivo para buffer */
static int read_file(const char *path, char *buf, int buf_size)
{
    FILE *f = fopen(path, "r");
    if (!f) { buf[0] = '\0'; return 0; }
    int n = (int)fread(buf, 1, buf_size - 1, f);
    buf[n] = '\0';
    fclose(f);
    return n;
}

static void cleanup_tmp(void)
{
    remove(TMP_HF_Q); remove("tmp_quadras.hfc");
    remove(TMP_HF_H); remove("tmp_hab.hfc");
    remove(TMP_HF_M); remove("tmp_mor.hfc");
    remove(TMP_GEO); remove(TMP_PM);
    remove(TMP_QRY); remove(TMP_SVG); remove(TMP_TXT);
}

void setUp(void)
{
    cleanup_tmp();

    h_quadras = hashOpenFile(TMP_HF_Q);
    h_hab     = hashOpenFile(TMP_HF_H);
    h_mor     = hashOpenFile(TMP_HF_M);

    /* Popula quadras via parseGeo */
    FILE *geo = fopen(TMP_GEO, "w");
    fprintf(geo, "q cep01 0.0 0.0 200.0 100.0\n");
    fprintf(geo, "q cep02 300.0 0.0 150.0 120.0\n");
    fclose(geo);

    FILE *svg_dummy = fopen(TMP_SVG, "w");
    parseGeo(TMP_GEO, h_quadras, svg_dummy);
    fclose(svg_dummy);

    /* Popula habitantes e moradores via parsePm */
    FILE *pm = fopen(TMP_PM, "w");
    fprintf(pm, "p 11111111111 Joao Silva M 01/01/1990\n");
    fprintf(pm, "p 22222222222 Maria Santos F 15/06/1985\n");
    fprintf(pm, "p 33333333333 Pedro Costa M 10/03/1975\n"); /* sem-teto */
    fprintf(pm, "m 11111111111 cep01 N 50 Apto1\n");
    fprintf(pm, "m 22222222222 cep01 S 30 Casa2\n");
    fclose(pm);
    parsePm(TMP_PM, h_hab, h_mor);

    /* Abre saídas para parseQry */
    svg_out = fopen(TMP_SVG, "w");
    txt_out = fopen(TMP_TXT, "w");
}

void tearDown(void)
{
    if (svg_out) { fclose(svg_out); svg_out = NULL; }
    if (txt_out) { fclose(txt_out); txt_out = NULL; }
    if (h_quadras) { hashCloseFile(h_quadras); h_quadras = NULL; }
    if (h_hab)     { hashCloseFile(h_hab);     h_hab     = NULL; }
    if (h_mor)     { hashCloseFile(h_mor);     h_mor     = NULL; }
    cleanup_tmp();
}

/* Escreve o arquivo .qry e executa parseQry, fechando as saídas */
static void write_qry(const char *content)
{
    FILE *f = fopen(TMP_QRY, "w");
    fprintf(f, "%s", content);
    fclose(f);
}

static void run_qry(void)
{
    parseQry(TMP_QRY, h_quadras, h_hab, h_mor, svg_out, txt_out);
    fclose(svg_out); svg_out = NULL;
    fclose(txt_out); txt_out = NULL;
}

/* ─── Testes de robustez ──────────────────────────────────────────── */

void test_parseQry_null_params(void)
{
    write_qry("censo\n");
    /* Nenhuma chamada deve crashar */
    parseQry(NULL,    h_quadras, h_hab, h_mor, svg_out, txt_out);
    parseQry(TMP_QRY, NULL,      h_hab, h_mor, svg_out, txt_out);
    parseQry(TMP_QRY, h_quadras, NULL,  h_mor, svg_out, txt_out);
    parseQry(TMP_QRY, h_quadras, h_hab, NULL,  svg_out, txt_out);
    TEST_ASSERT_TRUE(1);
}

void test_parseQry_nonexistent_file(void)
{
    parseQry("nao_existe_xyz.qry", h_quadras, h_hab, h_mor, svg_out, txt_out);
    TEST_ASSERT_TRUE(1);
}

void test_parseQry_empty_file(void)
{
    write_qry("");
    run_qry();
    char buf[256];
    read_file(TMP_TXT, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, (int)strlen(buf)); /* TXT vazio */
}

void test_parseQry_comments_and_blank_lines(void)
{
    write_qry("# comentario\n\n# outro\n\n");
    run_qry();
    char buf[256];
    read_file(TMP_TXT, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, (int)strlen(buf));
}

void test_parseQry_unknown_command(void)
{
    write_qry("comandoinvalido arg1\n");
    run_qry();
    TEST_ASSERT_TRUE(1); /* Não crashou */
}

/* ─── rq ──────────────────────────────────────────────────────────── */

void test_parseQry_rq_existing(void)
{
    write_qry("rq cep01\n");
    run_qry();

    char txt[2048];
    read_file(TMP_TXT, txt, sizeof(txt));

    /* Cabeçalho do comando */
    TEST_ASSERT_TRUE(strstr(txt, "[*] rq cep01") != NULL);
    /* CPFs dos moradores afetados (Joao e Maria) */
    TEST_ASSERT_TRUE(strstr(txt, "11111111111") != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "22222222222") != NULL);

    /* SVG: cruz vermelha (texto) na âncora da quadra */
    char svg[2048];
    read_file(TMP_SVG, svg, sizeof(svg));
    TEST_ASSERT_TRUE(strstr(svg, "text") != NULL);
}

void test_parseQry_rq_removes_residents(void)
{
    write_qry("rq cep01\n");
    run_qry();

    /* Moradores devem ter sido removidos do hashfile (viram sem-teto) */
    TEST_ASSERT_FALSE(hashExists(h_mor, "11111111111"));
    TEST_ASSERT_FALSE(hashExists(h_mor, "22222222222"));
    /* Mas ainda são habitantes */
    TEST_ASSERT_TRUE(hashExists(h_hab, "11111111111"));
}

void test_parseQry_rq_nonexistent_cep(void)
{
    write_qry("rq cep_inexistente\n");
    run_qry();

    char txt[512];
    read_file(TMP_TXT, txt, sizeof(txt));
    TEST_ASSERT_TRUE(strstr(txt, "ERRO") != NULL);
}

/* ─── pq ──────────────────────────────────────────────────────────── */

void test_parseQry_pq_existing(void)
{
    write_qry("pq cep01\n");
    run_qry();

    char txt[2048];
    read_file(TMP_TXT, txt, sizeof(txt));

    TEST_ASSERT_TRUE(strstr(txt, "[*] pq cep01") != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "Face N: 1")    != NULL); /* Joao */
    TEST_ASSERT_TRUE(strstr(txt, "Face S: 1")    != NULL); /* Maria */
    TEST_ASSERT_TRUE(strstr(txt, "Total : 2")    != NULL);

    /* SVG: marcações numéricas nas faces */
    char svg[2048];
    read_file(TMP_SVG, svg, sizeof(svg));
    TEST_ASSERT_TRUE(strstr(svg, "text") != NULL);
}

void test_parseQry_pq_nonexistent_cep(void)
{
    write_qry("pq cep_inexistente\n");
    run_qry();

    char txt[512];
    read_file(TMP_TXT, txt, sizeof(txt));
    TEST_ASSERT_TRUE(strstr(txt, "ERRO") != NULL);
}

/* ─── censo ───────────────────────────────────────────────────────── */

void test_parseQry_censo(void)
{
    write_qry("censo\n");
    run_qry();

    char txt[2048];
    read_file(TMP_TXT, txt, sizeof(txt));

    TEST_ASSERT_TRUE(strstr(txt, "Total de habitantes: 3") != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "Total de moradores: 2")  != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "Total de sem-tetos: 1")  != NULL);
}

/* ─── h? ──────────────────────────────────────────────────────────── */

void test_parseQry_h_morador(void)
{
    write_qry("h? 11111111111\n");
    run_qry();

    char txt[2048];
    read_file(TMP_TXT, txt, sizeof(txt));

    TEST_ASSERT_TRUE(strstr(txt, "11111111111") != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "Joao")        != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "cep01")       != NULL); /* endereço reportado */
}

void test_parseQry_h_semteto(void)
{
    write_qry("h? 33333333333\n");
    run_qry();

    char txt[2048];
    read_file(TMP_TXT, txt, sizeof(txt));

    TEST_ASSERT_TRUE(strstr(txt, "33333333333") != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "sem-teto")    != NULL);
}

void test_parseQry_h_nonexistent(void)
{
    write_qry("h? 99999999999\n");
    run_qry();

    char txt[512];
    read_file(TMP_TXT, txt, sizeof(txt));
    TEST_ASSERT_TRUE(strstr(txt, "ERRO") != NULL);
}

/* ─── nasc ────────────────────────────────────────────────────────── */

void test_parseQry_nasc_inserts_habitante(void)
{
    write_qry("nasc 44444444444 Ana Lima F 20/08/2000\n");
    run_qry();

    /* Deve existir no hashfile após o comando */
    TEST_ASSERT_TRUE(hashExists(h_hab, "44444444444"));
    /* Não deve ser morador */
    TEST_ASSERT_FALSE(hashExists(h_mor, "44444444444"));
}

/* ─── rip ─────────────────────────────────────────────────────────── */

void test_parseQry_rip_morador(void)
{
    write_qry("rip 11111111111\n");
    run_qry();

    char txt[2048];
    read_file(TMP_TXT, txt, sizeof(txt));

    TEST_ASSERT_TRUE(strstr(txt, "11111111111") != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "cep01")       != NULL);

    /* SVG: cruz vermelha no local do endereço */
    char svg[2048];
    read_file(TMP_SVG, svg, sizeof(svg));
    TEST_ASSERT_TRUE(strstr(svg, "text") != NULL);

    /* Removido de ambos os hashfiles */
    TEST_ASSERT_FALSE(hashExists(h_hab, "11111111111"));
    TEST_ASSERT_FALSE(hashExists(h_mor, "11111111111"));
}

void test_parseQry_rip_semteto(void)
{
    write_qry("rip 33333333333\n");
    run_qry();

    /* Sem-teto também é removido de h_hab */
    TEST_ASSERT_FALSE(hashExists(h_hab, "33333333333"));
}

void test_parseQry_rip_nonexistent(void)
{
    write_qry("rip 99999999999\n");
    run_qry();

    char txt[512];
    read_file(TMP_TXT, txt, sizeof(txt));
    TEST_ASSERT_TRUE(strstr(txt, "ERRO") != NULL);
}

/* ─── mud ─────────────────────────────────────────────────────────── */

void test_parseQry_mud_changes_address(void)
{
    write_qry("mud 11111111111 cep02 S 80 Fundos\n");
    run_qry();

    /* SVG: quadrado vermelho com CPF no destino */
    char svg[2048];
    read_file(TMP_SVG, svg, sizeof(svg));
    TEST_ASSERT_TRUE(strstr(svg, "rect")        != NULL);
    TEST_ASSERT_TRUE(strstr(svg, "11111111111") != NULL);

    /* Ainda deve ser morador */
    TEST_ASSERT_TRUE(hashExists(h_mor, "11111111111"));
}

void test_parseQry_mud_nonexistent_cpf(void)
{
    write_qry("mud 99999999999 cep01 N 10 Casa\n");
    run_qry();

    char txt[512];
    read_file(TMP_TXT, txt, sizeof(txt));
    TEST_ASSERT_TRUE(strstr(txt, "ERRO") != NULL);
}

/* ─── dspj ────────────────────────────────────────────────────────── */

void test_parseQry_dspj_morador(void)
{
    write_qry("dspj 11111111111\n");
    run_qry();

    char txt[2048];
    read_file(TMP_TXT, txt, sizeof(txt));
    TEST_ASSERT_TRUE(strstr(txt, "11111111111") != NULL);
    TEST_ASSERT_TRUE(strstr(txt, "cep01")       != NULL);

    /* SVG: círculo preto no local do despejo */
    char svg[2048];
    read_file(TMP_SVG, svg, sizeof(svg));
    TEST_ASSERT_TRUE(strstr(svg, "circle") != NULL);

    /* Deve ter virado sem-teto */
    TEST_ASSERT_FALSE(hashExists(h_mor, "11111111111"));
    TEST_ASSERT_TRUE(hashExists(h_hab, "11111111111")); /* ainda habitante */
}

void test_parseQry_dspj_semteto(void)
{
    write_qry("dspj 33333333333\n");
    run_qry();

    char txt[512];
    read_file(TMP_TXT, txt, sizeof(txt));
    TEST_ASSERT_TRUE(strstr(txt, "ERRO") != NULL);
}

/* ─── múltiplos comandos em sequência ────────────────────────────── */

void test_parseQry_multiple_commands(void)
{
    write_qry(
        "nasc 55555555555 Carlos Dias M 05/05/1995\n"
        "h? 22222222222\n"
        "censo\n"
        "pq cep01\n"
    );
    run_qry();

    char txt[4096];
    read_file(TMP_TXT, txt, sizeof(txt));

    /* nasc: novo habitante inserido */
    TEST_ASSERT_TRUE(hashExists(h_hab, "55555555555"));
    /* h?: dados de Maria reportados */
    TEST_ASSERT_TRUE(strstr(txt, "22222222222")         != NULL);
    /* censo: executado */
    TEST_ASSERT_TRUE(strstr(txt, "Total de habitantes") != NULL);
    /* pq: contagem por face */
    TEST_ASSERT_TRUE(strstr(txt, "[*] pq cep01")        != NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_parseQry_null_params);
    RUN_TEST(test_parseQry_nonexistent_file);
    RUN_TEST(test_parseQry_empty_file);
    RUN_TEST(test_parseQry_comments_and_blank_lines);
    RUN_TEST(test_parseQry_unknown_command);

    RUN_TEST(test_parseQry_rq_existing);
    RUN_TEST(test_parseQry_rq_removes_residents);
    RUN_TEST(test_parseQry_rq_nonexistent_cep);

    RUN_TEST(test_parseQry_pq_existing);
    RUN_TEST(test_parseQry_pq_nonexistent_cep);

    RUN_TEST(test_parseQry_censo);

    RUN_TEST(test_parseQry_h_morador);
    RUN_TEST(test_parseQry_h_semteto);
    RUN_TEST(test_parseQry_h_nonexistent);

    RUN_TEST(test_parseQry_nasc_inserts_habitante);

    RUN_TEST(test_parseQry_rip_morador);
    RUN_TEST(test_parseQry_rip_semteto);
    RUN_TEST(test_parseQry_rip_nonexistent);

    RUN_TEST(test_parseQry_mud_changes_address);
    RUN_TEST(test_parseQry_mud_nonexistent_cpf);

    RUN_TEST(test_parseQry_dspj_morador);
    RUN_TEST(test_parseQry_dspj_semteto);

    RUN_TEST(test_parseQry_multiple_commands);

    return UNITY_END();
}