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


  
/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// MERCH                                            ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

merch_t *create_merch(char *name, char *desc,
		      size_t price, ioopm_list_t *locs){

  // Create a merchendise item, allocate it, allocate is
  // locs list (if provided one is NULL)
  if ((!name) || (!desc))
    return NULL;
  
  merch_t *item = calloc(1, sizeof(merch_t));

  item->name          = name;
  item->desc          = desc;
  item->price         = price;
  item->total_amount  = 0;
  
  // If no shelf database was given create a new

  item->locs          = locs;

  return item;
}

void merch_destroy(merch_t *merch){
  ioopm_linked_list_destroy(merch->locs);
  //  free(merch->name);
  //  free(merch->desc);
  free(merch);  
}

bool add_merchendise(webstore_t *store, char *name, char *description, size_t price){
  if (ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("add_merchendise: Merch name in database.\n");
    return false;
  }

  ioopm_list_t *locs = ioopm_linked_list_create();
  merch_t *new_merch = create_merch(name, description, price, locs);

  ioopm_hash_table_insert(store->merch_db,
			  ptr_elem(new_merch->name),
			  ptr_elem(new_merch)); 
  
  return true;
}
bool remove_merchendise(webstore_t *store, char *name){
  // Remove the associated merch database entry to
  // a name.
  
  if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("remove_merchendise: Merch name not in database.\n");
    return false;
  }else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("remove_name_from_shelf: Non existing merch name.\n");
    return false;
  }

  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));
  // Remove the merch itself
  merch_destroy(merch_data);

  // Remove from merch database 
  ioopm_hash_table_remove(store->merch_db, ptr_elem(name));
  //linked list that holds all shopping carts

  //  new_webstore->all_shopping_carts = ioopm_linked_list_create();
  //  ioopm_linked_list_append(new_webstore->all_shopping_carts,
  //			   ptr_elem(create_cart(new_webstore)));
  
  return true;
}

void store_destroy(webstore_t *store){
  // Deallocate the argument handler, both hash tables
  // and the shopping cart list. And the whole webstore.
  if (!store){
    perror("store_destroy: Webstore is NULL.\n");
    return;
  }


  destroy_arg_opt(store->opt);


  destroy_all_merch(store);


  destroy_storage(store);


  ioopm_hash_table_destroy(store->merch_db);
  ioopm_hash_table_destroy(store->storage_db);
  
  destroy_all_carts(store);
  ioopm_linked_list_destroy(store->all_shopping_carts);  

  free(store);
}

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

  
  return new_webstore;
}

/// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// SHELF                                            ///
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

shelf_t *create_shelf(char *shelf, size_t amount){
  // Allocate and create shelf

  // Check for sub-zero stock
  if (amount <= 0) {
    perror("create_shelf: Negative stock.\n");
    return NULL;
  // Assign non-correctly formatted shelfs
  // into the shelf: ’A00’
  }if ((!shelf) || (!is_shelf(shelf))){
    perror("create_shelf: Shelf name is Incorrectly.\n");
    return NULL;
  }
  
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));    

  new_shelf->shelf   = shelf;
  new_shelf->amount  = amount;

  return new_shelf;
}

void destroy_shelf(shelf_t *shelf){
  // Deallocate a shelf
  if (!shelf){
    perror("destroy_shelf: Shelf is already NULL.\n");
    return;
  }
  free(shelf);
}

char *merch_description(webstore_t *store, char *name){
  if (!name || !store) return 0;
  if (!ioopm_hash_table_has_key(store->merch_db,
				ptr_elem(name))){
    perror("merch_description: Non existing merch.\n");
    return 0;
  }
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  


  return strdup(merch_data->desc);

}
int merch_price(webstore_t *store, char *name){
  // Return the stock of a certain name in
  // the merch database.
  
  if (!name || !store)
    return 0;
  else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_price: Non existing merch.\n");
    return 0;    
  }else {
    merch_t *merch_data =
      get_elem_ptr(ioopm_hash_table_lookup(store->merch_db, ptr_elem(name)));
    return merch_data->price;
  }
}

