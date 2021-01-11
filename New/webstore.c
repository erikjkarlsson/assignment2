#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "cart.h"
#include "webstore.h"


bool valid_index(webstore_t *store, int index);
char *lookup_merch_name(webstore_t *store, int index); 

void print_merch(merch_t *merch);

void add_to_storage(webstore_t *store, char *name, char *shelf);
void destroy_storage(webstore_t *store);

void remove_shelf(webstore_t *store, char *shelf);
ioopm_list_t *get_locations(webstore_t *store, char *shelf);
bool storage_contains(webstore_t *store, char *shelf, char *name);

void list_shelfs(webstore_t *store, char *name);
void display_shelf(webstore_t *store, char *shelf);

void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount);

void store_destroy(webstore_t *store);
webstore_t *store_create();

void list_merchandise(webstore_t *store);
void remove_from_storage(webstore_t *, char *, char *);
bool merch_in_stock(webstore_t *store, char *name);
int merch_stock_on_shelf(webstore_t *store, char *name, char *shelf);
bool sync_merch_stock(webstore_t *store, char *name);
size_t increase_stock(webstore_t *store, char *name,
		      char *shelf_name, size_t amount);

void show_stock(webstore_t *store);
void rename_merch(webstore_t *store, char *name, char *new_name);

size_t merch_stock(webstore_t *store, char *name);
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* location);

int merch_price(webstore_t *store, char *name);
void set_merch_price(webstore_t *store, char *name, size_t price);

char *merch_description(webstore_t *store, char *name);
void set_merch_description(webstore_t *store, char *name, char *desc);

void destroy_locs(webstore_t *store, char *name);


void add_merchendise(webstore_t *store, char *name, char *desc, size_t price);
void destroy_all_merch(webstore_t *store);
void remove_merchendise(webstore_t *store, char *name);
merch_t *create_merch(char *name, char *desc, size_t price, ioopm_list_t *locs);

shelf_t *create_shelf(char *shelf, size_t amount);
void destroy_shelf(shelf_t *shelf);

  
/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// MERCH                                            ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

merch_t *create_merch(char *name, char *desc,
		      size_t price, ioopm_list_t *locs){

  merch_t *item = calloc(1, sizeof(merch_t));

  item->name          = name;
  item->desc          = desc;

  item->price         = price;
  item->total_amount  = 0;
  item->locs          = locs;

  return item;
}

void remove_merchendise(webstore_t *store, char *name){
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("remove_merchendise: Non existing item, \
            The name to be removed does not exist.\n");
    return; // ERROR
  }else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("remove_name_from_shelf: Non existing merch name.\n");
    return;
  }

  shelf_t *shelf = NULL;
  // Free Locs  


  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));


  
  ioopm_link_t *merch_data_locs = merch_data->locs->first;
   


    
    do {
      shelf = get_elem_ptr(merch_data_locs->element);
      remove_from_storage(store, name, shelf->shelf);
      merch_data_locs = merch_data_locs->next;           
    } while (merch_data_locs != NULL);
    
  
    destroy_locs(store, name);
 
  ioopm_hash_table_remove(store->merch_db,
			  ptr_elem(name));
  
  //  free(merch_data->desc);
  //free(merch_data->name);


  // Free Merchendise
  free(merch_data);
}

void destroy_all_merch(webstore_t *store){
  // Remove all shelfs in storage_db, but not the hash-table.
  if ((store == NULL) || (store->merch_db == NULL)){
    perror("destroy_all_merch: Webstore is NULL\n");
    return;
  }

  ioopm_list_t *names  = ioopm_hash_table_keys(store->merch_db);
  ioopm_link_t *current = names->first;

 if (current == NULL){
   perror("destroy_all_merch: All mearch already destroyed.\n");
   ioopm_linked_list_destroy(names); 
   return;
 } 
 
 do {
   remove_merchendise(store, get_elem_ptr(current->element));
   current = current->next;
 } while (current != NULL);

 ioopm_linked_list_destroy(names); 
}

