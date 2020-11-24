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
  ioopm_hash_table_t *merch_db;
  ioopm_hash_table_t *storage_db;
};
typedef struct webstore webstore_t;
typedef struct database database_t;


//This adds a new merch to the warehouse with a name (string), description (string), and price (integer).
//A newly added merch is not in stock.
//Adding a new merch with the same name as an existing merch is not allowed.
void add_merchendise(webstore_t *store, char *name, char *desc, size_t price);

void remove_merchendise(webstore_t *store);

void list_merchandise(webstore_t *store);

webstore_t *store_create();

void store_destroy(webstore_t *store);

void print_merch(merch_t *merch);

shelf_t *create_shelf(char *shelf, int amount);

merch_t *create_merch(char *name,
		      char* desc,
		      size_t price,
		      ioopm_list_t *locs);

void create_destroy(webstore_t *store);
/*
//This should list all items in the store.
//Items should preferably (soft requirement) be printed in alphabetical order on their names.
//Because there may be more things in the database than might fit on a screen,
//items should be printed 20 at a time, and the user is asked to continue listing (if possible)
//or return to the main menu.
void list_merchandise();

//Removes an item completely from the warehouse, including all its stock.
void  remove_merchandise();

// Create merch
//Allows changing the name, description and price of a merch. Note that this does not affect its stock.
//Changing the name of a merch to the name of an existing merch is not allowed.
//Note that changing the name may mean changing the key unless you use a unique id for each merch.
void edit_merchandise();

//List all the storage locations for a particular merch,
//along with the quantities stored on each location.
//Storage locations should preferably be listed in alphabetical order (e.g., A20 before B01 and C01 before C10).
//Names of storage locations follow this format always: one capital letter (A-Z) followed by two digits (0-9).
void show_stock();

//Increases the stock of a merch by at least one.
//You can replenish on an existing storage location or a new one.
//The stock for a merch is the sum of all items on all storage locations holding that merch.
//A storage location stocks items of one (type of) merch, never more.
//For simplicity, there is no limit to the amount of storage locations nor is there a limit on the number of items a location can hold.
void replenish();

//Creates a new shopping cart in the system which is empty.
//A shopping cart represents a possible order.
//Adding/removing merch to/from
//a cart does not change the stock for that merch – stocks are changed only during checkout.
//Shopping carts are identified by a monotonically increasing number,
//i.e., the number of the i’th shopping cart created is i, regardless of how many shopping carts have been removed.
void create_cart();

//Removes a shopping cart from the system.
void remove_cart();

// Adds some quantity of a merch to a specific shopping cart.
//All possible orders in the system must be fullfillable.
//For example, we may only have one or more carts with 12 items of a merge M
//if the total stock of M in the system at least 12.
//Thus, if all users go to checkout at the same time, they should all succeed.
void add_to_cart();

//Removes zero or more items of some merch from a particular cart.
void remove_from_cart();

//Calculate the cost of a shopping cart.
//If a cart holds 2 items of a merch M1 with a price of 50 and 8 items of a merch M2
//with a price of 3 the cost of the cart is 2×50+8×3=124.
void calculate_cost();

//This action represent the user going through with a purchase of all the items in a particular shopping cart.
//Decrease the stock for the merches in the cart.
//Remove the shopping cart from the system.
void checkout();

//Undos an action.
//Multiple undos should be supported (i.e., pressing undo N times undos the N last actions for N≤16.)
//You cannot “undo an undo” (aka “redo”).
void undo();

//Quits the program.
void Quit();

*/
#endif
