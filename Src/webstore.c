#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "webstore.h"

#define get_elem_ptr(e) e.p

#define MLOG(store, fun, name, msg)		\
  if (store->opt->log_p)\
    merch_log(fun, name, msg, 0);

#define OMLOG(store, fun, name, msg,i)		\
  if (store->opt->log_p)\
    merch_log(fun, name, msg, i);

#define QLOG(store, fun, msg)		\
  if (store->opt->log_p)\
    merch_log(fun, "", msg, 0);

void merch_log(char *function, char *name, char *message, int number){
  printf(R_STR_STR, function);
  printf("%d (%s)", number, name);
  printf(B_STR_STR, message);
}



/// Merch


void merch_delete(webstore_t *store, char *name){
  /// Free Locs of Merch
  locs_delete(store, name);  
  merch_t *merch_data = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db, ptr_elem(name)));
  ioopm_hash_table_remove(store->merch_db, ptr_elem(name));    
  // Free Merchendise
  free(merch_data);
  
  // Remove from Hash Table
}

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
    return; // SUCCESS
  }  
}

void remove_merchendise(webstore_t *store, char *name){

  merch_delete(store, name);  
  return;
}

void prompt_remove_merchendise(webstore_t *store){
  list_merchandise(store);
  int number = ask_question_int("\
Enter the number of item that you wish to remove: \n");
  while(!valid_index(store, number)){
    number = ask_question_int("\
Enter a valid number of item that you wish to remove: \n");
  }
  char *name = lookup_merch_name(store, number);
  remove_merchendise(store, name);
  return; // SUCCESS
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

merch_t *merch_change_price_function(merch_t *merch_data, void *new_price){ // Arg =
  if (merch_data == NULL){
    perror("merch_change_price_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->price = (size_t)new_price; }
  return merch_data;
}

merch_t *merch_change_locs_function(merch_t *merch_data, void *new_locs){ // Arg =
  if (merch_data == NULL){
    perror("merch_change_locs_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->locs = (ioopm_list_t*)new_locs; }
  return merch_data;
}


void merchendise_new_desc(webstore_t *store, char *name, char *edited_desc){
  return merchendise_modify(store, (char*)name,
			    merch_change_description_function,
			    (char*)edited_desc);
}
void merchendise_new_price(webstore_t *store, char *name, size_t new_price){
  return merchendise_modify(store, (char*)name,
			    merch_change_price_function,
			    (size_t*)new_price);
}
void merchendise_new_locs(webstore_t *store, char *name, ioopm_list_t *new_locs){
  return merchendise_modify(store, (char*)name,
			    merch_change_locs_function,
			    (ioopm_list_t*)new_locs);
}

bool merch_in_scock(webstore_t *store, char *name){
  return ioopm_hash_table_has_key(store->merch_db, ptr_elem(name));
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
char *merch_get_desc_function(merch_t *merch_data){
  return merch_data->desc;
}
  
char *merch_description(webstore_t *store, char *name){
  // ERROR IF merch_db dont have the key  
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merchendise_modify: Duplicate Merch, \
            the name is not in the Merch Database.\n");
    exit(-1); // REMOVE THIS LATER  
  }  
  merch_t *data = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db, ptr_elem(name)));        
    
  return data->desc;    
}

int merch_price(webstore_t *store, char *name){
  // ERROR IF merch_db dont have the key  
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merchendise_modify: Duplicate Merch, \
            the name is not in the Merch Database.\n");
    exit(-1); // REMOVE THIS LATER  
  }  
  merch_t *data = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db, ptr_elem(name)));        
    
  return data->price;
    
}


char *lookup_merch_name(webstore_t *store, int index){ // RENAME LATER
  ioopm_list_t *list_merch = ioopm_hash_table_values(store->merch_db);
  elem_t value_ptr = ioopm_linked_list_get(list_merch, index);
  merch_t *merch = get_elem_ptr(value_ptr);
  printf("name%s\n", merch->name);
  return merch->name; 
}

