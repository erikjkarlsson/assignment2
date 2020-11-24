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

  
  add_merchendise(store, "Cola", "from coca cola", (size_t)10);
  add_merchendise(store, "A Chair", "Usable", (size_t)8);
  add_merchendise(store, "A Plant", "A plant from Brazil", (size_t)4);
  add_merchendise(store, "A Car", "A fast car", (size_t)2);
  add_merchendise(store, "A Computer", "Gaming computer", (size_t)0);

  printf("Inserted 5 merchendise!\n");

  ioopm_list_t *products = ioopm_hash_table_keys(store->merch_db);


  printf("Size of Products: %ld\n",  ioopm_linked_list_size(products));
  
  char * xm;

  for (size_t i = 0; i < ioopm_linked_list_size(products); i++){
    xm = ioopm_linked_list_get(products, i).p;    
    printf("Name: %s\n", xm);
  }
  
  //list_merchandise(store);
  
  remove_merchendise(store);
  
  list_merchandise(store);
  



  //  printf("name: %s\n", e-);

  
  

  ioopm_linked_list_destroy(products);

  store_destroy(store);  
  printf("Finished Running!\n");

  return 0;
}

