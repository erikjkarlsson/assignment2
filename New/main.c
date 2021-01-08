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

  show_stock(store);
  
  
  // --- Add to Storage
  add_to_storage(store, "Cola", "A8");
  add_to_storage(store, "Car", "A1");
  add_to_storage(store, "Bike", "A1");

  // --- Display Storage
  display_shelf(store, "A1");
  //  remove_all_storage_locations(store);
    
  
  //list_merchandise(store);
  
  // --- Remove Merch
  //remove_merchendise(store, "Bike");
  //remove_merchendise(store, "Car");
  //remove_merchendise(store, "Cola");
  //remove_merchendise(store, "Chair");
  //remove_merchendise(store, "Computer");
  
  //  list_merchandise(store);
 

  store_destroy(store);
  
  printf("Finished Running!\n");

  return 0;
}

