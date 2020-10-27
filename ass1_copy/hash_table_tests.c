#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "CUnit/Basic.h"
#include "hash_table.h"
#include "list_linked.h"
#include "common.h"

#define Initial_capacity 17

static size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381}; 

/// Helper functions for testing
int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}


void new_values(elem_t key, elem_t *value, void *extra) {
  *value = *(elem_t*)extra;
}

/*
////////////////////TESTS/////////////////////////////
*/

static void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
   CU_ASSERT_PTR_NOT_NULL(ht); 
   ioopm_hash_table_destroy(ht);
}


static void test_empty_lookup()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t lookup_value = int_elem(1);

  for (int i = 0; i < Initial_capacity; i++) 
     {
       elem_t lookup = ioopm_hash_table_lookup(ht, lookup_value);
       CU_ASSERT_EQUAL(No_error(), true); // If nothing was found, errno = EINVAL
       CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, lookup_value));
       lookup_value.i++;
     }
  ioopm_hash_table_destroy(ht);
}


static void test_lookup_inserted()
{
  elem_t key = int_elem(1);
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hi");
  ioopm_hash_table_insert(ht, key, value);
  elem_t lookup = ioopm_hash_table_lookup(ht, key);
  CU_ASSERT_PTR_NOT_NULL(&lookup);
  elem_t elem2 = ptr_elem("Bye");
  for (int i = 1; i < Initial_capacity; i++)
    {
      ioopm_hash_table_insert(ht, key, elem2);
      elem_t lookup = ioopm_hash_table_lookup(ht, key);
      CU_ASSERT_PTR_NOT_NULL(&lookup);
      CU_ASSERT_EQUAL(i, key.i)
      CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key));
      key.i++;
  }
  ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("ass"));
  elem_t lookupvalue = ioopm_hash_table_lookup(ht, int_elem(4));
  CU_ASSERT_EQUAL(lookupvalue.p, "ass");
  ioopm_hash_table_destroy(ht);
}

static void test_lookup_remove_1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hi");
  elem_t key = int_elem(1);
  for (int i = 0; i < Initial_capacity; i++)
    {
      ioopm_hash_table_insert(ht, key, value);
      key.i++;
    }
  elem_t key2 = int_elem(1);
  for (int i = 1; i < Initial_capacity; i++)
    {
      CU_ASSERT_EQUAL(i, key2.i);
      ioopm_hash_table_remove(ht, key2);
      elem_t lookup = ioopm_hash_table_lookup(ht, key2);
      CU_ASSERT_EQUAL(lookup.i, NULL);
      key2.i++;
    }
  ioopm_hash_table_destroy(ht);
}


static void test_size()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hello");
  elem_t start_key = int_elem(1);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  for (int i = 0; i < Initial_capacity; i++)
    {
      ioopm_hash_table_insert(ht, start_key, value);
      start_key.i++;
    }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), Initial_capacity);

  for (int i = 0; i < Initial_capacity; i++)
    {
      ioopm_hash_table_insert(ht, start_key, value);
      start_key.i++;
    }

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), Initial_capacity * 2);
  ioopm_hash_table_destroy(ht);
}


static void test_is_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hello"); 
  elem_t key = int_elem(1);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_remove(ht, key);
  elem_t lookup = ioopm_hash_table_lookup(ht, key);
  CU_ASSERT_EQUAL(lookup.c, NULL);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_clear() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(eq_elem_int, eq_elem_string, NULL);

  ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("Hi"));
  ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("there"));
  ioopm_hash_table_insert(ht, int_elem(3), ptr_elem("bye"));
  ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("byebye"));
  
  elem_t lookup = ioopm_hash_table_lookup(ht, int_elem(4));
  CU_ASSERT_EQUAL(lookup.c, "byebye");
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_clear(ht);
  
  lookup = ioopm_hash_table_lookup(ht, int_elem(4));
  CU_ASSERT_NOT_EQUAL(lookup.c, "byebye");
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

static void test_keys()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hello");
  elem_t key = int_elem(0);
  for (int i = 0; i < Initial_capacity; i++)
    {
      ioopm_hash_table_insert(ht, key, value);
      key.i++;
    }
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  for (int i = 0; i < ioopm_hash_table_size(ht); ++i)
    {
      CU_ASSERT_TRUE(ioopm_linked_list_get(keys, i).i == i);
    }
  ioopm_linked_list_destroy(keys);
  ioopm_hash_table_destroy(ht);
}