void add_merchendise(webstore_t *store,
		     char *name,
		     char *desc,
		     size_t price){

  
  if(store->merch_db == NULL){
    perror("add_merchendise: Merch database is NULL.\n");
    return; 
  }
  else if (ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("add_merchendise: Trying to add a duplicate merch.\n");
    return; 
  }else {
    ioopm_list_t *locs = ioopm_linked_list_create();    
    merch_t *new_merch = create_merch(name, desc,
				      price, locs);

    ioopm_hash_table_insert(store->merch_db,
			    ptr_elem(name),
			    ptr_elem(new_merch));
    return; 
  }  
}
// change merch on shelf   
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* location){
  // Look in the merch db for the location (shelf)
  // if it exists its stock will be set to amount
  // else it will be added with its stock set to amount

  if((store->merch_db == NULL) || (name == NULL) || (location == NULL)){
    perror("set_merch_stock: NULL Argument.\n");
    return;
    
  }else if (!ioopm_hash_table_has_key(store->merch_db,
				      ptr_elem(name))){
    perror("set_merch_stock: Non existing merch.\n");
    return; 
  }

  // Extract location database bound to name
  elem_t elem_data =
    ioopm_hash_table_lookup(store->merch_db,
			    ptr_elem(name));
  
  merch_t      *merch_data = get_elem_ptr(elem_data);

  ioopm_link_t *merch_data_locs = merch_data->locs->first;
   
  if (merch_data->locs->size > 0){
    shelf_t *shelf_data = get_elem_ptr(merch_data_locs->element);
    
    do {

      if (STR_EQ(shelf_data->shelf, location)){
	// Found existing shelf, set new amount, and exit
	merch_data->total_amount += (merch_data->total_amount + amount) - shelf_data->amount;
	
	return;
      }     

      merch_data_locs = merch_data_locs->next;
           
    } while (merch_data_locs != NULL);
    
  }
  merch_data->total_amount += amount;
  shelf_t *new_shelf = create_shelf(location, amount);  
  ioopm_linked_list_append(merch_data->locs,
			   ptr_elem(new_shelf));
    
}

/*
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
*/


bool merch_in_stock(webstore_t *store, char *name){
  return ioopm_hash_table_has_key(store->merch_db,
				  ptr_elem(name));
}

// Remove new name functionality  
void rename_merch(webstore_t *store, char *name, char *new_name){
  // Edit a merch item, setting a new description,
  // new price, new name (only in merch db) if
  // the are Non-NULL
  if(store == NULL){
    perror("merchendise_edit: Unallowed NULL argument.\n");
    return;
  }else if(store->merch_db == NULL){
    perror("merchendise_edit: Database is NULL.\n");
    return;
  }else if (!ioopm_hash_table_has_key(store->merch_db,
				     ptr_elem(name))){
    perror("merchendise_edit: Non existing merch.\n");
    return;
  }     
  // Name already exists 
  if (ioopm_hash_table_has_key(store->merch_db,
			       ptr_elem(new_name))){
    perror("merchendise_edit: Unallowed name change.\n");
    return;
  }
  // Get the related struct
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));
  // Insert the old data under the changed name
  merch_data->name = new_name;
  ioopm_hash_table_insert(store->merch_db,
			  ptr_elem(new_name),
			  ptr_elem(merch_data));        
  // Remove the old mapping (should not free the underlaying data) 
  ioopm_hash_table_remove(store->merch_db, ptr_elem(name));
}

ioopm_list_t *merch_locs(webstore_t *store, char *name){

  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  ioopm_list_t *merch_locs    = merch_data->locs;
  return merch_locs;
}


char *merch_description(webstore_t *store, char *name){
  // Return the description of merch item
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_description: Non existing merch.\n");
    return "";
  }else if ((name == NULL) || (store == NULL)){
    perror("merch_description: Unallowed NULL argument.\n");
    return "";
  }
  
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));        
    
  return data->desc;    
}
void set_merch_description(webstore_t *store, char *name, char *desc){
  // Set the description of merch item
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_description: Non existing merch.\n");
    return;
  }else if ((name == NULL) || (store == NULL) || (desc == NULL)){
    perror("merch_description: Unallowed NULL argument.\n");
    return;
  }
  
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));            
  //  free(data->desc);
  data->desc = desc;
}

