#include "../unity.h"
#include "../morador.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) 
{
}

void tearDown(void) 
{
}

/* Testa criação básica de um morador */
void test_moradorCreate_successful(void) 
{
    Morador m = moradorCreate("12345678900", "01234567890", 'N', 100, "Apto 101");
    
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("12345678900", moradorGetCpf(m));
    TEST_ASSERT_EQUAL_STRING("01234567890", moradorGetCep(m));
    TEST_ASSERT_EQUAL_CHAR('N', moradorGetFace(m));
    TEST_ASSERT_EQUAL_INT(100, moradorGetNumero(m));
    TEST_ASSERT_EQUAL_STRING("Apto 101", moradorGetComplemento(m));
    
    moradorDestroy(m);
}

/* Testa criação com diferentes faces */
void test_moradorCreate_different_faces(void) 
{
    Morador mN = moradorCreate("11111111111", "01234567890", 'N', 50, "");
    Morador mS = moradorCreate("22222222222", "01234567890", 'S', 75, "");
    Morador mL = moradorCreate("33333333333", "01234567890", 'L', 25, "");
    Morador mO = moradorCreate("44444444444", "01234567890", 'O', 150, "");
    
    TEST_ASSERT_EQUAL_CHAR('N', moradorGetFace(mN));
    TEST_ASSERT_EQUAL_CHAR('S', moradorGetFace(mS));
    TEST_ASSERT_EQUAL_CHAR('L', moradorGetFace(mL));
    TEST_ASSERT_EQUAL_CHAR('O', moradorGetFace(mO));
    
    moradorDestroy(mN);
    moradorDestroy(mS);
    moradorDestroy(mL);
    moradorDestroy(mO);
}

/* Testa setter de CPF */
void test_moradorSetCpf(void) 
{
    Morador m = moradorCreate("11111111111", "01234567890", 'N', 100, "");
    
    moradorSetCpf(m, "99999999999");
    TEST_ASSERT_EQUAL_STRING("99999999999", moradorGetCpf(m));
    
    moradorDestroy(m);
}

/* Testa setter de CEP */
void test_moradorSetCep(void) 
{
    Morador m = moradorCreate("12345678900", "01234567890", 'N', 100, "");
    
    moradorSetCep(m, "98765432100");
    TEST_ASSERT_EQUAL_STRING("98765432100", moradorGetCep(m));
    
    moradorDestroy(m);
}

/* Testa setter de Face */
void test_moradorSetFace(void) 
{
    Morador m = moradorCreate("12345678900", "01234567890", 'N', 100, "");
    
    moradorSetFace(m, 'S');
    TEST_ASSERT_EQUAL_CHAR('S', moradorGetFace(m));
    
    moradorDestroy(m);
}

/* Testa setter de Número */
void test_moradorSetNumero(void) 
{
    Morador m = moradorCreate("12345678900", "01234567890", 'N', 100, "");
    
    moradorSetNumero(m, 250);
    TEST_ASSERT_EQUAL_INT(250, moradorGetNumero(m));
    
    moradorDestroy(m);
}

/* Testa setter de Complemento */
void test_moradorSetComplemento(void) 
{
    Morador m = moradorCreate("12345678900", "01234567890", 'N', 100, "Apto 101");
    
    moradorSetComplemento(m, "Apto 202");
    TEST_ASSERT_EQUAL_STRING("Apto 202", moradorGetComplemento(m));
    
    moradorDestroy(m);
}

/* Testa múltiplas atualizações */
void test_moradorMultiple_updates(void) 
{
    Morador m = moradorCreate("11111111111", "01234567890", 'N', 100, "Casa");
    
    moradorSetCpf(m, "22222222222");
    moradorSetCep(m, "98765432100");
    moradorSetFace(m, 'S');
    moradorSetNumero(m, 150);
    moradorSetComplemento(m, "Fundos");
    
    TEST_ASSERT_EQUAL_STRING("22222222222", moradorGetCpf(m));
    TEST_ASSERT_EQUAL_STRING("98765432100", moradorGetCep(m));
    TEST_ASSERT_EQUAL_CHAR('S', moradorGetFace(m));
    TEST_ASSERT_EQUAL_INT(150, moradorGetNumero(m));
    TEST_ASSERT_EQUAL_STRING("Fundos", moradorGetComplemento(m));
    
    moradorDestroy(m);
}

/* Testa setter com NULL (proteção adicionada) */
void test_moradorSetCpf_null_pointer(void) 
{
    Morador m = moradorCreate("12345678900", "01234567890", 'N', 100, "");
    
    /* Não deve crashear com NULL */
    moradorSetCpf(NULL, "99999999999");
    moradorSetCpf(m, NULL);
    
    /* Valores originais devem permanecer */
    TEST_ASSERT_EQUAL_STRING("12345678900", moradorGetCpf(m));
    
    moradorDestroy(m);
}

/* Testa números negativos */
void test_moradorNumero_negative(void) 
{
    Morador m = moradorCreate("12345678900", "01234567890", 'N', -50, "");
    
    TEST_ASSERT_EQUAL_INT(-50, moradorGetNumero(m));
    
    moradorDestroy(m);
}

/* Testa números zero e grandes */
void test_moradorNumero_edge_cases(void) 
{
    Morador m1 = moradorCreate("11111111111", "01234567890", 'N', 0, "");
    Morador m2 = moradorCreate("22222222222", "01234567890", 'S', 999999, "");
    
    TEST_ASSERT_EQUAL_INT(0, moradorGetNumero(m1));
    TEST_ASSERT_EQUAL_INT(999999, moradorGetNumero(m2));
    
    moradorDestroy(m1);
    moradorDestroy(m2);
}

/* Testa criação de múltiplas instâncias independentes */
void test_moradorCreate_multiple_instances(void) 
{
    Morador m1 = moradorCreate("11111111111", "01111111111", 'N', 100, "Apto 1");
    Morador m2 = moradorCreate("22222222222", "02222222222", 'S', 200, "Apto 2");
    Morador m3 = moradorCreate("33333333333", "03333333333", 'L', 300, "Apto 3");
    
    TEST_ASSERT_EQUAL_STRING("11111111111", moradorGetCpf(m1));
    TEST_ASSERT_EQUAL_STRING("22222222222", moradorGetCpf(m2));
    TEST_ASSERT_EQUAL_STRING("33333333333", moradorGetCpf(m3));
    
    TEST_ASSERT_EQUAL_INT(100, moradorGetNumero(m1));
    TEST_ASSERT_EQUAL_INT(200, moradorGetNumero(m2));
    TEST_ASSERT_EQUAL_INT(300, moradorGetNumero(m3));
    
    moradorDestroy(m1);
    moradorDestroy(m2);
    moradorDestroy(m3);
}

int main(void)
{
    UNITY_BEGIN();
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
    return UNITY_END();
}