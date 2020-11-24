trello
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "db.c"
#include "utils.h"

typedef struct merch merch_t;
struct merch
{
  ioopm_hash_table_t name; //name of item as key and information of item i as values
  ioopm_hash_table_t locs;  //storage location names as keys and informatiom of items stored at that location as values
};

void merch_t_create(){
  merch_t *merch->name = ioopm_hash_table_create(ioopm_hash_function hf, ioopm_eq_function comp_key, ioopm_eq_function comp_value); //lägg in passande func
  merch_t *locs->name = ioopm_hash_table_create(ioopm_hash_function hf, ioopm_eq_function comp_key, ioopm_eq_function comp_value); //Lägg in passande func
}

void add_merchandise(merch_t merch_stock, elem_t name, elem_t shelf, elem_t item_pointer){
  ioopm_hash_table_insert(merch_stock->name, name, item_pointer);
  bool increased = ioopm_hash_table_increase(merch_stock->locs, shelf);
  if(!increased){
    ioopm_hash_table_insert(merch_stock->locs, shelf, int_elem(1));
  }
}

int main(int argc, char *argv[]){
}
