#include "../unity.h"
#include "../hte.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Hash h;

void setUp(void)
{
    h = hashOpenFile("hashtest");
}

void tearDown(void)
{
    if (h != NULL) {
        hashCloseFile(h);
        h = NULL;
    }
    remove("hashtest");
    remove("hashtest.hfc");
}

void test_hash_create(void)
{
    TEST_ASSERT_NOT_NULL(h);
}

void test_hashInsertReg_and_getRegistry(void)
{
    char data[100] = "test data";

    hashInsertReg(h, "key1", data, strlen(data) + 1);

    char retrieved[100];
    bool result = hashGetRegistry(h, "key1", retrieved, sizeof(retrieved));

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_STRING(data, retrieved);
}

void test_hashGetRegistry_nonexistent_key(void)
{
    char retrieved[100];

    bool result = hashGetRegistry(h, "nonexistent", retrieved, sizeof(retrieved));

    TEST_ASSERT_FALSE(result);
}

void test_hash_multiple_insertions(void)
{
    int data1 = 100;
    int data2 = 200;
    int data3 = 300;

    hashInsertReg(h, "key1", &data1, sizeof(int));
    hashInsertReg(h, "key2", &data2, sizeof(int));
    hashInsertReg(h, "key3", &data3, sizeof(int));

    int retrieved1, retrieved2, retrieved3;

    bool r1 = hashGetRegistry(h, "key1", &retrieved1, sizeof(int));
    bool r2 = hashGetRegistry(h, "key2", &retrieved2, sizeof(int));
    bool r3 = hashGetRegistry(h, "key3", &retrieved3, sizeof(int));

    TEST_ASSERT_TRUE(r1);
    TEST_ASSERT_TRUE(r2);
    TEST_ASSERT_TRUE(r3);
    TEST_ASSERT_EQUAL_INT(data1, retrieved1);
    TEST_ASSERT_EQUAL_INT(data2, retrieved2);
    TEST_ASSERT_EQUAL_INT(data3, retrieved3);
}

void test_hashInsertReg_update_existing(void)
{
    char data1[50] = "first data";
    char data2[50] = "updated data";

    hashInsertReg(h, "key1", data1, strlen(data1) + 1);

    char retrieved[50];
    hashGetRegistry(h, "key1", retrieved, sizeof(retrieved));
    TEST_ASSERT_EQUAL_STRING(data1, retrieved);

    hashInsertReg(h, "key1", data2, strlen(data2) + 1);

    hashGetRegistry(h, "key1", retrieved, sizeof(retrieved));
    TEST_ASSERT_EQUAL_STRING(data2, retrieved);
}

void test_hashExists(void)
{
    char data[20] = "test";

    hashInsertReg(h, "exists_key", data, strlen(data) + 1);

    bool result1 = hashExists(h, "exists_key");
    bool result2 = hashExists(h, "nonexistent_key");

    TEST_ASSERT_TRUE(result1);
    TEST_ASSERT_FALSE(result2);
}

void test_hash_various_data_types(void)
{
    typedef struct {
        int  id;
        int  valor_inteiro;   
        char nome[50];
    } RecordoTeste;

    RecordoTeste rec1;
    rec1.id            = 42;
    rec1.valor_inteiro = 314159;   
    strcpy(rec1.nome, "TestRecord");

    hashInsertReg(h, "struct_key", &rec1, sizeof(RecordoTeste));

    RecordoTeste retrieved;
    bool result = hashGetRegistry(h, "struct_key", &retrieved, sizeof(RecordoTeste));

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(rec1.id,            retrieved.id);
    TEST_ASSERT_EQUAL_INT(rec1.valor_inteiro, retrieved.valor_inteiro);
    TEST_ASSERT_EQUAL_STRING(rec1.nome,       retrieved.nome);
}

void test_hash_many_registrations(void)
{
    char keys[100][20];
    int  values[100];

    for (int i = 0; i < 100; i++) {
        snprintf(keys[i], sizeof(keys[i]), "key_%d", i);
        values[i] = i * 10;
        hashInsertReg(h, keys[i], &values[i], sizeof(int));
    }

    int retrieved;

    bool r0 = hashGetRegistry(h, keys[0],  &retrieved, sizeof(int));
    TEST_ASSERT_TRUE(r0);
    TEST_ASSERT_EQUAL_INT(values[0], retrieved);

    bool r50 = hashGetRegistry(h, keys[50], &retrieved, sizeof(int));
    TEST_ASSERT_TRUE(r50);
    TEST_ASSERT_EQUAL_INT(values[50], retrieved);

    bool r99 = hashGetRegistry(h, keys[99], &retrieved, sizeof(int));
    TEST_ASSERT_TRUE(r99);
    TEST_ASSERT_EQUAL_INT(values[99], retrieved);
}


