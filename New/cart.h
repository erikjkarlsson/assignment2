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
  if (!(check)) error; 

#define UNTIL_PLEASED(what) \
  do { what; } while (!choice_prompt("Correct?"))

#define BIPROMPT(swe, eng, what) \
  SWE(puts(swe)); \
  ENG(puts(eng)); \
  what

// Max and minimum bounds for price
#define CAPITAL_MAX   100000
#define CAPITAL_MIN   0
// Maximum bound for cart id
// (preventing integer overflow)
#define ID_BOUNDS_MAX 100000
// Maximum allowed stock when creating new
// merch using the prompt
#define STOCK_MAX     1000

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

/* Validates the amount of 
   stock entered in the TUI prompt 
   
   amount : Size of merch a stock.
*/

bool valid_stock_size(int amount);

/*
Creates an emoty cart in the store 
The cart is assigned an ID
*/
cart_t *create_cart(webstore_t *store);

/*
Removes a cart compleatly from the store 
*/
void remove_cart(webstore_t *store);

/*
Adds a merchname and an amount of that to the cart
This doesn´t affect the storage 
*/
void add_to_cart(webstore_t *store, char *merch_to_add_name, int amount);

/* 
Removes compleatly or removes a given amount of merch in the cart
*/
void remove_from_cart(webstore_t *store, int id, char *merch_to_remove_name, int amount_to_remove);
int active_cart_cost(webstore_t *store);

/*
Calculates the cost of the shopping cart
*/
int calculate_cost(webstore_t *store, int id);

/*
The represention of a purchase of all the items in a particular shopping cart.
Decrease the stock for the merches in the cart.
Removes the shopping cart from the store.
*/
void checkout(webstore_t *store); 

/*
Displays all merches and amounts in the given cart
*/
void display_cart(cart_t *cart); 

/*
Determines if the given id of a cart represents an existing cart
*/

bool is_merch(webstore_t *store, int id);
bool is_money(int size);

bool valid_id(webstore_t *store, size_t id);
bool valid_merch_index(webstore_t *store, int id);

/*
Lists all existing carts with their ID in the store
*/
void list_all_cart_id(webstore_t *store); 

//
/* USEFUL FUNCTIONS FOR TESTS*/
//

/*
Gets the number of merches in the given cart
*/
size_t nr_of_merch_in_cart(cart_t *cart);

/*
Gets the amount of a given merch in a given cart
*/
int get_amount_of_merch_in_cart(cart_t *cart, char *merch_name); 

/*
Returns true or false depending if a merch exists in the given cart
*/
char *get_merch_name_in_cart(cart_t *cart, int nr_merch);
  
bool merch_in_cart(cart_t *cart, char *merch_name);
bool cart_is_empty(cart_t *cart);
size_t cart_db_size(cart_t *cart);

void cart_destroy(cart_t *cart); 

void destroy_all_carts(webstore_t *store); 

/*
Gets the cart with the given ID
*/
cart_t *get_cart(webstore_t *store, int id);

//
/*PROMPT FUNCTIONS*/
//

/*
Uses add_to_cart
Asks for the name and amount of a merch that the user wants to 
add to in a specific cart
*/
void add_to_cart_prompt(webstore_t *store, int id);
void add_to_active_cart_prompt(webstore_t *store);

/*
Uses remove_from_cart
Asks for name and amount of a merch that the user wants to remove or remove
a ceratin amount from in the cart
*/
void remove_from_cart_prompt(webstore_t *store); 
void remove_from_active_cart_prompt(webstore_t *store);

void sort_keys(entry_ht_t *keys, size_t no_keys);
#endif
