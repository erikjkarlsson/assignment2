#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "webstore.h"
#include "cart.h"

#define get_elem_ptr(e) e.p
#define get_elem_int(e) e.i
#define get_elem_str(e) e.c

#define CURRENT_ITER_STR(iter) \
  get_elem_str(ioopm_iterator_current(iter))

#define CURRENT_ITER_INT(iter) \
  get_elem_int(ioopm_iterator_current(iter))

#define CURRENT_ITER_PTR(iter) \
  get_elem_ptr(ioopm_iterator_current(iter))

#define ITER_HAS_NXT(iter) \
  ioopm_iterator_has_next(iter)

#define ITER_NXT(iter) \
  ioopm_iterator_next(iter)

#define ITER_INIT(iter)	\
  ioopm_list_iterator(iter)
#define ITER_DEST(iter)	\
  ioopm_iterator_destroy(iter)

#define GET_CART_FROM_ID(store, id)					\
  get_elem_ptr(ioopm_linked_list_get(store->all_shopping_carts, int_ptr(id)))

#define SAFESET(what, check, error)				\
  do { what; } while (!choice_prompt("Satisfied?"));		\
  if (!(check)) error; 

#define ID_BOUNDS_MAX 100000
///
/*HELP FUNCTIONS*/
///

#define CAPITAL_MAX 100000
#define CAPITAL_MIN 0

bool valid_sum(int size){
  return ((size < CAPITAL_MAX) && (size > CAPITAL_MIN));
}

bool valid_cart_id(webstore_t *store, int id){
  // Cart id
  if (id == store->active_cart) return true;
  if (id < 0){
    perror("valid_cart_id: Id is under 0.\n");
    return false;
  }
  else if (id >= ID_BOUNDS_MAX){
    perror("valid_cart_id: Id is over the max-bounds.\n");
    return false;
  }  
  ioopm_link_t *current = (store->all_shopping_carts)->first;

    
  cart_t *cart = NULL;
  
  do {
    cart = get_elem_ptr(current->element);
    
    if (cart->id == id)
      return true;

    current = current->next;
  } while (current);

  return false;  
}


cart_t *get_cart(webstore_t *store, int id){
  if (!store){
    perror("get_cart: Webstore is NULL.\n");
    return NULL;
  }
  
  else if (valid_cart_id(store, id))
    return get_elem_ptr(ioopm_linked_list_get(store->all_shopping_carts, id));
  else {
    perror("get_cart: Non-Valid Cart ID.\n");
    return NULL;
  }
}


int amount_of_merch_in_cart(cart_t *cart, char *merch_name){
    //checks if this merch is in the cart
    if(ioopm_hash_table_has_key(cart->merch_in_cart, str_elem(merch_name)))
        return get_elem_int(ioopm_hash_table_lookup(cart->merch_in_cart,
						    str_elem(merch_name)));
    else return 0;
}


static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}

void sort_keys(entry_ht_t *keys, size_t no_keys)
{
  qsort(keys, no_keys, sizeof(entry_ht_t), cmpstringp);
}


bool cart_is_empty(cart_t *cart){
    return ioopm_hash_table_is_empty(cart->merch_in_cart);
}

//size_t cart_db_size(cart_t *cart){
size_t cart_db_size(cart_t *cart){
    return ioopm_hash_table_size(cart->merch_in_cart);
}

void list_all_cart_id(webstore_t *store){
  puts("┏──╸ Existing Cart ID's ");
  ioopm_link_t *current = (store->all_shopping_carts)->first; 
  if (!current){
    printf("┃ No existing carts!\n");
    return;
  }
  
  do {
    cart_t *cart = get_elem_ptr(current->element);

    if (!cart){
      perror("list_all_cart_id: Deallocated Cart In Cart Database.\n");
      return;      
    }
    
    printf("┃ Cart Id.%d\n", cart->id);    
    current = current->next;
  } while (current);
}

///
/*FUNCTIONS*/
///