int merch_stock_on_shelf(webstore_t *store, char *name, char *shelf){
  // Return the amount merchendise at a specific shelf

  if (!name || !shelf)
    return 0;
  else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_stock_on_shelf: Non existing merch.\n");
    return 0;
  }
  

  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  if (!merch_data){
    perror("merch_stock_on_shelf: Merch does not exist.\n");
    return 0;    
  }

  ioopm_link_t *merch_data_locs = merch_data->locs->first;  


  shelf_t *shelf_data = NULL;

  do {
    shelf_data = get_elem_ptr(merch_data_locs->element);
    if (STR_EQ(shelf_data->shelf, shelf)) return shelf_data->amount;
    
    merch_data_locs = merch_data_locs->next;
  } while (merch_data_locs);

  return 0;  
}
void remove_from_storage(webstore_t *store, char *name, char *shelf){

  if ((!store) || (!shelf) || (!name)){
    perror("remove_from_storage: Unallowed NULL arguments.\n");
    return; 
  }  
  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);

  if ((ioopm_linked_list_size(db_names) < 1) || !db_names){
    perror("remove_from_storage: No amounts on any shelf.\n");
    return; 
  }
  
  ioopm_link_t *db_item = db_names->first;
  ioopm_link_t *db_prev = db_names->first;

  if (!db_item) {
    perror("remove_from_storage: No shelfs exist.\n");
    return;
  }
  
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
  if ((!store) || (!shelf)){
    perror("display_storage: Unallowed NULL arguments.\n");
    return; 
  } else if ((!store->merch_db) || (!store->storage_db)){
    perror("display_storage: Databases is NULL");
    return; 
  }  

  // Names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);
  ioopm_link_t *db_item  = db_names->first;
  char *shelf_item_name  = NULL;
  // Print what shelf
  printf("┏──╸ Shelf Id.%s Contains...\n", shelf);
  // Print all items
  do {
    // Already exists in database
    shelf_item_name = get_elem_ptr(db_item->element);   
    printf("┃ Merch-name : %s\n", shelf_item_name);    
					      
    db_item = db_item->next;    
  } while (db_item);
  
}

void list_shelfs(webstore_t *store, char *name){
  int id = 0;
  
  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));
  if (!merch_data){
    perror("list_shelfs: Merch does not exist.\n");
    return;
  }
    
  ioopm_list_t *merch_data_locs = merch_data->locs;
  
  if (!merch_data_locs->first){
    perror("list_shelfs: Merch has no shelfs.\n");
    return;
  }
  
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_data_locs);
  shelf_t *shelf              = get_elem_ptr(ioopm_iterator_current(iter));

  // Empty merch locs
  if (!shelf) return;

  printf("┏──╸ %s Exists on Shelfs; \n", name);  
  do {
    printf("┃ Id.%d \n ┃ Shelf: %s \n ┃ Stock: %ldst\n",
	   id, shelf->shelf, shelf->amount);
    
    shelf  = get_elem_ptr(ioopm_iterator_next(iter));
    id++;   
  } while (ioopm_iterator_has_next(iter) || shelf);
   
  ioopm_iterator_destroy(iter);
}
void remove_name_from_shelf(webstore_t *store, char *shelf, char *name){

  if (!name || !shelf) {
    perror("remove_name_from_shelf: Unallowed NULL arguments.\n");
    return; 
  } else if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    perror("remove_name_from_shelf: Non existing shelf name.\n");
    return;
  }else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("remove_name_from_shelf: Non existing merch name.\n");
    return;
  }
  ioopm_list_t *list         =
    ioopm_hash_table_lookup(store->storage_db,
			    ptr_elem(shelf)).p;
  if (!list->first) return;
  
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

  char *current_name = get_elem_ptr(ioopm_iterator_current(iter));
  
  do {
    
    if(STR_EQ(current_name, name)){
      ioopm_iterator_remove(iter);
      ioopm_iterator_destroy(iter);
      return;
    }
  
    shelf = get_elem_ptr(ioopm_iterator_next(iter));      
}   while (ioopm_iterator_has_next(iter)); 
  ioopm_iterator_destroy(iter);
}

char *get_shelf_after_shelf_nr(webstore_t *store, size_t shelf_nr, char *name){
  
  merch_t *merch_data         =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
					 
  ioopm_list_t *merch_data_locs    = merch_data->locs;
  
  if(ioopm_linked_list_size(merch_data_locs) <= shelf_nr){
    perror("get_shelf_after_shelf_nr: Shelf Number Out of Bounds.\n");
    return NULL; 
  }
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_data_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (size_t i = 1;; i++){
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
  if (!(store) || (!shelf) || (!store->storage_db)){
    perror("get_locations: Unallowed NULL arguments.\n");
    return NULL; 
  }else 
    return get_elem_ptr(ioopm_hash_table_lookup(store->storage_db,
						ptr_elem(shelf)));
}
bool storage_contains(webstore_t *store, char *shelf, char *name);

void remove_shelf(webstore_t *store, char *shelf){
  // Remove a storage shelf from the storage_db hash-table.
  
  if ((store == NULL) || (shelf == NULL) || (!store->storage_db)){
    perror("remove_shelf: Unallowed NULL arguments.\n");
    return;
  }
  // Shelf does not exist in storage db  
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){
    perror("remove_shelf: Cannot remove, non-existing shelf.\n");
    return;
  } 
    
  ioopm_list_t *storage_list = get_locations(store, shelf);    

  if (!storage_list) return;
  
  ioopm_linked_list_destroy(storage_list); 
  ioopm_hash_table_remove(store->storage_db, ptr_elem(shelf));

   
}