static void test_values()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hello");
  elem_t key = int_elem(1);
  for (int i = 0; i < Initial_capacity; i++)
    {
      ioopm_hash_table_insert(ht, key, value);
      key.i++;
    }
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  for (int i = 0; i < Initial_capacity; ++i)
    {
      CU_ASSERT_EQUAL(ioopm_linked_list_get(values, i).c, "Hello"); 
    }
  ioopm_linked_list_destroy(values);
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_all() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(eq_elem_int, eq_elem_string, NULL);

  elem_t value = ptr_elem("hello");

  ioopm_hash_table_insert(ht, int_elem(1), value);
  ioopm_hash_table_insert(ht, int_elem(2), value);
  ioopm_hash_table_insert(ht, int_elem(3), value);
  ioopm_hash_table_insert(ht, int_elem(4), value);
  ioopm_hash_table_insert(ht, int_elem(5), value);
  ioopm_hash_table_insert(ht, int_elem(6), value);
  ioopm_hash_table_insert(ht, int_elem(7), value);
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, value_equiv, &value));

  ioopm_hash_table_insert(ht, int_elem(8), ptr_elem("goodbye"));

  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, value_equiv, &value));

  ioopm_hash_table_destroy(ht);
}

void test_hash_table_apply_all() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(eq_elem_int, eq_elem_string, NULL);

  elem_t value = ptr_elem("hello");
  elem_t replaced_value = ptr_elem("goodbye");

  ioopm_hash_table_insert(ht, int_elem(1), value);
  ioopm_hash_table_insert(ht, int_elem(2), value);
  ioopm_hash_table_insert(ht, int_elem(3), value);
  ioopm_hash_table_insert(ht, int_elem(4), value);
  ioopm_hash_table_insert(ht, int_elem(5), value);
  ioopm_hash_table_insert(ht, int_elem(6), value);
  ioopm_hash_table_insert(ht, int_elem(7), value);
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, value_equiv, &value));
  ioopm_hash_table_apply_to_all(ht, new_values, &replaced_value);
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, value_equiv, &value));
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, value_equiv, &replaced_value));

  ioopm_hash_table_destroy(ht);
}

static void test_has_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hello");
  elem_t second_value = ptr_elem("Bye");
  elem_t key = int_elem(1);
  elem_t second_key = int_elem(2);
  ioopm_hash_table_insert(ht, key, value);
  ioopm_hash_table_insert(ht, second_key, second_value);
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, value));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, second_value));
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("Nope")));
  ioopm_hash_table_destroy(ht);
}


static void test_has_key()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hello");
  elem_t key = int_elem(0);
  for (int i = 0; i < Initial_capacity; i++)
  {
    ioopm_hash_table_insert(ht,key, value);
    key.i++;
  }
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(5)));
  ioopm_hash_table_insert(ht, key, ptr_elem("bye"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(0)));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(20)));
  ioopm_hash_table_destroy(ht);
}


static void test_resize()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  elem_t value = ptr_elem("Hello");
  elem_t key  = int_elem(1);
  size_t capacity = ioopm_return_capacity(ht);
  for (int i = 0; i < primes[0] * Load_factor; i++)
    {
      ioopm_hash_table_insert(ht, key, value);
      key.i++;
    }
  size_t capacity2 = ioopm_return_capacity(ht);
  CU_ASSERT_EQUAL(capacity, primes[0]);
  for (int i = primes[0]*Load_factor; i < primes[1]*Load_factor; i++)
    {
      ioopm_hash_table_insert(ht, key, value);
      key.i++;
    }
  CU_ASSERT_EQUAL(capacity2, primes[1]);
  ioopm_hash_table_destroy(ht);
}


int main(void)
{
  CU_pSuite test_suite = NULL;
  
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
  
  test_suite = CU_add_suite("Hash_table", init_suite, clean_suite);
  
  if (NULL == test_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  if (NULL == CU_add_test(test_suite, "Create/destroy:", test_create_destroy) ||
      (NULL == CU_add_test(test_suite, "Lookup, empty:", test_empty_lookup)) ||
      (NULL == CU_add_test(test_suite, "Lookup, not empty:", test_lookup_inserted)) ||
      (NULL == CU_add_test(test_suite, "Lookup, after remove:", test_lookup_remove_1)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_size", test_size)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_is_empty", test_is_empty)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_clear", test_hash_table_clear)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_keys", test_keys)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_values", test_values)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_all", test_hash_table_all)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_apply_to_all", test_hash_table_apply_all)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_value", test_has_value)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_has_key", test_has_key)) ||
      (NULL == CU_add_test(test_suite, "Hash_table_resize", test_resize))
      ) {

    CU_cleanup_registry();
    return CU_get_error();
  }
  
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
