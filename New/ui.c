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

void event_loop_webstore(webstore_t *store);
void event_loop_cart(webstore_t *store);
void change_cart_id_menu(webstore_t *store);
int final_cost_menu(webstore_t *store);

int current_cart_id = -1; 

#define LANG 0
#define SWE(thing) if (LANG == 0) { thing; }
#define ENG(thing) if (LANG == 1) { thing; }


char unicode_merch_menu(){
  char command[10];

  if (LANG == 0) do {
      puts("┏──╸Webstore ╺────────┓");
      puts("┃ [N]ew Merch         ┃");
      puts("┃ [R]emove Merch      ┃");
      puts("┃ [E]dit Merch        ┃");
      puts("┃ [L]ist Database     ┃");
      puts("┃ [B]ack              ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n')) return 's';
      else if ((command[0] == 'R') || (command[0] == 'r')) return 'r';
      else if ((command[0] == 'E') || (command[0] == 'e')) return 'h';
      //      else if ((command[0] == 'U') || (command[0] == 'u')) return 'g';
      else if ((command[0] == 'L') || (command[0] == 'l')) return 'g';
      else if ((command[0] == 'B') || (command[0] == 'b')) return 'i';

    } while (true);
  else do {
      puts("┏──╸Webb Butik ╺──────┓");
      puts("┃ [N]y Vara           ┃");
      puts("┃ [R]adera Vara       ┃");
      puts("┃ [E]dita Vara        ┃");
      puts("┃ [L]ista Databasen   ┃");
      puts("┃ [B]akåt             ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n')) return 's';
      else if ((command[0] == 'R') || (command[0] == 'r')) return 'r';
      else if ((command[0] == 'E') || (command[0] == 'e')) return 'h';
      //      else if ((command[0] == 'G') || (command[0] == 'g')) return ’g’;
      else if ((command[0] == 'L') || (command[0] == 'l')) return 'g';
      else if ((command[0] == 'B') || (command[0] == 'b')) return 'i';

    } while (true);
 
  return 'i';
} 
char unicode_cart_menu(){
  char command[10];

  if (LANG == 0) do {
      puts("┏──╸Webstore ╺────────┓");
      puts("┃ [N]ew Cart          ┃");
      puts("┃ [R]emove Cart       ┃");
      puts("┃ [E]dit Cart         ┃");
      puts("┃ [D]isplay Cart      ┃");
      puts("┃ [F]inal cost        ┃");
      puts("┃ [C]heck out  [B]ack ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n')) return 'n';
      else if ((command[0] == 'R') || (command[0] == 'r')) return 'r';
      else if ((command[0] == 'E') || (command[0] == 'e')) return 'e';
      else if ((command[0] == 'D') || (command[0] == 'd')) return 'd';
      else if ((command[0] == 'F') || (command[0] == 'f')) return 'f';
      else if ((command[0] == 'B') || (command[0] == 'b')) return 'b';
      else if ((command[0] == 'C') || (command[0] == 'c')) return 'c';

    } while (true);
  else do {
      puts("┏──╸Webb Butik ╺───────────┓");
      puts("┃ [N]y Kundvagn            ┃");
      puts("┃ [R]adera Kundvagn        ┃");
      puts("┃ [E]dita Kundvagn         ┃");
      puts("┃ [V]isa Kundvagn          ┃");
      puts("┃ [T]otal Kostnad          ┃");
      puts("┃ [K]öp            [B]akåt ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n')) return 'n';
      else if ((command[0] == 'R') || (command[0] == 'r')) return 'r';
      else if ((command[0] == 'E') || (command[0] == 'e')) return 'e';
      else if ((command[0] == 'V') || (command[0] == 'v')) return 'd';
      else if ((command[0] == 'T') || (command[0] == 't')) return 'f';
      else if ((command[0] == 'K') || (command[0] == 'k')) return 'k';
      else if ((command[0] == 'B') || (command[0] == 'b')) return 'b';

    } while (true);
 
  return 'i';
} 


int unicode_edit_merch_menu(){
  char command[10];

  if (LANG == 0) do {
      puts("┏──╸Merch Edit ╺──────┓");
      puts("┃ [D]escription       ┃");
      puts("┃ [P]rice             ┃");
      puts("┃ [S]tock      [B]ack ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'D') || (command[0] == 'd')) return 1;
      else if ((command[0] == 'P') || (command[0] == 'p')) return 2;
      else if ((command[0] == 'S') || (command[0] == 's')) return 3;
      else if ((command[0] == 'B') || (command[0] == 'b')) return 0;

    } while (true);
  else do {
      puts("┏──╸Redigera Vara ╺───┓");
      puts("┃ [B]eskrivning       ┃");
      puts("┃ [P]ris              ┃");
      puts("┃ [A]ntal  [T]illbaka ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'B') || (command[0] == 'b')) return 1;
      else if ((command[0] == 'P') || (command[0] == 'p')) return 2;
      else if ((command[0] == 'A') || (command[0] == 'a')) return 3;
      else if ((command[0] == 'T') || (command[0] == 't')) return 0;

    } while (true);
 
  return 0;
} 

char unicode_edit_cart_menu(webstore_t *store){
  char command[10];

  if (LANG == 0) do {
      puts("┏──╸Merch Edit ╺──────┓");
      puts("┃ [A]dd to Cart       ┃");
      puts("┃ [R]emove from Cart  ┃");
      puts("┃ [S]et ID            ┃");
      puts("┃ [B]ack              ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'A') || (command[0] == 'a')) return 'a';
      else if ((command[0] == 'R') || (command[0] == 'r')) return 'r';
      else if ((command[0] == 'B') || (command[0] == 'b')) return 'b';    
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_menu(store);
      
    } while (true);
  else do {
      puts("┏──╸Redigera Vara ╺───┓");
      puts("┃ [L]ägg Till Vara    ┃");
      puts("┃ [T]a Bort Vara      ┃");
      puts("┃ [S]ätt ID           ┃");
      puts("┃ [B]akåt             ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'B') || (command[0] == 'b')) return 'b';
      else if ((command[0] == 'T') || (command[0] == 't')) return 'r';
      else if ((command[0] == 'L') || (command[0] == 'l')) return 'a';        
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_menu(store);
    } while (true);
 
  return 0;
} 

void event_loop_menu(webstore_t *store){
  char command[10];

  if (LANG == 0) do {
      puts("┏──╸Webstore ╺──┓");
      puts("┃ [W]arehouse   ┃");
      puts("┃ [C]art        ┃");
      puts("┃ [S]ätt ID     ┃");
      puts("┃ [Q]uit        ┃");
      puts("┗━━━━━━━━━━━━━━━┛");

      read_string(command, 10);
    
      if      ((command[0] == 'W') || (command[0] == 'w')) event_loop_webstore(store);
      else if ((command[0] == 'C') || (command[0] == 'c')) event_loop_cart(store);
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_menu(store);
      else if ((command[0] == 'Q') || (command[0] == 'q')) exit(0);

    } while (true);
  else do {
      puts("┏──╸Affär ╺─────┓");
      puts("┃ [V]aruhus     ┃");
      puts("┃ [K]org        ┃");
      puts("┃ [S]ätt ID     ┃");
      puts("┃ [A]vsluta     ┃");
      puts("┗━━━━━━━━━━━━━━━┛");
      
      read_string(command, 10);
    
      if      ((command[0] == 'V') || (command[0] == 'v')) event_loop_webstore(store);
      else if ((command[0] == 'K') || (command[0] == 'k')) event_loop_cart(store);
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_menu(store);
      else if ((command[0] == 'A') || (command[0] == 'a')) exit(0);

    } while (true);
}

void change_cart_id(webstore_t *store, int new_id){
  // Prompt and change the cart ID
  if(current_cart_id == 0){   
    SWE(puts("Det finns bara en tillgänglig kundvagn."));
    ENG(puts("There only exist one availible cart."));
  }
    
  while(!valid_id(store, new_id)){
    SWE(new_id = ask_question_int("Kundvagns ID: "));
    ENG(new_id = ask_question_int("Cart ID: "));
  }
    
  current_cart_id = new_id;
    
  SWE(printf("Kundvagns ID = %d\n", current_cart_id));
  ENG(printf("Cart ID = %d\n", current_cart_id));
}

void change_cart_id_menu(webstore_t *store){  
  if(current_cart_id >= 0){
    int new_id = 0;
    
    ENG(printf("Current Cart ID: %d\n", current_cart_id));
    SWE(printf("Kundvagns ID: %d\n", current_cart_id));
    
    list_all_cart_id(store); 

    SWE(new_id = ask_question_int("Nytt ID: "));
    ENG(new_id = ask_question_int("New ID: ")); 

    if (new_id > 0)
      change_cart_id(store, new_id);
    else {
      ENG(printf("Cannot set id below 0.\n"));
      SWE(printf("Kan inte ändra ID under 0.\n"));
    }
  }
}
void new_cart_menu(webstore_t *store){
  cart_t *new_cart = create_cart(store); 
  current_cart_id = new_cart->id;  // Change later
}

void event_loop_cart(webstore_t *store){
  char command = ' ';

  do {
    // Print and Prompt menu
    command = unicode_cart_menu();

    if (command == 'n')                    // New Cart
      new_cart_menu(store);
    
    // Add check for non-existing cart    
    else if(command == 'e'){                  // Edit Cart
      do {
	command = unicode_edit_cart_menu(store);	      
    
	if(command == 'a')                    // Add to Cart
	  add_to_cart_promt(store, current_cart_id); 
          
	else if(command == 'r')              // Remove from Cart
	  remove_from_cart_promt(store, current_cart_id);
          
	else if(command == 'd')              // Display Cart
	  display_cart(get_cart(store, current_cart_id));
	
      } while (command != 'b');              // Back
     command = ' ';
  }
    else if(command == 'd')                  // Display Cart
      display_cart(get_cart(store, current_cart_id));
    
    if(command == 'c')                       // Checkout
      checkout(store, current_cart_id);
    
    else if(command == 'f')                  // Final Cost
      final_cost_menu(store);
    
  } while (command != 'b');                  // Back
  
}

int final_cost_menu(webstore_t *store){
  int cost = calculate_cost(store, current_cart_id);
  
  ENG(printf("Cart Id.%d: %dKr\n",     current_cart_id, cost));
  SWE(printf("Kundvagn Id.%d: %dKr\n", current_cart_id, cost));

  return cost;
}
void edit_merch_menu(webstore_t *store, char *name_merch){
  int command      = -1;
  int price        = 0;
  int nr_shelf     = 0;
  size_t amount    = 0;
    
  char *location   = NULL;
  char *desc_merch = NULL;


  do {
    command = unicode_edit_merch_menu();
    
    if(command == 1){ // Desription
      ENG(desc_merch = ask_question_string("New Description: "));
      SWE(desc_merch = ask_question_string("Ny Beskrivning: "));
      
      set_merch_description(store, name_merch, desc_merch);
    }
    else if(command == 2){
      ENG(price = ask_question_int("New Price: "));
      SWE(price = ask_question_int("Nytt Pris: "));

      set_merch_price(store, name_merch, price);
    }
    else if(command == 3){ // Set Merch Stock             
      while(location == NULL){

	ENG(puts("All Shelfs Containing the Merch..."));
	SWE(puts("Alla Hyllor Innehållande Varan..."));

	puts("");
	list_shelfs(store, name_merch);
	puts("");
	
	ENG(puts("Enter an Valid Shelf ID."));
	SWE(puts("Skriv ett Giltigt Hylla ID."));

	puts("");
	ENG(nr_shelf = ask_question_int("Number of Shelf: "));
	SWE(nr_shelf = ask_question_int("Nummer på Hylla: "));
	  
	location = get_shelf_after_shelf_nr(store, nr_shelf, name_merch);	  
      }
      SWE(amount = ask_question_int("Antal på Hyllan: "));
      ENG(amount = ask_question_int("Amount on Shelf: "));
	
      set_merch_stock(store, name_merch, amount, location);
      
    }
  } while (command != 0);
}

void event_loop_webstore(webstore_t *store){
  char command     = unicode_merch_menu();
  char *name_merch = NULL;
  char *desc_merch = NULL;
  char *shelf_name = NULL;
      
  size_t price     = 0;
  size_t amount    = 0;      
  int    id        = 0;
  
  puts("");


  
  if(command == 'S' || command == 's'){
    ENG(puts("┏──╸Add New Merch     ╺─────╸"));
    SWE(puts("┏──╸Lägg Till Ny Vara ╺─────╸"));
  


      
    SWE(name_merch = ask_question_string("┃ Namn: "));
    ENG(name_merch = ask_question_string("┃ Name: "));
      
    SWE(desc_merch = ask_question_string("┃ Beskrivning: "));
    ENG(desc_merch = ask_question_string("┃ Description: "));
      
    SWE(price = ask_question_int("┃ Pris: "));
    ENG(price = ask_question_int("┃ Price: "));


    do { // Enter shelf until it is valid	  
      SWE(shelf_name = ask_question_string("┃ Hylla: "));
      ENG(shelf_name = ask_question_string("┃ Shelf: "));     
    } while (!is_shelf(shelf_name));
	
    ENG(amount = ask_question_int("┃ Amount: "));
    SWE(amount = ask_question_int("┃ Antal: "));
      
    new_item(store, name_merch, desc_merch, price, shelf_name, amount);

    puts("┗──────────────────────────╸");
    
  }
    
  if(command == 'r'){

    show_stock(store);
    ENG(puts("┏──╸Remove Shelf     ╺─────╸"));
    SWE(puts("┏──╸Ta Bort Hylla    ╺─────╸"));
    ENG(id = ask_question_int("┃ Remove Shelf With ID: \n"));
    SWE(id = ask_question_int("┃ Ta Bord Hylla Med ID: \n"));
    
    name_merch = get_merch_name_in_storage(store, id);
    
    printf("┃ %s Removed!\n", name_merch); 
    remove_item(store, name_merch);
    puts("┗───────────────────────────╸");

  }

    if(command == 'i'){
        do {
	  ENG(puts("┏──╸Lookup Merch Id     ╺─────╸"));
	  SWE(puts("┏──╸Kolla upp Varu-Id     ╺─────╸"));

	  show_stock(store);
    
	  ENG(id     = ask_question_int("┃ Merch ID: "));
	  SWE(id     = ask_question_int("┃ Varu-ID: "));
	  name_merch = get_merch_name_in_storage(store, id);

	  puts("┗───────────────────────────╸");        

	  if (name_merch == NULL){
	    ENG(puts("Not a Valid Merch ID!"));
	    SWE(puts("Inte ett riktigt varu-ID!"));
	  }
	}while (name_merch == NULL);	      
    }
    
  if(command == 'H' || command == 'h'){
    show_stock(store);
  }
    
  if(command == 'G' || command == 'g'){
    show_stock(store);
  }
    
  if(command == 'I' || command == 'i'){
    event_loop_menu(store); 
  }
}

int main(int argc, char *argv[]) {
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  INIT_DATABASE(store);
  perror("asd");
  while(true){
    event_loop_menu(store); 
  }
    
  current_cart_id = -1;
  store_destroy(store);
  return 0; 
}
