#include <string.h>
#include <stdio.h> 
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "hash_table.h"
#include "list_linked.h"
#include "webstore.h"
#include "common.h"
#include "utils.h"
#include "cart2.h"
#include "ui.h"
#include "merch.h"


int init_suite(void){
  return 0;
}

int clean_suite(void){
  return 0;
}

void test(){
    CU_ASSERT_TRUE(true);
}

void create_destroy_merch(){
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
  
  char *eggs            = "Eggs";
  char *eggs_shelf      = "G03";
    
  webstore_t *store = store_create();

//  save_str(store, new_item_name);
//  save_str(store, new_item_desc);
//
//  save_str(store, new_item_shelf);
//  save_str(store, eggs);
//  save_str(store, eggs_shelf);  
  CU_ASSERT_FALSE(shelf_exists(store, eggs_shelf));
  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);

  // Eggs remain
  CU_ASSERT_FALSE(merch_stock_on_shelf(store, eggs, eggs_shelf) == 0);
  CU_ASSERT_TRUE(merch_in_stock(store, eggs));
  // Was the shelf created?
  CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));
  //  CU_ASSERT_TRUE(storage_contains(store, eggs, eggs_shelf));

  // Remove eggs using a differently allocated, but equal string.
  remove_merchendise(store, eggs);

  // No eggs remain
  CU_ASSERT_TRUE(merch_stock_on_shelf(store, eggs, eggs_shelf) == 0);
  CU_ASSERT_FALSE(merch_in_stock(store, eggs));
  // Test if the shelf was deleted upon deletion of merch
  // CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));  
  store_destroy(store);
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

  if (NULL == CU_add_test(test_suite1, "Create and Remove Merch",
			  create_destroy_merch)){
       
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}



