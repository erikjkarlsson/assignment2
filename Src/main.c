#include "common.h"
#include "hash_hable.h"
#include "list_linked.h"
#include "stdio.h"
#include "utils.h"


void list_items(merch_t *merch){ // Fixa sen
  ioopm_list_t names = ioopm_hash_table_keys(merch->name, elem_t key);

  printf(" [Name]\t[Shelf]\t[Amount] ");  

  for (size_t i = 0; i < names->size; i++){
    // Leta upp ett namn som finns på index i 
    elem_t name  = ioopm_linked_list_get(names, i);
    
    elem_t *item = ioopm_hash_table_lookup(merch->name, name);
    
    // Extrahera all information från den 
    elem_t shelf  = item->shelf;  // hylla 
    elem_t amount = item->amount; // antal på hyllan
    
    printf(" %s\t%s\t%d ", name.c, shelf.c, amount.i);
  }
}

int main(int argc, char **argv) {
  printf("Hello world!");
  return 0;
}

