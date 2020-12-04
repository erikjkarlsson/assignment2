#include "common.h"
#include "hash_table.h"
#include "list_linked.h"
#include "stdio.h"
#include "hash_table.h"
#include "list_linked.h"
#include "utils.h"
#include "webstore.h"

//kundvagnen

void event_loop(item_t *items){
    char *command = ask_question_menu();
    if(*command == 'L' || *command == 'l'){
        //lägga till ett item i kundvagnen; 
        }
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
        //Kalculera kostnaden
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    //create warehouse
    return 0; 
}
