#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

typedef struct merch merch_t;
struct merch
{
  ioopm_hash_table_t name; //name of item as key and information of item i as values
  ioopm_hash_table_t locs;  //storage location names as keys and informatiom of items stored at that location as values
};
