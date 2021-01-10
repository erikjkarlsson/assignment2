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