cart_t *create_cart(webstore_t *store){
  // Create a new cart
  if (!store->all_shopping_carts){
    perror("create_cart: Deallocated Cart Database (NULL).\n");
    return NULL;      
    
  }
  
  cart_t *new_cart = calloc(1, sizeof(cart_t));
    
    //ht containing all merch in cart with string key and int elem 
    new_cart->merch_in_cart =
      ioopm_hash_table_create(string_knr_hash,
			      eq_elem_string,
			      eq_elem_int); 
    
    //Set id of cart to the total existing amount 
      new_cart->id = ioopm_linked_list_size(store->all_shopping_carts);
      store->active_cart = new_cart->id;
    
    //Add cart to the list of all shopping carts
    //if its not the first time the cart is added, add!

      ioopm_linked_list_append(store->all_shopping_carts, ptr_elem(new_cart));

            
    return new_cart;
}
void cart_destroy(cart_t *cart){    
  // Free a cart
  ioopm_hash_table_destroy(cart->merch_in_cart);
  free(cart);        
}

bool remove_cart(webstore_t *store){
  if (store->active_cart > 0)   store->active_cart--;
  
  cart_t *cart = get_cart(store,store->active_cart);
    
  if(!cart){
    return false;
  }

  if (cart->merch_in_cart)
    ioopm_hash_table_destroy(cart->merch_in_cart);
  
  cart_destroy(cart);    
  store->active_cart--;
  if (valid_cart_id(store, store->active_cart))
    ioopm_linked_list_remove(store->all_shopping_carts, store->active_cart);
  else return false;
  return true;
}

void add_to_cart(webstore_t *store, char *name, int amount){
    
    
    if(!valid_cart_id(store, store->active_cart)){
        perror("add_to_cart: Invalid cart ID.\n");
        return; 
    }
    cart_t *current_cart = NULL;
    int total_stock      = merch_stock(store, name);

    // If active cart is not valid, return

      current_cart = get_cart(store, store->active_cart);

    int current          =
      get_elem_int(ioopm_hash_table_lookup(current_cart->merch_in_cart,
					   str_elem(name)));
			     
    // Avoid duplicate merch in db, update existing entry
      if (ioopm_hash_table_has_key(current_cart->merch_in_cart,
				   str_elem(name))){
	if (total_stock <= (current + amount)){
	  
	  // Could this cause mem error? strdup?
	  ioopm_hash_table_remove(current_cart->merch_in_cart,
				  str_elem(name));
			
	  ioopm_hash_table_insert(current_cart->merch_in_cart,
				  str_elem(name),
				  int_elem(total_stock));

	
	}else {
	
	  int existing_amount =
	    get_elem_int(ioopm_hash_table_lookup(current_cart->merch_in_cart,
						 str_elem(name)));
	  ioopm_hash_table_remove(current_cart->merch_in_cart,
				  str_elem(name));
			
	  ioopm_hash_table_insert(current_cart->merch_in_cart,
				  str_elem(name),
				  int_elem(existing_amount + amount));
	}
      } 

}
void remove_from_active_cart(webstore_t *store, char *merch_to_remove_name, int amount_to_remove){
  remove_from_cart(store, store->active_cart, merch_to_remove_name, amount_to_remove);
}


/*
void add_to_cart(webstore_t *store, int id, char *merch_to_add_name, int amount){
    
    if(amount <= 0){
        perror("ADD TO CART: The amount of merch added to the cart must be 0 or higher.\n");
        return;
    }
    
    if(!valid_id(store,id)){
        perror("ADD TO CART: The id of the cart is invalid.\n");
        return; 
    }
    
    cart_t *current_cart = get_cart(store, id);
    
    int amount_of_merch_in_cart_already =  amount_of_merch_in_cart(current_cart, merch_to_add_name);
    int amount_of_merch_in_store = merch_stock(store, merch_to_add_name);//the amount of this merch in the store
    
    //If the asked amount and the amount already in the cart exceeds the amount of merch in the store
    if(amount_of_merch_in_store-(amount_of_merch_in_cart_already+amount) < 0){
        perror("ADD TO CART: Try to get more merch than there is in the store.\n");
        return;
    }
    
    //add merch to cart
    if(amount_of_merch_in_cart_already > 0){
        ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(merch_to_add_name), int_elem(amount_of_merch_in_cart_already + amount));
    }
    else{
        ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(merch_to_add_name), int_elem(amount));
    }
}*/


void destroy_all_carts(webstore_t *store){
    
  do {
    printf("!\n");
  } while (remove_cart(store));
  
  
  ioopm_linked_list_destroy(store->all_shopping_carts);  
  
}

