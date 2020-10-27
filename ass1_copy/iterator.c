#include "iterator.h"
#include "assert.h"
#include "common.h"
#include "list_linked.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR                                                                  \
  (elem_t) { .error = true }
#define PINT_ELEM(E) printf("Elem: %d\n", E.i)
/// @brief Create an iterator for a given list
/// @param the list to be iterated over
/// @return an iteration positioned at the start of list

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list) {
  ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
  // Current List Element
  iter->current = list->first;
  // Full List
  iter->list = list;
  return iter;
}

ioopm_link_t *create_link_(elem_t value, ioopm_link_t *next) // Internal
{
  /// Allocate memory the size of the linked entry.
  ioopm_link_t *link = calloc(1, sizeof(ioopm_link_t));
  /// Set the values and the (following) linked entry.
  link->element = value;
  link->next = next;

  return link;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter) {
  // Can continue if there is a following element.
  return (iter->current->next != NULL);
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter){

  if((!ioopm_iterator_has_next(iter))) {
    errno = EINVAL;
    return ptr_elem(NULL);
  }
  // Save value
  elem_t value = (iter->current)->next->element;
  // Move forward
  iter->current = iter->current->next;
  Successful();
  return value;



}

 //////////////Föregående/////////////////
/*elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter) {

  // Save value
  elem_t value = (iter->current)->next->element;

  // Move forward
  iter->current = iter->current->next;

  return value;
}
*/
/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter) {

  // Get the element before the current one
  ioopm_link_t *replacer = iter->list->first;
  ioopm_link_t *next = iter->current->next;

  elem_t e = iter->current->element;

  if (iter->current == iter->list->first) {
    // If first entry is removed, then there is
    // no need to find the previous one.
    iter->list->first = next;
    free(iter->current);
    iter->current = next;
    iter->list->size--;

    return e;
  }

  while (replacer->next != iter->current) {
    // Move forward from first entry
    // towards the current.
    replacer = replacer->next;
  }

  if (iter->current == iter->list->last) {
    // If the last is removed, the new last
    // should be updated in the linked list
    // structure

    iter->list->last = replacer;
    free(iter->current);
    iter->current = iter->list->last;

  } else {
    // If not the last, or first then just
    // link the previous entry to the next,
    // and remove the current.

    replacer->next = iter->current->next;
    free(iter->current);
    iter->current = replacer->next;
  }

  iter->list->size--;
  return e;
}

/*
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
  // Get the element before the current one
  ioopm_link_t *replacer = iter->list->first;
  ioopm_link_t *next = NULL;
  elem_t e = iter->current->element;

  for (;;) {
    // Move forward from first entry to the current

    if (replacer->next == iter->current)
      break;
    if (replacer->next == NULL)
      break;
    replacer = replacer->next;
  }

  next = iter->current->next;

  if (iter->current == iter->list->last) {
    iter->list->last = replacer;
    next = NULL;
  } else if (iter->current == iter->list->first) {
    iter->list->first = replacer;
  }

  free(iter->current);
  replacer->next = next;
  iter->current = replacer;

  iter->list->size--;
  return e;
}
*/
/// @brief Insert a new element into the underlying list making the current
/// element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element) {

  // Get the element before the current one
  ioopm_link_t *precurrent = iter->list->first;
  ioopm_link_t *current = NULL;

  if (iter->list->size == 0) {

    iter->current = create_link_(element, NULL);
    iter->list->first = iter->current;
    iter->list->last = iter->current;

  } else if (precurrent->next == NULL) {

    // Link the previous items to
    // a new (last) element
    current = create_link_(element, iter->current);
    iter->list->first = current;

  } else {
    for (;;) {
      // Move forward from first entry to the current
      if (precurrent->next == iter->current)
        break;
      precurrent = precurrent->next;
    }

    // Link the previous items
    // with a new which in turn
    // is linked with the ones after
    current = create_link_(element, iter->current);
    precurrent->next = current;
  }

  iter->list->size++;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter) {
  // Reset the current element (as the first)
  iter->current = iter->list->first;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter) {
  // Get the current element
  return iter->current->element;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter) {
  // Free up the iterator
  free(iter);
}
