#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "hash_table.h"
#include "iterator.h"
#include "list_linked.h"
#include "common.h"
#include "utils.h"
#include "merch.h"
#include "webstore.h"
#include "cart.h"


#define SAFESET(what, check, error)				\
  do { what; } while (!choice_prompt("Satisfied?"));		\
  if (!(check)) error; 


void event_loop_webstore(webstore_t *store);
void event_loop_cart(webstore_t *store);
int final_cost_menu(webstore_t *store);
void change_active_cart_id(webstore_t *store);
char unicode_edit_cart_menu(webstore_t *store);
void event_loop_menu(webstore_t *store);
void print_cart_id(webstore_t *store);
void change_cart_id_prompt(webstore_t *store);
void display_cart_id_prompt(webstore_t *store);
int final_cost_menu(webstore_t *store);
void set_merch_desc_menu(webstore_t *store, char *name);
void set_merch_price_menu(webstore_t *store, char *name);
void update_shelf_stock_menu(webstore_t *store, char *name);
void add_new_merch_prompt(webstore_t *store);
void remove_merch_prompt(webstore_t *store);
void lookup_merch_prompt(webstore_t *store);
void edit_merch_prompt(webstore_t *store);
int unicode_edit_merch_menu(webstore_t *store, char *name);

void unicode_cart_menu(webstore_t *store);
void unicode_merch_menu(webstore_t *store);

#define current_cart_id	store->active_cart

#define LANG 1

#define SWE(thing) if (LANG == 0) { thing; }
#define ENG(thing) if (LANG == 1) { thing; }

#define clear() printf("\033[H\033[J")

char unicode_edit_cart_menu(webstore_t *store);

