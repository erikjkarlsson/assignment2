#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"

#define get_elem_ptr(e) e.p

/// Merch

merch_t *create_merch(char *name,
		      char *desc,
		      size_t price,
		      ioopm_list_t *locs){

  merch_t *item = calloc(1, sizeof(merch_t));

  item->name          = name;
  item->desc          = desc;
  item->price         = price;

  item->total_amount  = 0;
  item->locs          = locs;

  return item;
}

void print_merch(merch_t *merch){
  printf("| Item:          %s\n", merch->name);  
  printf("| Description:   %s\n", merch->desc);
  printf("| Price:         %ld\n",merch->price);
  printf("| Stock (Total): %ld\n", merch->total_amount);

}



/// Shelf

shelf_t *create_shelf(char *shelf, int amount){
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));

  new_shelf->shelf  = shelf;
  new_shelf->amount = amount;

  return new_shelf;
}



/// Store

webstore_t *store_create(){
  webstore_t *new_webstore = calloc(1, sizeof(webstore_t));

  new_webstore->merch_db   = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);
  new_webstore->storage_db = ioopm_hash_table_create(extract_int_hash_key, eq_elem_int, eq_elem_string);

  return new_webstore;
}
void store_destroy(webstore_t *store){

  ioopm_hash_table_destroy(store->merch_db);
  ioopm_hash_table_destroy(store->storage_db);
}



void add_merchendise(webstore_t *store, char *name, char *desc, size_t price){

  // ERROR IF merch_db is NULL
  if(store->merch_db == NULL){
    perror("ADD_MERCHENDISE: Uninitizalized Merch-Database,\
            the database has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
    return; // ERROR
  }
  // ERROR IF merch_db has key  
  else if (ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("ADD_MERCHENDISE: Duplicate Merch, \
            the name is already registered in Merch-Table.\n");
    exit(-1); // REMOVE THIS LATER
    return; // ERROR
  }

  else {    
    ioopm_list_t *locs = ioopm_linked_list_create();
    merch_t *new_merch = create_merch(name, desc, price, locs);

    ioopm_hash_table_insert(store->merch_db, ptr_elem(name), ptr_elem(new_merch));

    // TODO: IS THIS NEEDED, check gdb ioopm_linked_list_destroy(locs);
    return; // SUCCESS
  }  
}




merch_t *merch_change_description_function(merch_t *merch_data, void *new_desc){ // Arg =
  if (merch_data == NULL){
    perror("merch_change_description_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->desc = (char*)new_desc; }

  return merch_data;

}
void merchendise_edit_desc(webstore_t *store, char *name, char *edited_desc){
  return merchendise_modify(store, (char*)name,
			    merch_change_description_function,
			    (char*)edited_desc);
}

void merchendise_modify(webstore_t *store, char *name, merch_modify_function *fun, void *fun_arg){

  // ERROR IF merch_db is NULL
  if(store->merch_db == NULL){
    perror("merchendise_modify: Uninitizalized Merch-Database,\
            the database has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  // ERROR IF merch_db dont have the key  
  else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merchendise_modify: Duplicate Merch, \
            the name is not in the Merch Database.\n");
    exit(-1); // REMOVE THIS LATER
    return; // ERROR
    
  } else {
    
    merch_t *data = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db, ptr_elem(name)));    
    merch_t *new_data = NULL;
    new_data = fun(data, fun_arg);

   // Remove associated data
   ioopm_hash_table_remove(store->merch_db, ptr_elem(name));

   // Reinsert the modified data
   ioopm_hash_table_insert(store->merch_db, ptr_elem(name), ptr_elem(new_data));
   
    return; // ERROR
  }
    
}


void remove_merchendise(webstore_t *store, char *name){
    // TODO: item->total_amount--;
    ioopm_hash_table_remove(store->merch_db, ptr_elem(name));

    return; // SUCCESS
  }


// list_merchandise(webstore_t *store);
// list all items in store
// TODO REQ: Alphabetical order (soft req) - qsort
// TODO REQ: print 20 at time
//TODO REQ: ask if more
// list of all ptr of merch

bool continue_printing(){

    char *ans = ask_question_string("Continue? y/n \n");
    char *ans_cmp = "y";
    if(strcmp(ans, ans_cmp)==0){

      return true;
    
    }
    return false;
  }


void list_merchandise(webstore_t *store){

  ioopm_list_t *list_merch    = ioopm_hash_table_values(store->merch_db);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list_merch);
  merch_t *current = NULL;
  
  
  current = get_elem_ptr(ioopm_iterator_current(iter));

  for (int i = 1;; i++){
      printf("Begin Merchendise Item [No.%d]\n", i);
      print_merch(current);
      printf("End \n\n");
      
    if(ioopm_iterator_has_next(iter)){
      current = get_elem_ptr(ioopm_iterator_next(iter));
      
    }else {
      break;
      
    }
    
  }
     
  // FIX: Added destructors
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list_merch);
  
}