bool valid_index(webstore_t *store, int index){
  ioopm_list_t *list = ioopm_hash_table_values(store->merch_db);
  if(index-1 >= list->size){
    return false;
  }
  return true;
}

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
  
  int continue_alert_number = 20;
  


  for (int i = 1;; i++){
    if ((i % (continue_alert_number) == 0) && !continue_printing()) break;

         
    printf("Begin Merchendise Item [No.%d]\n", i);

    // printf("No.%d: \n", i);

    print_merch(current);
    printf("End \n\n");
      
    if(ioopm_iterator_has_next(iter)){
      current = get_elem_ptr(ioopm_iterator_next(iter));      
    }else { break; }

    
  }
     
  // FIX: Added destructors
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list_merch);
  
}

//show shelf and quantity
/*
void show_stock(webstore_t *store){
  ioopm_list_t *list_shelfs = ioopm_hash_table_keys(store->storage_db);
  //ioopm_list_iterator_t *iter = ioopm_list_iterator(list_shelfs);
  
  //shelf_t *current = NULL;
  //current = get_elem_ptr(ioopm_iterator_current(iter)); //lista över items 
  
  for(int i =0; i < ioopm_linked_list_size(list_shelfs); i++){
    ioopm_list_t *db_names = look_in_storage(store, shelf);
    shelf_t shelf_key = get_elem_ptr(ioopm_linked_list_get(list_shelfs, i));
    ioopm_list_t *list_names = get_elem_ptr(ioopm_hash_table_lookup(store->storage_db, ioopm_linked_list_get(list_shelfs, i)));
    int stock = ioopm_linked_list_size(list_names);
    char *shelf_name = shelf_key->shelf; 
    printf("Shelf [%s]:%d\n", shelf_name, stock);
    
  }
  ioopm_linked_list_destroy(list_shelfs);
}

*/
/// Shelf

shelf_t *create_shelf(char *shelf, int amount){
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));

  new_shelf->shelf  = shelf;
  new_shelf->amount = amount;

  return new_shelf;
}
void shelf_delete(shelf_t *shelf){
  free(shelf);
}


/// Store

webstore_t *store_create(){
  webstore_t *new_webstore = calloc(1, sizeof(webstore_t));
  new_webstore->opt = create_arg_opt();

  new_webstore->merch_db   =
    ioopm_hash_table_create(extract_int_hash_key,
			    eq_elem_int, eq_elem_string);
  new_webstore->storage_db =
    ioopm_hash_table_create(extract_int_hash_key,
			    eq_elem_int, eq_elem_string);

  return new_webstore;
}

void store_destroy(webstore_t *store){
  destroy_arg_opt(store->opt);
  ioopm_hash_table_destroy(store->merch_db);
  ioopm_hash_table_destroy(store->storage_db);
  free(store);
}


/// Shelf

void add_shelf(webstore_t *store, char *name, shelf_t *shelf){
  change_or_add_shelf(store, name, shelf->amount, shelf->shelf);
}

void display_storage(webstore_t *store, char *shelf){
  // Names stored at requested shelf location
  ioopm_list_t *db_names = look_in_storage(store, shelf);
  ioopm_link_t *db_item = db_names->first;

  printf("Shelf[%s]:", shelf);
  
  do {
    // Already exists in database
    printf(" %s", (char*)get_elem_ptr(db_item->element));
					      
    db_item = db_item->next;
    
  } while (db_item != NULL);
  printf("\n");
}

ioopm_list_t *look_in_storage(webstore_t *store, char *shelf){
  return 
    get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
					 ptr_elem(shelf)));
}

void add_to_storage(webstore_t *store, char *name, char *shelf){

  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    ioopm_list_t *storage_list = ioopm_linked_list_create();
    ioopm_linked_list_append(storage_list, ptr_elem(name));
    ioopm_hash_table_insert(store->storage_db,
			    ptr_elem(shelf),
			    ptr_elem(storage_list));    
  }
  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = look_in_storage(store, shelf);
  ioopm_link_t *db_item = db_names->first;

  do {
    // Already exists in database
    if (STR_EQ(db_item->element.c, name)) return;
					      
    db_item = db_item->next;           
  } while (db_item != NULL);

  ioopm_linked_list_append(db_names, ptr_elem(name));
}
   
