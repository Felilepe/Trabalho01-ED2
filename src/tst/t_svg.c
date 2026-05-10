#include "../unity.h"
#include "../svg.h"
#include "../quadra.h"
#include "../lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Variáveis globais para testes */
FILE *svg_file;
const char *test_svg_file = "test_output.svg";

void setUp(void) 
{
    svg_file = NULL;
}

void tearDown(void) 
{
    if (svg_file != NULL) {
        stopSVG(svg_file);
    }
    
    /* Remover arquivo de teste se existir */
    remove(test_svg_file);
}

/* Testa criação de arquivo SVG */
void test_startSVG_creates_file(void) 
{
    svg_file = startSVG(test_svg_file);
    
    TEST_ASSERT_NOT_NULL(svg_file);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    /* Verificar se arquivo foi criado */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    fclose(check);
}

/* Testa se o arquivo SVG tem estrutura básica */
void test_startSVG_has_structure(void) 
{
    svg_file = startSVG(test_svg_file);
    TEST_ASSERT_NOT_NULL(svg_file);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    /* Ler arquivo e verificar conteúdo */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[256];
    fgets(buffer, sizeof(buffer), check);
    TEST_ASSERT_TRUE(strstr(buffer, "xml") != NULL);
    
    fclose(check);
}

/* Testa inserção de quadra no SVG */
void test_svgInsertQuadra(void) 
{
    svg_file = startSVG(test_svg_file);
    
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 100.0, 150.0);
    svgInsertQuadra(svg_file, q);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    quadraDestroy(q);
    
    /* Verificar se conteúdo foi escrito */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "CEP001") != NULL);
    TEST_ASSERT_TRUE(strstr(content, "rect") != NULL);
    
    fclose(check);
}

/* Testa marcação de red cross */
void test_svgMarcaRedCross(void) 
{
    svg_file = startSVG(test_svg_file);
    
    svgMarcaRedCross(svg_file, 50.0, 100.0);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    /* Verificar se texto foi inserido */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "text") != NULL);
    
    fclose(check);
}

/* Testa marcação de círculo preto */
void test_svgMarcaBlackCircle(void) 
{
    svg_file = startSVG(test_svg_file);
    
    svgMarcaBlackCircle(svg_file, 100.0, 200.0);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    /* Verificar se círculo foi inserido */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "circle") != NULL);
    
    fclose(check);
}

/* Testa marcação de quadrado vermelho com CPF */
void test_svgMarcaRedSquare(void) 
{
    svg_file = startSVG(test_svg_file);
    
    svgMarcaRedSquare(svg_file, 50.0, 50.0, "12345678900");
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    /* Verificar se quadrado foi inserido */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "rect") != NULL);
    TEST_ASSERT_TRUE(strstr(content, "12345678900") != NULL);
    
    fclose(check);
}

/* Testa marcação de CEP */
void test_svgMarcaCep(void) 
{
    svg_file = startSVG(test_svg_file);
    
    Quadra q = quadraCreate("01234567", 0.0, 0.0, 100.0, 100.0);
    svgMarcaCep(svg_file, q);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    quadraDestroy(q);
    
    /* Verificar se CEP foi inserido */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "01234567") != NULL);
    
    fclose(check);
}

/* Testa marcação de contagem por face */
void test_svgMarcaContagemFace(void) 
{
    svg_file = startSVG(test_svg_file);
    
    Quadra q = quadraCreate("CEP001", 10.0, 10.0, 100.0, 100.0);
    svgMarcaContagemFace(svg_file, q, 'N', 25);
    svgMarcaContagemFace(svg_file, q, 'S', 30);
    svgMarcaContagemFace(svg_file, q, 'L', 15);
    svgMarcaContagemFace(svg_file, q, 'O', 20);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    quadraDestroy(q);
    
    /* Verificar se números foram inseridos */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "25") != NULL);
    TEST_ASSERT_TRUE(strstr(content, "30") != NULL);
    
    fclose(check);
}

