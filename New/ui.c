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
        puts("Det finns bara en tillgänglig kundvagn!");
    }
    
    while(!valid_id(store, new_id)){
        new_id = ask_question_int("Var vänligen och skriv in ett giltigt id: \n"); 
    }
    
    current_cart_id = new_id; 
    printf("Det nuvuande kundvagns id:et är : %d\n", current_cart_id); 
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
            printf("Den nuvarande kundvagnens id är %d\n", current_cart_id); 
            list_all_cart_id(store); 
            int new_id = ask_question_int("Skriv in ett nytt id: \n"); 
            change_cart_id(store, new_id); 
        }else{
            puts("Det finns ingen tillgänglig kundvagn!");
        }
    }
    
    if(*command == 'L' || *command == 'l'){
        add_to_cart_promt(store, current_cart_id); 
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
        printf("Det totala priset på vara No.%d är %d:-\n", current_cart_id, cost); 
    }
    
    if(*command == 'I' || *command == 'i'){
        event_loop_menu(store); 
    }
    
    puts(""); 
}

void event_edit(webstore_t *store, char *name_merch){
    int command = ask_question_edit(); 
    
    if(command == 1){
        char *desc_merch = ask_question_string("\n Skriv in den nya beskrivningen för varan: \n");
        set_merch_description(store, name_merch, desc_merch);
    }
    if(command == 2){
        size_t price = ask_question_int("\n Skriv in det nya priset på varan: \n");
        set_merch_price(store, name_merch, price);
    }
    if(command == 3){
        list_shelfs(store, name_merch); 
        puts(""); 
        int nr_shelf = ask_question_int("Skriv in nummret på den hyllan där du vill ändra varans stock: \n");
        char *location = get_shelf_after_shelf_nr(store, nr_shelf, name_merch);
        
        while(location == NULL){
            puts("\n Skriv in ett nummer på en giltig hylla:"); 
            list_shelfs(store, name_merch); 
            nr_shelf = ask_question_int("\n Skriv in nummret på den hyllan där du vill ändra varans stock: \n");
            location = get_shelf_after_shelf_nr(store, nr_shelf, name_merch);
        }
        size_t amount = ask_question_int("Skriv in den nya mängden av varan som ska förvaras på hyllan: \n"); 
        set_merch_stock(store, name_merch, amount, location);
    }
}

void event_loop_webstore(webstore_t *store){
    char *command = ask_question_menu_webstore();
    
    puts(""); 
    if(*command == 'S' || *command == 's'){
        puts("--- Create New Merch ---"); 
        char *name_merch = ask_question_string("Skriv in varans namn: \n");
        char *desc_merch = ask_question_string("Skriv in beskrivningen för varan: \n");
        size_t price = ask_question_int("Skriv in priset på varan: \n");
        char *shelf_name = ask_question_string("Skriv in hyllan som varan ska förvaras på: \n"); 
        size_t amount = ask_question_int("skriv in mängden av varan som ska förvaras på hyllan: \n"); 
        
        new_item(store, name_merch, desc_merch, price, shelf_name, amount);

    }
    
    if(*command == 'T' || *command == 't'){
        show_stock(store);
        int nr_merch = ask_question_int("Skriv in nummret på varan du vill ta bort från lagret: \n");
        char *name_merch = get_merch_name_in_storage(store, nr_merch); 
        printf("Du har valt att ta bort varan med namet: %s\n", name_merch); 
        remove_item(store, name_merch);
    }
    if(*command == 'R' || *command == 'r'){
        show_stock(store);
        int nr_merch = ask_question_int("Skriv in nummret på varan du vill redigera i lagret: \n");
        char *name_merch = get_merch_name_in_storage(store, nr_merch);
        
        if(name_merch != NULL){
            printf("Du har valt att redigera varan med namet: %s\n", name_merch); 
            event_edit(store, name_merch);
        }else{
            puts("Du skrev inte in ett gilitigt nummer! Avbryter!");
        }
    }
    
    if(*command == 'H' || *command == 'h'){
        show_stock(store);
    }
    
    if(*command == 'G' || *command == 'g'){
        puts("OBS! Den här funktionen har ej blivit implementerad ännu!");
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