int merch_price(webstore_t *store, char *name){
  // Return the price of the specified merch name
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_price: Non existing merch.\n");
    return -1;
  }else if ((name == NULL) || (store == NULL)){
    perror("merch_price: Unallowed NULL argument.\n");
    return -1;
  }
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));            
  return data->price;    
}

void set_merch_price(webstore_t *store, char *name, size_t price){
  // Return the price of the specified merch name
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_price: Non existing merch.\n");
    return;
  }else if ((name == NULL) || (store == NULL)){
    perror("merch_price: Unallowed NULL argument.\n");
    return;
  }
  merch_t *data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));            
  data->price = price;
}

size_t merch_stock(webstore_t *store, char *name){
  // Calculate and return the total amount of a
  // merch in stock

  if (!ioopm_hash_table_has_key(store->merch_db,
				ptr_elem(name))){
    perror("merch_stock: Non existing merch.\n");
    return 0;
  }
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  

  ioopm_link_t *merch_data_locs = merch_data->locs->first;  
  size_t amount = 0;
  
 if (merch_data_locs == NULL){
    perror("merch_stock: Merch Locs is NULL.\n");
    return 0;
  }
 
  // Iterate through all of locs adding up
  // the amounts
  while (merch_data_locs != NULL) {
    shelf_t *shelf_data = (get_elem_ptr(merch_data_locs->element));
    //printf("Amount: %d", amount);
    amount    += shelf_data->amount;
    merch_data_locs = merch_data_locs->next;           
  }

  return amount;
}

void list_merchandise(webstore_t *store){
  // List all merchendise as a short list of 20
  // prompting for displaying more 
  if (store == NULL){
    perror("list_merchendise: Unallowed NULL argument.\n");
    return;
  }
  // All existing merch names
  ioopm_list_t *list_merch    =
    ioopm_hash_table_values(store->merch_db);

  ioopm_list_iterator_t *iter =
    ioopm_list_iterator(list_merch);

  merch_t *current            =
    get_elem_ptr(ioopm_iterator_current(iter));
  
  int continue_alert_number   = 20;
  
  // Iterate through all names
  for (int i = 1;; i++){
    // Prompt for continuing to display merch
    if ((i % (continue_alert_number) == 0) && \
	!continue_printing()) break;
    // Print current merch
    printf("Merch Item [No.%d]\n", i);
    print_merch(current);
    printf("End \n\n");

    if(ioopm_iterator_has_next(iter)){
      current = get_elem_ptr(ioopm_iterator_next(iter));      
    }else { break; }    
  }     
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list_merch);  
}

void destroy_locs(webstore_t *store, char *name){ 
  // Free up a merch locs list (list & shelfs)

  if ((store == NULL) || (name == NULL)){
    perror("destroy_locs: Unallowed NULL arguments.\n");
    return;
  }
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  
  ioopm_link_t *merch_data_locs = merch_data->locs->first;  

  // Free each shelf in the merch locs
  while (merch_data_locs != NULL) {
    free(get_elem_ptr(merch_data_locs->element));
    merch_data_locs = merch_data_locs->next;;           
  }
  // Free the locs list
  ioopm_linked_list_destroy(merch_data->locs);
}

/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// STORE                                            ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

webstore_t *store_create(){
  // Allocate the argument handler, both hash tables
  // and the shopping cart list. And the whole webstore.

  webstore_t *new_webstore = calloc(1, sizeof(webstore_t));
  
  new_webstore->opt = create_arg_opt();

  // Storage and Merch databases
  new_webstore->merch_db   =
    ioopm_hash_table_create(extract_int_hash_key,
			    eq_elem_int, eq_elem_string);
  new_webstore->storage_db =
    ioopm_hash_table_create(extract_int_hash_key,
			    eq_elem_int, eq_elem_string);	

  //linked list that holds all shopping carts
  new_webstore->all_shopping_carts = 
    ioopm_linked_list_create();
  ioopm_linked_list_append(new_webstore->all_shopping_carts,
			   ptr_elem(create_cart(new_webstore)));
  



  return new_webstore;
}

