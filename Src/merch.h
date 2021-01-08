#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "webstore.h"

#define NEW_ITEM(store, name, description, price)	\
  add_merchendise(store, name, description, (size_t)price);

#define SET_ITEM_LOC(store, name, shelf_name, amount)		\
  global_change_shelf(store, name, shelf_name, (size_t)amount); 


webstore_t *initialize_database();
