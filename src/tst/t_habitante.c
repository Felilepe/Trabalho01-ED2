#include "../unity.h"
#include "../habitante.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {
}

void tearDown(void) {
}

void test_habitanteCreate_successful(void) {
    habitante h = habitanteCreate("12345678900", "João", "Silva", 'M', "01/01/1990");
    
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_STRING("12345678900", habitanteGetCpf(h));
    TEST_ASSERT_EQUAL_STRING("João", habitanteGetNome(h));
    TEST_ASSERT_EQUAL_STRING("Silva", habitanteGetSobrenome(h));
    TEST_ASSERT_EQUAL_CHAR('M', habitanteGetSexo(h));
    TEST_ASSERT_EQUAL_STRING("01/01/1990", habitanteGetNascimento(h));
    
    habitanteDestroy(h);
}

/* Testa criação com dados diferentes */
void test_habitanteCreate_female(void) {
    habitante h = habitanteCreate("98765432100", "Maria", "Santos", 'F', "15/06/1985");
    
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_CHAR('F', habitanteGetSexo(h));
    TEST_ASSERT_EQUAL_STRING("Maria", habitanteGetNome(h));
    
    habitanteDestroy(h);
}

/* Testa setter de CPF */
void test_habitanteSetCpf(void) {
    habitante h = habitanteCreate("11111111111", "Test", "User", 'M', "01/01/2000");
    
    habitanteSetCpf(h, "22222222222");
    TEST_ASSERT_EQUAL_STRING("22222222222", habitanteGetCpf(h));
    
    habitanteDestroy(h);
}

/* Testa setter de Nome */
void test_habitanteSetNome(void) {
    habitante h = habitanteCreate("12345678900", "João", "Silva", 'M', "01/01/1990");
    
    habitanteSetNome(h, "Pedro");
    TEST_ASSERT_EQUAL_STRING("Pedro", habitanteGetNome(h));
    
    habitanteDestroy(h);
}

/* Testa setter de Sobrenome */
void test_habitanteSetSobrenome(void) {
    habitante h = habitanteCreate("12345678900", "João", "Silva", 'M', "01/01/1990");
    
    habitanteSetSobrenome(h, "Oliveira");
    TEST_ASSERT_EQUAL_STRING("Oliveira", habitanteGetSobrenome(h));
    
    habitanteDestroy(h);
}

/* Testa setter de Sexo */
void test_habitanteSetSexo(void) {
    habitante h = habitanteCreate("12345678900", "João", "Silva", 'M', "01/01/1990");
    
    habitanteSetSexo(h, 'F');
    TEST_ASSERT_EQUAL_CHAR('F', habitanteGetSexo(h));
    
    habitanteDestroy(h);
}

/* Testa setter de Data de Nascimento */
void test_habitanteSetNascimento(void) {
    habitante h = habitanteCreate("12345678900", "João", "Silva", 'M', "01/01/1990");
    
    habitanteSetNascimento(h, "15/03/1992");
    TEST_ASSERT_EQUAL_STRING("15/03/1992", habitanteGetNascimento(h));
    
    habitanteDestroy(h);
}

/* Testa múltiplas atualizações */
void test_habitanteMultiple_updates(void) {
    habitante h = habitanteCreate("11111111111", "Ana", "Costa", 'F', "10/10/1995");
    
    habitanteSetCpf(h, "22222222222");
    habitanteSetNome(h, "Beatriz");
    habitanteSetSobrenome(h, "Ferreira");
    habitanteSetSexo(h, 'F');
    habitanteSetNascimento(h, "20/05/1998");
    
    TEST_ASSERT_EQUAL_STRING("22222222222", habitanteGetCpf(h));
    TEST_ASSERT_EQUAL_STRING("Beatriz", habitanteGetNome(h));
    TEST_ASSERT_EQUAL_STRING("Ferreira", habitanteGetSobrenome(h));
    TEST_ASSERT_EQUAL_CHAR('F', habitanteGetSexo(h));
    TEST_ASSERT_EQUAL_STRING("20/05/1998", habitanteGetNascimento(h));
    
    habitanteDestroy(h);
}

/* Testa setter com NULL (proteção adicionada) */
void test_habitanteSetCpf_null_pointer(void) {
    habitante h = habitanteCreate("12345678900", "João", "Silva", 'M', "01/01/1990");
    
    /* Não deve crashear com NULL */
    habitanteSetCpf(NULL, "22222222222");
    habitanteSetCpf(h, NULL);
    
    /* Valores originais devem permanecer */
    TEST_ASSERT_EQUAL_STRING("12345678900", habitanteGetCpf(h));
    
    habitanteDestroy(h);
}

/* Testa criação e destruição multipla */
void test_habitanteCreate_multiple_instances(void) {
    habitante h1 = habitanteCreate("11111111111", "Person1", "Last1", 'M', "01/01/1990");
    habitante h2 = habitanteCreate("22222222222", "Person2", "Last2", 'F', "02/02/1991");
    habitante h3 = habitanteCreate("33333333333", "Person3", "Last3", 'M', "03/03/1992");
    
    TEST_ASSERT_NOT_NULL(h1);
    TEST_ASSERT_NOT_NULL(h2);
    TEST_ASSERT_NOT_NULL(h3);
    
    TEST_ASSERT_EQUAL_STRING("Person1", habitanteGetNome(h1));
    TEST_ASSERT_EQUAL_STRING("Person2", habitanteGetNome(h2));
    TEST_ASSERT_EQUAL_STRING("Person3", habitanteGetNome(h3));
    
    habitanteDestroy(h1);
    habitanteDestroy(h2);
    habitanteDestroy(h3);
}

int main(void)
{
    UNITY_BEGIN();
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
    return UNITY_END();
}