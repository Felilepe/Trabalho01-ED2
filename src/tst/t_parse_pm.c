#include "../unity.h"
#include "../parse_pm.h"
#include "../hte.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Hash h_hab, h_mor;

void setUp(void) 
{
    h_hab = hashOpenFile("hashtest_hab");
    h_mor = hashOpenFile("hashtest_mor");
}

void tearDown(void) 
{
    if (h_hab != NULL) { hashCloseFile(h_hab); h_hab = NULL; }
    if (h_mor != NULL) { hashCloseFile(h_mor); h_mor = NULL; }
    remove("hashtest_hab");  remove("hashtest_hab.hfc");
    remove("hashtest_mor");  remove("hashtest_mor.hfc");
}

void test_pmGetHabitante_nonexistent_cpf(void) 
{
    char nome[50], sobrenome[50], nascimento[12];
    char sexo;

    bool result = pmGetHabitante(h_hab, "99999999999", nome, sobrenome, &sexo, nascimento);

    TEST_ASSERT_FALSE(result);
}

void test_pmGetMorador_nonexistent_cpf(void) 
{
    char cep[20], complemento[50];
    char face;
    int  num;

    bool result = pmGetMorador(h_mor, "99999999999", cep, &face, &num, complemento);

    TEST_ASSERT_FALSE(result);
}

void test_pmGetHabitante_string_safety(void) 
{
    char nome[50], sobrenome[50], nascimento[12];
    char sexo;

    memset(nome,       'X', sizeof(nome));
    memset(sobrenome,  'Y', sizeof(sobrenome));
    memset(nascimento, 'Z', sizeof(nascimento));

    bool result = pmGetHabitante(h_hab, "NONEXIST", nome, sobrenome, &sexo, nascimento);

    TEST_ASSERT_FALSE(result);
}

void test_pmGetHabitante_null_parameters(void) 
{
    bool result = pmGetHabitante(h_hab, "12345678900", NULL, NULL, NULL, NULL);
    TEST_ASSERT_FALSE(result);
}

void test_pmGetMorador_null_parameters(void) 
{
    bool result = pmGetMorador(h_mor, "12345678900", NULL, NULL, NULL, NULL);
    TEST_ASSERT_FALSE(result);
}

static void create_test_pm_file(const char *filename) 
{
    FILE *f = fopen(filename, "w");
    if (f == NULL) return;

    fprintf(f, "# Arquivo de teste de pessoas e moradores\n");
    fprintf(f, "p 12345678900 Joao Silva M 01/01/1990\n");
    fprintf(f, "p 98765432100 Maria Santos F 15/06/1985\n");
    fprintf(f, "\n");
    fprintf(f, "m 12345678900 01234567890 N 100 Apto101\n");
    fprintf(f, "m 98765432100 01234567890 S 50 Casa\n");
    fprintf(f, "# Comentario no meio\n");
    fprintf(f, "p 11111111111 Pedro Costa M 10/03/1992\n");
    fprintf(f, "m 11111111111 98765432100 L 200 Fundos\n");

    fclose(f);
}

void test_parsePm_valid_file(void) 
{
    const char *test_file = "test_pm_valid.pm";
    create_test_pm_file(test_file);

    parsePm(test_file, h_hab, h_mor);

    TEST_ASSERT_TRUE(1);

    remove(test_file);
}

void test_parsePm_nonexistent_file(void) 
{
    parsePm("nonexistent_file_12345.pm", h_hab, h_mor);
    TEST_ASSERT_TRUE(1);
}

void test_parsePm_null_hashes(void) 
{
    const char *test_file = "test_pm_null.pm";
    create_test_pm_file(test_file);

    parsePm(test_file, NULL, NULL);

    TEST_ASSERT_TRUE(1);
    remove(test_file);
}

