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
    remove_cart(store, id);
}

void add_to_cart(){
    webstore_t *store = store_create();
    cart_t cart = create_cart(store);
    
    //add merch to store
    add_merchendise(store, "Cola", "from coca cola", (size_t)10);
    add_merchendise(store, "Chair", "Usable", (size_t)8);
    add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
    add_merchendise(store, "Car", "A fast car", (size_t)2);
    add_merchendise(store, "Computer", "Gaming computer", (size_t)0);
    
    char *merch_to_add;
    int amount = 0; 
    
    //add merch to cart
    for(int nr_merch = 1; nr_merch < 6; nr_merch++){
        amount++;
        add_to_cart(store, cart->id, nr_merch, amount);
    }
    
    size_t cart_size = nr_of_merch_in_cart(cart);
    CU_ASSERT_EQUAL(5, cart_size); 
    
    
    //int get_amount_of_merch(cart_t *cart, char *merch_name); 

    //bool merch_in_cart(cart_t *cart, char *merch_name)
}

void add_to_cart_wrong_id(){
    webstore_t *store = store_create();
    cart_t cart = create_cart(store);
    
    //add merch to store
    add_merchendise(store, "Cola", "from coca cola", (size_t)10);
    add_merchendise(store, "Chair", "Usable", (size_t)8);
    add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
    add_merchendise(store, "Car", "A fast car", (size_t)2);
    add_merchendise(store, "Computer", "Gaming computer", (size_t)0);
    
    add_to_cart(store, cart->id, nr_merch, amount);
    
}

void add_merch_large_amount(){
    
}

void add_zero_merch(){
    
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

  if ((NULL == CU_add_test(test_suite1, "test",   test))){
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
