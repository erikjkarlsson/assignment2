#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iterator.h"
#include "assert.h"
/// @brief Create an iterator for a given list
/// @param the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list){
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
  link->next    = next;
  
  return link;
}



bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter){
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
  errno = 0;
  return value;
  


}

/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter){
  ioopm_link_t *replacement   = (iter->current)->next;
  ioopm_link_t *after         = (iter->current)->next->next;    
  elem_t removed_element      = (iter->current)->element;
  
  // Need to move up to the element before current.
  ioopm_link_t *before = (iter->list)->first;
  
  do {
    before = before->next;
    
  } while ((before->next != NULL) &&
	   (before->next != iter->current));
  
  free(iter->current);
  
  iter->current        = replacement;
  iter->current->next  = after;
  before->next         = iter->current;
  
  return removed_element;
}

/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator

/// @param element the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element){
  
  // Get the element before the current one
  ioopm_link_t *precurrent = iter->list->first;
  ioopm_link_t *current    = iter->current;
  printf("init\n");
  do {
    printf("Finding\n");
    precurrent = precurrent->next;
    
  } while ((precurrent->next != current) &&
	   (precurrent->next != NULL));
	   
  
  printf("Found\n");
  iter->current    = create_link_(element, current);
  printf("After\n");
  precurrent->next = iter->current;  
}


void ioopm_iterator_reset (ioopm_list_iterator_t *iter){
  // Reset the current element (as the first)
  iter->current = iter->list->first;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter){
  // Get the current element 
  return iter->current->element;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter){
    // Free up the iterator
    free(iter);
}
