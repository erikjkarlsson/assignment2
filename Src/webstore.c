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


typedef struct  shelf shelf_t;
struct shelf
{
  char *shelf;
  char *amount;
};


typedef struct store store_t;
struct store
{
  // Key: Item Name
  // Value: Item Information
  ioopm_hash_table_t *name;

  // Key: Storage Name
  // Value: Linked List of Item Instances   
  ioopm_hash_table_t *locs;  
};


elem_t set_edesc(elem_t elem, char *name);

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

void create_info(ioopm_list_t *info_list,
		 elem_t desc,
		 elem_t price,
		 elem_t name){  
  // Add basic information to a item
  add_info(info_list, name,  "name");
  add_info(info_list, desc,  "desc");
  add_info(info_list, price, "price");
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
