#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef struct entry entry_t;
typedef struct bucket bucket_t;

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct bucket {
  entry_t *first;
};

struct hash_table
{
  ioopm_eq_function value_eq_function;
  ioopm_eq_function key_eq_function;
  ioopm_hash_function hash_function;
  bucket_t* buckets;
  size_t count;
  size_t num_buckets;
  float load_factor;
};

static int get_hash_for_key(const ioopm_hash_table_t *ht, elem_t key)
{
  int hash;
  if(ht->hash_function)
  {
    hash = ht->hash_function(key);
  }
  else
  {
    hash = key.i;
  }
  if (hash < 0)
  {
    hash = -hash;
  }
  return hash;
}

static bucket_t *get_bucket_for_hash(const ioopm_hash_table_t *ht, int key_hash)
{
  return &ht->buckets[key_hash % ht->num_buckets];
}

static void resize_and_rehash_all(ioopm_hash_table_t *ht, size_t new_num_buckets)
{
  ioopm_hash_table_t *new_ht = ioopm_hash_table_create_advanced(ht->load_factor, new_num_buckets, ht->value_eq_function, ht->key_eq_function, ht->hash_function);
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_t *entry = ht->buckets[i].first;
    while(entry)
    {
      ioopm_hash_table_insert(new_ht, entry->key, entry->value);
      entry = entry->next;
    }
  }
  // destroy old ht in a temporary memory location so the old pointer can still be used.
  ioopm_hash_table_t *temp_ht = calloc(1, sizeof(ioopm_hash_table_t));
  *temp_ht = *ht;
  ioopm_hash_table_destroy(temp_ht);
  *ht = *new_ht;
  free(new_ht);
}


size_t primes[] = {11, 17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};

static void resize_if_needed(ioopm_hash_table_t *ht, size_t new_count)
{
  if((size_t)(ht->load_factor*new_count) > ht->num_buckets)
  {
    for(int i = 0; i < 12; ++i)
    {
      size_t prime = primes[i];
      if(ht->num_buckets < prime)
      {
        resize_and_rehash_all(ht, prime);
        return;
      }
    }
    size_t new_num_buckets = ht->num_buckets*2;
    resize_and_rehash_all(ht, new_num_buckets);
  }
}

static entry_t* entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = calloc(1 ,sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;
  return entry;
}

static void entry_destroy(entry_t* entry) {
  entry_t* next = entry;
  while(next != NULL) {
    entry_t* next_next = next->next;
    free(next);
    next = next_next;
  }
}

static entry_t **find_entry_pointer_for_key(const ioopm_hash_table_t *ht, bucket_t *bucket, elem_t key)
{
  entry_t **current_entry = &bucket->first;
  while (*current_entry)
  {
    if(ht->key_eq_function((*current_entry)->key, key)) /// if current entry is past the key we are looking for, then we return previous entry
    {
      return current_entry;
    }
    else
    {
      current_entry = &(*current_entry)->next;
    }
  }
  return current_entry;
}

static entry_t *find_entry_for_key(const ioopm_hash_table_t *ht, bucket_t *bucket, elem_t key)
{
  entry_t *current_entry = bucket->first;
  while (current_entry)
  {
    if(ht->key_eq_function(current_entry->key, key)) /// if current entry is past the key we are looking for, then we return previous entry
    {
      return current_entry;
    }
    else
    {
      current_entry = current_entry->next;
    }
  }
  return current_entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function value_eq, ioopm_eq_function key_eq, ioopm_hash_function hash)
{
  return ioopm_hash_table_create_advanced(1.33f, 31, value_eq, key_eq, hash);
}