void store_destroy(webstore_t *store){
  // Deallocate the argument handler, both hash tables
  // and the shopping cart list. And the whole webstore.
  if (store == NULL){
    perror("store_destroy: Webstore is NULL.\n");
    return;
  }
  
  destroy_arg_opt(store->opt);


  destroy_all_merch(store);


  destroy_storage(store);
  
  ioopm_hash_table_destroy(store->merch_db);
  ioopm_hash_table_destroy(store->storage_db);

  ioopm_linked_list_destroy(store->all_shopping_carts);  

  free(store);
}

/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// SHELF                                            ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

shelf_t *create_shelf(char *shelf, size_t amount){
  // Allocate and create shelf

  // Check for sub-zero stock
  if (amount <= 0) {
    perror("create_shelf: Negative stock.\n");
    perror("create_shelf: Using 0 as stock.\n");
    amount = 0;
  // Assign non-correctly formatted shelfs
  // into the shelf: ’A00’
  }if ((shelf == NULL) || (!is_shelf(shelf))){
    perror("create_shelf: Shelf name is incorrectly formatted.\n");
    perror("create_shelf: Using placeholder shelf: A00.\n");
    shelf = "A00";    
  }

  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));    
  new_shelf->shelf   = shelf;
  new_shelf->amount  = amount;

  return new_shelf;
}

void destroy_shelf(shelf_t *shelf){
  // Deallocate a shelf
  if (shelf == NULL){
    perror("destroy_shelf: Unallowed NULL argument.\n");
    return;
  }
  free(shelf);
}


int merch_stock_on_shelf(webstore_t *store, char *name, char *shelf){
  // Return the amount merchendise at a specific shelf


  if (!ioopm_hash_table_has_key(store->merch_db,
				     ptr_elem(name))){
    perror("merch_stock_on_shelf: Non existing merch.\n");
    return 0;
  }
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  

  ioopm_link_t *merch_data_locs = merch_data->locs->first;  

  if (merch_data_locs == NULL){
    perror("merch_stock_on_shelf: Merch Locs is NULL.\n");
    return 0;
  }
  
  shelf_t *shelf_data = NULL;

  do {
    shelf_data = get_elem_ptr(merch_data_locs->element);

    if (STR_EQ(shelf_data->shelf, shelf))
	return shelf_data->amount;
    
    merch_data_locs = merch_data_locs->next;
  } while (merch_data_locs != NULL);


  perror("merch_stock_on_shelf: The merch has no stock on the shelf.\n");
  return 0;  
}
void remove_from_storage(webstore_t *store, char *name, char *shelf){
  // Prettyprint the contents of a shelf
  if ((store == NULL) || (shelf == NULL)){
    perror("remove_from_storage: Unallowed NULL arguments.\n");
    return; 
  }  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);

  if (ioopm_linked_list_size(db_names) < 1){
    perror("remove_from_storage: No amounts on any shelf.\n");
    return; 
  }
  
  ioopm_link_t *db_item = db_names->first;
  ioopm_link_t *db_prev = db_names->first;
  // Print what shelf
  
  // Print all items
  do {
    // Already exists in database

    if ((char*)get_elem_ptr(db_item->element) == name){
      db_prev->next = db_item->next;
      free(db_item);
      return;
    }
    db_prev = db_item;
    db_item = db_item->next;    
  } while (db_item != NULL);
  
}

void display_shelf(webstore_t *store, char *shelf){
  // Prettyprint the contents of a shelf
  if ((store == NULL) || (shelf == NULL)){
    perror("display_storage: Unallowed NULL arguments.\n");
    return; 
  }  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);
  ioopm_link_t *db_item = db_names->first;
  // Print what shelf
  printf("%s:", shelf);
  // Print all items
  do {
    // Already exists in database
    printf(" %s", (char*)get_elem_ptr(db_item->element));
					      
    db_item = db_item->next;    
  } while (db_item != NULL);
  
  printf("\n");
}

