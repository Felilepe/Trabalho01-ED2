#include "../unity.h"
#include "../lista.h"
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

static int contador_passthrough = 0;
static void callback_incrementar(item i, item aux_data) 
{
    (void)i; (void)aux_data;
    contador_passthrough++;
}

void test_lista_create(void) 
{
    Lista *l = lista_create();

    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_TRUE(lista_isEmpty(l));
    TEST_ASSERT_EQUAL_INT(0, lista_getSize(l));

    lista_destroy(l);
}

void test_lista_insertHead_single(void) 
{
    Lista *l = lista_create();
    int data = 42;

    lista_insertHead(l, (item)(intptr_t)data);

    TEST_ASSERT_FALSE(lista_isEmpty(l));
    TEST_ASSERT_EQUAL_INT(1, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(data, (int)(intptr_t)lista_getHead(l));
    TEST_ASSERT_EQUAL_INT(data, (int)(intptr_t)lista_getTail(l));

    lista_destroy(l);
}

void test_lista_insertTail_single(void) 
{
    Lista *l = lista_create();
    int data = 99;

    lista_insertTail(l, (item)(intptr_t)data);

    TEST_ASSERT_FALSE(lista_isEmpty(l));
    TEST_ASSERT_EQUAL_INT(1, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(data, (int)(intptr_t)lista_getTail(l));

    lista_destroy(l);
}

void test_lista_insertHead_multiple(void) 
{
    Lista *l = lista_create();

    lista_insertHead(l, (item)(intptr_t)1);
    lista_insertHead(l, (item)(intptr_t)2);
    lista_insertHead(l, (item)(intptr_t)3);

    TEST_ASSERT_EQUAL_INT(3, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(3, (int)(intptr_t)lista_getHead(l));
    TEST_ASSERT_EQUAL_INT(1, (int)(intptr_t)lista_getTail(l));

    lista_destroy(l);
}

void test_lista_insertTail_multiple(void) 
{
    Lista *l = lista_create();

    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertTail(l, (item)(intptr_t)2);
    lista_insertTail(l, (item)(intptr_t)3);

    TEST_ASSERT_EQUAL_INT(3, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(1, (int)(intptr_t)lista_getHead(l));
    TEST_ASSERT_EQUAL_INT(3, (int)(intptr_t)lista_getTail(l));

    lista_destroy(l);
}

void test_lista_removeHead(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertTail(l, (item)(intptr_t)2);
    lista_insertTail(l, (item)(intptr_t)3);

    lista_removeHead(l);
    TEST_ASSERT_EQUAL_INT(2, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(2, (int)(intptr_t)lista_getHead(l));

    lista_destroy(l);
}

void test_lista_removeTail(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertTail(l, (item)(intptr_t)2);
    lista_insertTail(l, (item)(intptr_t)3);

    lista_removeTail(l);
    TEST_ASSERT_EQUAL_INT(2, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(2, (int)(intptr_t)lista_getTail(l));

    lista_destroy(l);
}

void test_lista_removeNode_by_index(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)10);
    lista_insertTail(l, (item)(intptr_t)20);
    lista_insertTail(l, (item)(intptr_t)30);
    lista_insertTail(l, (item)(intptr_t)40);

    lista_removeNode(l, 1); /* remove o índice 1 → valor 20 */

    TEST_ASSERT_EQUAL_INT(3,  lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(10, (int)(intptr_t)lista_getItem(l, 0));
    TEST_ASSERT_EQUAL_INT(30, (int)(intptr_t)lista_getItem(l, 1));
    TEST_ASSERT_EQUAL_INT(40, (int)(intptr_t)lista_getItem(l, 2));

    lista_destroy(l);
}

void test_lista_getItem_by_index(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)100);
    lista_insertTail(l, (item)(intptr_t)200);
    lista_insertTail(l, (item)(intptr_t)300);

    TEST_ASSERT_EQUAL_INT(100, (int)(intptr_t)lista_getItem(l, 0));
    TEST_ASSERT_EQUAL_INT(200, (int)(intptr_t)lista_getItem(l, 1));
    TEST_ASSERT_EQUAL_INT(300, (int)(intptr_t)lista_getItem(l, 2));

    lista_destroy(l);
}

void test_lista_passthrough(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertTail(l, (item)(intptr_t)2);
    lista_insertTail(l, (item)(intptr_t)3);
    lista_insertTail(l, (item)(intptr_t)4);
    lista_insertTail(l, (item)(intptr_t)5);

    contador_passthrough = 0;
    lista_passthrough(l, callback_incrementar, NULL);

    TEST_ASSERT_EQUAL_INT(5, contador_passthrough);

    lista_destroy(l);
}

void test_lista_removeAll(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertTail(l, (item)(intptr_t)2);
    lista_insertTail(l, (item)(intptr_t)3);

    lista_removeHead(l);
    lista_removeHead(l);
    lista_removeHead(l);

    TEST_ASSERT_TRUE(lista_isEmpty(l));
    TEST_ASSERT_EQUAL_INT(0, lista_getSize(l));

    lista_destroy(l);
}

void test_lista_removeTail_until_empty(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertTail(l, (item)(intptr_t)2);

    lista_removeTail(l);
    TEST_ASSERT_EQUAL_INT(1, lista_getSize(l));

    lista_removeTail(l);
    TEST_ASSERT_TRUE(lista_isEmpty(l));

    lista_destroy(l);
}

void test_lista_mixed_operations(void) 
{
    Lista *l = lista_create();

    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertHead(l, (item)(intptr_t)0);
    lista_insertTail(l, (item)(intptr_t)2);

    TEST_ASSERT_EQUAL_INT(3, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(0, (int)(intptr_t)lista_getHead(l));
    TEST_ASSERT_EQUAL_INT(2, (int)(intptr_t)lista_getTail(l));

    lista_removeHead(l);
    TEST_ASSERT_EQUAL_INT(2, lista_getSize(l));

    lista_destroy(l);
}

void test_lista_getItem_empty_list(void) 
{
    Lista *l = lista_create();

    item result = lista_getItem(l, 0);
    TEST_ASSERT_NULL(result);

    lista_destroy(l);
}

void test_lista_getItem_out_of_bounds(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)1);
    lista_insertTail(l, (item)(intptr_t)2);

    item result = lista_getItem(l, 5);
    TEST_ASSERT_NULL(result);

    lista_destroy(l);
}

void test_lista_removeHead_single_element(void) 
{
    Lista *l = lista_create();
    lista_insertTail(l, (item)(intptr_t)42);

    lista_removeHead(l);

    TEST_ASSERT_TRUE(lista_isEmpty(l));
    TEST_ASSERT_EQUAL_INT(0, lista_getSize(l));

    lista_destroy(l);
}

void test_lista_refill_after_empty(void) 
{
    Lista *l = lista_create();

    lista_insertTail(l, (item)(intptr_t)1);
    lista_removeHead(l);

    lista_insertTail(l, (item)(intptr_t)2);
    lista_insertTail(l, (item)(intptr_t)3);

    TEST_ASSERT_EQUAL_INT(2, lista_getSize(l));
    TEST_ASSERT_EQUAL_INT(2, (int)(intptr_t)lista_getHead(l));
    TEST_ASSERT_EQUAL_INT(3, (int)(intptr_t)lista_getTail(l));

    lista_destroy(l);
}

int main(void)
{
    UNITY_BEGIN();

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

    return UNITY_END();
}