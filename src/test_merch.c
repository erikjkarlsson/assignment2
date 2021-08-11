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
#include "cart.h"
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
////////////////////////////////////////////
// Needs to be tested	     // Is Tested //
////////////////////////////////////////////
// add_merchendise	     //
// add_to_storage	     //
// create_merch		     // Yes
// create_shelf		     // Yes
// destroy_all_merch	     // Yes
// destroy_locs		     //
// destroy_merchendise	     // Yes
// destroy_shelf	     // Yes
// destroy_storage	     //
// ? display_shelf	     // ?
// free_saved_strs	     //
// free_str		     //
// get_locations	     //
// get_merch_name_in_storage //
// get_shelf_after_shelf_nr  //
// increase_equal_stock	     //
// increase_stock	     //
// is_saved_str		     //
// list_merchandise	     //
// list_shelfs		     //
// lookup_merch_name	     //
// merch_description	     //
// merch_in_stock	     //
// merch_locs		     //
// merch_stock		     //
// merch_stock_on_shelf	     //
// X parse_args		     //
// print_merch		     //
// remove_from_storage	     //
// remove_merchendise	     //
// remove_name_from_shelf    //
// remove_shelf		     //
// rename_merch		     //
// save_str		     //
// set_merch_description     //
// set_merch_price	     //
// set_merch_stock	     //
// set_shelf		     //
// ? show_stock		     //
// shelf_exists		     //
// storage_contains	     //
// store_create		     //
// store_destroy	     //
// sync_merch_stock	     //
// valid_index		     //
////////////////////////////////////////////



// 
// ################################### CREATE AND DESTROY MERCH
void create_destroy_merch_test(void){
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
  
  char *eggs            = "Eggs";
  char *eggs_shelf      = "G03";
    
  webstore_t *store = store_create();

  CU_ASSERT_FALSE(shelf_exists(store, eggs_shelf));
  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);
  
  // Remove eggs using a differently allocated, but equal string.
  remove_merchendise(store, eggs);

  store_destroy(store);
}


void destroy_all_merch_test(void) {
  webstore_t *store = store_create();
  INIT_DATABASE(store);

  destroy_all_merch(store);


  
  store_destroy(store);  
}
void create_destroy_merch_shelf_test(void){
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
  
  char *eggs            = "Eggs";
  char *eggs_shelf      = "G03";
    
  webstore_t *store = store_create();

  // Shelf does not exist prior to adding item
  CU_ASSERT_FALSE(shelf_exists(store, eggs_shelf));  

  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);

  // Shelf was created after adding merch
  CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));  

  // Correct amount of the merch stock was added on the correct shelf
  CU_ASSERT_TRUE(merch_stock_on_shelf(store, eggs, eggs_shelf) == new_item_stock);
  
  // Remove eggs using a differently allocated, but equal string.n
  remove_merchendise(store, eggs);

  // No eggs remain
  CU_ASSERT_TRUE(merch_stock_on_shelf(store, eggs, eggs_shelf) == 0);
  CU_ASSERT_FALSE(merch_in_stock(store, eggs));

  // Test if the shelf was deleted upon deletion of merch
  CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));
  
  store_destroy(store);
}


void create_autodestroy_merch_test(void){
  // Add merchendise, but let the store destroy function remove the merch
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
    
  webstore_t *store = store_create();

  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   	   new_item_shelf, new_item_stock);

  // Dont Remove eggs
  // remove_merchendise(store, eggs);

  // No eggs remain

  // CU_ASSERT_TRUE(shelf_exists(store, eggs_shelf));  
  store_destroy(store);

}

void create_duplicate_merch_test(void){
  char  *new_item_name  = "Eggs";
  char  *new_item_desc  = "Yummy!";
  size_t new_item_price = (size_t)16.99;

  char  *new_item_shelf = "G03";
  int    new_item_stock =  2000;
  
  char *eggs            = "Eggs";

    
  webstore_t *store = store_create();

  
  // Add eggs as merchendise 
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);

  // ######################### ADD ASSERTIONS FOR THIS TODO
  // Duplicate merch! Error!
  new_item(store, new_item_name, new_item_desc, new_item_price,
	   new_item_shelf, new_item_stock);

  
  // Remove eggs using a differently allocated, but equal string.n
  remove_merchendise(store, eggs);
  
  store_destroy(store);
}

void create_merch_test_empty_locs(void){ // (create_merch | destroy_merchendise) 
  ioopm_list_t *new_locs  = ioopm_linked_list_create();
  merch_t      *new_merch = 
    create_merch("Tuna", "Raw Tuna", 123, new_locs);    

  CU_ASSERT_TRUE(STR_EQ(new_merch->name, "Tuna"));
  CU_ASSERT_TRUE(STR_EQ(new_merch->desc, "Raw Tuna"));
  CU_ASSERT_EQUAL(new_merch->price, 123);
  CU_ASSERT_EQUAL(new_merch->total_amount, 0);
  CU_ASSERT_EQUAL(new_merch->locs->size, 0);

  destroy_merchendise(new_merch);
}



