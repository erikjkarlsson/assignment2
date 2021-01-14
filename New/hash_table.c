#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include <errno.h>
#include "list_linked.h"
#include "common.h"
/* * @file hash_table.c
 * @author write both your names here
 * @date 1 Sep 2019
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */


/*  Goals
  1. Create a new, empty hash table
  2. Insert a new key–value mapping in a hash table
  3. Lookup the value for a given key
  4. Remove a key–value mapping from a hash table
  5. Delete a hash table and free its used memory
*/
struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t **buckets;
  ioopm_hash_function hash_func;
  size_t size;
  ioopm_eq_function eq_func;
  ioopm_eq_function comp_key;
  ioopm_eq_function comp_value;
  size_t growth_stage;
  size_t capacity;
  double load_factor;
};


static size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};

static entry_t *entry_create(elem_t new_key, elem_t new_value, entry_t *new_next){
  entry_t *new_entry;
  new_entry        = calloc(1, sizeof(entry_t)); /// NEED TO FREE LATER [DONE]
  new_entry->key   = new_key;
  new_entry->value = new_value;
  new_entry->next  = new_next;

  return new_entry;
}


static entry_t **create_buckets(ioopm_hash_table_t *ht, size_t capacity) {
  entry_t **buckets = calloc(capacity, sizeof(entry_t*));

  for (size_t i = 0; i < capacity; i++){
    buckets[i] = entry_create(ptr_elem(NULL), ptr_elem(NULL), NULL);
  }

  return buckets;
}


static size_t new_capacity (ioopm_hash_table_t *ht) // gets the new capacity
{
  ht->growth_stage = ht->growth_stage+1; // indexing in a list of primes
  size_t capacity = primes[ht->growth_stage];
  return capacity;
}

ioopm_hash_table_t *ioopm_hash_table_create_dynamic(
  ioopm_hash_function hf,
  ioopm_eq_function comp_key,
  ioopm_eq_function comp_value,
  double load_factor, size_t capacity)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->size = 0; /// Contains the amount of entries
  ht->capacity = capacity; /// Contains the amount of buckets
  ht->comp_key = comp_key; /// Function for comparing keys
  ht->comp_value = comp_value; /// Function for compaing values
  ht->load_factor = load_factor; /// Factor for when to resize the hash table
  ht->growth_stage = 0; /// Amount of times the hash table has been resized, used as an index
                        /// in a prime array to choose the next capacity

  if (hf == NULL) /// If the user didn't supply a hash function |
  {//                                                           v
    ht->hash_func = extract_int_hash_key; /// Use the key as hash function
  }

  else
  {
    ht->hash_func = hf;
  }

  ht->buckets = create_buckets(ht, primes[ht->growth_stage]);
  return ht;

}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hf, ioopm_eq_function comp_key, ioopm_eq_function comp_value)
{
  return ioopm_hash_table_create_dynamic(hf, comp_key, comp_value, Load_factor, primes[0]);
}


/// Returns true if it's time to grow the hash table
static bool check_capacity (ioopm_hash_table_t *ht, float load_factor, size_t size, size_t capacity)
{
  return (load_factor * primes[ht->growth_stage]) < size;
}

void ioopm_hash_table_clear (ioopm_hash_table_t *ht)
{
  entry_t *iter;
  entry_t *next_entry;
  entry_t *tmp;
  for (int i = 0; i < ht->capacity; i++)
  {
    iter = ht->buckets[i];
    next_entry = iter->next;
    iter->next = NULL;

    while(next_entry)
    {
      tmp = next_entry->next;
      free(next_entry);
      next_entry=tmp;
      ht->size--;
    }
  }
}

