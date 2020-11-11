#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "db.c"
#include "utils.h"


// Name --> Merch
typedef struct merch merch_t;
struct merch
{
  // Item information
  ioopm_link_t *info;
  // Location informaion
  ioopm_link_t *locs;  
};


typedef struct shelf shelf_t;
struct shelf
{
  char *shelf;
  int amount;
};


typedef struct store store_t;
struct store
{
  // Key: Item Name
  // Value: Item Information
  ioopm_hash_table_t *names;

  // Key: Storage Name
  // Value: Linked List of Item Instances   
  ioopm_hash_table_t *locs;  
};

merch_t create_merch(){
  return (merch_t){
    .info=ioopm_linked_list_create(ioopm_elem_cmp),
    .locs=ioopm_linked_list_create(ioopm_elem_cmp)
  };
}

shelf_t create_shelf(char *shelf, int amount){
  return (shelf_t){.shelf=shelf, amount=amount}; 
}

store_t create_store(){
  return (store_t) {
    .name=ioopm_hash_table_create(),
    .locs=ioopm_hash_table_create()
  };
}
elem_t set_edesc(elem_t elem, char *name);

void item_add_name(ioopm_hash_table_t names,
		   elem_t name,
		   elem_t desc,
		   elem_t price,
		   elem_t shelf){  
  
  ioopm_hash_table_insert(names, name, set_edesc(desc,  "desc"));
  ioopm_hash_table_insert(names, name, set_edesc(price, "price"));
  ioopm_hash_table_insert(names, name, set_edesc(shelf, "shelf"));
  
};

bool item_name_exists(ioopm_hash_table_t names, elem_t name){
  return ioopm_hash_table_has_key(names, name);
}

void item_rem_name(ioopm_hash_table_t names, elem_t name){
  // Remove (just) a name
  ioopm_hash_table_remove(names, name);
};

entry_t *get_bucket(ioopm_hash_table_t names, elem_t name){
  unsigned long hashed_key = ht->hash_func(name);
  unsigned long bucket = hashed_key % ht->capacity;

  return names->buckets[bucket];
}


void item_add_shelf(){};
void item_rem_shelf(){};





void add_info(ioopm_list_t *info_list, elem_t info, char *edesc){
  // Add an info link to a info list
  ioopm_linked_list_append(info_list, set_edesc(info, edesc));
}

elem_t set_edesc(elem_t elem, char *name){
  // Set item description
  elem_t e = elem;
  e.edesc = name;
  return e;

}





/*
void merch_t_create(){
  merch_t *merch->name = ioopm_hash_table_create(ioopm_hash_function hf, ioopm_eq_function comp_key, ioopm_eq_function comp_value); //lägg in passande func
  merch_t *merch->name = ioopm_hash_table_create(ioopm_hash_function hf, ioopm_eq_function comp_key, ioopm_eq_function comp_value); //Lägg in passande func
}

void add_merchandise(merch_t merch_stock, elem_t name, elem_t shelf, elem_t item_pointer){
  ioopm_hash_table_insert(merch_stock->name, name, value);
  bool increased = ioopm_hash_table_increase(merch_stock->locs, shelf);
  if(!increased){
    ioopm_hash_table_insert(merch_stock->locs, shelf, int_elem(1));
  }
}

int main(int argc, char *argv[]){
  item_t item = input_item();
  add_merchandise(merch_t merch_stock, elem_t name, elem_t item_pointer)
}
*/