void destroy_all_merch(webstore_t *store){
  // Remove all shelfs in storage_db, but not the hash-table.
  if (!store){
    perror("destroy_storage: Webstore is NULL\n");
    return;
  }
  if (!store->merch_db){
    return;
  }

 ioopm_list_t *merches  = ioopm_hash_table_keys(store->merch_db);
 ioopm_link_t *current = merches->first;
 
 if (!current){
   ioopm_linked_list_destroy(merches);   
   return;
 }

 // Iterate all shelfs removing them
 do {
   if (!get_elem_ptr(current->element))
     remove_merchendise(store, get_elem_ptr(current->element));
   current = current->next;
 } while (current);
 
  ioopm_linked_list_destroy(merches);  
}

void destroy_storage(webstore_t *store){
  // Remove all shelfs in storage_db, but not the hash-table.
  if (!store){
    perror("destroy_storage: Webstore is NULL\n");
    return;
  }
  if (!store->storage_db){
    perror("destroy_storage: No storage database\n");
    return;
  }

 ioopm_list_t *shelfs  = ioopm_hash_table_keys(store->storage_db);
 ioopm_link_t *current = shelfs->first;
 
 if (!current){
   ioopm_linked_list_destroy(shelfs);   
   return;
 }

 // Iterate all shelfs removing them
 do {
   if (!get_elem_ptr(current->element))
     remove_shelf(store, get_elem_ptr(current->element));
   current = current->next;
 } while (current);
 
  ioopm_linked_list_destroy(shelfs);  
}

void add_to_storage(webstore_t *store, char *name, char *shelf){

  if (!store || !name || !shelf){
    perror("add_to_storage: Unallowed NULL argument.\n");
    return;
    
  }else if (STR_EQ(name, "") || (STR_EQ(shelf, ""))){
    perror("add_to_storage: Cannot add empty names or shelfs.\n");
    return;
  } else if (storage_contains(store, shelf, name)) {
    perror("add_to_storage: Shelf already contains merch name.\n");
    return;
  }
    
  // Create a new storage list if shelf does not exist
  if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(shelf))){

    ioopm_list_t *storage_list = ioopm_linked_list_create();
    ioopm_linked_list_append(storage_list, ptr_elem(name));

    ioopm_hash_table_insert(store->storage_db,
			    ptr_elem(shelf),
			    ptr_elem(storage_list));    
  }
  
  // Add to names stored at requested shelf location
  ioopm_list_t *db_names = get_locations(store, shelf);
  ioopm_linked_list_append(db_names, ptr_elem(name));
}


bool storage_contains(webstore_t *store, char *shelf, char *name){
  if ((!store) | (!shelf) | (!name)){
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
  } while (db_item);
  
  // Does not exist in database
  return false;
}

// add merch on shelf
void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount){
  // Add / Update shelf to both the merch database and the
  // storage database. If it already exists, update amount.

  if (!name || !shelf || !store) {
    perror("set_shelf: NULL arguments.\n");
    return;
  }
   if (!ioopm_hash_table_has_key(store->merch_db,
				 ptr_elem(name)))
     return;
  // Add name to shelf if it already doesnt not contain it.
  if (!storage_contains(store, name, shelf))
      add_to_storage(store, name, shelf);

  if (!storage_contains(store, name, shelf)){
    perror("set_shelf: Could not add shelf.\n");
    return;
  }


  set_merch_stock(store, name, amount, shelf);

  // Update Total stock
  sync_merch_stock(store, name);
}