void remove_from_cart(webstore_t *store, int id, char *merch_to_remove_name, int amount_to_remove){
    
    cart_t *current_cart = get_cart(store, id);
    
    if(!merch_in_cart(current_cart, merch_to_remove_name)){
        perror("REMOVE FROM CART: There is no such merch in the cart.\n");
        return; 
    }
    if(!valid_cart_id(store,id)){
        perror("REMOVE FROM CART: The id of the cart is invalid.\n");
        return; 
    }
    
    //char *merch_to_remove_name = merch_at_storage_id(store, nr_merch);
    
    int amount_of_merch =  amount_of_merch_in_cart(current_cart, merch_to_remove_name);
    
    //Tries to remove more merch than there is
    if(amount_of_merch < amount_to_remove){
        perror("REMOVE FROM CART: Can´t reomve more merch than there is in the cart.\n");
        return;
    }
    
    //if the amount of merch in the cart is 0 then remove the merch, else just decrease the amount
    if(amount_of_merch == amount_to_remove){
        ioopm_hash_table_remove(current_cart->merch_in_cart, str_elem(merch_to_remove_name));;
        //printf("You have removed %s from cart with id %d", removed, current_cart->id);
    }else{
        ioopm_hash_table_insert(current_cart->merch_in_cart,
				str_elem(merch_to_remove_name),
				int_elem(amount_of_merch-amount_to_remove)); 
    }
}

int active_cart_cost(webstore_t *store){
  return calculate_cost(store, store->active_cart);
}

int calculate_cost(webstore_t *store, int id){
    
  if(!valid_cart_id(store,id)){
    perror("calculate_cost: The cart id is invalid.\n");
    return 0; 
  }
    
  cart_t *current_cart = get_cart(store, id);

  // Individual Merch Price
  int price ;
  // Stock amount
  int current_amount;

  int total_price      = 0;
    
  //if the cart is empty
  if(cart_is_empty(current_cart)){
    perror("calculate_cost: Cart contains nothing.\n"); 
    return total_price;
  }
    
  ioopm_list_t *names =
    ioopm_hash_table_keys(current_cart->merch_in_cart);
  ioopm_list_t *amounts =
    ioopm_hash_table_values(current_cart->merch_in_cart);
    
    
  if(cart_db_size(current_cart) == 1){
    price = merch_price(store, get_elem_str(ioopm_linked_list_get(names, 0)));
    current_amount = get_elem_int(ioopm_linked_list_get(amounts, 0));
    total_price = price * current_amount; 
  }
    
  else{

    size_t no_names = ioopm_linked_list_size(names);
        
    ioopm_list_iterator_t *iter_n = ITER_INIT(names); 
    ioopm_list_iterator_t *iter_a = ITER_INIT(amounts);
        
    //checks the price for each merch in the cart
    //and multiplies it with the amount
    for (int i = 0; i < no_names; i++) {      
      char *current_name;
      // Sum up all (of merch in cart)
      // prices * amount 
      // for the final price
      current_name   = CURRENT_ITER_STR(iter_n);
      current_amount = CURRENT_ITER_INT(iter_a);
      price = merch_price(store, current_name);
      total_price += (price*current_amount);

      // Continue iterating through merch in cart 
      if(ITER_HAS_NXT(iter_n) && ITER_HAS_NXT(iter_a)){
	ITER_NXT(iter_n); ITER_NXT(iter_a);
      }

    }
        
    ITER_DEST(iter_n);
    ITER_DEST(iter_a);
  }
    
  ioopm_linked_list_destroy(names);
  ioopm_linked_list_destroy(amounts);

  return total_price;
}

