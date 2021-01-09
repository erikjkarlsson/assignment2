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

  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  // --- Initialize Argument Handler
  arg_parse(argc, argv, store->opt);  
  
  //  show_stock(store);  
  printf("Stock Apple: %ld\n", merch_stock(store, (char*)"Apple"));
  // --- Add to Storage
  //  remove_all_storage_locations(store);
  show_stock(store);
  
  list_merchandise(store);
  
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

