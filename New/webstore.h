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



/////// Merch
bool valid_index(webstore_t *store, int index);
char *lookup_merch_name(webstore_t *store, int index); 

void print_merch(merch_t *merch);

void add_to_storage(webstore_t *store, char *name, char *shelf);
void destroy_storage(webstore_t *store);

void remove_shelf(webstore_t *store, char *shelf);
ioopm_list_t *get_locations(webstore_t *store, char *shelf);
bool storage_contains(webstore_t *store, char *shelf, char *name);

void list_shelfs(webstore_t *store, char *name);
void display_shelf(webstore_t *store, char *shelf);

void set_shelf(webstore_t *store, char *name,
	       char *shelf, size_t amount);

void store_destroy(webstore_t *store);
webstore_t *store_create();

void list_merchandise(webstore_t *store);

bool merch_in_stock(webstore_t *store, char *name);
int merch_stock_on_shelf(webstore_t *store, char *name, char *shelf);
bool sync_merch_stock(webstore_t *store, char *name);
size_t increase_stock(webstore_t *store, char *name,
		      char *shelf_name, size_t amount);

void show_stock(webstore_t *store);
void rename_merch(webstore_t *store, char *name, char *new_name);

size_t merch_stock(webstore_t *store, char *name);
void set_merch_stock(webstore_t *store, char *name,
		     size_t amount, char* location);

void remove_name_from_shelf(webstore_t *store,
			    char *shelf, char *name);
int merch_price(webstore_t *store, char *name);
void set_merch_price(webstore_t *store, char *name, size_t price);

char *merch_description(webstore_t *store, char *name);
void set_merch_description(webstore_t *store, char *name, char *desc);

ioopm_list_t *merch_locs(webstore_t *store, char *name);
void destroy_locs(webstore_t *store, char *name);


void add_merchendise(webstore_t *store, char *name, char *desc, size_t price);
void destroy_all_merch(webstore_t *store);
void remove_merchendise(webstore_t *store, char *name);
merch_t *create_merch(char *name, char *desc, size_t price, ioopm_list_t *locs);

shelf_t *create_shelf(char *shelf, size_t amount);
void destroy_shelf(shelf_t *shelf);

bool continue_printing();
bool valid_index(webstore_t *store, int index);

char *get_merch_name_in_storage(webstore_t *store, int nr_merch);


//void locs_delete(ioopm_list_t *locs);
//void merch_delete(merch_t *merch_data);

//This should list all items in the store.
//Items should preferably (soft requirement) be printed in alphabetical order on their names.
//Because there may be more things in the database than might fit on a screen,
//items should be printed 20 at a time, and the user is asked to continue listing (if possible)
//or return to the main menu.
//void list_merchandise();

//Removes an item completely from the warehouse, including all its stock.
//void  remove_merchandise();

// Create merch
//Allows changing the name, description and price of a merch. Note that this does not affect its stock.
//Changing the name of a merch to the name of an existing merch is not allowed.
//Note that changing the name may mean changing the key unless you use a unique id for each merch.
//void edit_merchandise();

//List all the storage locations for a particular merch,
//along with the quantities stored on each location.
//Storage locations should preferably be listed in alphabetical order (e.g., A20 before B01 and C01 before C10).
//Names of storage locations follow this format always: one capital letter (A-Z) followed by two digits (0-9).
//void show_stock();

//Increases the stock of a merch by at least one.
//You can replenish on an existing storage location or a new one.
//The stock for a merch is the sum of all items on all storage locations holding that merch.
//A storage location stocks items of one (type of) merch, never more.
//For simplicity, there is no limit to the amount of storage locations nor is there a limit on the number of items a location can hold.
//void replenish();

#endif