/*
char *shelf_with_most_stock(webstore_t *store, char *name){
    ioopm_list_t *locs = merch_locs(store, name);
    
    shelf_t *current_shelf = NULL;
    int current_amount = 0;
    int most_stock = 0; 
    shelf_t *shelf_max = NULL;  
    size_t no_locs = ioopm_linked_list_size(locs);
    
    if(no_locs == 1){
        
        shelf_max = ioopm_linked_list_get(locs, 0).p;

        
    }else{
            
        ioopm_list_iterator_t *iter = ioopm_list_iterator(locs); 
        
        for (size_t i = 0; i < no_locs; i++) {
            current_shelf = get_elem_ptr(ioopm_iterator_current(iter));
            current_amount = merch_stock_on_shelf(store, name, current_shelf->shelf);
            
            if(current_amount > most_stock){
                most_stock = current_amount; 
                shelf_max = current_shelf; 
            }
            if(ioopm_iterator_has_next(iter)){
                ioopm_iterator_next(iter); 
            }
        }
        ioopm_iterator_destroy(iter);
    }
    
    // This would remove the (inner) actual
    // locs database as its a pointer
    //    ioopm_linked_list_destroy(locs);    

    return shelf_max->shelf; 
}

void change_stock_in_webstore(webstore_t *store, char *current_name, int current_amount){
    char *location; 
    while(true){
        location = shelf_with_most_stock(store, current_name); 
        printf("location: %s\n", location); 
        printf("current_amount: %d\n", current_amount); 
        if(merch_stock_on_shelf(store, current_name, location) >= current_amount){
            printf("merch_stock_on_shelf(store, current_name, location): %d\n", merch_stock_on_shelf(store, current_name, location)); 
            int new_stock = (merch_stock_on_shelf(store, current_name, location)-current_amount); 
            printf("stock: %d\n", new_stock); 
            printf("location: %s\n", location); 
            set_merch_stock(store, current_name, new_stock, location);
            
            break; 
        }else{
            current_amount = current_amount - merch_stock_on_shelf(store, current_name, location);
            printf("merch_stock_on_shelf(store, current_name, location): %d\n", merch_stock_on_shelf(store, current_name, location)); 
            printf("current_amount after else: %d\n", current_amount); 
            set_merch_stock(store, current_name, 0, location);
        }
    }
}
*/
void checkout(webstore_t *store){
  int id = store->active_cart;
  cart_t *current_cart = get_cart(store, id);
    
  char *current_name;
  int current_amount;
  ioopm_list_t *names = ioopm_hash_table_keys(current_cart->merch_in_cart);
    
  if(cart_db_size(current_cart) == 1){
    current_name = get_elem_str(ioopm_linked_list_get(names, 0));
    current_amount = amount_of_merch_in_cart(current_cart, current_name);
    //    change_stock_in_webstore(store, current_name, current_amount);
    change_stock_relative_amount(store, current_name, current_amount);
  }
    
  else{
        
    size_t no_names = ioopm_linked_list_size(names);
        
    ioopm_list_iterator_t *iter_n = ioopm_list_iterator(names); 

    for (int i = 0; i < no_names; i++) {
      current_name = get_elem_str(ioopm_iterator_current(iter_n));
      current_amount = amount_of_merch_in_cart(current_cart, current_name);
      change_stock_relative_amount(store, current_name, current_amount);
      //      change_stock_in_webstore(store, current_name, current_amount); 
                

      if(ioopm_iterator_has_next(iter_n)){
	ioopm_iterator_remove(iter_n); 
      }
    }
        
    ioopm_iterator_destroy(iter_n);
  }
  ioopm_linked_list_destroy(names);
  remove_cart(store); 
}

/*  void checkout(webstore_t *store){
    ioopm_linked_list_destroy(names);
    remove_cart(store, current_cart->id); 
>>>>>>> 98ffbcb45dd1b57b7335755097b9fc621ec313fd
}
*/
/*
void checkout(webstore_t *store){
  cart_t *cart = get_cart(store, store->active_cart);


    ioopm_list_t *names   =
    ioopm_hash_table_keys(cart->merch_in_cart);

  ioopm_list_t *amounts =
    ioopm_hash_table_values(cart->merch_in_cart);

  ioopm_list_iterator_t *iter_n = ITER_INIT(names); 
    ioopm_list_iterator_t *iter_a = ITER_INIT(amounts);

    // Transfer all stock amounts & merch names
    // into an array for sorting    
    for (size_t i = 0; i < ioopm_linked_list_size(names); i++) {
      size_t current_amount = CURRENT_ITER_INT(iter_a);
      char *current_name    = CURRENT_ITER_STR(iter_n);
      
      // Create value for array
      change_stock_relative_amount(store, current_name, -current_amount);
      // Iterate through all merch names and
      // stock amounts
      if(ITER_HAS_NXT(iter_n) && ITER_HAS_NXT(iter_a)){
	ITER_NXT(iter_n); ITER_NXT(iter_a);
      }
    }
    
    
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);
    ITER_DEST(iter_n);
    ITER_DEST(iter_a);
  
}*/