void unicode_merch_menu(webstore_t *store){
  char command[10];

  ENG(do {
      puts("┏──╸Webstore ╺────────┓");
      puts("┃ [N]ew Merch         ┃");
      puts("┃ [R]emove Merch      ┃");
      puts("┃ [I]nspect Merch ID  ┃");
      puts("┃ [E]dit Merch        ┃");
      puts("┃ [L]ist Database     ┃");
      puts("┃ [B]ack              ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();
      if      ((command[0] == 'N') || (command[0] == 'n')) 
	add_new_merch_prompt(store);
      else if ((command[0] == 'R') || (command[0] == 'r')) 
	remove_merch_prompt(store);
      else if ((command[0] == 'E') || (command[0] == 'e')) 
	edit_merch_prompt(store);
      else if ((command[0] == 'I') || (command[0] == 'i'))
	lookup_merch_prompt(store);
      else if ((command[0] == 'L') || (command[0] == 'l'))
	show_stock(store);
      else if ((command[0] == 'B') || (command[0] == 'b'))
	break;

    } while (true));
  SWE(do {
      puts("┏──╸Webb Butik ╺──────┓");
      puts("┃ [N]y Vara           ┃");
      puts("┃ [R]adera Vara       ┃");
      puts("┃ [E]dita Vara        ┃");
      puts("┃ [S]ök ID            ┃");
      puts("┃ [L]ista Databasen   ┃");
      puts("┃ [B]akåt             ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();
      if      ((command[0] == 'N') || (command[0] == 'n'))
	add_new_merch_prompt(store);
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_merch_prompt(store);
      else if ((command[0] == 'E') || (command[0] == 'e'))
	edit_merch_prompt(store);
      else if ((command[0] == 'S') || (command[0] == 's'))
	lookup_merch_prompt(store);
      else if ((command[0] == 'L') || (command[0] == 'l')) 
	show_stock(store);     
      else if ((command[0] == 'B') || (command[0] == 'b'))
	break;

    } while (true));
}






void unicode_cart_menu(webstore_t *store){
  char command[10];

  ENG(do {
      puts("┏──╸Webstore ╺────────┓");
      printf("┃ [N]ew Cart    Id. %d ┃\n",
	     store->active_cart);
      puts("┃ [R]emove Cart       ┃");
      puts("┃ [E]dit Cart         ┃");
      puts("┃ [A]ll Items         ┃");
      puts("┃ [D]isplay Cart      ┃");
      puts("┃ [F]inal cost        ┃");
      puts("┃ [C]heck out  [B]ack ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();
      if      ((command[0] == 'N') || (command[0] == 'n'))
	create_cart(store);
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_from_cart_prompt(store);
      else if ((command[0] == 'E') || (command[0] == 'e')) 
	unicode_edit_cart_menu(store);   
      else if (((command[0] == 'D') || (command[0] == 'd')) &&
	       (valid_id(store, store->active_cart)))
	display_cart(get_cart(store, store->active_cart));
      else if ((command[0] == 'A') || (command[0] == 'a'))
	show_stock(store);
      else if ((command[0] == 'F') || (command[0] == 'f'))
	final_cost_menu(store);
      else if ((command[0] == 'B') || (command[0] == 'b'))
	break;
      else if ((command[0] == 'C') || (command[0] == 'c'))
	checkout(store);

    } while (true));
  SWE(do {
      puts("┏──╸Webb Butik ╺───────────┓");
      printf("┃ [N]y Kundvagn     Id. %d ┃\n",
	     store->active_cart);
      puts("┃ [R]adera Kundvagn        ┃");
      puts("┃ [E]dita Kundvagn         ┃");
      puts("┃ [V]isa Kundvagn          ┃");
      puts("┃ [A]lla Varor             ┃");
      puts("┃ [T]otal Kostnad          ┃");
      puts("┃ [K]öp            [B]akåt ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();
      if      ((command[0] == 'N') || (command[0] == 'n'))
	add_to_active_cart_prompt(store); 
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_from_cart_prompt(store);
      else if ((command[0] == 'A') || (command[0] == 'a'))
	show_stock(store);
      else if ((command[0] == 'E') || (command[0] == 'e'))
	unicode_edit_cart_menu(store);   
      else if (((command[0] == 'V') || (command[0] == 'v')) &&
	       (valid_id(store, store->active_cart)))
	display_cart(get_cart(store, store->active_cart));
      else if ((command[0] == 'T') || (command[0] == 't')) 
	final_cost_menu(store);
      else if ((command[0] == 'K') || (command[0] == 'k'))
	checkout(store);
      else if ((command[0] == 'B') || (command[0] == 'b'))
	break;

    } while (true));
} 


int unicode_edit_merch_menu(webstore_t *store, char *name){
  char command[10];

  ENG(do {
      puts("┏──╸Merch Edit ╺──────┓");
      puts("┃ [D]escription       ┃");
      puts("┃ [P]rice             ┃");
      puts("┃ [S]tock      [B]ack ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();
      if      ((command[0] == 'D') || (command[0] == 'd')) 
	set_merch_desc_menu(store, name);
      else if ((command[0] == 'P') || (command[0] == 'p')) 
	set_merch_price_menu(store, name);
      else if ((command[0] == 'S') || (command[0] == 's')) 
	update_shelf_stock_menu(store, name);
      else if ((command[0] == 'B') || (command[0] == 'b'))
	break;

    } while (true));
  SWE(do {
      puts("┏──╸Redigera Vara ╺───┓");
      puts("┃ [B]eskrivning       ┃");
      puts("┃ [P]ris              ┃");
      puts("┃ [A]ntal  [T]illbaka ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();
      if      ((command[0] == 'B') || (command[0] == 'b')) 
      	set_merch_desc_menu(store, name);
      else if ((command[0] == 'P') || (command[0] == 'p')) 
	set_merch_price_menu(store, name);
      else if ((command[0] == 'A') || (command[0] == 'a')) 
	update_shelf_stock_menu(store, name);
      else if ((command[0] == 'T') || (command[0] == 't'))
	break;

    } while (true));
 
  return 0;
} 

char unicode_edit_cart_menu(webstore_t *store){
  char command[10];

  ENG(do {
      puts("┏──╸Merch Edit ╺──────┓");
      puts("┃ [A]dd to Cart       ┃");
      puts("┃ [R]emove from Cart  ┃");
      puts("┃ [D]isplay Cart      ┃");
      puts("┃ [S]et ID            ┃");
      puts("┃ [B]ack              ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();    
      if      ((command[0] == 'A') || (command[0] == 'a'))
	add_to_active_cart_prompt(store);           
      else if ((command[0] == 'R') || (command[0] == 'r')) 
	remove_from_cart_prompt(store);
      else if ((command[0] == 'S') || (command[0] == 's'))
	change_cart_id_prompt(store);
      else if ((command[0] == 'D') || (command[0] == 'd'))
	display_cart_id_prompt(store);
      else if ((command[0] == 'B') || (command[0] == 'b')) return 'b';    
      // Change this up if time
      
    } while (true));
  SWE(do {
      puts("┏──╸Redigera Kundvagn ╺───┓");
      puts("┃ [L]ägg Till Vara        ┃");
      puts("┃ [T]a Bort Vara          ┃");
      puts("┃ [V]isa kundvagn         ┃");
      puts("┃ [S]ätt ID               ┃");
      puts("┃ [B]akåt                 ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      read_string(command, 10);
      // Clearing screen after read: clear();    

      if ((command[0] == 'T') || (command[0] == 't')) 
	remove_from_cart_prompt(store);
      else if ((command[0] == 'L') || (command[0] == 'l'))
	add_to_active_cart_prompt(store);
      else if ((command[0] == 'V') || (command[0] == 'v'))
	display_cart_id_prompt(store);
      else if ((command[0] == 'S') || (command[0] == 's'))
	change_cart_id_prompt(store);
      else if ((command[0] == 'B') || (command[0] == 'b'))
	break;
    } while (true));
 
  return 0;
} 

void event_loop_menu(webstore_t *store){
  char command[10];

  ENG(do {
      puts("┏──╸Webstore ╺────┓");
      puts("┃ [W]arehouse     ┃");
      puts("┃ [C]art          ┃");
      puts("┃ [S]et Active ID ┃");
      puts("┃ [Q]uit          ┃");
      puts("┗━━━━━━━━━━━━━━━━━┛");
      printf("┃ > ");

      read_string(command, 10);
      
      if      ((command[0] == 'W') || (command[0] == 'w')) unicode_merch_menu(store);
      else if ((command[0] == 'C') || (command[0] == 'c')) unicode_cart_menu(store);
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_prompt(store);
      else if ((command[0] == 'Q') || (command[0] == 'q')) return;

    } while (true));

  SWE(do {
      puts("┏──╸Affär ╺─────┓");
      puts("┃ [V]aruhus     ┃");
      puts("┃ [K]org        ┃");
      puts("┃ [S]ätt ID     ┃");
      puts("┃ [A]vsluta     ┃");
      puts("┗━━━━━━━━━━━━━━━┛");
      printf("┃ > ");
      
      read_string(command, 10);
      // Clearing screen after read: clear();    
      if      ((command[0] == 'V') || (command[0] == 'v')) event_loop_webstore(store);
      else if ((command[0] == 'K') || (command[0] == 'k')) event_loop_cart(store);
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_prompt(store);
      else if ((command[0] == 'A') || (command[0] == 'a')) return;

    } while (true));
}

void print_cart_id(webstore_t *store){
  SWE(printf("┃ Nuvarande Kundvagns ID: %d\n",  store->active_cart));
  ENG(printf("┃ Current Cart ID: %d\n",         store->active_cart));
}

void change_cart_id_prompt(webstore_t *store){
  int new_id;
  // Prompt and change the cart ID  
   
  ENG(puts("┏──╸ Change Active Cart"));
  SWE(puts("┏──╸ Ändra Aktiv Kundvagn"))
    do {
      SWE(new_id = ask_question_int("┃ Byt till ID: "));
	ENG(new_id = ask_question_int("┃ Change to ID: "));

	if ((!valid_id(store, new_id)) &&
	    choice_prompt("Incorrect Cart ID, Go Back?"))
	  return;
	} while (!valid_id(store, new_id));
  

  store->active_cart = new_id;
  //  display_cart_id_prompt(store);
  //  if (choice_prompt("Show Active ID?"))
  //    print_cart_id(store);
  
  puts("┗──────────────────────────╸");
}

void display_cart_id_prompt(webstore_t *store){
 if (choice_prompt("Print All Carts?"))
    list_all_cart_id(store);
 else if (choice_prompt("Print Active Cart?"))
    print_cart_id(store);  

}


int final_cost_menu(webstore_t *store){
  int cost = calculate_cost(store, current_cart_id);
  
  ENG(printf("Cart Id.%d: %dKr\n",     current_cart_id, cost));
  SWE(printf("Kundvagn Id.%d: %dKr\n", current_cart_id, cost));

  return cost;
}

void set_merch_desc_menu(webstore_t *store, char *name){
  char *desc_merch = NULL;

  ENG(puts("┏──╸Set New Description  "));
  SWE(puts("┏──╸Sätt Ny Beskrivning   "));
      
  ENG(desc_merch = ask_question_string("| New Description: "));      
  SWE(desc_merch = ask_question_string("| Ny Beskrivning: "));
      
  set_merch_description(store, name, desc_merch);  
}

void set_merch_price_menu(webstore_t *store, char *name){
  int price        = 0;

  ENG(puts("┏──╸Set New Price  "));
  SWE(puts("┏──╸Sätt Nytt Price    "));
  do {
  ENG(price = ask_question_int("| New Price: "));
  SWE(price = ask_question_int("| Nytt Pris: "));
  } while (!choice_prompt("Correct?"));
  set_merch_price(store, name, price);
}

void update_shelf_stock_menu(webstore_t *store, char *name){
  char *location = NULL;
  int shelf_id   = 0;
  int amount     = 0;
  
  while(location == NULL){

        
    if (choice_prompt("List Available Shelfs?"))
      list_shelfs(store, name);
    	
    ENG(puts("| Enter Shelf Id, and Update Stock."));
    SWE(puts("| Skriv ett Giltigt Hylla ID."));
    puts("┃ Correct Shelf Format; <A-Z><0-9><0-9>");  
    // Enter Shelf Id until Valid, prompting for canceling
    
  do {    
    ENG(shelf_id = ask_question_int("| Shelf ID: "));
    SWE(shelf_id = ask_question_int("| Hyll-ID : "));
    
    // wrong shelf format
    if ((!is_shelf(shelf_name))  &&
	(choice_prompt("Incorrect Shelf Format, Go back to Menu?")))

    } while (!valid_id(store, shelf_id));
    
    location = get_shelf_after_shelf_nr(store, shelf_id, name);	  
  }

  // Ask for stock amount
  do {
  SWE(amount = ask_question_int("| Antal på Hyllan: "));
  ENG(amount = ask_question_int("| Amount on Shelf: "));
  } while ((amount < 0) || (amount > AMOUNT_UPPER_MAX));
  set_merch_stock(store, name, amount, location);
}

bool change_merch_shelf_prompt(webstore_t *store, char *name, int *amount, char *shelf_name){

  ENG(puts("┏──╸ Enter Shelf "));
  SWE(puts("┏──╸ Fyll in Hylla "));


  do { // Enter shelf until it is valid    
    SWE(*shelf_name = ask_question_string("┃ Hylla: "));
    ENG(*shelf_name = ask_question_string("┃ Shelf: "));
    // Prompt for option to go back to menu on
    // wrong shelf format
    if ((!is_shelf(*shelf_name))  &&
	(!choice_prompt("Incorrect Shelf Format, Try Again?")))
      return false;    
  } while (!is_shelf(*shelf_name));

  do {
    ENG(*amount = ask_question_int("┃ Amount: "));
    SWE(*amount = ask_question_int("┃ Antal: "));
    // If price is invald, customer can choose to return to menu
    if ((!valid_stock_size(*amount)) &&
	(!choice_prompt("Amount not Allowed, Try Again?")))
      return false;	
  } while (!valid_stock_size(*amount));

  return true;
}
void add_new_merch_prompt(webstore_t *store){
  // Prompt and if the information is correct,
  // add it to the merch database.
  
  char *desc_merch = NULL;
  char *shelf_name = NULL;
  char *name_merch = NULL;
      
  size_t price     = 0;
  size_t amount    = 0;
  
  ENG(puts("┏──╸Add New Merch "));
  SWE(puts("┏──╸Lägg Till Ny Vara "));

  // Prompt Nessisary information for creating merch
  // Name, Description and Price. If input is invalid,
  // display choice to exit.
  do { 
    SWE(name_merch = ask_question_string("┃ Namn: "));
    ENG(name_merch = ask_question_string("┃ Name: "));
      
    SWE(desc_merch = ask_question_string("┃ Beskrivning: "));
    ENG(desc_merch = ask_question_string("┃ Description: "));
      
    SWE(price = ask_question_int("┃ Pris: "));
    ENG(price = ask_question_int("┃ Price: "));
    // This prevents integer overflow, if
    // CAPITAL_MAX and CAPITAL_MIN is set correctly.	  
    if (!is_money(price)){
      puts("┃ Price is not Allowed.");
      printf("┃ Allowed Price-range: %d-%d",
	     (int)CAPITAL_MIN, (int)CAPITAL_MAX);
      // Prompt letting the customer decide to 
      // re-enter information
      if (!choice_prompt("Try Again?")) return;   
    }} while (!is_money(price));	 

  // Check if a shelf was indeed created
  if (change_merch_shelf_prompt(store, name_merch, &amount, &shelf_name))
    new_item(store, name_merch, desc_merch, price, shelf_name, amount);
  else {
     puts("┃ Not Adding Merchendise\nReturning to Main Menu ...");
  }
}
void remove_merch_prompt(webstore_t *store){
  // Prompt for removal of a merch through its ID.
  
  int id           = store->active_cart;
  char *name_merch = NULL;

  if (choice_prompt("Display Merchendise?"))
    show_stock(store);
  
  ENG(puts("┏──╸Remove Merch from Shelf "));
  SWE(puts("┏──╸Ta Bort Vara från Hylla "));

  do {
    ENG(id = ask_question_int("┃ Merch ID: "));
    SWE(id = ask_question_int("┃ Varu-Id: "));

    if ((!valid_merch_index(store, id)) &&
	(!choice_prompt("Invalid Merch ID, Try again?")))
      return;    
  } while (!valid_merch_index(store, id)); 
  
  name_merch = get_merch_name_in_storage(store, id);
    
  printf("┃ Shelf '%s' Removed!\n", name_merch); 
  
  remove_item(store, name_merch);
  
  //  puts("┗───────────────────────────────╸");  
}
void lookup_merch_prompt(webstore_t *store){
  int id           = 0;
  char *name_merch = NULL;

  ENG(puts("┏──╸Lookup Merch Id     "));
  SWE(puts("┏──╸Kolla upp Varu-Id   "));
  do {   
    ENG(id     = ask_question_int("┃ Merch ID: "));
    SWE(id     = ask_question_int("┃ Varu-ID: "));

    // On invalid merch index, prompt for a return
    // to menu
    if ((!valid_merch_index(store, id)) &&
	(!choice_prompt("Incorrect Merch ID, Try Again??")))
      return;	
  } while (!valid_merch_index(store, id));
  
  name_merch = get_merch_name_in_storage(store, id);

  // Check if this is a valid_merch_index, with it
  // always should be. Just incase 
  if (name_merch == NULL){
    perror("lookup_merch_prompt: Index Lookup Error (Critical).\n");
  }else {
    ENG(printf("┃ Merch with Id.%d is %s.\n", id, name_merch));
    SWE(printf("┃ Merch with Id.%d is %s\n", id,  name_merch));	
  }
}
  

void edit_merch_prompt(webstore_t *store){
  // Prompt for edit of a merch through Id,
  // where upon it moves on to an edit menu.
  
  int id           = store->active_cart;  
  char *name_merch = NULL;

  if (choice_prompt("Display Stock?"))
    show_stock(store);

  ENG(puts("┏──╸Edit Merch Id     "));
  SWE(puts("┏──╸Ändra Varu-ID     "));

  // Get a valid merch Id from the customer
  do {
    ENG(id     = ask_question_int("┃ Merch ID: "));
    SWE(id     = ask_question_int("┃ Varu-ID: "));

    // On invalid Merch Id, prompt for retry.
    if ((!is_merch(store, id)) &&
	(!choice_prompt("Invalid Merch Id, Try Again?.")))      
      return;
  } while (!is_merch(store, id));
  
  name_merch = get_merch_name_in_storage(store, id);

  // Call new event loop
  unicode_edit_merch_menu(store, name_merch);
}         

int main(int argc, char *argv[]) {
  webstore_t *store = store_create();
  // Parse incomming arguments
  parse_args(store, argc, argv);

  // Load hardcoded merch 
  INIT_DATABASE(store);

  // Start the main menu
  event_loop_menu(store); 
    
  current_cart_id = -1;            // Varför? ????????????????????????

  store_destroy(store);
  return 0; 
}