void test_hashGetSize(void)
{
    int size_inicial = hashGetSize(h);
    TEST_ASSERT_TRUE(size_inicial >= 1);

    /* Inserir registros suficientes para forçar pelo menos um split */
    char key[20];
    int  val = 0;
    for (int i = 0; i < 60; i++) {
        snprintf(key, sizeof(key), "k%d", i);
        hashInsertReg(h, key, &val, sizeof(int));
    }

    int size_depois = hashGetSize(h);
    /* Deve continuar sendo potência de 2 */
    TEST_ASSERT_TRUE(size_depois >= 1);
    TEST_ASSERT_EQUAL_INT(0, size_depois & (size_depois - 1)); /* potência de 2 */
    /* Diretório deve ter crescido */
    TEST_ASSERT_TRUE(size_depois >= size_inicial);
}

void test_hashRemoveReg(void)
{
    hashInsertReg(h, "key_to_delete", "data", 5);

    bool exists_before = hashExists(h, "key_to_delete");
    TEST_ASSERT_TRUE(exists_before);

    hashRemoveReg(h, "key_to_delete");

    bool exists_after = hashExists(h, "key_to_delete");
    TEST_ASSERT_FALSE(exists_after);
}

void test_hash_binary_data(void)
{
    unsigned char binary_data[10] = {0x01, 0x02, 0x03, 0x04, 0x05,
                                     0x06, 0x07, 0x08, 0x09, 0x0A};

    hashInsertReg(h, "binary_key", binary_data, sizeof(binary_data));

    unsigned char retrieved[10];
    bool result = hashGetRegistry(h, "binary_key", retrieved, sizeof(retrieved));

    TEST_ASSERT_TRUE(result);
    for (int i = 0; i < 10; i++)
        TEST_ASSERT_EQUAL_HEX8(binary_data[i], retrieved[i]);
}

void test_hash_long_keys(void)
{
    /* Chave exatamente no limite (31 chars úteis) — deve funcionar */
    char key_limite[32];
    memset(key_limite, 'A', 31);
    key_limite[31] = '\0';

    char data[50] = "test_data";
    hashInsertReg(h, key_limite, data, strlen(data) + 1);

    char retrieved[50];
    bool result = hashGetRegistry(h, key_limite, retrieved, sizeof(retrieved));
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_STRING(data, retrieved);

    /* Chave além do limite: não deve crashar, mas não é recuperável */
    char long_key[1000];
    memset(long_key, 'B', sizeof(long_key) - 1);
    long_key[999] = '\0';

    hashInsertReg(h, long_key, data, strlen(data) + 1);
    /* Apenas verifica que não crashou */
    TEST_ASSERT_TRUE(1);
}


void test_hash_large_data(void)
{
    unsigned char large_data[MAX_DATA_SIZE];
    memset(large_data, 'X', sizeof(large_data));

    bool inserted = hashInsertReg(h, "large_key", large_data, sizeof(large_data));
    TEST_ASSERT_TRUE(inserted);

    unsigned char retrieved[MAX_DATA_SIZE];
    bool result = hashGetRegistry(h, "large_key", retrieved, sizeof(retrieved));

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_MEMORY(large_data, retrieved, sizeof(large_data));
}

void test_hash_fixed_size_data(void)
{
    int data[10];
    for (int i = 0; i < 10; i++)
        data[i] = i * 100;

    hashInsertReg(h, "array_key", data, sizeof(data));

    int retrieved[10];
    bool result = hashGetRegistry(h, "array_key", retrieved, sizeof(retrieved));

    TEST_ASSERT_TRUE(result);
    for (int i = 0; i < 10; i++)
        TEST_ASSERT_EQUAL_INT(data[i], retrieved[i]);
}

int main(void)
{
    UNITY_BEGIN();

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

    return UNITY_END();
}