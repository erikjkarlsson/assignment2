#include <string.h>
#include<stdio.h> 
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "hash_table.h"
#include "list_linked.h"
#include "webstore.h"
#include "common.h"


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
/////////////////////////////////////////////////////////////
void test_merch(){
  webstore_t *store = store_create();
  
  add_merchendise(store, "Bike", "A sports bike from Brazil",
		  (size_t)4);
  add_merchendise(store, "Car", "A fast car",
		  (size_t)2);

  // --- Bike
  CU_ASSERT_TRUE(merch_in_stock(store, "Bike"));
    
  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Bike"),
			"A sports bike from Brazil"));

  CU_ASSERT_TRUE(merch_price(store, "Bike") == (size_t)4);

  // --- Car
  CU_ASSERT_TRUE(merch_in_stock(store, "Car"));
  
  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Car"),
			"A fast car"));
  CU_ASSERT_TRUE(merch_price(store, "Car") == (size_t)2);
  
  remove_merchendise(store, "Car");
  remove_merchendise(store, "Bike");
  
  store_destroy(store);
}
/////////////////////////////////////////////////////////////
void test_storage(){
  
}
/////////////////////////////////////////////////////////////
void test_locs(){
  
}
/////////////////////////////////////////////////////////////
void test_sync(){
  
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

  if ((NULL == CU_add_test(test_suite1, "Merch Test",   test_merch))   ||
      (NULL == CU_add_test(test_suite1, "Storage Test", test_storage)) ||
      (NULL == CU_add_test(test_suite1, "Locs Test",    test_locs))    ||
      (NULL == CU_add_test(test_suite1, "Sync Test",    test_sync))){
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