void list_shelfs(webstore_t *store, char *name){
  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  ioopm_list_t *merch_data_locs    = merch_data->locs;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_data_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));

  printf("--- Shelfs containing '%s' ---\n", name);
  
  for (int i = 1;; i++){
    printf("No. %d | Shelf %s : %ldst\n",
	   i,
	   shelf->shelf,
	   shelf->amount);
      
    if(ioopm_iterator_has_next(iter)){
        
      shelf   = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }    
  }    
  ioopm_iterator_destroy(iter);
}
void remove_name_from_shelf(webstore_t *store, char *shelf, char *name){

  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    perror("remove_name_from_shelf: Non existing shelf name.\n");
    return;
  }else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("remove_name_from_shelf: Non existing merch name.\n");
    return;
  }
  ioopm_list_t *list         =
    ioopm_hash_table_lookup(store->storage_db,
			    ptr_elem(shelf)).p;  
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

  char *current_name = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (int i = 1;; i++){
    
    if(STR_EQ(current_name, name)){
      ioopm_iterator_remove(iter);
      ioopm_iterator_destroy(iter);
      return;
    }
    if(ioopm_iterator_has_next(iter)){
        
      shelf = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }    
  }    
  ioopm_iterator_destroy(iter);
}

char *get_shelf_after_shelf_nr(webstore_t *store, int shelf_nr, char *name){
  
  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
					 
  ioopm_list_t *merch_data_locs    = merch_data->locs;
  
  if(ioopm_linked_list_size(merch_data_locs) < shelf_nr){
    perror("get_shelf_after_shelf_nr: Shelf Number Out of Bounds.\n");
    return NULL; 
  }
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_data_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (int i = 1;; i++){
    if(shelf_nr == i){
      ioopm_iterator_destroy(iter);
      return shelf->shelf; 
    }
    
    if(ioopm_iterator_has_next(iter)){
        
      shelf   = get_elem_ptr(ioopm_iterator_next(iter));

    }else { break; }    
  }    
  

  ioopm_iterator_destroy(iter);
  return NULL; 
}

/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// STORAGE                                          ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

ioopm_list_t *get_locations(webstore_t *store, char *shelf){
  // Return the associated list to a shelf in storage
  if ((store == NULL) || (shelf == NULL)){
    perror("get_locations: Unallowed NULL arguments.\n");
    return NULL; // Can this be a problem??
  }else 
    return get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
					      ptr_elem(shelf)));
}

void remove_shelf(webstore_t *store, char *shelf){
  // Remove a storage shelf from the storage_db hash-table.
  
  if ((store == NULL) || (shelf == NULL)){
    perror("remove_shelf: Unallowed NULL arguments.\n");
    return;
  }
  // Shelf does not exist in storage db  
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    perror("remove_shelf: Cannot remove, non-existing shelf.\n");
    return;
  } 
    
  ioopm_list_t *storage_list = get_locations(store, shelf);    

  ioopm_linked_list_destroy(storage_list); 
  ioopm_hash_table_remove(store->storage_db, ptr_elem(shelf));

   
}

void destroy_storage(webstore_t *store){
  // Remove all shelfs in storage_db, but not the hash-table.
  if (store == NULL){
    perror("destroy_storage: Webstore is NULL\n");
    return;
  }
  if (store->storage_db == NULL){
    perror("destroy_all_merch: Storage DB already free'd.\n");
    return;
  }

 ioopm_list_t *shelfs  = ioopm_hash_table_keys(store->storage_db);
 ioopm_link_t *current = shelfs->first;
 
 if (current == NULL){
   perror("destroy_storage:  Storage db is NULL.\n");
   ioopm_linked_list_destroy(shelfs); 
   return;
 }

 // Iterate all shelfs removing them
 do {  
   remove_shelf(store, get_elem_ptr(current->element));
   current = current->next;
 } while (current != NULL);

  ioopm_linked_list_destroy(shelfs); 
}

void add_to_storage(webstore_t *store, char *name, char *shelf){

  if (store == NULL){
    perror("add_to_storage: Unallowed NULL argument.\n");
    return;
  }else if (STR_EQ(name, "") || (STR_EQ(shelf, ""))){
    perror("add_to_storage: Cannot add empty names or shelfs.\n");
  }
  
  // Create a new storage list if shelf does not exist
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    ioopm_list_t *storage_list = ioopm_linked_list_create();
    ioopm_linked_list_append(storage_list, ptr_elem(name));
    ioopm_hash_table_insert(store->storage_db,
			    ptr_elem(shelf),
			    ptr_elem(storage_list));    
  }
  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);
  ioopm_link_t *db_item  = db_names->first;

  do {
    // Already exists in database
    if (STR_EQ(get_elem_ptr(db_item->element), name)) return;
					      
    db_item = db_item->next;           
  } while (db_item != NULL);

  ioopm_linked_list_append(db_names, ptr_elem(name));
}


