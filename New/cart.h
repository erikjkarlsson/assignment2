#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "webstore.h"

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

/*
Creates an emoty cart in the store 
The cart is assigned an ID
*/
cart_t *create_cart(webstore_t *store);

/*
Removes a cart compleatly from the store 
*/
void remove_cart(webstore_t *store, int id);

/*
Adds a merchname and an amount of that to the cart
This doesn´t affect the storage 
*/
void add_to_cart(webstore_t *store, char *merch_to_add_name, int amount);

/* 
Removes compleatly or removes a given amount of merch in the cart
*/
void remove_from_cart(webstore_t *store, int id, char *merch_to_remove_name, int amount_to_remove);

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
bool valid_id(webstore_t *store, int id);

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
bool merch_in_cart(cart_t *cart, char *merch_name);

/*
Gets the cart with the given ID
*/
cart_t *get_cart(webstore_t *store, int id);

//
/*PROMT FUNCTIONS*/
//

/*
Uses add_to_cart
Asks for the name and amount of a merch that the user wants to 
add to in a specific cart
*/
void add_to_cart_promt(webstore_t *store, int id); 

/*
Uses remove_from_cart
Asks for name and amount of a merch that the user wants to remove or remove
a ceratin amount from in the cart
*/
void remove_from_cart_promt(webstore_t *store, int id); 
