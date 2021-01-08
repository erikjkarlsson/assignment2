#include "common.h"
#include "hash_table.h"
#include "list_linked.h"
#include "stdio.h"
#include "hash_table.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"
#include "merch.h"


int main(int argc, char *argv[]) {


  printf("Now Running!\n");

  // --- Load hardcoded test merch
  webstore_t *store = initialize_database();
  
  // --- Initialize Argument Handler
  arg_parse(argc, argv, store->opt);  

  // --- Add Merch    
  add_merchendise(store, "Cola", "from coca cola", (size_t)10);
  add_merchendise(store, "Chair", "Usable", (size_t)8);
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
  add_merchendise(store, "Car", "A fast car", (size_t)2);
  add_merchendise(store, "Computer", "Gaming computer", (size_t)0);



    show_stock(store);
  /*

  // Changed from 4
  if (merch_in_stock(store, "Bike"))
    printf("merch new name: Failed to remove old name!\n");
  else
    printf("merch new name: Correctly removed old name!\n");

  if (merch_in_stock(store, "New Bike"))
    printf("merch new name: Correctly added new name!\n");
  else
    printf("merch new name: Failed to add new name!\n");

    
  merchendise_edit(store, "New Bike",  
		   (size_t*) 4,
		   "A sports bike from Brazil",
		   "Bike");
  
  printf("Inserted 5 merchendise!\n");

  // --- Add to Storage
  add_to_storage(store, "Cola", "A8");
  add_to_storage(store, "Car", "A1");
  add_to_storage(store, "Bike", "A1");

  // --- Display Storage
  display_storage(store, "A1");
  remove_all_storage_locations(store);
    
  ioopm_list_t *products = ioopm_hash_table_keys(store->merch_db);
  
  for (size_t i = 0; i < ioopm_linked_list_size(products); i++){
    printf("Name: %s\n", ioopm_linked_list_get(products, i).p);
  }
  printf("Size of Products: %ld\n",  ioopm_linked_list_size(products));
  ioopm_linked_list_destroy(products);

  printf("##########%d",merch_locs_total(store, "Cola"));
  
  //list_merchandise(store);
  
  // --- Remove Merch
  remove_merchendise(store, "Bike");
  remove_merchendise(store, "Car");
  remove_merchendise(store, "Cola");
  remove_merchendise(store, "Chair");
  remove_merchendise(store, "Computer");

  //  list_merchandise(store);
  */

  store_destroy(store);
  
  printf("Finished Running!\n");

  return 0;
}

