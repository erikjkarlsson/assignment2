#include <string.h>
#include<stdio.h> 
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "hash_table.h"
#include "list_linked.h"
#include "webstore.h"
#include "common.h"
#include "cart.h"


int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}


/////////////////////////////////////////////////////////////

void test(){
    CU_ASSERT_TRUE(true);
}

void create_destroy_cart(){
    webstore_t *store = store_create();
    cart_t *cart = create_cart(store);
    remove_cart(store, cart->id);
}

/////////////////////////////////////////////////////////////
int main()
{
  CU_pSuite test_suite1 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Tests API", init_suite, clean_suite);
  if (NULL == test_suite1){
      CU_cleanup_registry();
      return CU_get_error();
  }

  if ((NULL == CU_add_test(test_suite1, "Create Destroy Cart Test",   create_destroy_cart)))   //||
     // (NULL == CU_add_test(test_suite1, "Add to Cart Test",   add_to_cart){
     {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