void change_or_add_shelf(webstore_t *store, char *name, int amount, char* location){
  slog("change_or_add_shelf", name, 1);

  if(store->merch_db == NULL){
    perror("change_or_add_shelf: Uninitizalized Merch-Database,\
            the database has not been initialized.\n");
    return; // REMOVE THIS LATER
    
  }else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("change_or_add_shelf: 404 Merch Not Found, \
            item name not in the Merch Database.\n");
    return; // REMOVE THIS LATER
  }

  // Extract location database bound to name
  elem_t elem_data = ioopm_hash_table_lookup(store->merch_db, ptr_elem(name));
  merch_t      *merch_data = get_elem_ptr(elem_data);  
  ioopm_link_t *merch_locs = merch_data->locs->first;
  
 
  if(merch_data->locs == NULL){ 
    perror("change_or_add_shelf: No Merch Database"); return;    
  }if(amount < 0){
    perror("change_or_add_shelf: Negative amount"); return;
  }

  slog("change_or_add_shelf", name, 2);
  
  if (merch_data->locs->size > 0){
    shelf_t *shelf_data = get_elem_ptr(merch_locs->element);
    do {

      if (STR_EQ(shelf_data->shelf, location)){
	// Found existing shelf, set new amount, and exit
	shelf_data->amount = amount;
	return;
      }     

      merch_locs = merch_locs->next;
           
    } while (merch_locs != NULL);

  }

  slog("change_or_add_shelf", name, 3);

  shelf_t *new_shelf = create_shelf(location, amount);  
  ioopm_linked_list_append(merch_data->locs, ptr_elem(new_shelf));
    
  // FIX: Added destructors

  slog("change_or_add_shelf", name, 4);
  
      
}



ioopm_list_t *merch_locs(webstore_t *store, char *name){

  merch_t *merch_data         = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
								     ptr_elem(name)));  
  ioopm_list_t *merch_locs    = merch_data->locs;
  return merch_locs;
}

void list_shelfs(webstore_t *store, char *name){
  merch_t *merch_data         = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
								     ptr_elem(name)));  
  ioopm_list_t *merch_locs    = merch_data->locs;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));

  printf("Shelfs containing'%s':\n", name);
  
  for (int i = 1;; i++){         

    
    printf("Shelf %s : %ld\n",
	   shelf->shelf,
	   shelf->amount);
      
    if(ioopm_iterator_has_next(iter)){
        
      shelf   = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }

    
  }
     
  // FIX: Added destructors
  ioopm_iterator_destroy(iter);

  
}
int merch_locs_total(webstore_t *store, char *name){
  merch_t *merch_data = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
							     ptr_elem(name)));  

  ioopm_link_t *merch_locs = merch_data->locs->first;  
  int amount = 0;
  
  shelf_t *shelf_data;
  while (merch_locs != NULL) {

    shelf_data = (get_elem_ptr(merch_locs->element));
    amount    += shelf_data->amount;
    merch_locs = merch_locs->next;
           
  }

  return amount;
}
int merch_locs_at_shelf(webstore_t *store, char *name, char *shelf){
  // Return the amount merchendise at a specific shelf
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  

  ioopm_link_t *merch_locs = merch_data->locs->first;
  
  int amount = 0;
  
  shelf_t *shelf_data;
  while (merch_locs != NULL) {

    shelf_data = (get_elem_ptr(merch_locs->element));

    if (STR_EQ(shelf_data->shelf, shelf)) 
      amount    += shelf_data->amount;
    
    merch_locs = merch_locs->next;
           
  }

  return amount;
}


void locs_delete(webstore_t *store, char *name){
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  
  ioopm_link_t *merch_locs = merch_data->locs->first;  
  
  while (merch_locs != NULL) {
    free(get_elem_ptr(merch_locs->element));
    merch_locs = merch_locs->next;;           
  }

  ioopm_linked_list_destroy(merch_data->locs);
}

/// Other


// list_merchandise(webstore_t *store);
// list all items in store
// TODO REQ: Alphabetical order (soft req) - qsort
// TODO REQ: print 20 at time
//TODO REQ: ask if more
// list of all ptr of merch

