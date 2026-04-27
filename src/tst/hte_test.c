#include "../src/unity.h"
#include "../src/hte.h"

Hash h;

void setUp(void) 
{
    h = hash_openFile("hte_test.bin");
}

void tearDown(void) 
{
    hash_closeFile(h);
    remove("hte_test.bin");
    remove("diretorio.bin");
}

void test_InsercaoBasica(void) 
{
    TEST_ASSERT_TRUE(hash_insertReg(h, "jogador_1", 99));
    TEST_ASSERT_EQUAL_INT(99, hash_getRegistry(h, "jogador_1"));
}

void test_StressTestMultiplosSplits(void) 
{
    char chave_temp[MAX_KEY_LENGTH];
    
    for (int i = 0; i < 250; i++) {
        snprintf(chave_temp, MAX_KEY_LENGTH, "user_%d", i);
        TEST_ASSERT_TRUE(hash_insertReg(h, chave_temp, i));
    }

    TEST_ASSERT_TRUE(hash_exists(h, "user_15"));
    TEST_ASSERT_TRUE(hash_exists(h, "user_180"));
    TEST_ASSERT_EQUAL_INT(249, hash_getRegistry(h, "user_249"));
    
    TEST_ASSERT_GREATER_THAN_INT(1, hash_getSize(h)); 
}

void test_ChavesMuitoLongas(void) 
{
    char chave_gigante[] = "esta_chave_e_absurdamente_longa_para_o_limite";
    
    TEST_ASSERT_TRUE(hash_insertReg(h, chave_gigante, 999));
    
    TEST_ASSERT_TRUE(hash_exists(h, chave_gigante));
    TEST_ASSERT_EQUAL_INT(999, hash_getRegistry(h, chave_gigante));
}

void test_PonteirosNulos(void) {
    TEST_ASSERT_FALSE(hash_insertReg(NULL, "chave", 10));
    TEST_ASSERT_FALSE(hash_insertReg(h, NULL, 10));
    
    TEST_ASSERT_FALSE(hash_removeReg(NULL, "chave"));
    TEST_ASSERT_FALSE(hash_removeReg(h, NULL));
    
    TEST_ASSERT_FALSE(hash_exists(h, NULL));
    TEST_ASSERT_EQUAL_INT(-1, hash_getRegistry(h, NULL)); 
}

void test_Persistencia(void) {
    TEST_ASSERT_TRUE(hash_insertReg(h, "dado_persistente", 404));
    hash_closeFile(h);
    
    h = hash_openFile("hte_test.bin");
    TEST_ASSERT_NOT_NULL(h); 
    
    TEST_ASSERT_TRUE(hash_exists(h, "dado_persistente"));
    TEST_ASSERT_EQUAL_INT(404, hash_getRegistry(h, "dado_persistente"));
}

int main(void) 
{
    UNITY_BEGIN();
    
    RUN_TEST(test_InsercaoBasica);
    RUN_TEST(test_Persistencia);
    RUN_TEST(test_PonteirosNulos);
    RUN_TEST(test_ChavesMuitoLongas);
    RUN_TEST(test_StressTestMultiplosSplits);
    
    return UNITY_END();
}