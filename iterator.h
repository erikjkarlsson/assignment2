#pragma once
#include <stdbool.h>
#include "common_2.h"
typedef struct list ioopm_list_t;
typedef struct link ioopm_link_t;/// Meta: struct definition goes in C file
//typedef bool(*ioopm_char_predicate)(int value, void *extra);
//typedef void(*ioopm_apply_char_function)(int *value, void *extra);

struct iterator
{
  ioopm_link_t *current;
  ioopm_list_t *list;
};
/// An element
//typedef int elem_t;
/// A link in the linked structure 

struct link 
{
  elem_t element;
  ioopm_link_t *next;
};

struct list 
{
    ioopm_link_t *first;
    ioopm_link_t *last;
    int size;
};
typedef struct iterator ioopm_list_iterator_t;
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);

/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one ste
/// @param iter the iterator
/// @return the next element
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter);

/// NOTE: REMOVE IS OPTIONAL TO IMPLEMENT 
/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter);

/// NOTE: INSERT IS OPTIONAL TO IMPLEMENT 
/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @return the current element
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);