bool storage_contains(webstore_t *store, char *shelf, char *name){
  if ((store == NULL) | (shelf == NULL) | (name == NULL)){
    perror("storage_contains: Unallowed NULL argument.\n");
    return false;
  }
  // If shelf does not exist, it cannot contain item  
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    return false;    
  }
  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);
  ioopm_link_t *db_item  = db_names->first;

  do {
    // Already exists in database
    if (STR_EQ(get_elem_ptr(db_item->element), name))
      return true;
					      
    db_item = db_item->next;           
  } while (db_item != NULL);
  // Does not exist in database
  return false;
}

// add merch on shelf
void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount){
  // Add / Update shelf to both the merch database and the
  // storage database. If it already exists, update amount.
  
  // Add name to shelf if it already doesnt not contain it.
  if (!storage_contains(store, name, shelf))
    add_to_storage(store, name, shelf);

  set_merch_stock(store, name, amount, shelf);

  sync_merch_stock(store, name);
}

bool sync_merch_stock(webstore_t *store, char *name){
  // Calculate the total stock of a merch item from
  // the locs list and update merch->total_amount
  // reflecting that. If a syncronization happened
  // the function returns true.
  
  if ((store == NULL) || (name == NULL)){
    perror("sync_merch_stock: Unallowed NULL argument\n");
    return false;
  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("sync_merch_stock: Non existing merch.\n");
    return false;
  }
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  size_t old_amount        = merch_data->total_amount;
  merch_data->total_amount = merch_stock(store, name);
  // Return true if the total amount was changed
  if (old_amount != merch_data->total_amount)
    return true;
  else
    return false;
}

size_t increase_equal_stock(webstore_t *store, char *name, size_t amount){
  // Increase (or decrease) the stock at an existing
  // shelf. A negative (amount) decreases stock, positive
  // increases.  
  if ((store == NULL) || (name == NULL)){
    perror("increase_stock: Unallowed NULL argument\n");
    return 0;
  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("increase_stock: Non existing merch.\n");
    return 0;
  }
  // Add a specified amount of an item at a shelf.
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  // Get the current amount of the item on the shelf
  int new_amount = 0;
  int tmp_amount = amount;
  ioopm_link_t *db_item  = merch_data->locs->first;

  do {
    shelf_t *shelf = get_elem_ptr(db_item->element);
   
    // Remove stock gradually from shelfs
    if (shelf->amount > 0){
      new_amount = shelf->amount - tmp_amount;
      if (new_amount < 0){	
	shelf->amount = 0;
	tmp_amount = -new_amount;
      }else{
	shelf->amount = new_amount;
        break;
      }
    }
					      
    db_item = db_item->next;           
  } while (db_item != NULL);

  // Update the total stock
  sync_merch_stock(store, name);
  // Return the new stock at the shelf
  return new_amount;
}
size_t increase_stock(webstore_t *store, char *name,
		      char *shelf_name, size_t amount){
  // Increase (or decrease) the stock at an existing
  // shelf. A negative (amount) decreases stock, positive
  // increases.  
  if ((store == NULL) || (name == NULL) || (shelf_name == NULL)){
    perror("increase_stock: Unallowed NULL argument\n");
    return 0;
  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("increase_stock: Non existing merch.\n");
    return 0;

  }else if (!ioopm_hash_table_has_key(store->storage_db,
				      ptr_elem(shelf_name))){
    perror("increase_stock: Storage doesnt contain shelf.\n");
    return 0;;
  }
  // Add a specified amount of an item at a shelf.
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  // Get the current amount of the item on the shelf
  size_t old_amount = merch_stock_on_shelf(store, name,
					  shelf_name);
  size_t new_amount = old_amount + (size_t)amount;

  //  if (new_amount < 0){
  //    perror("increase_stock: Storage amount cannot be negative.\n");
  //  }
  // Update the shelf stock
  set_merch_stock(store, name, new_amount, shelf_name);
  // Update the total stock
  merch_data->total_amount += (size_t)amount;
  // Return the new stock at the shelf
  return new_amount;
}