bool sync_merch_stock(webstore_t *store, char *name){
  // Calculate the total stock of a merch item from
  // the locs list and update merch->total_amount
  // reflecting that. If a syncronization happened
  // the function returns true.
  
  if ((!store) || (!name)){
    perror("sync_merch_stock: Unallowed NULL argument\n");
    return false;

  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("sync_merch_stock: Non existing merch.\n");
    return false;
  }

  // Get merch
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));

  if (!merch_data) {
    perror("sync_merch_stock: Merch mapped to NULL.\n");
    return false;
  }
  // Get merch stock
  size_t old_amount        = merch_data->total_amount;
  // Update merch stock
  merch_data->total_amount = merch_stock(store, name);
  
  // Return true if the total amount was changed
  if (old_amount != merch_data->total_amount) return true;
  else return false;
}

size_t change_stock_relative_amount(webstore_t *store, char *name, size_t amount){
  // Increase (or decrease) the stock at an existing
  // shelf. A negative (amount) decreases stock, positive
  // increases.
  
  if ((!store) || (!name)){
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
  int new_amount         = 0;
  int tmp_amount         = amount;
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
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* shelf_name){
  // Increase (or decrease) the stock at an existing
  // shelf. A negative (amount) decreases stock, positive
  // increases.  
  if ((store == NULL) || (name == NULL) || (shelf_name == NULL)){
    perror("increase_stock: Unallowed NULL argument\n");
    return;
  } else if (!ioopm_hash_table_has_key(store->merch_db,
				       ptr_elem(name))){
    perror("increase_stock: Non existing merch.\n");
    return;

  }else if (!ioopm_hash_table_has_key(store->storage_db,
				      ptr_elem(shelf_name))){
    perror("increase_stock: Storage doesnt contain shelf.\n");
    return;;
  }
  // Add a specified amount of an item at a shelf.
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  // Get the current amount of the item on the shelf
					 
  ioopm_list_t *merch_data_locs    = merch_data->locs;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_data_locs);

  shelf_t *shelf = get_elem_ptr(ioopm_iterator_current(iter));
  
  for (size_t i = 1;; i++){
    if(STR_EQ(shelf_name, shelf->shelf)){
      shelf->amount = amount;
      ioopm_iterator_destroy(iter);
    }
    
    if(ioopm_iterator_has_next(iter)){        
      shelf   = get_elem_ptr(ioopm_iterator_next(iter));
    }else { break; }    
  }    
  

  ioopm_iterator_destroy(iter);

  //  if (new_amount < 0){
  //    perror("increase_stock: Storage amount cannot be negative.\n");
  //  }
  // Update the shelf stock

  // Update the total stock
  merch_data->total_amount += (size_t)amount;
  // Return the new stock at the shelf
  return;
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

  if (!store){
    perror("show_stock: Webstore is NULL.\n");
    return;
  }
  ioopm_list_t *shelfs = ioopm_hash_table_keys(store->storage_db);

  if (!shelfs) {
    perror("show_stock: No shelfs in storage database.\n");
    return;
  }

  ioopm_link_t *shelf  = shelfs->first;
  if (!shelf) return;
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
    printf("┏──╸ Id.%d \n┃", i);
    print_merch(current);
    printf("\n");

    if(ioopm_iterator_has_next(iter)){
      current = get_elem_ptr(ioopm_iterator_next(iter));      
    }else { break; }    
  }     
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list_merch);  
}

void set_merch_price(webstore_t *store, char *name, size_t price){
  if ((!name) || (!store) || (!store->merch_db)){
    perror("merch_price: Unallowed NULL argument.\n");
    return;
  } else if ((int)price <= 0){
    perror("merch_price: Invalid price.\n");
    return;
  } else if (!ioopm_hash_table_has_key(store->merch_db, ptr_elem(name))){
    perror("merch_price: Non existing merch.\n");
    return;
  } 

  merch_t *data = get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
						       ptr_elem(name)));            
  data->price = price;
}

