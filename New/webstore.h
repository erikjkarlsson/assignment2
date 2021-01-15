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

void parse_args(webstore_t *store, int argc, char **argv);

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

///@brief Creates a new merch given name, description, price and list of locations 
///@param a char name of the merchendise
///@param a char desciption of the merchendise
///@param a price of the merchendise
///@param a linked list of all the locations that the merch exists on, can be empty
///@returns a merchendise 
merch_t *create_merch(char *name, char *desc, size_t price, ioopm_list_t *locs);

///@breif Destroys all the merchendise in the Merchendise database
///@param the store where all the merchendise shoiuld be deleted
void destroy_all_merch(webstore_t *store);

///@breif Adds a merch to the Merch database
// When added the stock of the merch is 0
// An already existing merch can´t be added
///@param 
void add_merchendise(webstore_t *store, char *name, char *desc, size_t price);

///@breif Removes an existing merchendise completely from the store
///@param The store where the merchendise should be removed from
///@param The name of the merchendise that should be removed
void remove_merchendise(webstore_t *store, char *name);

///@breif Calculates the total stock of a merchendise
// and then updates the total_amount of the merch 
// reflecting that. If the amount was updated
// the function returns true.
///@param The store 
///@param The name of the merchendise that should be removed
bool sync_merch_stock(webstore_t *store, char *name);

///@breif Increases or decreases the stock at an existing merch
// The stock is also removed from the shelfs
// A negative amount decreases stock and a positive increases.  
///@param The store 
///@param The name of the merchendise in which the stock should be changed
///@param The amount (+) or (-) that should change the stock 
size_t change_stock_relative_amount(webstore_t *store, char *name, size_t amount);

///@breif Sets the given describtion to a specific merch given the name of the merch
///@param The store 
///@param The name of the merchendise in which the description should be changed 
///@param The new description
void set_merch_description(webstore_t *store, char *name, char *desc);

///@breif Sets the given price on the specified merch given the merch name
///@param The store 
///@param The name of the merchendise in which the price should be changed 
///@param The new price
void set_merch_price(webstore_t *store, char *name, size_t price);

/// HELP FUNCTIONS///

///@breif Frees up the list of all locations/shelfs a specific
// merch exsits on given it´s name.
///@param The store 
///@param The name of the merchendise in which the linked list of 
//locations should be destroyed
void destroy_locs(webstore_t *store, char *name);

///@breif Checks if a index ?????????? TODO
//in the merch database
///@param The store 
bool valid_index(webstore_t *store, int index);

/// @breif Used in combination with list_merch 
// checks after merchendise in the Merchdendise database 
// given an index
///@param The store 
///@param The index of the desired merchendise
///@returns the name of the merch at a specified
//index in the merch database
char *lookup_merch_name(webstore_t *store, int index); 

///@breif Ckecks if a merchendise given its name exists in the Merchendise database
///@param The store 
///@returns true if the merchendise is in the database and else, false
bool merch_in_stock(webstore_t *store, char *name);

///@breif Calculates total stock of a merch
///@param The store 
///@param The name of the merchendise
///@returns The total stock of a given merchendise
size_t merch_stock(webstore_t *store, char *name);

///@breif Gets the price of an existing merch in the store
///@param The store 
///@param The name of the merchendise
///@returns The price of the specified merch name
int merch_price(webstore_t *store, char *name);

///@breif Gets the description of a mercherdise
///@param The store 
///@param The name of the merchendise
///@returns The description of merchendise
char *merch_description(webstore_t *store, char *name);

///@breif Gets all the locations/shelfs that
// a specific merch is given it´s name
///@param The store 
///@param The name of the merchendise
///@returns A list of all the locations/shelfs that
// a specific merch is given it´s name
ioopm_list_t *merch_locs(webstore_t *store, char *name);

/// PRINT FUNCTIONS ///

///@breif Prints the name,description, price and stock of one merch
///@param The merchendise that should be printed
void print_merch(merch_t *merch);

///@breif Prints all the merchendise in the Merch database 
//with their name, describtion, price and stock
///@param The store 
void list_merchandise(webstore_t *store);

//////////////////////////////// Storage and Shelf ////////////////////////////////

/// @breif Creates a new shelf given a name and amount it can hold
///@param The name of the shelf
///@param The amount that the shelf should hold
///@returns The created shelf
shelf_t *create_shelf(char *shelf, size_t amount);

///@breif Destroys a given shelf compleatly
///@param The shelf
void destroy_shelf(shelf_t *shelf);

///@breif Deletes the storage database compleatly
///@param The store
void destroy_storage(webstore_t *store);

///@breif Adds a shelf to both the Mercendise database and the
//storage database. 
//If the shelf and merchendise on this shelf already exists, 
//the function updates amount of merch on this shelf.
///@param The stpre
///@param The name of the merchendise
///@param The name of the shelf 
///@param The amount of the merchendise that the shelf should hold
void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount);

///@breif Removes a shelf name from the storage database and 
//all merch names connected to it
///@param The store
///@param The name of the shelf
void remove_shelf(webstore_t *store, char *shelf);
  
///@breif Adds a shelf and a merch name connected to it into the 
//storage database. 
//If the shelf already exists the merch name is added to that shelf
//if the merch name aldready exists on the shelf, nothing happens 
///@param The store
///@param The name of the merchendise
///@param The name of the shelf
void add_to_storage(webstore_t *store, char *name, char *shelf);

///@breif Looks in the Merchendise database for the location/shelf
// if it exists its stock will be set to the given amount
// else it will be added with its stock set to amount
///@param The store
///@param The name of the merchendise
///@param The new amount of merchendise
///@param The name of the shelf
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* location);

///@param The store
///@param The name of the merchendise
///@param The new amount of merchendise
///@param The name of the shelf		     
size_t increase_stock(webstore_t *store, char *name,
		      char *shelf_name, size_t amount);

/// HELP FUNCTIONS ///


///@returns true or false depending if a merchendise is in the 
// list of a given shelf name in the storage database
// If the shelf doesn´t exist in the storage database, the function
// returns false
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
