#include "common.h"
#include "hash_table.h"
#include "list_linked.h"
#include "stdio.h"
#include "hash_table.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"



int main(int argc, char *argv[]) {
  
  printf("Now Running!\n");

  // --- Initialize Argument Handler
  webstore_t *store = store_create();
  arg_parse(argc, argv, store->opt);  

  // --- Add Merch    
  add_merchendise(store, "Cola",  "from coca cola",            (size_t)10);
  add_merchendise(store, "Chair", "Usable",                    (size_t)8);
  add_merchendise(store, "Bike",  "A sports bike from Brazil", (size_t)4);

  add_to_storage(store, "A", "A10");
  add_to_storage(store, "B", "A10");
  add_to_storage(store, "C", "A10");

  if(
  storage_contains(store, "A10", "A") &&
  storage_contains(store, "A10", "B") &&
  storage_contains(store, "A10", "C")
     ) printf("YES\n");
  
  // --- Remove Merch
  remove_merchendise(store, "Bike");
  remove_merchendise(store, "Chair");
  remove_merchendise(store, "Cola");

  remove_storage_location(store, "A10");
  
  store_destroy(store);
  
  printf("Finished Running!\n");

  return 0;
}

