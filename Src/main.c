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
  webstore_t *store = store_create();
  arg_parse(argc, argv, store->opt);  
  // Add Merch    
  add_merchendise(store, "Cola", "from coca cola", (size_t)10);
  add_merchendise(store, "Chair", "Usable", (size_t)8);
  add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
  add_merchendise(store, "Car", "A fast car", (size_t)2);
  add_merchendise(store, "Computer", "Gaming computer", (size_t)0);

  change_or_add_shelf(store, "Cola", 1, "a");
  change_or_add_shelf(store, "Cola", 2, "b");
  list_shelfs(store, "Cola");
  // Merch edit desc 
  merchendise_edit_desc(store, "Car", "A slow car");
  merchendise_edit_desc(store, "Chair", "Terrible");
  merchendise_edit_desc(store, "Bike", "16 Gears");
  merchendise_edit_desc(store, "Computer", "Non-Gaming computer");
  
  printf("Inserted 5 merchendise!\n");

  ioopm_list_t *products = ioopm_hash_table_keys(store->merch_db);

  printf("Size of Products: %ld\n",  ioopm_linked_list_size(products));
  
  char * xm;

  for (size_t i = 0; i < ioopm_linked_list_size(products); i++){
    xm = ioopm_linked_list_get(products, i).p;    
    printf("Name: %s\n", xm);
  }
  
  //list_merchandise(store);
  
  prompt_remove_merchendise(store);
  
  list_merchandise(store);
    
  ioopm_linked_list_destroy(products);
  store_destroy(store);
  
  printf("Finished Running!\n");

  return 0;
}

