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
/////////////////////////////////////////////////////////////

// Store 

//passes if there is no memory leaks
void create_destroy_store(){
  webstore_t *store = store_create();
  store_destroy(store);
}

//Merch

void create_destoy_merch(){
  webstore_t *store = store_create();
  //add_merchendise(store, "Bike", "A sports bike from Brazil",	(size_t)4);
	store_destroy(store);
}

void test_add_merch(){
  
  webstore_t *store = store_create();
  
  add_merchendise(store, "Bike", "A sports bike from Brazil",
		  (size_t)4);
  add_merchendise(store, "Car", "A fast car",
		  (size_t)2);

  // --- Bike
  CU_ASSERT_TRUE(merch_in_stock(store, "Bike"));
    
  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Bike"),
			"A sports bike from Brazil"));
  set_merch_description(store, "Bike", "x");  
  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Bike"),
			"x"));

  CU_ASSERT_TRUE(merch_price(store, "Bike") == (size_t)4);
  set_merch_price(store, "Bike", 2);
  CU_ASSERT_TRUE(merch_price(store, "Bike") == (size_t)2);
  
  // --- Car
  CU_ASSERT_TRUE(merch_in_stock(store, "Car"));
  
  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Car"),
			"A fast car"));
  set_merch_description(store, "Car", "x");  
  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Car"),
			"x"));

  CU_ASSERT_TRUE(merch_price(store, "Car") == (size_t)2);
  set_merch_price(store, "Car", 4);
  CU_ASSERT_TRUE(merch_price(store, "Car") == (size_t)4);
  
  store_destroy(store);
}

//new
void test_remove_merch(){
  webstore_t *store = store_create();
  
  add_merchendise(store, "Bike", "A sports bike from Brazil",
		  (size_t)4);
  add_merchendise(store, "Car", "A fast car",
		  (size_t)2);
	
	remove_merchendise(store, "Car");
  remove_merchendise(store, "Bike");

  CU_ASSERT_FALSE(merch_in_stock(store, "Car"));
  CU_ASSERT_FALSE(merch_in_stock(store, "Bike"));

    
  CU_ASSERT_FALSE(STR_EQ(merch_description(store, "Car"),
			"A fast car"));
  CU_ASSERT_FALSE(merch_price(store, "Car") == (size_t)2);

  CU_ASSERT_FALSE(STR_EQ(merch_description(store, "Bike"),
			"A sports bike from Brazil"));
  CU_ASSERT_FALSE(merch_price(store, "Bike") == (size_t)4);
	
	store_destroy(store);
}

//new
void test_remove_non_existing_merch(){
  webstore_t *store = store_create();
  
  add_merchendise(store, "Bike", "A sports bike from Brazil",
		  (size_t)4);
  add_merchendise(store, "Car", "A fast car",
		  (size_t)2);
	
	CU_ASSERT_FALSE(merch_in_stock(store, "House"));
  CU_ASSERT_FALSE(merch_in_stock(store, "Big House"));
	
	remove_merchendise(store, "House");
  remove_merchendise(store, "Big House");

  CU_ASSERT_TRUE(merch_in_stock(store, "Car"));
  CU_ASSERT_TRUE(merch_in_stock(store, "Bike"));

	store_destroy(store);
}

//new 
void test_remove_merch_with_shelfs(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  char *new_shelf_name = "A22";
  size_t amount = 3; 
  
  //remove merch wioth one shelf
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);

  set_shelf(store, "Bike",shelf_name, amount);
  
  remove_merchendise(store, "Bike");
  
  CU_ASSERT_FALSE(merch_in_stock(store, "Bike"));

  //remove merch with multiple shelfs
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
  
  set_shelf(store, "Bike",shelf_name, amount);
  set_shelf(store, "Bike", new_shelf_name, amount);

  remove_merchendise(store, "Bike");
  
  CU_ASSERT_FALSE(merch_in_stock(store, "Bike"));

	store_destroy(store);
}

//new 
void test_change_stock_relative_amount(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  size_t amount = 3; 
  char *name = "Bike"; 
  
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
  set_shelf(store, "Bike",shelf_name, amount);
  CU_ASSERT_EQUAL(merch_stock(store, "Bike"), amount); 
  
  //add 3 to the stock
  change_stock_relative_amount(store, name, amount);
  CU_ASSERT_EQUAL(merch_stock(store, "Bike"), (amount*2)); 
  
  //decrease stock with -3
  change_stock_relative_amount(store, name, (amount*-1));
  CU_ASSERT_EQUAL(merch_stock(store, "Bike"), amount); 
  
  //don´t change the stock
  change_stock_relative_amount(store, name, 0);
  CU_ASSERT_EQUAL(merch_stock(store, "Bike"), amount); 

	store_destroy(store);
}