void create_merch_test_populated_locs(void){
  ioopm_list_t *new_locs  = ioopm_linked_list_create();
  merch_t      *new_merch = 
    create_merch("Tuna", "Raw Tuna", 123, new_locs);    

  CU_ASSERT_TRUE(STR_EQ(new_merch->name, "Tuna"));
  CU_ASSERT_TRUE(STR_EQ(new_merch->desc, "Raw Tuna"));
  CU_ASSERT_EQUAL(new_merch->price, 123);
  CU_ASSERT_EQUAL(new_merch->total_amount, 0);
  CU_ASSERT_EQUAL(new_merch->locs->size, 0);

  
  char *shelfs[5] = {"A01", "A02", "A03", "A04", "A05"};
  size_t amounts[5] = {20, 21, 22, 23, 24};
  for (int i = 0; i < 5; i++){
    shelf_t *shelf = create_shelf(shelfs[i], amounts[i]);
    ioopm_linked_list_append(new_merch->locs, ptr_elem(shelf));
    // Assert that the merch shelf db grows correctly
    CU_ASSERT_TRUE(new_merch->locs->size == i + 1);
  }

  destroy_merchendise(new_merch);
}

void str_memory_management_system_test(void){
  char  *str1  = NULL;
  char  *str2  = NULL;
  char  *str3  = NULL;
  
  webstore_t *store = store_create();
  //  str1 = malloc(sizeof(char) * 5);
  
  str1 = strdup("heap allocated 1\0");
  str2 = strdup("heap allocated 2\0");
  str3 = strdup("heap allocated 3\0");

  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 1\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 2\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 3\0"));  
  
  save_str(store, str1);
  save_str(store, str2);
  save_str(store, str3);

  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 1\0"));  
  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 2\0"));  
  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 3\0"));  

  free_saved_strs(store);

//  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 1\0"));  
//  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 2\0"));  
//  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 3\0"));  
//  
  // free(str1);  
  store_destroy(store);
}

void str_memory_management_system_manual_test(void){
  char  *str1  = NULL;
  char  *str2  = NULL;
  char  *str3  = NULL;
  
  webstore_t *store = store_create();
  //  str1 = malloc(sizeof(char) * 5);
  
  str1 = strdup("heap allocated 1\0");
  str2 = strdup("heap allocated 2\0");
  str3 = strdup("heap allocated 3\0");

  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 1\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 2\0"));  
  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 3\0"));  
  
  save_str(store, str1);
  save_str(store, str2);
  save_str(store, str3);

  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 1\0"));  
  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 2\0"));  
  CU_ASSERT_TRUE(is_saved_str(store, "heap allocated 3\0"));  

  free_str(store, "heap allocated 1\0");
  free_str(store, "heap allocated 2\0");
  free_str(store, "heap allocated 3\0");
  
//  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 1\0"));  
//  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 2\0"));  
//  CU_ASSERT_FALSE(is_saved_str(store, "heap allocated 3\0"));  
//  
  // free(str1);  
  store_destroy(store);
}

void create_shelf_test(void){
  shelf_t *new_shelf = create_shelf("A23", 123);    

  CU_ASSERT_TRUE(STR_EQ(new_shelf->shelf, "A23"));
  CU_ASSERT_EQUAL(new_shelf->amount, 123);

  destroy_shelf(new_shelf);
}




/////////////////////////////////////////////////////////////
int main()
{
  CU_pSuite merch_test_suite = NULL;
  //  CU_pSuite misc_test_suite = NULL;



  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  merch_test_suite = CU_add_suite("Tests Merch API Features", init_suite, clean_suite);
  //  misc_test_suite  = CU_add_suite("Test Misc Features",       init_suite, clean_suite);
  
  if (NULL == merch_test_suite){
      CU_cleanup_registry();
      return CU_get_error();
  }

  if ((NULL == CU_add_test(merch_test_suite,
			   "Create and Remove Merch: Creation and Deletion of merch\n",
			   create_destroy_merch_test)) ||
      (NULL == CU_add_test(merch_test_suite,
      "Construct Merch: Creation and Deletion of merch (empty location db) without adding to store.\n",
			   create_merch_test_empty_locs)) ||
      (NULL == CU_add_test(merch_test_suite,
      "Construct Merch: Creation and Deletion of merch (with location db) without adding to store.\n",
			   create_merch_test_populated_locs)) ||
      (NULL == CU_add_test(merch_test_suite,
			   "Create and Remove Merch: Shelf correct creation, stock and deletion\n",
			   create_destroy_merch_shelf_test)) ||

      (NULL == CU_add_test(merch_test_suite,
			   "Create and Remove Merch: Duplicate creation of merch\n",
			   create_duplicate_merch_test)) ||

      (NULL == CU_add_test(merch_test_suite,
			   "Create and Remove Merch: Automatic deallocation of merch\n",
			   create_autodestroy_merch_test)) ||
      (NULL == CU_add_test(merch_test_suite,
			   "Create Shelf: Creation of shelf.\n",
			   create_shelf_test)) ||
      
      (NULL == CU_add_test(merch_test_suite,
			   "Create Shelf: Creation of shelf.\n",
			   destroy_all_merch_test)) ||
      (NULL == CU_add_test(merch_test_suite,
			   "Memory Management: Manual-deallocation of strings\n",
			   str_memory_management_system_manual_test)) ||
      (NULL == CU_add_test(merch_test_suite,
			   "Memory Management: Auto-deallocation of strings\n",
			   str_memory_management_system_test))){

      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  
  CU_basic_run_tests();
  
  CU_cleanup_registry();
  return CU_get_error();
}




