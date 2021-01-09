#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//used to place keys and values in one
struct entry_ht
{
  char *key; 
  int value;
};

typedef struct entry_ht entry_ht_t;

struct cart {
  int id; 
  ioopm_hash_table_t *merch_in_cart; 
}; 

typedef struct cart cart_t;

cart_t *create_cart(webstore_t *store);

void remove_cart(webstore_t *store, int id);

void add_to_cart(webstore_t *store, int id, int nr_merch, int amount);

void remove_from_cart(webstore_t *store, int id, int nr_merch, int amount_to_remove);

int calculate_cost(webstore_t *store, int id);

void checkout(webstore_t *store, int id); 

void display_cart(cart_t *cart); 

//
/* USEFUL FUNCTIONS FOR TESTS*/
//

size_t nr_of_merch_in_cart(cart_t *cart);

int get_amount_of_merch(cart_t *cart, char *merch_name); 

bool merch_in_cart(cart_t *cart, char *merch_name);