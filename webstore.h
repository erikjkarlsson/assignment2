#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//This adds a new merch to the warehouse with a name (string), description (string), and price (integer).
//A newly added merch is not in stock.
//Adding a new merch with the same name as an existing merch is not allowed.
void add_merchandise();

//This should list all items in the store.

void list_merchandise();
