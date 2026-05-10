#include "../unity.h"
#include "../quadra.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void setUp(void) {}

void tearDown(void) {}

/* Testa criação básica de uma quadra */
void test_quadraCreate_successful(void) 
{
    Quadra q = quadraCreate("12345678", 10.0, 20.0, 30.0, 40.0);
    
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_STRING("12345678", quadraGetCep(q));
    TEST_ASSERT_EQUAL_DOUBLE(10.0, quadraGetCoordX(q));
    TEST_ASSERT_EQUAL_DOUBLE(20.0, quadraGetCoordY(q));
    TEST_ASSERT_EQUAL_DOUBLE(30.0, quadraGetWidth(q));
    TEST_ASSERT_EQUAL_DOUBLE(40.0, quadraGetHeight(q));
    
    quadraDestroy(q);
}

/* Testa cálculo de área */
void test_quadraCalcArea(void) 
{
    Quadra q = quadraCreate("CEP001", 0.0, 0.0, 10.0, 20.0);
    
    double area = quadraCalcArea(q);
    TEST_ASSERT_EQUAL_DOUBLE(200.0, area);
    
    quadraDestroy(q);
}

/* Testa cálculo de área com valores diferentes */
void test_quadraCalcArea_various_sizes(void) 
{
    Quadra q1 = quadraCreate("CEP001", 0.0, 0.0, 5.0, 5.0);
    TEST_ASSERT_EQUAL_DOUBLE(25.0, quadraCalcArea(q1));
    quadraDestroy(q1);
    
    Quadra q2 = quadraCreate("CEP002", 0.0, 0.0, 100.0, 50.0);
    TEST_ASSERT_EQUAL_DOUBLE(5000.0, quadraCalcArea(q2));
    quadraDestroy(q2);
    
    Quadra q3 = quadraCreate("CEP003", 0.0, 0.0, 1.5, 2.5);
    TEST_ASSERT_EQUAL_DOUBLE(3.75, quadraCalcArea(q3));
    quadraDestroy(q3);
}

/* Testa setter de coordenada X */
void test_quadraSetCoordX(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetCoordX(q, 50.0);
    TEST_ASSERT_EQUAL_DOUBLE(50.0, quadraGetCoordX(q));
    
    quadraDestroy(q);
}

/* Testa setter de coordenada Y */
void test_quadraSetCoordY(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetCoordY(q, 100.0);
    TEST_ASSERT_EQUAL_DOUBLE(100.0, quadraGetCoordY(q));
    
    quadraDestroy(q);
}

/* Testa setter de altura */
void test_quadraSetHeight(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetHeight(q, 60.0);
    TEST_ASSERT_EQUAL_DOUBLE(60.0, quadraGetHeight(q));
    
    quadraDestroy(q);
}

/* Testa setter de largura */
void test_quadraSetWidth(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetWidth(q, 70.0);
    TEST_ASSERT_EQUAL_DOUBLE(70.0, quadraGetWidth(q));
    
    quadraDestroy(q);
}

/* Testa setter de largura de borda */
void test_quadraSetBordaWidth(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetBordaWidth(q, 2.5);
    TEST_ASSERT_EQUAL_DOUBLE(2.5, quadraGetBordaWidth(q));
    
    quadraDestroy(q);
}

/* Testa setter de cor de borda */
void test_quadraSetCorBorda(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetCorBorda(q, "#FF0000");
    TEST_ASSERT_EQUAL_STRING("#FF0000", quadraGetCorBorda(q));
    
    quadraDestroy(q);
}

/* Testa setter de cor de preenchimento */
void test_quadraSetCorPreench(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetCorPreench(q, "#00FF00");
    TEST_ASSERT_EQUAL_STRING("#00FF00", quadraGetCorPreench(q));
    
    quadraDestroy(q);
}

/* Testa getter de tipo */
void test_quadraGetType(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    int type = quadraGetType(q);
    TEST_ASSERT_EQUAL_INT(2, type); /* TIPO_R = 2 */
    
    quadraDestroy(q);
}