void display_cart(cart_t *cart){ //id?

  if(cart_is_empty(cart)){
    puts("┏──╸ Cart is Empty."); 
    return;
  }

  printf("┏──╸ Cart Id.%d\n", cart->id);  
  
  ioopm_list_t *names   =
    ioopm_hash_table_keys(cart->merch_in_cart);

  
  if (!names){
    perror("display_cart: Merch Cart Database is Deallocated.\n");
    ioopm_linked_list_destroy(names);    
    return;
  } else if (!get_elem_str(names->first->element)){
    perror("display_cart: Cart Database is Empty.\n");
    ioopm_linked_list_destroy(names);    
    return;
  }

  ioopm_list_t *amounts =
    ioopm_hash_table_values(cart->merch_in_cart);

  // Should always be false
  if (!get_elem_ptr(amounts->first->element)){
    perror("display_cart: Merch Amount in Cart is Empty.\n");
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);    
    return;
  }
  
  size_t no_names = ioopm_linked_list_size(names);
  if (no_names == 0){
    perror("display_cart: Cart Database is Empty.\n");
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);
    return;    
  }
  ioopm_list_iterator_t *iter_n = ITER_INIT(names); 
  ioopm_list_iterator_t *iter_a = ITER_INIT(amounts);

  // Could this cause memory leak?
  entry_ht_t kv_array[no_names]; 
    
  // Transfer all stock amounts & merch names
  // into an array for sorting    
  for (size_t i = 0; i < no_names; i++) {
    int current_amount = CURRENT_ITER_INT(iter_a);;
    char *current_name = CURRENT_ITER_STR(iter_n);

    if (!current_name){
      perror("display_cart: Merch Name in Database is Deallocated.\n");
      ioopm_linked_list_destroy(names); 
      ioopm_linked_list_destroy(amounts);
      ITER_DEST(iter_n);
      ITER_DEST(iter_a);      
      return;      
    }
    // Create value for array
    entry_ht_t keyval = (entry_ht_t) {
      .key   = current_name,
      .value = current_amount
    };
	
    kv_array[i] = keyval;
    // Iterate through all merch names and
    // stock amounts
    if(ITER_HAS_NXT(iter_n) && ITER_HAS_NXT(iter_a)){
      ITER_NXT(iter_n); ITER_NXT(iter_a);
    }
  }
    
  sort_keys(kv_array, no_names);
    
  for (size_t i = 0; i < no_names; ++i) {
    printf("┏─╸Item No.%d\n",(int)i+1);
    printf("┃ Name: %s\n",   kv_array[i].key);
    printf("┃ Amount: %d\n", kv_array[i].value);
  }
    
  ioopm_linked_list_destroy(names);
  ioopm_linked_list_destroy(amounts);
  ITER_DEST(iter_n);
  ITER_DEST(iter_a);        
}

char *get_merch_name_in_cart(cart_t *cart, size_t nr_merch){
  // Return the merch item associated with nr_merch

  ioopm_list_t *names = ioopm_hash_table_keys(cart->merch_in_cart);
  if (!names){
    perror("get_merch_name_in_cart: Merch Cart Database is Deallocated.\n");
    ioopm_linked_list_destroy(names);    
    return NULL;
  } else if (!get_elem_str(names->first->element)){
    perror("get_merch_names_in_cart: Cart Database is Empty.\n");
    ioopm_linked_list_destroy(names);    
    return NULL;
  }

  size_t no_names = ioopm_linked_list_size(names);
      
  ioopm_list_iterator_t *iter_n = ITER_INIT(names);   
    
  for (size_t i = 0; i < no_names; i++) {

    if (nr_merch == i)
      return get_elem_str(ioopm_iterator_current(iter_n));
    if(ITER_HAS_NXT(iter_n)){
      ITER_NXT(iter_n); 
    }else break;
  }
    
  perror("get_merch_name_in_cart: The Merch Id is not in Cart.\n");
  return NULL;
}

