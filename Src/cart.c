#include "common.h"
#include "hash_table.h"
#include "list_linked.h"
#include "stdio.h"
#include "hash_table.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"

#define get_elem_ptr(e) e.p

//kundvagnen
struct cart {
  int id; 
  ioopm_hash_table_t *merch_in_cart; 
}; 

typedef struct cart cart_t;

void create_cart(webstore_t *store){
    cart_t *new_cart = calloc(1, sizeof(cart_t));
    
    //ht containing all merch in cart with int key and string elem 
    new_cart->merch_in_cart = ioopm_hash_table_create(extract_int_hash_key, eq_elem_string, eq_elem_int); 
    
    //add cart to the list of all shopping carts
    ioopm_linked_list_prepend(store->all_shopping_carts, ptr_elem(new_cart));
    
    //get id of cart
    new_cart->id= ioopm_linked_list_size(store->all_shopping_carts);
    printf("The id of your cart is %d\n", new_cart->id); //TODO: function or remove 
}

cart_t *get_cart(webstore_t *store, int id){
    return get_elem_ptr(ioopm_linked_list_get(store->all_shopping_carts, id));
}

bool valid_id(){
    return false;
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

void add_to_cart(webstore_t *store, int id, int nr){
    
}

////
/* PROMT FUNCTIONS */
///


void event_loop(webstore_t *store){
    char *command = ask_question_menu();
    
    if(*command == 'S' || *command == 's'){
        //skapa en ny kundvagn 
        
        create_cart(store);
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
    
    bool run = true; 
    /*while(run){
        event_loop();
    }*/
    
    return 0; 
}
