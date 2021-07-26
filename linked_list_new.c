#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list_new.h"
#include "common_2.h"

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function a, elem_t b )
{
  /// Allocate Memory for a list and return the structure.
  
  ioopm_list_t *linked_list= calloc(1, sizeof(ioopm_list_t));
  return linked_list;
}

void ioopm_clear_list(ioopm_list_t *list) // Internal
{
    ioopm_link_t *index = list->first;

    /// Iterate through whole List
    while(index != NULL)
    {
      /// Save next Linked Entry
      ioopm_link_t *next = index->next;
      /// Free Linked Entry
      free(index);
      /// Iterate
      index = next;
    }
    free(index);
}

void ioopm_linked_list_destroy(ioopm_list_t *list) // External
{
  /// Free the allocated memory of the linked list.
  ioopm_clear_list(list);
  free(list);
}

ioopm_link_t *create_link(elem_t value, ioopm_link_t *next) // Internal
{
  /// Allocate memory the size of the linked entry.
  ioopm_link_t *link = calloc(1, sizeof(ioopm_link_t));
  /// Set the values and the (following) linked entry.
  link->element = value;
  link->next    = next;


  return link;
}



void ioopm_linked_list_append(ioopm_list_t * list, elem_t value) // External
{ 
    ioopm_link_t *new_entry = create_link(value, NULL);

  if (list->size == 0){
      /// First and Last entry is the one appended
      list->first = new_entry;
      list->last = new_entry;
    }

    else{
      /// Get Last entry and append the new
      /// as the next entry to the last
      /// and update the last as the one inserted.
      ioopm_link_t *last_entry = list->last;
      last_entry->next = new_entry;
      list->last = new_entry;
    }   
    (list->size)++;
}

void ioopm_linked_list_prepend (ioopm_list_t *list, elem_t value){

    if (list->size == 0){
      /// If inserting into empty list,
      /// the first and last value becomes
      /// the new entry.

      ioopm_link_t *new_entry = create_link(value, NULL);
      list->first = new_entry;
      list->last  = new_entry;
    }
    else{
      /// If inserting into a non-empty list
      /// the new entry is linked to the first,
      /// not changing the last.

      ioopm_link_t *first_entry = list->first;
      ioopm_link_t *new_entry = create_link(value, first_entry);
      list->first = new_entry;
    }

    /// Update size
    (list->size)++;
}

int ioopm_insert_at_index(ioopm_list_t *list, size_t index, elem_t value)
{
    ioopm_link_t *elem = list->first;
    ioopm_link_t *prev = NULL;

    size_t i = index;

    /// When (i == 0) then elem should be
    /// the desired insertion point.
    while (i != 0){

      /// Invalid index
      if (elem == NULL){
	printf("ERROR: The provided index is invalid (Too Large).");//\n", index);
	/// Return Failure
	return -1;
      }

      /// Store previous entry
      prev = elem;
      elem = elem->next;
      i = i - 1;
    }

    /// Insert the new_entry before elem->next and prev-next.
    ioopm_link_t *new_entry = create_link(value, elem);
    prev->next = new_entry;

    /// Update size
    (list->size)++;

    /// Return Success
    return 0;
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
    if (index < 0 || index > list->size){
      /// Fail if index is too large or too small.
      printf("ERROR: Provided index");//%d", index);

      /// Print of what kind the error is, too small/large.
      if (index < 0) printf(" is too small.\n");
      else printf("is too large.\n");
    }

    else if (list->size == 0){
      /// If list is empty, append value.
      ioopm_linked_list_append(list, value);
    }

    else if (index == 0){
      /// If the insert point is at the beginning,
      /// then prepend the value.
      ioopm_linked_list_prepend(list,value);
    }

    else if (index == (list->size-1)) {
      /// If insertion point is at the end,
      /// append the value.
      ioopm_linked_list_append(list, value);
    }

    else {
      /// Inject a value at the position provided,
      /// (not at beginning, or at end)
      ioopm_insert_at_index(list, index, value);
      // NOTE: This function can fail at runtime.
      // add test for failure?
    }

    return;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
    ioopm_link_t *current = list->first;
    //    int listsize = ioopm_linked_list_size(list);

    /// Cannot remove entries in the list
    /// with index over the list size or
    /// under zero. [1]
    if (index > list->size || index < 0){
      /// Raise Error for (1)
      //      printf("ERROR: Provided index %d", index);
      return (elem_t) {.p=NULL};
    }

    else if (index == 0){
      /// Save value of removed entry
      elem_t value = current->element;
      /// The new first element is the next to
      /// the removed
      ioopm_link_t *new_first_elem = current->next;
      /// Free entry
      free(current);
      /// Update the first element pointer
      list->first = new_first_elem;
      /// Decrement list size by one
      (list->size)--;

      return value;
    }

    /// Iterate up to the entry to be removed.
    for (int i = 0; i < index - 1; i++){
      current = current->next;
    }
    /// Save next entry
    ioopm_link_t *next_link = current->next;
    /// Link the current entry to the entry after
    current->next           = next_link->next;
    /// Save the value
    elem_t value = next_link->element;
    /// Free memory
    free(next_link);

    return value;

}



elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
    ioopm_link_t *link = list->first;

    for (size_t i = 0; i <= index; i++)
    {
      if (i >= list->size){

	return (elem_t) {.p=NULL};
      }
      else{
	/// Iterate link
        link = link->next;
      }
    }
    /// Return the element of the entry.
    return link->element;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t value)
{
  ioopm_link_t *link = list->first;

  while (link != NULL)
    {
      /// Iterate through the list until a value
      /// equal to the searched is found (return True)
      /// if not, then (return False).

      if (elem_cmp((link->element), value))
      {
	      return true;
      }

      else
      {
	      link = link->next;
      }
    }

    return false;
}


size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return (ioopm_linked_list_size(list) == 0);
}


bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_char_predicate prop, void *extra)
{
  ioopm_link_t *index = list->first;
  //int listsize = ioopm_linked_list_size(list);

  /// Iterate though all linked entries of the list
  /// and (return true) iif all applications of function
  /// is true, and false if not.
  /// where function := prop
  for (int i = 0; i < list->size; i++)
    {
      if (prop(index->element, extra)){
	/// If (function(entry) == true) then continue
	/// to iterate through list
	index = index->next;
      }

      else{
       	/// If (function(entry) == false) then stop
	/// and return false
	return false;
      }
    }
  /// If all applications of function(list[i]) from 0 to (size of list)
  /// are equal to true, then return true
  return true;
}

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_char_predicate prop, void *extra)
{
  ioopm_link_t *index = list->first;
  ///int listsize = ioopm_linked_list_size(list);

  /// Iterate through the list and apply prop to each
  /// entry. If the application equals true immediatly
  /// return true. Else return false.
    for (int i = 0; i < list->size; i++)
      {
        if (prop(index->element, extra)){
	  /// If prop applied to the current entry is true,
	  /// return true.
	  return true;
        }

        else{
	  /// If prop applied on the current entry is false
	  /// continue to iterate.
	  index = index->next;
        }

      }

    /// If all applications of prop are false, then the function
    /// will return false.
    return false;
}


void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_char_function prop, void *extra)
{
    ioopm_link_t *index = list->first;
    // int listsize   = ioopm_linked_list_size(list);

    /// For all entries in list,
    /// apply the function prop
    for (int i = 0; i < list->size; i++){
        prop(&index->element, extra);
        index = index->next;
    }








}
