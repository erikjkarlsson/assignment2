#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "merch.h"
#include "webstore.h"
#include "cart2.h"


#define current_cart_id	store->active_cart

#define LANG 1

#define SWE(thing) if (LANG == 0) { thing; }
#define ENG(thing) if (LANG == 1) { thing; }

char unicode_edit_cart_menu(webstore_t *store);
 