/* Testa marcação de contagem total */
void test_svgMarcaContagemTotal(void) 
{
    svg_file = startSVG(test_svg_file);
    
    Quadra q = quadraCreate("CEP001", 10.0, 10.0, 100.0, 100.0);
    svgMarcaContagemTotal(svg_file, q, 100);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    quadraDestroy(q);
    
    /* Verificar se total foi inserido */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "100") != NULL);
    
    fclose(check);
}

/* Testa createSVG com lista de quadras */
void test_createSVG(void) 
{
    Lista *l = lista_create();
    
    Quadra q1 = quadraCreate("CEP001", 0.0, 0.0, 100.0, 100.0);
    Quadra q2 = quadraCreate("CEP002", 100.0, 0.0, 100.0, 100.0);
    
    lista_insertTail(l, (item)q1);
    lista_insertTail(l, (item)q2);
    
    FILE *svg = createSVG(test_svg_file, l);
    
    TEST_ASSERT_NOT_NULL(svg);
    
    stopSVG(svg);
    svg = NULL;
    
    /* Verificar se arquivo foi criado com conteúdo */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[4096] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "CEP001") != NULL);
    TEST_ASSERT_TRUE(strstr(content, "CEP002") != NULL);
    
    fclose(check);
    
    /* Limpeza */
    quadraDestroy(q1);
    quadraDestroy(q2);
    lista_destroy(l);
}

/* Testa createSVG com lista NULL */
void test_createSVG_null_list(void) 
{
    FILE *svg = createSVG(test_svg_file, NULL);
    
    TEST_ASSERT_NULL(svg);
}

/* Testa múltiplas inserções no SVG */
void test_svg_multiple_insertions(void) 
{
    svg_file = startSVG(test_svg_file);
    
    Quadra q1 = quadraCreate("CEP001", 0.0, 0.0, 50.0, 50.0);
    Quadra q2 = quadraCreate("CEP002", 50.0, 50.0, 50.0, 50.0);
    Quadra q3 = quadraCreate("CEP003", 100.0, 100.0, 50.0, 50.0);
    
    svgInsertQuadra(svg_file, q1);
    svgInsertQuadra(svg_file, q2);
    svgInsertQuadra(svg_file, q3);
    
    svgMarcaCep(svg_file, q1);
    svgMarcaCep(svg_file, q2);
    svgMarcaCep(svg_file, q3);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    quadraDestroy(q1);
    quadraDestroy(q2);
    quadraDestroy(q3);
    
    /* Verificar tamanho do arquivo (deve ter conteúdo) */
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    fseek(check, 0, SEEK_END);
    long size = ftell(check);
    fclose(check);
    
    TEST_ASSERT_TRUE(size > 100); /* Arquivo deve ter tamanho razoável */
}

/* Testa arquivo SVG fecha corretamente */
void test_stopSVG_closes_file(void) 
{
    svg_file = startSVG(test_svg_file);
    TEST_ASSERT_NOT_NULL(svg_file);
    
    stopSVG(svg_file);
    svg_file = NULL;
    
    /* Tentar abrir arquivo com 'r+' - se fechar corretamente, deve funcionar */
    FILE *check = fopen(test_svg_file, "r+");
    TEST_ASSERT_NOT_NULL(check);
    fclose(check);
}

/* Testa arquivo SVG tem estrutura de fechamento */
void test_stopSVG_closes_svg_tag(void) 
{
    svg_file = startSVG(test_svg_file);
    stopSVG(svg_file);
    svg_file = NULL;
    
    FILE *check = fopen(test_svg_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    char buffer[512];
    char content[2048] = "";
    while (fgets(buffer, sizeof(buffer), check) != NULL) {
        strcat(content, buffer);
    }
    
    TEST_ASSERT_TRUE(strstr(content, "</svg>") != NULL);
    
    fclose(check);
}

int main(void)
{
    UNITY_BEGIN();
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
    return UNITY_END();
}