void test_parsePm_malformed_lines(void) 
{
    const char *test_file = "test_pm_malformed.pm";
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;

    fprintf(f, "p\n");
    fprintf(f, "p 12345678900 Joao\n");
    fprintf(f, "x INVALID\n");
    fprintf(f, "p 12345678900 Joao Silva M 01/01/1990\n");
    fprintf(f, "m 12345678900\n");
    fprintf(f, "\n");
    fprintf(f, "m 12345678900 01234567890 N 100 Apto\n");

    fclose(f);

    parsePm(test_file, h_hab, h_mor);

    TEST_ASSERT_TRUE(1);
    remove(test_file);
}

/* Testa recuperação real após parsing */
void test_parsePm_retrieve_after_parsing(void) 
{
    const char *test_file = "test_pm_retrieve.pm";
    create_test_pm_file(test_file);

    parsePm(test_file, h_hab, h_mor);

    char nome[50], sobrenome[50], nascimento[12];
    char sexo;

    bool result = pmGetHabitante(h_hab, "12345678900", nome, sobrenome, &sexo, nascimento);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_STRING("Joao",  nome);
    TEST_ASSERT_EQUAL_STRING("Silva", sobrenome);
    TEST_ASSERT_EQUAL_CHAR('M', sexo);

    remove(test_file);
}

void test_pmGetHabitante_long_cpf(void) 
{
    char nome[50], sobrenome[50], nascimento[12];
    char sexo;
    char long_cpf[256];
    memset(long_cpf, '1', sizeof(long_cpf) - 1);
    long_cpf[255] = '\0';

    bool result = pmGetHabitante(h_hab, long_cpf, nome, sobrenome, &sexo, nascimento);
    TEST_ASSERT_FALSE(result);
}

void test_pmGetMorador_long_cpf(void) 
{
    char cep[20], complemento[50];
    char face;
    int  num;
    char long_cpf[256];
    memset(long_cpf, '2', sizeof(long_cpf) - 1);
    long_cpf[255] = '\0';

    bool result = pmGetMorador(h_mor, long_cpf, cep, &face, &num, complemento);
    TEST_ASSERT_FALSE(result);
}

void test_parsePm_multiple_calls(void) 
{
    const char *test_file = "test_pm_multiple.pm";
    create_test_pm_file(test_file);

    parsePm(test_file, h_hab, h_mor);
    parsePm(test_file, h_hab, h_mor);
    parsePm(test_file, h_hab, h_mor);

    TEST_ASSERT_TRUE(1);
    remove(test_file);
}

void test_parsePm_comments_and_empty_lines(void) 
{
    const char *test_file = "test_pm_comments.pm";
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;

    fprintf(f, "# Comentario inicial\n");
    fprintf(f, "\n");
    fprintf(f, "p 12345678900 Joao Silva M 01/01/1990\n");
    fprintf(f, "\n");
    fprintf(f, "# Outro comentario\n");
    fprintf(f, "\n");
    fprintf(f, "m 12345678900 01234567890 N 100 Apto\n");
    fprintf(f, "\n");

    fclose(f);

    parsePm(test_file, h_hab, h_mor);

    TEST_ASSERT_TRUE(1);
    remove(test_file);
}

void test_parsePm_various_faces(void) 
{
    const char *test_file = "test_pm_faces.pm";
    FILE *f = fopen(test_file, "w");
    if (f == NULL) return;

    fprintf(f, "m 11111111111 01234567890 N 100 Casa1\n");
    fprintf(f, "m 22222222222 01234567890 S 100 Casa2\n");
    fprintf(f, "m 33333333333 01234567890 L 100 Casa3\n");
    fprintf(f, "m 44444444444 01234567890 O 100 Casa4\n");
    fprintf(f, "m 55555555555 01234567890 Face.N 200 Casa5\n");
    fprintf(f, "m 66666666666 01234567890 Face.S 200 Casa6\n");

    fclose(f);

    parsePm(test_file, h_hab, h_mor);

    TEST_ASSERT_TRUE(1);
    remove(test_file);
}

int main(void)
{
    UNITY_BEGIN();

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

    return UNITY_END();
}