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

#define PROMPT(str) \
  ENG(printf("┃ %s", str));			\
  ENG(printf("┃ %s", str))

#define READ_INT(str) \
  printf("┃ ");   \
  ask_question_int(str)			

#define READ_STR(str)				\
  printf("┃ ");    \
  ask_question_str(str)
    
#define TITLE(str)	      \
  ENG(printf("┏──╸%s", str)); \
  SWE(printf("┏──╸%s", str))

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
void remove_cart_by_id(webstore_t *store);
void unicode_cart_menu(webstore_t *store);
void unicode_merch_menu(webstore_t *store);

#define current_cart_id	store->active_cart

#define LANG 1

#define SWE(thing) if (LANG == 0) { thing; }
#define ENG(thing) if (LANG == 1) { thing; }

char unicode_edit_cart_menu(webstore_t *store);

void unicode_merch_menu(webstore_t *store){
  char command[10];

  ENG(do {
      puts("┏──╸Webstore ╺─────────┓");
      puts("┃ [N]ew Merch          ┃");
      puts("┃ [R]emove Merch       ┃");
      puts("┃ [I]nspect Merch ID   ┃");
      puts("┃ [E]dit Merch         ┃");      
      puts("┃ [L]ist Database      ┃");
      puts("┃ [B]ack               ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n')) 
	add_new_merch_prompt(store);
      else if ((command[0] == 'R') || (command[0] == 'r')) 
	remove_merch_prompt(store);
      else if ((command[0] == 'E') || (command[0] == 'e')) 
	edit_merch_prompt(store);
      else if ((command[0] == 'D') || (command[0] == 'd'))
	remove_cart_by_id(store);
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
      puts("┃ [S]ök Merch ID      ┃");
      puts("┃ [L]ista Databasen   ┃");
      puts("┃ [B]akåt             ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
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
      puts("┃ [P]urge Cart by ID  ┃");
      puts("┃ [E]dit Cart         ┃");
      puts("┃ [A]ll Items         ┃");
      puts("┃ [D]isplay Cart      ┃");
      puts("┃ [F]inal cost        ┃");
      puts("┃ [C]heck out  [B]ack ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n'))
	create_cart(store);
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_from_cart_prompt(store);
      else if ((command[0] == 'P') || (command[0] == 'p'))
	remove_cart_by_id(store);
      else if ((command[0] == 'E') || (command[0] == 'e')) 
	unicode_edit_cart_menu(store);   
      else if (((command[0] == 'D') || (command[0] == 'd')) &&
	       (valid_cart_id(store, store->active_cart)))
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
      puts("┃ [P]urge Kundvagn med ID  ┃");
      puts("┃ [E]dita Kundvagn         ┃");
      puts("┃ [V]isa Kundvagn          ┃");
      puts("┃ [A]lla Varor             ┃");
      puts("┃ [T]otal Kostnad          ┃");
      puts("┃ [K]öp            [B]akåt ┃");
      puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
      printf("> ");
      read_string(command, 10);
    
      if      ((command[0] == 'N') || (command[0] == 'n'))
	add_to_active_cart_prompt(store); 
      else if ((command[0] == 'R') || (command[0] == 'r'))
	remove_from_cart_prompt(store);
      else if ((command[0] == 'A') || (command[0] == 'a'))
	show_stock(store);
      else if ((command[0] == 'E') || (command[0] == 'e'))
	unicode_edit_cart_menu(store);
      else if ((command[0] == 'P') || (command[0] == 'p'))
	remove_cart_by_id(store);
      else if (((command[0] == 'V') || (command[0] == 'v')) &&
	       (valid_cart_id(store, store->active_cart)))
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
      printf("> ");
      read_string(command, 10);
    
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
      printf("> ");
      read_string(command, 10);
      
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
      printf("> ");
      read_string(command, 10);
    
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
      printf("> ");
      read_string(command, 10);
    

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
      printf(">");

      read_string(command, 10);
    
      if      ((command[0] == 'W') || (command[0] == 'w')) unicode_merch_menu(store);
      else if ((command[0] == 'C') || (command[0] == 'c')) unicode_cart_menu(store);
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_prompt(store);
      else if ((command[0] == 'Q') || (command[0] == 'q')) exit(0);

    } while (true));

  SWE(do {
      puts("┏──╸Affär ╺─────┓");
      puts("┃ [V]aruhus     ┃");
      puts("┃ [K]org        ┃");
      puts("┃ [S]ätt ID     ┃");
      puts("┃ [A]vsluta     ┃");
      puts("┗━━━━━━━━━━━━━━━┛");
      printf(">");
      
      read_string(command, 10);
    
      if      ((command[0] == 'V') || (command[0] == 'v')) event_loop_webstore(store);
      else if ((command[0] == 'K') || (command[0] == 'k')) event_loop_cart(store);
      else if ((command[0] == 'S') || (command[0] == 's')) change_cart_id_prompt(store);
      else if ((command[0] == 'A') || (command[0] == 'a')) exit(0);

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

  SAFESET(SWE(new_id = ask_question_int("┃ Byt till ID: "))
	  ENG(new_id = ask_question_int("┃ Change to ID: ")),
	  valid_cart_id(store, new_id), return);
  

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
  
  ENG(printf("┃ Cart Id.%d: %dKr\n",     current_cart_id, cost));
  SWE(printf("┃ Kundvagn Id.%d: %dKr\n", current_cart_id, cost));

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
    ENG(price = READ_INT("| New Price: "));
    SWE(price = READ_INT("| Nytt Pris: "));
  } while (!choice_prompt("Correct?"));
  
  set_merch_price(store, name, price);
}

void remove_cart_by_id(webstore_t *store){
  // Prompt for a cart id, if it exists,
  // remove it.
  int id        = 0;

  ENG(puts("┏──╸Remove Cart by ID "));
  SWE(puts("┏──╸Ta Bort Kundvagn med ID "));
  do {
    ENG(id = ask_question_int("┃ Remove ID: "));
    SWE(id = ask_question_int("┃ Ta Bort ID: "));

    // Incorrect id
    if(!valid_cart_id(store, id)){
      SWE(PROMPT("Not a valid "));
      ENG(PROMPT("Inte ett Korrekt ID!"));
      ENG(if (!choice_prompt("Try Again?")) return;);
      SWE(if (!choice_prompt("Försök Igen?")) return;);
      id = 0;
      // Correct id
    } else return;
    
  } while (!valid_cart_id(store, id));


  
  remove_cart(store, id);

  ENG(printf("┃──╸ Finished Removing Cart %d", id));
  ENG(printf("┃──╸ Klar med Borttagning av Kundvagn %d", id));
  
}

void update_shelf_stock_menu(webstore_t *store, char *name){
  char *location = NULL;
  int shelf_id   = 0;
  int amount     = 0;
  
  while(location == NULL){

    
    ENG(puts("┃ All Shelfs Containing the Merch..."));
    SWE(puts("┃ Alla Hyllor Innehållande Varan..."));

    newline;
    list_shelfs(store, name);
    newline;
	
    ENG(puts("┃ Enter an Valid Shelf ID."));
    SWE(puts("┃ Skriv ett Giltigt Hylla ID."));

    newline;

    do {
      ENG(shelf_id = ask_question_int("┃ Shelf ID: "));
      SWE(shelf_id = ask_question_int("┃ Hyll-ID: "));
    } while (!valid_shelf_id(store, shelf_id));
    
    location = get_shelf_after_shelf_nr(store, shelf_id, name);	  
  }

  // Ask for stock amount
  do {
  SWE(amount = ask_question_int("┃ Antal på Hyllan: "));
  ENG(amount = ask_question_int("┃ Amount on Shelf: "));
  } while ((amount < 0) || (amount > AMOUNT_UPPER_MAX));
  set_merch_stock(store, name, amount, location);
}

void add_new_merch_prompt(webstore_t *store){
  char *desc_merch = NULL;
  char *shelf_name = NULL;
  char *name_merch = NULL;
      
  size_t price     = 0;
  size_t amount    = 0;      

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


  do { // Enter amount until it is valid	  
    SWE(amount = ask_question_int("┃ Antal: "));
    ENG(amount = ask_question_int("┃ Amount: "));     
  } while (!valid_sum(amount));
    
  add_merchendise(store, name_merch, desc_merch, price);
  set_shelf(store, name_merch, shelf_name, amount);  

  puts("┗──────────────────────────╸");  
}
void remove_merch_prompt(webstore_t *store){
  int id           = 0;
  char *name_merch = NULL;
  
  show_stock(store);
  ENG(puts("┏──╸Remove Merch     "));
  SWE(puts("┏──╸Ta Bort Vara    "));

  do { // Enter shelf until it is valid
      ENG(id = ask_question_int("┃ Merch ID: "));
      SWE(id = ask_question_int("┃ Varu-ID: "));
  } while (!valid_merch_id(store, id));
  
  name_merch = get_merch_name_in_storage(store, id);
    
  printf("┃ %s Removed!\n", name_merch); 
  remove_merchendise(store, name_merch);
  puts("┗───────────────────────────────╸");  
}
void lookup_merch_prompt(webstore_t *store){
  int id           = 0;
  char *name_merch = NULL;
  
  ENG(puts("┏──╸Lookup Merch Id     "));
  SWE(puts("┏──╸Kolla upp Varu-Id   "));
  // Ask for merch id
  do {    
    ENG(id     = ask_question_int("┃ Merch ID: "));
    SWE(id     = ask_question_int("┃ Varu-ID: "));
  } while (!valid_merch_id(store, id));
  // Grap merch name
  name_merch = get_merch_name_in_storage(store, id);
  // Print merch name
  if (name_merch == NULL){
    perror("lookup_merch_prompt: Merch not in storage.\n");
    return;
  }else {
    ENG(printf("┃ Nr.%d: is %s\n", id, name_merch));
    SWE(printf("┃ Nr.%d: %s\n",    id, name_merch));	
  }    
}
  

void edit_merch_prompt(webstore_t *store){
  int id           = 0;
  char *name_merch = NULL;

  if (choice_prompt("Display Stock?"))
    show_stock(store);

  SAFESET(ENG(puts("┏──╸Edit Merch Id     "))
	  SWE(puts("┏──╸Ändra Varu-ID     "))
	  ENG(id     = ask_question_int("┃ Merch ID: "))
	  SWE(id     = ask_question_int("┃ Varu-ID: ")),
	  valid_merch_id(store, id), return);
    
  name_merch = get_merch_name_in_storage(store, id);

  // Call new event loop
  unicode_edit_merch_menu(store, name_merch);
}         

int main(int argc, char *argv[]) {
  webstore_t *store = store_create();  // --- Load hardcoded test merch
  //  INIT_DATABASE(store);

//  event_loop_menu(store); 
    
//  current_cart_id = -1;
  store_destroy(store);
  return 0; 
}