/* Testa múltiplas atualizações */
void test_quadraMultiple_updates(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    quadraSetCoordX(q, 15.0);
    quadraSetCoordY(q, 25.0);
    quadraSetWidth(q, 50.0);
    quadraSetHeight(q, 60.0);
    quadraSetBordaWidth(q, 3.0);
    quadraSetCorBorda(q, "#0000FF");
    quadraSetCorPreench(q, "#FFFF00");
    
    TEST_ASSERT_EQUAL_DOUBLE(15.0, quadraGetCoordX(q));
    TEST_ASSERT_EQUAL_DOUBLE(25.0, quadraGetCoordY(q));
    TEST_ASSERT_EQUAL_DOUBLE(50.0, quadraGetWidth(q));
    TEST_ASSERT_EQUAL_DOUBLE(60.0, quadraGetHeight(q));
    TEST_ASSERT_EQUAL_DOUBLE(3.0, quadraGetBordaWidth(q));
    TEST_ASSERT_EQUAL_STRING("#0000FF", quadraGetCorBorda(q));
    TEST_ASSERT_EQUAL_STRING("#FFFF00", quadraGetCorPreench(q));
    
    quadraDestroy(q);
}

/* Testa setter com NULL (proteção adicionada) */
void test_quadraSetCorBorda_null_pointer(void) 
{
    Quadra q = quadraCreate("CEP001", 10.0, 20.0, 30.0, 40.0);
    
    /* Não deve crashear com NULL */
    quadraSetCorBorda(NULL, "#FF0000");
    quadraSetCorBorda(q, NULL);
    
    /* Valor original deve permanecer */
    TEST_ASSERT_EQUAL_STRING("#E06F2D", quadraGetCorBorda(q));
    
    quadraDestroy(q);
}

/* Testa destruição com NULL (proteção adicionada) */
void test_quadraDestroy_null_pointer(void) 
{
    /* Não deve crashear */
    quadraDestroy(NULL);
    
    /* Se chegou aqui, teste passou */
    TEST_ASSERT_TRUE(1);
}

/* Testa valores negativos de coordenadas */
void test_quadraCoordinates_negative(void) 
{
    Quadra q = quadraCreate("CEP001", -10.0, -20.0, 30.0, 40.0);
    
    TEST_ASSERT_EQUAL_DOUBLE(-10.0, quadraGetCoordX(q));
    TEST_ASSERT_EQUAL_DOUBLE(-20.0, quadraGetCoordY(q));
    
    quadraDestroy(q);
}

/* Testa criação de múltiplas quadras independentes */
void test_quadraCreate_multiple_instances(void) 
{
    Quadra q1 = quadraCreate("CEP001", 0.0, 0.0, 10.0, 10.0);
    Quadra q2 = quadraCreate("CEP002", 20.0, 20.0, 15.0, 15.0);
    Quadra q3 = quadraCreate("CEP003", 40.0, 40.0, 20.0, 20.0);
    
    TEST_ASSERT_EQUAL_STRING("CEP001", quadraGetCep(q1));
    TEST_ASSERT_EQUAL_STRING("CEP002", quadraGetCep(q2));
    TEST_ASSERT_EQUAL_STRING("CEP003", quadraGetCep(q3));
    
    TEST_ASSERT_EQUAL_DOUBLE(100.0, quadraCalcArea(q1));
    TEST_ASSERT_EQUAL_DOUBLE(225.0, quadraCalcArea(q2));
    TEST_ASSERT_EQUAL_DOUBLE(400.0, quadraCalcArea(q3));
    
    quadraDestroy(q1);
    quadraDestroy(q2);
    quadraDestroy(q3);
}

/* Testa valores muito pequenos (próximos a zero) */
void test_quadraSmall_values(void) 
{
    Quadra q = quadraCreate("CEP001", 0.001, 0.002, 0.003, 0.004);
    
    TEST_ASSERT_EQUAL_DOUBLE(0.001, quadraGetCoordX(q));
    TEST_ASSERT_EQUAL_DOUBLE(0.000012, quadraCalcArea(q));
    
    quadraDestroy(q);
}

/* Testa valores muito grandes */
void test_quadraLarge_values(void) 
{
    Quadra q = quadraCreate("CEP001", 10000.0, 20000.0, 5000.0, 8000.0);
    
    TEST_ASSERT_EQUAL_DOUBLE(40000000.0, quadraCalcArea(q));
    
    quadraDestroy(q);
}

int main(void)
{
    UNITY_BEGIN();
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
    return UNITY_END();
}