void show_stock(webstore_t *store){
  // Prettyprint all shelfs, together with all merchendise metadata.
  // Displays: All shelfs, All merch (in shelfs), their;
  // Name, Description, Price, Stock Amount.

  // Pretty Print Template used
  // --- <shelf>
  // <name> - <price>Kr < OUT OF STOCK| IN STOCK> (<stock>st)
  // <description>
  // [...]

  if (store == NULL){
    perror("show_stock: Webstore is NULL.\n");
    return;
  }
  ioopm_list_t *shelfs = ioopm_hash_table_keys(store->storage_db);

  if (shelfs == NULL) {
    perror("show_stock: No shelfs in storage database.\n");
    return;
  }

  ioopm_link_t *shelf  = shelfs->first;
  ioopm_link_t *name;
  char *current_name;
  int current_stock;
  int current_nr = 1; 
  
    do {
      char * current_shelf = (char *)get_elem_ptr(shelf->element);
      name          = get_locations(store, current_shelf)->first;
      
      printf("====== Shelf:%s ======\n", current_shelf);      
      
      do {
	current_name  = (char *)get_elem_ptr(name->element);
	current_stock = merch_stock_on_shelf(store, (char*)current_name,
					    (char *)current_shelf);      
	if (current_stock > 0){
	printf("┏──╸Id.%d %s\n",
	       current_nr, current_name); 

	//	printf("┃ Name: %s",      current_name);

	

	printf("┃ Price: %dKr\n", merch_price(store, current_name));
	printf("┃ Stock: %dSt\n", current_stock);

	printf("┃ Desc: %s\n",    merch_description(store, current_name));
	puts("┗───────────────────────────╸");
	// Next Name
	}else printf("───╸Id.%d %s (NOT IN STOCK)\n", current_nr, current_name);
	name          = name->next;
	current_nr += 1;	
      } while (name != NULL);

      // Next shelf
      shelf         = shelf->next;
    } while (shelf != NULL);

    puts("");
  ioopm_linked_list_destroy(shelfs);

}

char *get_merch_name_in_storage(webstore_t *store, int nr_merch){
  
  ioopm_list_t *shelfs = ioopm_hash_table_keys(store->storage_db);

  ioopm_link_t *shelf  = shelfs->first;
  ioopm_link_t *name;
  char *current_name;
  int current_nr = 0; 
  
    do {
      char * current_shelf = (char *)get_elem_ptr(shelf->element);
      name          = get_locations(store, current_shelf)->first;
      
      do {
	current_name  = (char *)get_elem_ptr(name->element);
	
	current_nr += 1;
	if(current_nr == nr_merch){ 
	  ioopm_linked_list_destroy(shelfs);
	  return current_name; 
	}

	// Next Name
	name          = name->next;
      } while (name != NULL);

      // Next shelf
      shelf         = shelf->next;
    } while (shelf != NULL);
    
    return NULL;
    // <-----------------------------------------------------
    //    ioopm_linked_list_destroy(shelfs);
}

