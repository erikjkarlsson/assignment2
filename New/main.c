#include "common.h"
#include "hash_table.h"
#include "list_linked.h"
#include "stdio.h"
#include "hash_table.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"
#include "common.h"
#include "merch.h"


int main(int argc, char *argv[]) {

  printf("Now Running!\n");
  webstore_t *store = store_create();
  // --- Initialize Argument Handler
  arg_parse(argc, argv, store->opt);

  
  add_to_storage(store, "A", "A10");
  add_to_storage(store, "B", "A10");
  add_to_storage(store, "C", "A10");

  display_shelf(store, "A10");

  remove_name_from_shelf(store, "A10", "A");
  remove_name_from_shelf(store, "A10", "B");

  show_stock(store);
  SLOG(store, "boom");
  add_to_storage(store, "A", "A10");
  add_to_storage(store, "B", "A10");

  set_shelf(store, "A", "A10", 100);
  set_shelf(store, "B", "A10", 100);

  show_stock(store);  
  
  remove_name_from_shelf(store, "A10", "C");
  remove_name_from_shelf(store, "A10", "A");

  show_stock(store);

  //  show_stock(store);

  store_destroy(store);


  //  show_stock(store);  
  //  printf("Stock Apple: %ld\n", merch_stock(store, (char*)"Apple"));

  // --- Add to Storage

  //  remove_all_storage_locations(store);
  //  show_stock(store);
  
  //  list_merchandise(store);
  
  // --- Remove Merch
  //remove_merchendise(store, "Bike");
  //remove_merchendise(store, "Car");
  //remove_merchendise(store, "Cola");
  //remove_merchendise(store, "Chair");
  //remove_merchendise(store, "Computer");
  
  //  list_merchandise(store);

  
  printf("Finished Running!\n");

  return 0;
}

