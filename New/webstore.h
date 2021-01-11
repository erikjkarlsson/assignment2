#ifndef __WEBS_H__
#define __WEBS_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"

#include "utils.h"
#include "common.h"

struct merch
{
  char *name;
  char *desc;
  size_t price;

  size_t total_amount;   // Total amount  in storage
  ioopm_list_t *locs;    // Storage of shelfs
};
typedef struct merch merch_t;


struct shelf
{
  char *shelf;
  size_t amount;
};

typedef struct shelf shelf_t;


struct webstore
{
  arg_opt_t *opt; // Logging library
  ioopm_hash_table_t *merch_db;
  ioopm_hash_table_t *storage_db;
  ioopm_list_t *all_shopping_carts;
  int active_cart;
};
typedef struct webstore webstore_t;
typedef struct database database_t;

typedef merch_t *(merch_modify_function)(merch_t *merch, void *extra);
typedef char *(merch_get_str_function)(merch_t *merch);
typedef int *(merch_get_int_function)(merch_t *merch);



#define get_elem_ptr(e) e.p

#define MLOG(store, fun, name, msg)		\
  if (store->opt->log_p)\
    merch_log(fun, name, msg, 0);

#define OMLOG(store, fun, name, msg,i)		\
  if (store->opt->log_p)\
    merch_log(fun, name, msg, i);

#define QLOG(store, fun, msg)		\
  if (store->opt->log_p)\
    merch_log(fun, "", msg, 0);


////////////////////////////////////// Store //////////////////////////////////////

//Deletes a store and destroyes everything in it
void store_destroy(webstore_t *store);

//Creates an empty store with empty databases
webstore_t *store_create();

////////////////////////////////////// Merch //////////////////////////////////////

//Creates a new merch given a name, description, price and list of locations 
merch_t *create_merch(char *name, char *desc, size_t price, ioopm_list_t *locs);

//Destroys all the merch in the Merch database
void destroy_all_merch(webstore_t *store);

//Adds a merch to the Merch database
//When added the stock of the merch is 0
//An already existing merch can´t be added
void add_merchendise(webstore_t *store, char *name, char *desc, size_t price);

//Removes a merch completely from the store
void remove_merchendise(webstore_t *store, char *name);

// Frees up the list of all locations/shelfs a specific
//merch exsits on given it´s name
void destroy_locs(webstore_t *store, char *name);

// Calculates the total stock of a merch item from
// the locs list (Storage) and then updates the total_amount
// of the merch reflecting that. If the amount was updated
// the function returns true.
bool sync_merch_stock(webstore_t *store, char *name);

//Increases or decreases the stock at an existing merch
//A negative (amount) decreases stock, positive increases.  
size_t change_stock_relative_amount(webstore_t *store, char *name, size_t amount);

/// HELP FUNCTIONS///

//Return true if index is valid meaning 
//less or equal to the amount of merch 
//in the merch database
bool valid_index(webstore_t *store, int index);

//Used in combination with list_merch 
//Returns the name of the merch at a specified
//index in the merch database
char *lookup_merch_name(webstore_t *store, int index); 

//If the merch given its name exists in the Merch database
//the function returns true and else, false
bool merch_in_stock(webstore_t *store, char *name);

// Calculates and returns the total amount/stock of a
// merch
size_t merch_stock(webstore_t *store, char *name);

// Returns the price of the specified merch name
int merch_price(webstore_t *store, char *name);

//Sets the given price on the specified merch given the merch name
void set_merch_price(webstore_t *store, char *name, size_t price);

// Return the description of merch item
char *merch_description(webstore_t *store, char *name);

//Sets the given describtion to a specific merch given the name of the merch
void set_merch_description(webstore_t *store, char *name, char *desc);

//Returns a list of all the locations/shelfs that
//a specific merch is given it´s name
ioopm_list_t *merch_locs(webstore_t *store, char *name);

/// PRINT FUNCTIONS ///

//Prints the name,description, price and stock of one merch
void print_merch(merch_t *merch);

//Prints all the merchendise in the Merch database 
//with their name, describtion, price and stock
void list_merchandise(webstore_t *store);

//////////////////////////////// Storage and Shelf ////////////////////////////////

//Creates a new shelf given a name and amount it can hold
//and returns it
shelf_t *create_shelf(char *shelf, size_t amount);

//Destroys a given shelf compleatly
void destroy_shelf(shelf_t *shelf);

//Deletes the storage database compleatly
void destroy_storage(webstore_t *store);

//Adds a shelf and a merch name connected to it into the 
//storage database. 
//If the shelf already exists the merch name is added to that shelf
//if the merch name aldready exists on the shelf, happens nothing 
void add_to_storage(webstore_t *store, char *name, char *shelf);


//Removes a shelf name from the storage database and 
//all merch names connected to it
void remove_shelf(webstore_t *store, char *shelf);


//Adds shelf to both the merch database and the
//storage database. 
//If the shelf and merch on this shelf already exists, 
//the function updates amount of merch on this shelf.
void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount);

// Looks in the Merch database for the location (shelf)
// if it exists its stock will be set to the given amount
// else it will be added with its stock set to amount
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* location);
		     
size_t increase_stock(webstore_t *store, char *name,
		      char *shelf_name, size_t amount);

/// HELP FUNCTIONS ///


//Returns true or false depending if a certian merch name is in the 
//list of a given shelf name in the storage database
//If the shelf doesn´t exist in the storage database, the function
//returns false
bool storage_contains(webstore_t *store, char *shelf, char *name);

//Returns the asscosiative list to a shelf in the storage database
ioopm_list_t *get_locations(webstore_t *store, char *shelf);

//Returns the amount merch at a specific shelf
int merch_stock_on_shelf(webstore_t *store, char *name, char *shelf);


//Used with show_stock
//Gets a merch name given the index of the merch listed in 
//show_stock
char *get_merch_name_in_storage(webstore_t *store, int nr_merch);

//Gets a shelf name given the index of the shelf_nr listed
//and merch name
char *get_shelf_after_shelf_nr(webstore_t *store, int shelf_nr, char *name);

/// PRINT FUNCTIONS ///

//Prints a list of all shelfs containing 
//a specific merch
void list_shelfs(webstore_t *store, char *name);

//Prints all the merches on the given shelf
void display_shelf(webstore_t *store, char *shelf);

// Prints all shelfs, together with all merchendise metadata.
void show_stock(webstore_t *store);





void rename_merch(webstore_t *store, char *name, char *new_name); //används ej

void remove_name_from_shelf(webstore_t *store,
			    char *shelf, char *name); //används ej


#endif
