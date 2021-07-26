#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "common_2.h"
#include "linked_list_new.h"
/**
 * @file hash_table.h
 * @author Erik Karlsson & Robert Martinis
 * @date 15 September 2020
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */

// The number of "buckets" in the hash-table
#define Load_factor 0.75

/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create();

/// @brief Delete a hash table and free its memory
/// param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

//static entry_t *entry_create(int key, char *value, entry_t *previous_entry);
//static entry_t *find_previous_entry_for_key(entry_t **first_entry, int key);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key (FIXME: incomplete)
elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key (FIXME: incomplete)
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

///@brief view the current amount of entries in the hash table
///@param ht hash table to be operated upon
///@return sum of entries
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

///@brief display all keys in ht
///@param ht hash table to be operated upon
///@return linked list containing all keys in ht
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

///@brief display all values in ht
///@param ht hash table to be operated upon
///@return linked list containing all values in ht
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

///@brief checks if a key is in ht
///@param ht hash table to be operated upon
///@param key key to look up in ht
///@returns true if it's contained in ht, false otherwise
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

///@brief checks if a value is in ht
///@param ht hash table to be operated upon
///@param key key to look up in ht
///@returns true if it's contained in ht, false otherwise
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

///@brief checks if p satisfies any entry in ht
///@param ht hash table to be operated upon
///@param p function which asserts the predicate
///@param x extra argument to the predicate, if needed
///@returns true if p satisfies any entry, false otherwise
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate p, void *x);

///@brief checks if ht is empty
///@param ht hash table to be operated upon
///@returns true if empy, false otherwise
bool ioopm_hash_table_is_empty (ioopm_hash_table_t *ht);

///@brief checks if p satisfies all entries in ht
///@param ht hash table to be operated upon
///@param p function which asserts the predicate
///@param x extra argument to the predicate, if needed
///@returns true if p satisfies all, false otherwise
bool ioopm_hash_table_all (ioopm_hash_table_t *ht, ioopm_predicate p, void *x);