char *get_merch_name_in_storage(webstore_t *store, int nr_merch){

  if ((!store) || (!store->storage_db)){
    perror("get_merch_name_in_storage: Storage database is deallocated.\n");
    return NULL;
  }
 
  if ((nr_merch <= 0) || !valid_merch_id(store, nr_merch)){
      perror("get_merch_name_in_storage: Merch Id does not exist.\n");
    return NULL;
  } 


  ioopm_list_t *shelfs = ioopm_hash_table_keys(store->storage_db);
  ioopm_link_t *shelf  = shelfs->first;

  if (!shelf){
    perror("get_merch_name_in_storage: Storage database is empty\n");
    return NULL;
  }
 
  
  ioopm_link_t *name;
  char *current_name;
  int current_nr = 1; 
  
    do {
      char * current_shelf = (char *)get_elem_ptr(shelf->element);

      if (!ioopm_hash_table_has_key(store->storage_db, ptr_elem(current_shelf))){
	perror("merch_price: Non existing merch.\n");

      }
      
      name          = get_locations(store, current_shelf)->first;
      
      do {
	current_name  = (char *)get_elem_ptr(name->element);
	

	if(current_nr == nr_merch){ 
	  ioopm_linked_list_destroy(shelfs);
	  return current_name; 
	}

	// Next Name
	current_nr += 1;
	name          = name->next;
      } while (name != NULL);

      // Next shelf
      shelf         = shelf->next;
    } while (shelf != NULL);


    perror("get_merch_name_in_storage: Merch \n");

    ioopm_linked_list_destroy(shelfs);
    return NULL;    
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
			    _change_description_function,
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


char *merch_at_storage_id(webstore_t *store, size_t index){ 
  // Return the name of the merch at a specified
  // index in the list returned by hash_table_values
  if ((!store) || (!store->merch_db)){
    perror("merch_at_storage_id: Webstore is NULL.\n");
    return NULL;
  }  
  ioopm_list_t *list_merch = ioopm_hash_table_values(store->merch_db); 

  if (!valid_merch_id(store, index)){
    perror("merch_at_storage_id: Invalid index.\n");
    ioopm_linked_list_destroy(list_merch);  
    return NULL;
  }
  
  elem_t value_ptr = ioopm_linked_list_get(list_merch, index);
  merch_t *merch   = get_elem_ptr(value_ptr);
  char *merch_name = merch->name;
  
  ioopm_linked_list_destroy(list_merch);  
  return merch_name;
}

bool valid_shelf_id(webstore_t *store, int id){
  // True if the id is smaller then or equal to
  // the amount of merch in merch_db, and
  // over 0
  
  return ((id > 0) &&
	  (id <= (int)ioopm_hash_table_size(store->storage_db)));
}
bool valid_merch_id(webstore_t *store, int id){
  // True if the id is smaller then or equal to
  // the amount of merch in merch_db, and
  // over 0
  
  return ((id > 0) &&
	  (id <= (int)ioopm_hash_table_size(store->merch_db)));
}



void print_merch(merch_t *merch){
  if (!merch) {
    perror("print_merch: Merch does not exist");
    return;
    }else if (!merch->locs) {

    printf("| Item:          %s\n", merch->name);  
    printf("| Description:   %s\n", merch->desc);
    printf("| Price:         %ld\n",merch->price);
    return;
  }
  
  printf("| Item:          %s\n", merch->name);  
  printf("| Description:   %s\n", merch->desc);
  printf("| Price:         %ld\n",merch->price);
  printf("| Stock (Total): %ld\n", merch->total_amount);
}

/// Other
size_t merch_stock(webstore_t *store, char *name){
  if ((!store) || (!name)){
    perror("merch_stock: NULL Arguments.\n");
    return 0;    
  }
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  
  if (!merch_data){
    perror("merch_stock: Unallocated Merch stock.\n");
    return 0;    
  }
  ioopm_link_t *merch_locs = merch_data->locs->first;
    if ((!merch_locs)){
    perror("merch_stock: No merch locs.\n");
    return 0;    
  }

  int amount = 0;
  
  shelf_t *shelf_data = NULL;
  while (merch_locs != NULL) {

    shelf_data = (get_elem_ptr(merch_locs->element));
    amount    += shelf_data->amount;
    merch_locs = merch_locs->next;
           
  }

  return amount;
}
size_t merch_stock_at_shelf(webstore_t *store, char *name, char *shelf){
  // Return the amount merchendise at a specific shelf
  
  merch_t *merch_data =
    get_elem_ptr(ioopm_hash_table_lookup(store->merch_db,
					 ptr_elem(name)));  

  ioopm_link_t *merch_locs = merch_data->locs->first;  

  if (merch_locs == NULL){
    perror("merch_locs_at_shelf: Merch Locs is NULL.\n");
  }
  
  shelf_t *shelf_data = NULL;

  do {

    shelf_data = get_elem_ptr(merch_locs->element);

    if (STR_EQ(shelf_data->shelf, shelf))
	return shelf_data->amount;
    
    merch_locs = merch_locs->next;
  } while (merch_locs != NULL);


  perror("merch_locs_at_shelf: The merch has no stock on the shelf.\n");

  return 0;  
}



// list_merchandise(webstore_t *store);
// list all items in store
// TODO REQ: Alphabetical order (soft req) - qsort
// TODO REQ: print 20 at time
//TODO REQ: ask if more
// list of all ptr of merch

