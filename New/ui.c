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
#include "merch.h"

int current_cart_id = -1; 

void event_loop_webstore(webstore_t *store);
void event_loop_cart(webstore_t *store);

void event_loop_menu(webstore_t *store){
    int command = ask_question_menu(); 
    
    puts(""); 
    
    if(command == 1){
        while(true){
            event_loop_webstore(store);
        }
    }
    
    if(command == 2){
        while(true){
            event_loop_cart(store);
        }
    }
    
    if(command == 3){
        puts("--- Program exited ---");
        exit(0);
    }
    
    puts(""); 
    
}

void change_cart_id(webstore_t *store, int new_id){
    if(current_cart_id == 0){
        puts("There is only one cart avaible!");
    }
    
    while(!valid_id(store, new_id)){
        new_id = ask_question_int("Please enter a valid cart id: \n"); 
    }
    
    current_cart_id = new_id; 
    printf("Your cart id is now: %d\n", current_cart_id); 
}

void event_loop_cart(webstore_t *store){
    char *command = ask_question_menu_cart();
    
    puts(""); 
    
    if(*command == 'S' || *command == 's'){
        cart_t *new_cart = create_cart(store); 
        current_cart_id = new_cart->id; 
    }
    
    if(*command == 'B' || *command == 'b'){
        if(current_cart_id >= 0){
            printf("Your current cart id is %d\n", current_cart_id); 
            list_all_cart_id(store); 
            int new_id = ask_question_int("Enter a new cart id: \n"); 
            change_cart_id(store, new_id); 
        }else{
            puts("There is no cart avaible!");
        }
    }
    
    if(*command == 'L' || *command == 'l'){
        add_to_cart_promt(store, current_cart_id); 
        /*char *name = ask_question_string("Enter the name of the merch you would like to add to the cart: "); 
        int merch_amount = ask_question_int("Enter the amount of this merch that you would like to add to the cart: "); 
        char *merch_name = name; 
        printf("name: %s", merch_name); 
        add_to_cart(store, current_cart_id, (char *) merch_name, merch_amount); */
    }
    
    if(*command == 'T' || *command == 't'){
        remove_from_cart_promt(store, current_cart_id);
    }
    
    if(*command == 'H' || *command == 'h'){
        display_cart(get_cart(store, current_cart_id)); 
    }
    
    if(*command == 'G' || *command == 'g'){
        puts("OBS! Den här funktionen har ej blivit implementerad ännu!");
    }
    if(*command == 'A' || *command == 'a'){
        int cost = calculate_cost(store, current_cart_id);
        printf("The total cost for cart No.%d is %d:-\n", current_cart_id, cost); 
    }
    
    if(*command == 'I' || *command == 'i'){
        event_loop_menu(store); 
    }
    
    puts(""); 
}

void event_loop_webstore(webstore_t *store){
    char *command = ask_question_menu_webstore();
    
    puts(""); 
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
        exit(0);
    }
    
    if(*command == 'I' || *command == 'i'){
        event_loop_menu(store); 
    }
}

int main(int argc, char *argv[]) {
    webstore_t *store = store_create();  // --- Load hardcoded test merch
    INIT_DATABASE(store);
    
    while(true){
        event_loop_menu(store); 
    }
    
    current_cart_id = -1; 
    return 0; 
}
