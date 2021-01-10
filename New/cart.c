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

///
/*HELP FUNCTIONS*/
///

bool valid_id(webstore_t *store, int id)
{ 
    if(id >= 0 && id < ioopm_linked_list_size(store->all_shopping_carts)){
        return true;
    }
    return false;
}

bool valid_merch_index(){ //TODO
    return false;
}

cart_t *get_cart(webstore_t *store, int id){
    return get_elem_ptr(ioopm_linked_list_get(store->all_shopping_carts, id));
}

int amount_of_merch_in_cart(cart_t *cart, char *merch_name){
    //checks if this merch is in the cart
    if(ioopm_hash_table_has_key(cart->merch_in_cart, str_elem(merch_name))){
        return get_elem_int(ioopm_hash_table_lookup(cart->merch_in_cart, str_elem(merch_name)));//the amount of this merch in the cart
    }
    
    //if there where none of this merch in the cart
    return 0;
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

size_t cart_size(cart_t *cart){
    return ioopm_hash_table_size(cart->merch_in_cart);
}

void list_all_cart_id(webstore_t *store){
    puts("--- Existing Cart ID:s ---"); 
    int size = ioopm_linked_list_size(store->all_shopping_carts); 
    for(int i = 0; i <size; i++){
        printf("CART ID: %d\n", i); 
    }
}

///
/*FUNCTIONS*/
///

cart_t *create_cart(webstore_t *store){
    cart_t *new_cart = calloc(1, sizeof(cart_t));
    
    //ht containing all merch in cart with string key and int elem 
    new_cart->merch_in_cart = ioopm_hash_table_create(string_knr_hash, eq_elem_string, eq_elem_int); 
    
    //get id of cart
    new_cart->id= ioopm_linked_list_size(store->all_shopping_carts);
    
    //add cart to the list of all shopping carts
    ioopm_linked_list_append(store->all_shopping_carts, ptr_elem(new_cart));
    
    printf("The id of your cart is %d\n", new_cart->id); //TODO: function or remove 
    
    return new_cart;
}

void remove_cart(webstore_t *store, int id){
    cart_t *cart_remove = get_cart(store,id);
    
    if(cart_remove == NULL){
        perror("REMOVE CART: There is no cart to remove.\n");
        return;
    }
    
    ioopm_linked_list_remove(store->all_shopping_carts, id);
    
    ioopm_hash_table_destroy(cart_remove->merch_in_cart);
    free(cart_remove);
    cart_remove=NULL;
}

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
}

void remove_from_cart(webstore_t *store, int id, char *merch_to_remove_name, int amount_to_remove){
    
    cart_t *current_cart = get_cart(store, id);
    
    if(!merch_in_cart(current_cart, merch_to_remove_name)){
        perror("REMOVE FROM CART: There is no such merch in the cart.\n");
        return; 
    }
    if(!valid_id(store,id)){
        perror("REMOVE FROM CART: The id of the cart is invalid.\n");
        return; 
    }
    
    //char *merch_to_remove_name = lookup_merch_name(store, nr_merch);
    
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
        ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(merch_to_remove_name), int_elem(amount_of_merch-amount_to_remove)); 
    }
}

int calculate_cost(webstore_t *store, int id){
    
    if(!valid_id(store,id)){
        perror("CALCULATE COST: The id of the cart is invalid.\n");
        return 0; 
    }
    
    cart_t *current_cart = get_cart(store, id);
    int total_price = 0;
    
    //if the cart is empty
    if(cart_is_empty(current_cart)){
        puts("empoty!"); 
        return total_price;
    }
    

    ioopm_list_t *names = ioopm_hash_table_keys(current_cart->merch_in_cart);
    ioopm_list_t *amounts = ioopm_hash_table_values(current_cart->merch_in_cart);
    
    int price; 
    char *current_name;
    int current_amount;
    
    if(cart_size(current_cart) == 1){
        price = merch_price(store, get_elem_str(ioopm_linked_list_get(names, 0)));
        current_amount = get_elem_int(ioopm_linked_list_get(amounts, 0));
        total_price = price * current_amount; 
    }
    
    else{
        size_t no_names = ioopm_linked_list_size(names);
        
        ioopm_list_iterator_t *iter_n = ioopm_list_iterator(names); 
        ioopm_list_iterator_t *iter_a = ioopm_list_iterator(amounts);
        
        //checks the price for each merch in the cart
        //and multiplies it with the amount
        for (int i = 0; i < no_names; i++) {
                current_name = get_elem_str(ioopm_iterator_current(iter_n));
                current_amount = get_elem_int(ioopm_iterator_current(iter_a));
                price = merch_price(store, current_name);
                total_price += (price*current_amount);
                
                if(ioopm_iterator_has_next(iter_n) && ioopm_iterator_has_next(iter_a)){
                    ioopm_iterator_next(iter_n); 
                    ioopm_iterator_next(iter_a);
                }

            }
        
        ioopm_iterator_destroy(iter_n);
        ioopm_iterator_destroy(iter_a);
    }
    
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);

    return total_price;
}

void checkout(webstore_t *store, int id){
    
}