ioopm_hash_table_t *ioopm_hash_table_create_advanced(float load_factor, size_t num_buckets, ioopm_eq_function value_eq, ioopm_eq_function key_eq, ioopm_hash_function hash)
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to entry_t's
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->value_eq_function = value_eq;
  result->key_eq_function = key_eq;
  result->hash_function = hash;
  result->load_factor = load_factor;
  result->num_buckets = num_buckets;
  result->buckets = calloc(result->num_buckets, sizeof(bucket_t));
  /// Initialise the entry_t pointers to NULL
  for (int i = 0; i < result->num_buckets; ++i)
  {
    result->buckets[i].first = NULL;
  }
  return result;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_destroy(ht->buckets[i].first);
  }
  free(ht->buckets);
  free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
  resize_if_needed(ht, ht->count + 1);
  
  int key_hash = get_hash_for_key(ht, key);
  bucket_t *bucket = get_bucket_for_hash(ht, key_hash);
  entry_t **entry_ptr = find_entry_pointer_for_key(ht, bucket, key);
  if(*entry_ptr == NULL)
  {
    *entry_ptr = entry_create(key, value, *entry_ptr);
    ht->count += 1;
  }
  else
  {
    (*entry_ptr)->value = value;
  }
}

ioopm_option_t ioopm_hash_table_lookup(const ioopm_hash_table_t *ht, elem_t key)
{
  int key_hash = get_hash_for_key(ht, key);
  bucket_t *bucket = get_bucket_for_hash(ht, key_hash);
  /// Find the previous entry for key
  entry_t *entry = find_entry_for_key(ht, bucket, key);
  
  if(entry != NULL)
  {
    return ioopm_option_success(entry->value);
  }
  return ioopm_option_failure();
}

ioopm_option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  int key_hash = get_hash_for_key(ht, key);
  bucket_t *bucket = get_bucket_for_hash(ht, key_hash);
  entry_t **entry_ptr = find_entry_pointer_for_key(ht, bucket, key);
  if(*entry_ptr == NULL)
  {
    return ioopm_option_failure();
  }
  entry_t *removed = *entry_ptr;
  *entry_ptr = (*entry_ptr)->next;
  elem_t return_value = removed->value;
  free(removed);
  ht->count -= 1;
  return ioopm_option_success(return_value);
}


size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht)
{
  return ht->count;
}

bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht)
{
  return ht->count == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_destroy(ht->buckets[i].first);
    ht->buckets[i].first = NULL;
  }
  ht->count = 0;
}

ioopm_list_t *ioopm_hash_table_keys(const ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ht->key_eq_function);
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_t *entry = ht->buckets[i].first;
    while(entry)
    {
      ioopm_linked_list_append(list, entry->key);
      entry = entry->next;
    }
  }
  return list;
}

ioopm_list_t *ioopm_hash_table_values(const ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ht->value_eq_function);
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_t *entry = ht->buckets[i].first;
    while(entry)
    {
      ioopm_linked_list_append(list, entry->value);
      entry = entry->next;
    }
  }
  return list;
}

struct eq_payload {
  const ioopm_hash_table_t *ht;
  elem_t elem;
};

bool key_equiv(elem_t key, elem_t value, struct eq_payload* pl)
{
  return pl->ht->key_eq_function(key, pl->elem);
}
bool value_equiv(elem_t key, elem_t value, struct eq_payload* pl)
{
  return pl->ht->value_eq_function(value, pl->elem);
}

bool ioopm_hash_table_has_key(const ioopm_hash_table_t *ht, elem_t key)
{
  struct eq_payload pl = { ht, key };
  return ioopm_hash_table_any(ht, (ioopm_predicate)key_equiv, &pl);
}


bool ioopm_hash_table_has_value(const ioopm_hash_table_t *ht, elem_t value)
{
  struct eq_payload pl = { ht, value };
  return ioopm_hash_table_any(ht, (ioopm_predicate)value_equiv, &pl);
}

bool ioopm_hash_table_all(const ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_t *entry = ht->buckets[i].first;
    while(entry)
    {
      if(pred(entry->key, entry->value, arg) == false)
      {
        return false;
      }
      entry = entry->next;
    }
  }
  return true;
}

bool ioopm_hash_table_any(const ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_t *entry = ht->buckets[i].first;
    while(entry)
    {
      if(pred(entry->key, entry->value, arg) == true)
      {
        return true;
      }
      entry = entry->next;
    }
  }
  return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{
  for (int i = 0; i < ht->num_buckets; ++i)
  {
    entry_t *entry = ht->buckets[i].first;
    while(entry)
    {
      apply_fun(entry->key, &entry->value, arg);
      entry = entry->next;
    }
  }
}