#ifndef __CART_H__
#define __CART_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "webstore.h"


#define LANG 1
#define SWE(thing) if (LANG == 0) { thing; }
#define ENG(thing) if (LANG == 1) { thing; }
#define clear() printf("\033[H\033[J")
#define newline puts("")
#define AMOUNT_UPPER_MAX 100000

#define EITHER_CHAR(str, a, c)			\
  if ((str == a) || (str == b)) return c

#define UNTIL_CORRECT(what, check) \
  do { what; } while (!check)

#define SET_UNTIL(what, check)					\
  do { what; } while ((!(check)) && (!choice_prompt("Correct?")))

#define SAFESET(what, check, error)				\
  do { what; } while (!choice_prompt("Satisfied?"));		\
  if (!(check)


#define UNTIL_PLEASED(what) \
  do { what; } while (!choice_prompt("Correct?"))

#define BIPROMPT(swe, eng, what) \
  SWE(puts(swe)); \
  ENG(puts(eng)); \
  what

//used to place keys and values in one
struct entry_ht
{
  char *key; 
  int value;
};
bool valid_shelf_index(webstore_t *store, int id); 
typedef struct entry_ht entry_ht_t;

struct cart {
  int id; 
  ioopm_hash_table_t *merch_in_cart; 
}; 

typedef struct cart cart_t;

cart_t *create_cart(webstore_t *store);

void remove_cart(webstore_t *store, int id);

void add_to_cart(webstore_t *store, char *merch_to_add_name, int amount);

void remove_from_cart(webstore_t *store, int id, char *merch_to_remove_name, int amount_to_remove);
int active_cart_cost(webstore_t *store);

int calculate_cost(webstore_t *store, int id);

void checkout(webstore_t *store); 

void display_cart(cart_t *cart); 


bool is_merch(webstore_t *store, int id);
bool is_money(int size);
bool valid_id(webstore_t *store, int id);
bool valid_merch_index(webstore_t *store, int id);

void list_all_cart_id(webstore_t *store); 

//
/* USEFUL FUNCTIONS FOR TESTS*/
//

size_t nr_of_merch_in_cart(cart_t *cart);
int get_amount_of_merch_in_cart(cart_t *cart, char *merch_name); 
char *get_merch_name_in_cart(cart_t *cart, int nr_merch);
  
bool merch_in_cart(cart_t *cart, char *merch_name);
bool cart_is_empty(cart_t *cart);
size_t cart_db_size(cart_t *cart);

cart_t *get_cart(webstore_t *store, int id);

//
/*PROMPT FUNCTIONS*/
//



void remove_from_cart_prompt(webstore_t *store); 
void remove_from_active_cart_prompt(webstore_t *store);
void add_to_cart_prompt(webstore_t *store, int id);
void add_to_active_cart_prompt(webstore_t *store);



void sort_keys(entry_ht_t *keys, size_t no_keys);
#endif