void display_cart(cart_t *cart){ //id?

    if(cart_is_empty(cart)){
        printf("------ Cart No.%d ------\n", cart->id);
        puts("Is Empty!"); 
        return;
    }
    
    ioopm_list_t *names = ioopm_hash_table_keys(cart->merch_in_cart);
    ioopm_list_t *amounts = ioopm_hash_table_values(cart->merch_in_cart);
    size_t no_names = ioopm_linked_list_size(names);
      
    ioopm_list_iterator_t *iter_n = ioopm_list_iterator(names); 
    ioopm_list_iterator_t *iter_a = ioopm_list_iterator(amounts);
    
    entry_ht_t kv_array[no_names];
    
    elem_t current_name;
    elem_t current_amount;
    
    for (int i = 0; i < no_names; i++) {
        current_name = ioopm_iterator_current(iter_n);
        current_amount = ioopm_iterator_current(iter_a);
        entry_ht_t keyval = (entry_ht_t) {.key = (char *) current_name.p, .value = current_amount.i};
        kv_array[i] = keyval;
        if(ioopm_iterator_has_next(iter_n) && ioopm_iterator_has_next(iter_a)){
            ioopm_iterator_next(iter_n); 
            ioopm_iterator_next(iter_a);
        }
    }
    
    sort_keys(kv_array, no_names);
    printf("------ Cart No.%d ------\n", cart->id);
    
    for (int i = 0; i < no_names; ++i) {
        printf("No.%d | Name: %s, Amount: %d\n", (i+1), kv_array[i].key, kv_array[i].value);
    }
    
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);
    ioopm_iterator_destroy(iter_n);
    ioopm_iterator_destroy(iter_a);
        
}

char *get_merch_name_in_cart(cart_t *cart, int nr_merch){

    ioopm_list_t *names = ioopm_hash_table_keys(cart->merch_in_cart);
    ioopm_list_t *amounts = ioopm_hash_table_values(cart->merch_in_cart);
    
    size_t no_names = ioopm_linked_list_size(names);
      
    ioopm_list_iterator_t *iter_n = ioopm_list_iterator(names); 
    ioopm_list_iterator_t *iter_a = ioopm_list_iterator(amounts);
    
    entry_ht_t kv_array[no_names];
    
    elem_t current_name;
    elem_t current_amount;
    
    for (int i = 0; i < no_names; i++) {
        current_name = ioopm_iterator_current(iter_n);
        current_amount = ioopm_iterator_current(iter_a);
        entry_ht_t keyval = (entry_ht_t) {.key = (char *) current_name.p, .value = current_amount.i};
        kv_array[i] = keyval;
        if(ioopm_iterator_has_next(iter_n) && ioopm_iterator_has_next(iter_a)){
            ioopm_iterator_next(iter_n); 
            ioopm_iterator_next(iter_a);
        }
    }
    
    sort_keys(kv_array, no_names);
    
    for (int i = 0; i < no_names; ++i) {
        if((i+1) == nr_merch){
            return kv_array[i].key;
        }
    }
}

size_t nr_of_merch_in_cart(cart_t *cart){
    return ioopm_hash_table_size(cart->merch_in_cart);
}

int get_amount_of_merch_in_cart(cart_t *cart, char *merch_name){
    return get_elem_int(ioopm_hash_table_lookup(cart->merch_in_cart, str_elem(merch_name))); 
}

bool merch_in_cart(cart_t *cart, char *merch_name){
    return ioopm_hash_table_has_key(cart->merch_in_cart, str_elem(merch_name));
}

////
/* PROMT FUNCTIONS */
///

void add_to_cart_promt(webstore_t *store, int id){
    list_merchandise(store); 
    int nr_merch  = ask_question_int("Enter the number of the merch you would like to add to the cart: "); 
    char *merch_to_add_name = lookup_merch_name(store, nr_merch-1);
    int merch_amount = ask_question_int("Enter the amount of this merch that you would like to add to the cart: "); 
    add_to_cart(store, id, merch_to_add_name, merch_amount); 
}

void remove_from_cart_promt(webstore_t *store, int id){
    if(cart_is_empty(get_cart(store,id))){
        puts("The cart is empty! There is nothing to remove!"); 
        return;
    }else{
        display_cart(get_cart(store,id)); 
        int nr_merch  = ask_question_int("Enter the number of the merch you would like to remove from the cart: "); 
        char *merch_name = get_merch_name_in_cart(get_cart(store,id), nr_merch);
        int merch_amount = ask_question_int("Enter the amount of this merch that you would like to remove from the cart: "); 
        remove_from_cart(store, id, merch_name, merch_amount); 
    }
}

/*int main(int argc, char *argv[]) {
    webstore_t *store = store_create();
    arg_parse(argc, argv, store->opt);  
    // Add Merch    
    add_merchendise(store, "Cola", "from coca cola", (size_t)10);
    add_merchendise(store, "Chair", "Usable", (size_t)8);
    add_merchendise(store, "Bike", "A sports bike from Brazil", (size_t)4);
    add_merchendise(store, "Car", "A fast car", (size_t)2);
    add_merchendise(store, "Computer", "Gaming computer", (size_t)0);
    
    add_to_storage(store, "Cola", "A8");
    add_to_storage(store, "Car", "A1");
    add_to_storage(store, "Bike", "A1");
    
    list_merchandise(store);
    create_cart(store);
    //add_to_cart(store, 0, 1, 2);
    
    //display_cart(get_cart(store, 0));
    
    return 0; 
}*/