//new 
void test_det_merch_describtion(){
  webstore_t *store = store_create();
  char *new_desc = "new desc!"; 
  
  add_merchendise(store, "Bike", "A sports bike from Brazil",
		  (size_t)4);
  add_merchendise(store, "Car", "A fast car",
		  (size_t)2);
	
	CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Car"),
			"A fast car"));

  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Bike"),
			"A sports bike from Brazil"));
	
  set_merch_description(store,  "Bike", new_desc);
  set_merch_description(store,  "Car", new_desc);
  
  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Car"),
			new_desc));

  CU_ASSERT_TRUE(STR_EQ(merch_description(store, "Bike"),
			new_desc));
			
	store_destroy(store);
	
}

//new void set_merch_price(webstore_t *store, char *name, size_t price);
void test_set_merch_price(){
  webstore_t *store = store_create();
  
  add_merchendise(store, "Bike", "A sports bike from Brazil",
		  (size_t)4);
  add_merchendise(store, "Car", "A fast car",
		  (size_t)2);
		  
	CU_ASSERT_TRUE(merch_price(store, "Bike")==((size_t) 4)); 
	CU_ASSERT_TRUE(merch_price(store, "Car")==((size_t) 2));
	
	set_merch_price(store, "Bike", (size_t)1); 
	set_merch_price(store, "Bike", (size_t)3); 
	
	CU_ASSERT_TRUE(merch_price(store, "Bike")==((size_t) 1)); 
	CU_ASSERT_TRUE(merch_price(store, "Car")==((size_t) 3));

  //Set price to the same as before
  set_merch_price(store, "Bike", (size_t)1); 
	CU_ASSERT_TRUE(merch_price(store, "Bike")==((size_t) 1)); 
	
	store_destroy(store);
}

void test_set_price_zero_or_lessa(){
  webstore_t *store = store_create();
  
  add_merchendise(store, "Bike", "A sports bike from Brazil",
		  (size_t)4);
  add_merchendise(store, "Car", "A fast car",
		  (size_t)2);
		  
	CU_ASSERT_TRUE(merch_price(store, "Bike")==((size_t) 4)); 
	CU_ASSERT_TRUE(merch_price(store, "Car")==((size_t) 2));
	
	set_merch_price(store, "Bike", (size_t) -1); 
	set_merch_price(store, "Bike", (size_t) 0); 
	
	CU_ASSERT_TRUE(merch_price(store, "Bike")==((size_t) 4)); 
	CU_ASSERT_TRUE(merch_price(store, "Car")==((size_t) 2));

	
	store_destroy(store);
}

//Shelf

//passes if there is no memory leaks
void create_destroy_shelf(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  shelf_t *shelf_created = create_shelf(shelf_name, 3);
  CU_ASSERT_EQUAL(shelf_name, shelf_created->shelf);
  destroy_shelf(shelf_created);
  store_destroy(store);
}

void set_shelf_test(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  size_t amount = 3; 
  
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);

  set_shelf(store, "Bike",shelf_name, amount);

  CU_ASSERT_TRUE(merch_stock(store, "Bike") == amount);
  increase_stock(store, "Bike", shelf_name, 1);
  CU_ASSERT_TRUE(merch_stock(store, "Bike") == amount + 1);
  
  set_shelf(store, "Bike",  "", 2);
  CU_ASSERT_FALSE(merch_stock(store, "") == 2);
  set_shelf(store, "",  "Bike", 2);
  CU_ASSERT_FALSE(merch_stock(store, "") == 2);
  
  store_destroy(store);
}

//new 
void set_shelf_neg_amount(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  size_t amount = -1; 
  
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);

  CU_ASSERT_TRUE(merch_stock(store, "Bike") == 0);

  set_shelf(store, "Bike",shelf_name, amount);

  CU_ASSERT_FALSE(merch_stock(store, "Bike") == amount);
 
  store_destroy(store);
}

void test_remove_shelf(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  char *new_shelf_name = "A22";
  size_t amount = 3; 
  
  //remove merch wioth one shelf
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);

  set_shelf(store, "Bike",shelf_name, amount);
  
  remove_merchendise(store, "Bike");
  
  CU_ASSERT_FALSE(merch_in_stock(store, "Bike"));

  //remove merch with multiple shelfs
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
  
  set_shelf(store, "Bike",shelf_name, amount);
  set_shelf(store, "Bike", new_shelf_name, amount);

  remove_merchendise(store, "Bike");
  
  CU_ASSERT_FALSE(merch_in_stock(store, "Bike"));

	store_destroy(store);
}