/// Resizes the hash table when its size reaches a certain load factor
static void ioopm_hash_table_resize (ioopm_hash_table_t *ht)
{
  ht->size = 0;
  entry_t **previous_buckets = ht->buckets; /// Save the old buckets for later insertion
  size_t previous_capacity = ht->capacity;
  ht->capacity = new_capacity(ht); /// Choose next capacity from prime array

  entry_t *entry = NULL;
  entry_t *next = NULL;
  ht->buckets = create_buckets(ht, ht->capacity); /// Overwrites all current buckets with null-buckets, and expands hash table

  for (size_t i = 0; i < previous_capacity; i++)
  {
    entry = previous_buckets[i] ->next; /// Reinsert previous buckets

    while (entry)
    {
      next = entry->next;
      ioopm_hash_table_insert(ht, entry->key, entry->value); /// Reinsert previous entries
      free(entry);
      entry = next;
    }
    free(previous_buckets[i]);  /// Free temp holder for old values
  }
  free(previous_buckets); /// Free the temp holder for old buckets
}


 void destroy_single_bucket(ioopm_hash_table_t *ht){

  entry_t *iter = NULL;
  entry_t *swap = NULL;
for (int i = 0; i < ht->capacity; i++)
  {
    iter = ht->buckets[i];

  while (iter){
     /// Store the adress for the next entry
     swap = iter->next;
     /// Free the current entry
     free(iter);
     /// Swap back to the stored entry and cont.
     iter = swap;
    }}
  }
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht){

    ioopm_hash_table_clear(ht);
    destroy_single_bucket(ht);
  free(ht->buckets);
  free(ht);
}

 entry_t *find_previous_entry_for_key(ioopm_eq_function comp_key, entry_t *entry, elem_t key) {
  entry_t *current = entry;

  while (current->next && !comp_key(current->next->key, key)) {
    current = current->next;
  }

  if (current->next == NULL) {
    Error();
    return entry;
  }
  Successful();
  return current;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
  unsigned long hashed_key = ht->hash_func(key);
  unsigned long bucket = hashed_key % ht->capacity;

  entry_t *entry = find_previous_entry_for_key(ht->comp_key, ht->buckets[bucket], key);
  entry_t *hmm = entry->next;

  if (errno != EINVAL)
  {
    hmm->value = value;
  }

  else
  {
    entry->next = entry_create(key, value, hmm);
    ht->size++;
  }

  if (check_capacity(ht, Load_factor,ht->size,ht->capacity))
  {
    ioopm_hash_table_resize(ht);
  }

Successful();
}


elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key) {
  unsigned long hashed_key = ht->hash_func(key);
  unsigned long bucket = hashed_key % ht->capacity;

  entry_t *tmp = find_previous_entry_for_key(ht->comp_key, ht->buckets[bucket], key);
  entry_t *next = tmp->next;

  if (errno != EINVAL)
  {
    return next->value;
  }

  return ptr_elem(NULL);
}

bool ioopm_hash_table_increase(ioopm_hash_table_t *ht, elem_t key) {
  unsigned long hashed_key = ht->hash_func(key);
  unsigned long bucket = hashed_key % ht->capacity;

  entry_t *tmp = find_previous_entry_for_key(ht->comp_key, ht->buckets[bucket], key);
  entry_t *next = tmp->next;

  if (errno != EINVAL)
  {
    next->value.i = next->value.i + 1;
    return true;
  }
  else{
    return false;
  }
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  unsigned long hashed_key = ht->hash_func(key);
  unsigned long bucket = hashed_key % ht->capacity;
  entry_t *entry = NULL;
  entry = ht->buckets[bucket];

  if (entry->next)
  {
    entry_t *prev_entry = find_previous_entry_for_key (ht->comp_key, entry, key);
    entry_t *current = prev_entry->next;

    if (errno != EINVAL) /// If previous entry was found
    {   Successful();
        ht->size = ht->size - 1;
        prev_entry->next = current->next;
        elem_t removed_value = current->value;
        free(current);
        return removed_value;
    }
  }

  Error();  /// Key was not found in the hash-table
  return ptr_elem(NULL);
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}


ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *keys = ioopm_linked_list_create(ht->comp_key);
  int index = 0;
  for(size_t i = 0; i < ht->capacity; i++)
  {
    entry_t *entry = ht->buckets[i];
    while(entry->next!=NULL)
    {
      ioopm_linked_list_append(keys, entry->next->key);
      entry = entry->next;
      index++;
    }
  }

return keys;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ht->size == 0;
}


ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ht->comp_key);
  int index = 0;
  for (size_t i = 0; i < ht->capacity; i++)
  {
    entry_t *entry = ht->buckets[i];
    while(entry->next)
    {
      ioopm_linked_list_append(list, entry->next->value);
      entry = entry->next;
      index++;
    }
  }
return list;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate p, void *x)
  {
    entry_t *entry = NULL;
    bool result = false;
    for (size_t i = 0; i < ht->capacity && result == false; i++)
      {
        entry = ht->buckets[i];
        while(entry && !result)
          {
            result = p(entry->key, entry->value, x);
            entry = entry->next;
          }
      }
    return result;
  }

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate p, void *x)
{
  entry_t *entry = NULL;
  bool result = false;
  for (size_t i = 0; i < ht->capacity; i++)
  {
    entry = ht->buckets[i]->next;
    while(entry && !result)
    {
      result = p(entry->key, entry->value, x);
      if (result == false)
      {
        return false;
      }
      entry=entry->next;
    }
  }
return true;
}


void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function f, void *x)
{
  entry_t *entry = NULL;
  for (size_t i = 0; i < ht->capacity; i++)
  {
    entry = ht->buckets[i]->next;
    while (entry)
    {
      f(entry->key, &entry->value, x);
      entry=entry->next;
    }
  }
}


bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
  {
    return (ioopm_hash_table_any(ht,key_equiv,&key));
  }


bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return (ioopm_hash_table_any(ht,value_equiv, &value));
}


size_t ioopm_return_capacity(ioopm_hash_table_t *ht)
{
    return ht->capacity;
}
