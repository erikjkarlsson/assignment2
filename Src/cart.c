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

#define get_elem_ptr(e) e.p
#define get_elem_int(e) e.i
#define get_elem_str(e) e.c

//used to place keys and values in one
struct entry_ht
{
  char *key; 
  int value;
};

typedef struct entry_ht entry_ht_t;


struct cart {
  int id; 
  ioopm_hash_table_t *merch_in_cart; 
}; 

typedef struct cart cart_t;

///
/*HELP FUNCTIONS*/
///

bool valid_id(){ //TODO
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

///
/*FUNCTIONS*/
///

void create_cart(webstore_t *store){
    cart_t *new_cart = calloc(1, sizeof(cart_t));
    
    //ht containing all merch in cart with string key and int elem 
    new_cart->merch_in_cart = ioopm_hash_table_create(string_knr_hash, eq_elem_string, eq_elem_int); 
    
    //get id of cart
    new_cart->id= ioopm_linked_list_size(store->all_shopping_carts);
    
    //add cart to the list of all shopping carts
    ioopm_linked_list_prepend(store->all_shopping_carts, ptr_elem(new_cart));
    
    printf("The id of your cart is %d\n", new_cart->id); //TODO: function or remove 
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

void add_to_cart(webstore_t *store, int id, int nr_merch, int amount){
    
    char *merch_to_add_name = lookup_merch_name(store, nr_merch);
    cart_t *current_cart = get_cart(store, id);
    
    int amount_of_merch_in_cart_already =  amount_of_merch_in_cart(current_cart, merch_to_add_name);
    int amount_of_merch_in_store = merch_locs_total(store, merch_to_add_name);//the amount of this merch in the store
    
    //If the asked amount and the amount already in the cart exceeds tha amount of merch in the store
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

void remove_from_cart(webstore_t *store, int id, int nr_merch, int amount_to_remove){
    char *merch_to_remove_name = lookup_merch_name(store, nr_merch);
    cart_t *current_cart = get_cart(store, id);
    
    int amount_of_merch =  amount_of_merch_in_cart(current_cart, merch_to_remove_name);
    
    //Tries to remove more merch than there is
    if(amount_of_merch < amount_to_remove){
        perror("REMOVE FROM CART: Can´t reomve more merch than there is in the cart.\n");
        return;
    }
    
    //if the amount of merch in the cart is 0 then remove the merch, else just decrease the amount
    if(amount_of_merch == amount_to_remove){
        char *removed = get_elem_str(ioopm_hash_table_remove(current_cart->merch_in_cart, str_elem(merch_to_remove_name)));
        printf("You have removed %s from cart with id %d", removed, current_cart->id);
    }else{
        ioopm_hash_table_insert(current_cart->merch_in_cart, str_elem(merch_to_remove_name), int_elem(amount_of_merch-amount_to_remove)); 
    }
}

int calculate_cost(webstore_t *store, int id){
    cart_t *current_cart = get_cart(store, id);
    int total_price = 0;
    
    //if the cart is empty
    if(cart_is_empty(current_cart)){
        return total_price;
    }
    
    ioopm_list_t *names = ioopm_hash_table_keys(current_cart->merch_in_cart);
    ioopm_list_t *amounts = ioopm_hash_table_values(current_cart->merch_in_cart);
    
    size_t no_names = ioopm_linked_list_size(names);
    
    ioopm_list_iterator_t *iter_n = ioopm_list_iterator(names); 
    ioopm_list_iterator_t *iter_a = ioopm_list_iterator(amounts);
    
    int price; 
    char *current_name;
    int current_amount;
    
    //checks the price for each merch in the cart
    //and multiplies it with the amount
    for (int i = 0; i < no_names; i++) {
        if(ioopm_iterator_has_next(iter_n) && ioopm_iterator_has_next(iter_a)){
            current_name = get_elem_str(ioopm_iterator_next(iter_n));
            current_amount = get_elem_int(ioopm_iterator_next(iter_a));
            
            price = merch_price(store, current_name);
            total_price += (price*current_amount);
        }
    }
    
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);
    ioopm_iterator_destroy(iter_n);
    ioopm_iterator_destroy(iter_a);
    
    return total_price;
}

void checkout(webstore_t *store, int id){
    
}

void display_cart(cart_t *cart){ //id?
    ioopm_list_t *names = ioopm_hash_table_keys(cart->merch_in_cart);
    ioopm_list_t *amounts = ioopm_hash_table_values(cart->merch_in_cart);
    size_t no_names = ioopm_linked_list_size(names);
      
    ioopm_list_iterator_t *iter_n = ioopm_list_iterator(names); 
    ioopm_list_iterator_t *iter_a = ioopm_list_iterator(amounts);
    
    entry_ht_t kv_array[no_names];
    
    elem_t current_name;
    elem_t current_amount; 
    for (int i = 0; i < no_names; i++) {
        if(ioopm_iterator_has_next(iter_n) && ioopm_iterator_has_next(iter_a)){
          current_name = ioopm_iterator_next(iter_n);
          current_amount = ioopm_iterator_next(iter_a);
          entry_ht_t keyval = (entry_ht_t) {.key = (char *) current_name.p, .value = current_amount.i};
          kv_array[i] = keyval;
        }
    }
    
    sort_keys(kv_array, no_names);
    
    printf("------ Cart No.%d ------", cart->id);
    for (int i = 0; i < no_names; ++i) {
        printf("| Name: %s, Amount: %d\n", kv_array[i].key, kv_array[i].value);
        free(kv_array[i].key);
    }
    
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(amounts);
    ioopm_iterator_destroy(iter_n);
    ioopm_iterator_destroy(iter_a);
        
}

////
/* PROMT FUNCTIONS */
///

void add_to_cart_promt(){
    //ask for cart
    //list
    //ask for number

}

void event_loop(webstore_t *store){
    char *command = ask_question_menu();
    
    if(*command == 'S' || *command == 's'){
        //skapa en ny kundvagn 
    }
    if(*command == 'L' || *command == 'l'){
        //lägga till ett item i kundvagnen; 
    }
    if(*command == 'T' || *command == 't'){
        //Ta bort ett item i kundvagnen
    }
    if(*command == 'R' || *command == 'r'){
        //Redigera ett item i kundvagnen
    }
    
    if(*command == 'H' || *command == 'h'){
        //lista alla items i kundvagnen
    }
    
    if(*command == 'G' || *command == 'g'){
        //Ångra senaste ändringen
    }
    if(*command == 'A' || *command == 'a'){
        //minska på items i warehouse
        //Kalkulera kostnaden
        exit(0);
    }
}

int main(int argc, char *argv[]) {
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
}
