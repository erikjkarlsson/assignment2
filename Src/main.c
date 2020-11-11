#include "common.h"
#include "hash_table.h"
#include "list_linked.h"
#include "webstore.h"
#include "stdio.h"
#include "utils.h"

/*




// Hash_table: namn -> info
// Key: Shelf, Value: Amount


void edit_item(merch_t *merch, elem_t name){

  item_t name_info = ioopm_hash_table_lookup(merch->name, name);
  elem_t shelf_info = ioopm_hash_table_lookup(merch->locs, name_info->shelf);

  
}

bool edit_shelf(ioopm_hash_table_t *locs, elem_t shelf, elem_t value) {
  unsigned long hashed_key = locs->hash_func(key);
  unsigned long bucket = hashed_key % locs->capacity;

  entry_t *tmp = find_previous_entry_for_key(locs->comp_key, locs->buckets[bucket], shelf);
  entry_t *next = tmp->next;

  if (errno != EINVAL)
  {
    next->value.i = value;
    return true;
  }
  else{
    return false;
  }
}

elem remove_item(merch_t *merch, elem_t name){
  // Lookup bound information
  elem_t name_info  = ioopm_hash_table_lookup(merch->name, name);
  // Set shelf amount to zero
  elem_t shelf_info = edit_shelf(merch->locs, name_info->shelf, int_elem(0));
  
  // Remove name from "name list"
  ioopm_hash_table_remove(merch->name, name);
  
  // Return removed information
  return name_info
  
}

void list_items(merch_t *merch){
  // Alla namn
  ioopm_list_t names = ioopm_hash_table_keys(merch->name);
  // Pretty printout
  printf(" [Name]\t[Shelf]\t[Amount] ");  
  // Loopa alla namn och printa ut dom
  for (size_t i = 0; i < names->size; i++){
    // Leta upp ett namn som finns på index i 
    elem_t name  = ioopm_linked_list_get(names, i);
    
    //elem_t *item = ioopm_hash_table_lookup(merch->name, name);    
    // Extrahera all information från den 
    // elem_t shelf  = item->shelf;  // hylla 
    // elem_t amount = item->amount; // antal på hyllan
    
    printf(" %s ", name.c); // shelf.c, amount.i);
  }
}

*/

void store_display_item(ioopm_list_t *item_list){
  elem_t info;
  
  for (int i = 0; i < (int)item_list->size; i++){
    info = ioopm_linked_list_get(item_list, i);

    if (STR_EQ(info.edesc, "desc")){
      printf("Description: %s\n", info.c);
	
    }else if (STR_EQ(info.edesc, "price")){	
      printf("Price: %d€\n", info.i);
	
    }else if (STR_EQ(info.edesc, "name")){
      printf("Name: %s\n", info.c);
    }
    

  }  
    
}
int main(int argc, char **argv) {
  ioopm_list_t *info_list1 = ioopm_linked_list_create(ioopm_elem_cmp);
  ioopm_list_t *info_list2 = ioopm_linked_list_create(ioopm_elem_cmp);
  ioopm_list_t *info_list3 = ioopm_linked_list_create(ioopm_elem_cmp);

  create_info(info_list1,	     
	      str_elem("Stol från IKEA"),
	      int_elem(123),
	      str_elem("stol"));

  create_info(info_list2,
	      str_elem("bord från IKEA"),
	      int_elem(321),
	      str_elem("bord"));

  create_info(info_list3,
	      str_elem("hylla från IKEA"),
	      int_elem(54321),
	      str_elem("hylla"));

  store_display_item(info_list1);
  store_display_item(info_list2);
  store_display_item(info_list3);
  printf("Hello world!");
  return 0;
}