size_t nr_of_merch_in_cart(cart_t *cart){
    return ioopm_hash_table_size(cart->merch_in_cart);
}
size_t cart_total_stock(cart_t *cart){
    return ioopm_hash_table_size(cart->merch_in_cart);
}
int cart_merch_stock(cart_t *cart, char *merch_name){
    return get_elem_int(ioopm_hash_table_lookup(cart->merch_in_cart, str_elem(merch_name))); 
}

int get_amount_of_merch_in_cart(cart_t *cart, char *merch_name){
    return get_elem_int(ioopm_hash_table_lookup(cart->merch_in_cart, str_elem(merch_name))); 
}
bool merch_in_cart(cart_t *cart, char *merch_name){
    return ioopm_hash_table_has_key(cart->merch_in_cart, str_elem(merch_name));
}
bool merch_in_active_cart(cart_t *cart, char *merch_name){
    return ioopm_hash_table_has_key(cart->merch_in_cart, str_elem(merch_name));
}

////
/* PROMPT FUNCTIONS */
///
void add_to_active_cart_prompt(webstore_t *store){
  add_to_cart_prompt(store, store->active_cart);
}


void add_to_cart_prompt(webstore_t *store, int id){
  int nr_merch = 0;

  if (!store){
    perror("add_to_cart_prompt: Webstore is deallocated (NULL).\n");
    return;
  }
 
  list_merchandise(store);
  printf("┏─╸ Add Merch Id to Cart\n");

  // Ask for merch id until valid if 
  do {
   nr_merch  = ask_question_int("┃ Merch Nr.");

   // If invalid merch id choice to exit
   if ((!valid_merch_id(store, nr_merch)) &&
       (!choice_prompt("Invalid Merch Id, Exit?"))) return;
  } while (valid_merch_id(store, nr_merch));

  char *merch_name = merch_at_storage_id(store, nr_merch);

  if (!merch_name){
    perror("add_to_cart_prompt: Merch Id not in Storage (Critical)\n");
    return;
  }
  
  printf("┏─╸Cart Nr.%d; Set Amount of %s \n",
	 (int)id, merch_name);

  size_t merch_amount = 0;

  // Set a correct amount, if incorrect return
do {
   merch_amount  = ask_question_int("┃ Amount: ");

   // Exit upon invalid Id or too large stock, exit if wanted.
   if ((!valid_sum(merch_amount)) &&
       (merch_stock(store, merch_name) <= merch_amount) &&
       (!choice_prompt("Amount not allowed, Exit?"))) return;
  } while (!valid_sum(merch_amount));

 
  add_to_cart(store, merch_name, merch_amount); 
}

void remove_from_cart_prompt(webstore_t *store){

  if (!store){
    perror("remove_from_cart_prompt: Webstore is Deallocated (NULL).\n");
    return;
  }

  int id = store->active_cart;

  printf("┏─╸Remove Amount of Cart Nr.%d\n",
	 (int)store->active_cart);
  
  if(cart_is_empty(get_cart(store,id))){
    puts("┃ The cart is empty! There is nothing to remove!"); 
    return;
    
  }else{
    int nr_merch     = 0;
    int merch_amount = 0;
    // Disp cart if answer is yes 

    if (!valid_cart_id(store, id)){	
      perror("remove_from_cart_prompt: Active Cart Id is Invalid (Critical) .\n");
      return;
    }
    
    display_cart(get_cart(store,id));
	
    do {
      nr_merch  = ask_question_int("┃ Merch Id: ");

      // Check if over zero before casting
      if ((nr_merch >= 0) &&
	  (valid_merch_id(store, (size_t)nr_merch))) break;
  } while (choice_prompt("Invalid Merch Id, Try Again?"));

    
    char *merch_name = get_merch_name_in_cart(get_cart(store,id), (size_t)nr_merch);

    if (!merch_name){
      perror("remove_from_cart_prompt: Merch Id Correct but not in Storage.\n");
      return;
    }
    /*    
    // Re-ask until one is pleased return on error
    do {
      merch_amount  = ask_question_int("┃ Amount: ");
     
      // Enough in stock?
      if ((merch_stock(store, merch_name) >= (size_t)merch_amount) &&
	  valid_sum(merch_amount)) break;
      
    } while (choice_prompt("Selected Amount not in Stock, Try Again?"));
    */

    remove_from_cart(store, id, merch_name, merch_amount); 
  }
}
