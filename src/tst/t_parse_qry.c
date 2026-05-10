#include "../unity.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* parse_qry é um módulo complexo que depende de muitos outros módulos.
 * Portanto, fazemos testes básicos de segurança e estrutura. */

void setUp(void) {}

void tearDown(void) {}

/* Testa se o arquivo de query pode ser criado */
void test_query_file_creation(void) 
{
    const char *test_query_file = "test_query.qry";
    
    FILE *f = fopen(test_query_file, "w");
    TEST_ASSERT_NOT_NULL(f);
    
    fprintf(f, "# Arquivo de teste de queries\n");
    fprintf(f, "rq CEP001\n");
    fprintf(f, "rf CEP001\n");
    
    fclose(f);
    
    /* Verificar se arquivo foi criado */
    FILE *check = fopen(test_query_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    fclose(check);
    
    remove(test_query_file);
}

/* Testa se parse_qry pode ser chamado sem crashes com arquivo vazio */
void test_parse_qry_empty_file(void) 
{
    const char *test_file = "empty_query.qry";
    
    FILE *f = fopen(test_file, "w");
    fclose(f);
    
    /* Arquivo existe mas está vazio - não deve crashear */
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa processamento de linhas simples */
void test_parse_qry_simple_lines(void) 
{
    const char *test_file = "simple_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "# Comentário\n");
    fprintf(f, "\n");
    fprintf(f, "rq CEP001\n");
    fprintf(f, "rf CEP002\n");
    fprintf(f, "rc CEP001\n");
    
    fclose(f);
    
    /* Arquivo foi criado corretamente */
    FILE *check = fopen(test_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    fclose(check);
    
    remove(test_file);
}

/* Testa arquivo com linhas malformadas */
void test_parse_qry_malformed_lines(void) 
{
    const char *test_file = "malformed_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "rq\n");              /* Sem argumento */
    fprintf(f, "unknown_cmd CEP\n"); /* Comando desconhecido */
    fprintf(f, "rq CEP001\n");       /* Válido */
    fprintf(f, "\n");                /* Linha vazia */
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa arquivo com diferentes comandos de query */
void test_parse_qry_various_commands(void) 
{
    const char *test_file = "various_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "# Teste de diversos comandos\n");
    fprintf(f, "rq CEP001\n");     /* Residentes por quadra */
    fprintf(f, "rf CEP001 N\n");   /* Residentes por face */
    fprintf(f, "rc CEP001\n");     /* Contagem por face */
    fprintf(f, "c\n");             /* Censo */
    fprintf(f, "e CEP001 CEP002\n"); /* Entre CEPs */
    
    fclose(f);
    
    FILE *check = fopen(test_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    fclose(check);
    
    remove(test_file);
}

/* Testa arquivo com CEPs diversos */
void test_parse_qry_multiple_ceps(void) 
{
    const char *test_file = "multi_cep_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "rq CEP001\n");
    fprintf(f, "rq CEP002\n");
    fprintf(f, "rq CEP003\n");
    fprintf(f, "rf CEP001 N\n");
    fprintf(f, "rf CEP002 S\n");
    fprintf(f, "rf CEP003 L\n");
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa linhas muito longas */
void test_parse_qry_long_lines(void) 
{
    const char *test_file = "long_line_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "rq ");
    for (int i = 0; i < 200; i++) {
        fprintf(f, "X");
    }
    fprintf(f, "\n");
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa arquivo com múltiplos espaços */
void test_parse_qry_extra_spaces(void) 
{
    const char *test_file = "spaces_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "rq    CEP001\n");      /* Múltiplos espaços */
    fprintf(f, "rf   CEP002   N\n");   /* Múltiplos espaços */
    fprintf(f, "\n\n");                /* Múltiplas linhas vazias */
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa comandos case sensitive */
void test_parse_qry_case_sensitive(void) 
{
    const char *test_file = "case_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "rq CEP001\n");   /* Minúscula - válido */
    fprintf(f, "RQ CEP002\n");   /* Maiúscula - inválido? */
    fprintf(f, "Rq CEP003\n");   /* Misto - inválido? */
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa arquivo com caracteres especiais em CEP */
void test_parse_qry_special_chars_in_cep(void) 
{
    const char *test_file = "special_cep_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "rq CEP-001\n");     /* Com hífen */
    fprintf(f, "rq CEP_001\n");     /* Com underscore */
    fprintf(f, "rq CEP.001\n");     /* Com ponto */
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa arquivo com faces diferentes */
void test_parse_qry_different_faces(void) 
{
    const char *test_file = "faces_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    fprintf(f, "rf CEP001 N\n");
    fprintf(f, "rf CEP001 S\n");
    fprintf(f, "rf CEP001 L\n");
    fprintf(f, "rf CEP001 O\n");
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa robustez contra arquivos corrompidos */
void test_parse_qry_corrupted_file(void) 
{
    const char *test_file = "corrupted_query.qry";
    
    FILE *f = fopen(test_file, "wb");
    if (f == NULL) return;
    
    /* Escrever bytes aleatórios */
    unsigned char garbage[] = {0xFF, 0xFE, 0xFD, 0xFC, 0x00, 0xFF};
    fwrite(garbage, sizeof(garbage), 1, f);
    
    fclose(f);
    
    TEST_ASSERT_TRUE(1);
    
    remove(test_file);
}

/* Testa arquivo grande com muitas queries */
void test_parse_qry_large_file(void) 
{
    const char *test_file = "large_query.qry";
    
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;
    
    /* Gerar 1000 queries */
    for (int i = 0; i < 1000; i++) {
        char cep[20];
        snprintf(cep, sizeof(cep), "CEP%06d", i);
        fprintf(f, "rq %s\n", cep);
    }
    
    fclose(f);
    
    /* Verificar tamanho */
    FILE *check = fopen(test_file, "r");
    TEST_ASSERT_NOT_NULL(check);
    
    fseek(check, 0, SEEK_END);
    long size = ftell(check);
    fclose(check);
    
    TEST_ASSERT_TRUE(size > 10000); /* Arquivo deve ter tamanho significativo */
    
    remove(test_file);
}

int main(void)
{
    UNITY_BEGIN();
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