/*
void merchendise_modify(webstore_t *store, char *name,
			merch_modify_function *fun,
			void *fun_arg){
  if ((store == NULL) || (name == NULL) || (fun == NULL)){
    perror("merchendise_modify: Unallowed NULL arguments.\n");
  } else if(store->merch_db == NULL){
    perror("merchendise_modify: Merch database is NULL.\n");
  } else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merchendise_modify: Non existing merch.\n");    
  } else {
    
    merch_t *data =
      get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					   ptr_elem(name)));    
    merch_t *new_data = fun(data, fun_arg);

   // Remove the old merch from the merch database
   ioopm_hash_table_remove(store->merch_db,
			   ptr_elem(name));

   // Reinsert the modified merch into the database
   ioopm_hash_table_insert(store->merch_db,
			   ptr_elem(name),
			   ptr_elem(new_data));   
   return;
  }    
}
void merchendise_new_internal_name(webstore_t *store,
				   char *name,
				   char *new_name){
  merchendise_modify(store, (char*)name,
			    merch_change_internal_name_function,
			    (char*)new_name);
}
void merchendise_new_desc(webstore_t *store,
			  char *name,
			  char *edited_desc){
  
   merchendise_modify(store, (char*)name,
			    merch_change_description_function,
			    (char*)edited_desc);
}
void merchendise_new_price(webstore_t *store,
			   char *name,
			   size_t new_price){
  
   merchendise_modify(store, (char*)name,
			    merch_change_price_function,
			    (size_t*)new_price);
}
void merchendise_new_locs(webstore_t *store,
			  char *name,
			  ioopm_list_t *new_locs){
  
  merchendise_modify(store, (char*)name,
			    merch_change_locs_function,
			    (ioopm_list_t*)new_locs);
}

char *merch_get_desc_function(merch_t *merch_data){
  // Helper function for extracting destription
  // from a merch
  return merch_data->desc;
}
  
merch_t *merch_change_internal_name_function(merch_t *merch_data,
					     void *new_name){
  merch_data->name = (char*)new_name;
  return merch_data;
}

merch_t *merch_change_description_function(merch_t *merch_data,
					   void *new_desc){
  if (merch_data == NULL){
    perror("merch_change_description_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->desc = (char*)new_desc; }
  return merch_data;
}

merch_t *merch_change_price_function(merch_t *merch_data,
				     void *new_price){ 
  if (merch_data == NULL){
    perror("merch_change_price_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->price = (size_t)new_price; }
  return merch_data;
}

merch_t *merch_change_locs_function(merch_t *merch_data,
				    void *new_locs){ 
  if (merch_data == NULL){
    perror("merch_change_locs_function: Merch,\
            the merchendise has not been initialized.\n");
    exit(-1); // REMOVE THIS LATER
  }
  else { merch_data->locs = (ioopm_list_t*)new_locs; }
  return merch_data;
}
*/

char *lookup_merch_name(webstore_t *store, int index){ 
  // Return the name of the merch at a specified
  // index in the list returned by hash_table_values
  if (store == NULL){
    perror("lookup_merch_name: Unallowed NULL argument.\n");
    return "";
  }  
  ioopm_list_t *list_merch =
    ioopm_hash_table_values(store->merch_db); 

  if (index < 0){
    perror("lookup_merch_name: Too Small Index.\n");
    ioopm_linked_list_destroy(list_merch);  
    return "";
   } 
  if (((size_t)index >= ioopm_linked_list_size(list_merch))){
    perror("lookup_merch_name: Too Large index.\n");
    ioopm_linked_list_destroy(list_merch);  
    return "";
  }
  elem_t value_ptr         =
    ioopm_linked_list_get(list_merch, index);
  merch_t *merch           =
    get_elem_ptr(value_ptr);
  char *merch_name = merch->name;
  
  ioopm_linked_list_destroy(list_merch);  
  return merch_name;
}

bool valid_index(webstore_t *store, int index){
  // Return true if index is less or equal to
  // the amount of merch in the merch database
  ioopm_list_t *list = ioopm_hash_table_values(store->merch_db);

  if ((size_t)index > ioopm_linked_list_size(list)){
    perror("valid_index: Too Large index.\n");
    ioopm_linked_list_destroy(list);
    return false;
  }  
  else if (index < 0){
    perror("valid_index: Too Small Index.\n");
    ioopm_linked_list_destroy(list);
    return false;
  }  
  else if(index-1 >= list->size){
    ioopm_linked_list_destroy(list);
    return false;
  }
  ioopm_linked_list_destroy(list);
  return true;
}



void print_merch(merch_t *merch){
  printf("| Item:          %s\n", merch->name);  
  printf("| Description:   %s\n", merch->desc);
  printf("| Price:         %ld\n",merch->price);
  printf("| Stock (Total): %ld\n", merch->total_amount);
}

/// Other


// list_merchandise(webstore_t *store);
// list all items in store
// TODO REQ: Alphabetical order (soft req) - qsort
// TODO REQ: print 20 at time
//TODO REQ: ask if more
// list of all ptr of merch