void test_add_remove_storage(){
  webstore_t *store = store_create();

  add_to_storage(store, "A", "A10");
  add_to_storage(store, "B", "A10");
  add_to_storage(store, "C", "A10");


  CU_ASSERT_TRUE(storage_contains(store, "A10", "A"));
  CU_ASSERT_TRUE(storage_contains(store, "A10", "B"));
  CU_ASSERT_TRUE(storage_contains(store, "A10", "C"));
  
  CU_ASSERT_TRUE(ioopm_linked_list_size(get_locations(store, "A10")) == 3);

  remove_name_from_shelf(store, "A10", "A");
  remove_name_from_shelf(store, "A10", "B");
  
  CU_ASSERT_FALSE(storage_contains(store, "A10", "A"));
  CU_ASSERT_FALSE(storage_contains(store, "A10", "B"));
  CU_ASSERT_TRUE(storage_contains(store, "A10", "C"));
 
  add_to_storage(store, "A", "A10");
  add_to_storage(store, "C", "A10");

  remove_shelf(store, "A10");

  CU_ASSERT_FALSE(storage_contains(store, "A10", "A"));
  CU_ASSERT_FALSE(storage_contains(store, "A10", "B"));
  CU_ASSERT_FALSE(storage_contains(store, "A10", "C"));

  store_destroy(store);
}

//new
void test_set_merch_stock(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  size_t amount = 3; 
  size_t new_amount = 17; 
  
  //remove merch wioth one shelf
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
  add_merchendise(store, "Car", "A fast car", (size_t)2);

  set_shelf(store, "Bike",shelf_name, amount);
  set_shelf(store, "Car",shelf_name, amount);
  
  CU_ASSERT_EQUAL(merch_stock(store,"Bike"), amount);
  CU_ASSERT_EQUAL(merch_stock(store,"Car"), amount);
  
  set_merch_stock(store, "Bike",new_amount, shelf_name);
  set_merch_stock(store, "Car", (new_amount-amount), shelf_name);
  
  CU_ASSERT_EQUAL(merch_stock(store,"Bike"), new_amount);
  CU_ASSERT_EQUAL(merch_stock(store,"Car"), (new_amount-amount));

	store_destroy(store);
}

//new
void test_set_merch_stock_zero_less(){
  webstore_t *store = store_create();
  char *shelf_name = "A21";
  size_t amount = 3; 
  size_t new_amount = -1;
  size_t zero_amount = 0;

  
  //remove merch wioth one shelf
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
  add_merchendise(store, "Car", "A fast car", (size_t)2);

  set_shelf(store, "Bike",shelf_name, amount);
  set_shelf(store, "Car",shelf_name, amount);
  
  CU_ASSERT_EQUAL(merch_stock(store,"Bike"), amount);
  CU_ASSERT_EQUAL(merch_stock(store,"Car"), amount);
  
  set_merch_stock(store, "Bike",new_amount, shelf_name);
  set_merch_stock(store, "Car", (zero_amount), shelf_name);
  
  CU_ASSERT_EQUAL(merch_stock(store,"Bike"), amount);
  CU_ASSERT_EQUAL(merch_stock(store,"Car"), amount);

	store_destroy(store);
}

void index_lookup_test(){
  webstore_t *store = store_create();
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);

  CU_ASSERT_TRUE(valid_index(store, 0));
  CU_ASSERT_TRUE(valid_index(store, 1));
  
 CU_ASSERT_TRUE(STR_EQ("Bike", lookup_merch_name(store, 0)));

 CU_ASSERT_TRUE(STR_EQ("", lookup_merch_name(store, 1)));
 CU_ASSERT_FALSE(STR_EQ("B", lookup_merch_name(store, 0)));
   
 
 store_destroy(store);
  
}

void num_shelf_validation_test(){
  CU_ASSERT_FALSE((!is_shelf("A00")) || (is_shelf("AA0")) ||
		  (is_shelf("000"))  || (is_shelf("0A0")) ||
		  (is_shelf("0AA"))  || (is_shelf(""))    ||
		  (is_shelf("AAAA")  || (is_shelf("A00A"))));

  CU_ASSERT_FALSE((is_number("A111")) || is_number("1A11") ||
		  (is_number("111A")) || is_number("11A1") ||
		  (is_number("")) || is_number("AAAA"));
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

  if ((NULL == CU_add_test(test_suite1, "Create Destroy Store Test",   create_destroy_store)) ||
      (NULL == CU_add_test(test_suite1, "Create Destroy Merch Test",   create_destoy_merch))   ||
      //(NULL == CU_add_test(test_suite1, "Destroy All Merch Test",   test_destroy_all_merch))   //||
      (NULL == CU_add_test(test_suite1, "Create Destroy Shelf Test",   create_destroy_shelf))   ||
      //(NULL == CU_add_test(test_suite1, "Add Merch Test",   test_add_merch))   ||
      (NULL == CU_add_test(test_suite1, "Storage Test", test_storage)) ||
      (NULL == CU_add_test(test_suite1, "Locs Test",    test_locs))    ||
      //(NULL == CU_add_test(test_suite1, "Set Shelf Test",    set_shelf_test))  //||
      (NULL == CU_add_test(test_suite1, "Add Remove Storage Test", test_add_remove_storage)) //||
      //(NULL == CU_add_test(test_suite1, "Sync Test",    test_sync))    ||
      //(NULL == CU_add_test(test_suite1, "Index Lookup (Misc) Test",    index_lookup_test)))
      )
      {
      CU_cleanup_registry();
      return CU_get_error();
    }
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
