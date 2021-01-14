#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "common.h"
#include "list_linked.h"

struct link 
{
  elem_t element;
  ioopm_link_t *next;
};

struct list 
{
    ioopm_link_t *first;
    ioopm_link_t *last;
    size_t size;
    ioopm_eq_function func;
};

  
//// ioopm_catch_error(element)
// Pass through a value, if it is
// marked as an error, throw the
// error message and vexit.


bool ioopm_elem_cmp(elem_t fst, elem_t snd){
  return ((fst.i == snd.i) ||
	  (fst.u == snd.u) ||
	  (fst.b == snd.b) ||
	  (fst.f == snd.f) ||
	  (fst.p == snd.p));
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function func)
{
  /// Allocate Memory for a list and return the structure.
  ioopm_list_t *linked_list= calloc(1, sizeof(ioopm_list_t));
  linked_list->size = 0;
  linked_list->func = func;
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
}

void ioopm_linked_list_destroy(ioopm_list_t *list) // External
{
  if(list != NULL){
    /// Free the allocated memory of the linked list.
    //ioopm_clear_list(list);
    if(list->first == NULL){
      puts("NULL!!!!!!!!!");
    }
      while (list->first != NULL) {
        ioopm_link_t *tmp = list->first->next;
        free(list->first);
        list->first = NULL;
        list->first = tmp;
    }
  }
   
   free(list);
   list = NULL;
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

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value) // External
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



void ioopm_insert_at_index(ioopm_list_t *list, size_t index, elem_t value)
{

  ioopm_link_t *new_entry = create_link(value, NULL);
     
  //  insert->element = value;

  ioopm_link_t *cursor = list->first;
  
  if (list->first == NULL) {
    // First element, set list "flag-links"
    list->first = new_entry;
    list->last  = new_entry;
    
  }else if (index == 0){
    // If index is zero
    // just prepend    
    new_entry->next = list->first;
    list->first = new_entry;
    
  }else{

    for (size_t i = 1; i < index; ++i){
      // Move forward from first
      // entry
      cursor = cursor->next;
    }
    // Replace the entry "cursor"
    // with new_entry
    new_entry->next = cursor->next;

    // If new_entryed element is the
    // last, set last flag 
    if (cursor->next == NULL)
      list->last = new_entry;

    // Inject the entry inbetween
    // cursor and its linked next
    cursor->next = new_entry;

  }
  list->size++;
}
    


/* F2IXME: /!\  Needs better ERROR-Handeling /!\ 
 */
void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value)
  {
    ioopm_insert_at_index(list, index, value);
}

/*
 elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
 { 
   elem_t save;

   /// Only remove if in bounds
   if ((list->size != 0) || (index > list->size)){
     
     ioopm_link_t *after   = list->first;
     ioopm_link_t *before  = list->first;     

     /// Iterate up to the entry to be removed    
     for (size_t i = 1; i <= index; i++){
       before = after;
       after = after->next;
     };
     save = after->element;
     
     before->next = after->next;     

     free(after);

     list->size--;
              
     return save;
   }

   return (elem_t) {.error = true};
 }
*/

int list_inner_adjust_index(int index, int upper_bound)
{
    
    if(index > (upper_bound)){
        index = (upper_bound-1);
    }
    
    if(index < 0){
        index = 0;
    }
    
    return index;
}

void ioopm_linked_list_remove(ioopm_list_t *list, int index){ //return int (*)
    index = list_inner_adjust_index(index, ioopm_linked_list_size(list));
    ioopm_link_t *cursor = list->first; 
    if(index == 0){
      list->first = cursor->next;
      free(cursor);
      return;
      
    }
    /// Note index - 1 to stop on the preceeding node
    for (int i = 0; i < index - 1; ++i){
        cursor = cursor->next;
        
    }
    
    ioopm_link_t *to_remove = cursor->next;  /// Save pointer to link
    cursor->next = to_remove->next;    /// unlink link
    free(to_remove);     
    
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{  

  ioopm_link_t *cursor = list->first;
  elem_t elem = list->first->element;

  
  if (list->first == NULL) {
    /// Error when the index provided was out of bounds
    
    printf("NULL\n");
        
  }else if (index > list->size){
    // If index is zero
    printf("BOUNDS\n");
   
  }else{
    for (size_t i = 0; i  < index; i++){
      // Move forward from first entry
      cursor = cursor->next;
    }  
  
  if (cursor != NULL) {
    // Replace the element
    // with the iterated entries element    
    elem  = cursor->element;
    
  }else {
    elem = (elem_t) {.error = true};
    printf("NULL");
  }
  }

  return elem;
}
  
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t value)
{
  if (list->size == 0) false;
  
  ioopm_link_t *link = list->first;

  while (link != NULL)
    {
      /// Iterate through the list until a value
      /// equal to the searched is found (return True)
      /// if not, then (return False).
      
      if ((list->func(link->element, value)))
	/// A Match is found, hence list
	/// contains the value.
	return true;

      else{
	/// Iterate
	link = link->next;
      }

    }


    return false;
}

/*int ioopm_linked_list_size (ioopm_list_t *list)
{   int counter;
    ioopm_link_t *link = list->first;
    while (link !=NULL)
    {   link = link->next;
        counter++;
    }
    
    return counter;
}*/

//OR

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

    for (int i = 0; i < list->size; i++){
        prop(&index->element, extra);
        index = index->next;
    }
}
