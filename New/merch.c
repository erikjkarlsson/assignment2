#include "webstore.h"

#include "merch.h"

#define NEW_ITEM(store, name, description, price)	\
  add_merchendise(store, name, description, price);

#define SET_ITEM_LOC(store, name, shelf_name, amount)		\
  set_shelf(store, name, shelf_name, amount); 

void initialize_database(webstore_t *store){
  // Fruits

  NEW_ITEM(store, "Apple",    "Edible fruit.", 10);
  NEW_ITEM(store, "Orange",   "Imported fruit from Italy.", 15);
  NEW_ITEM(store, "Coconut",  "Edible tropical fruit from Congo.", 30);
  NEW_ITEM(store, "Mandarin", "Not GMO-free.", 5);
  NEW_ITEM(store, "Mountain Bike", "Prefessional competition bike.", 1000);
  NEW_ITEM(store, "LADA (Car)",    "Soviet import, manifactured by AvtoVAZ", 4000);
  NEW_ITEM(store, "Electric Bike", "AAA batteries not included.", 800);
  NEW_ITEM(store, "Skateboard",    "Small Longboard.", 40);
  NEW_ITEM(store, "Fake Rolex Watch", "A Fake Golden Rolex Watch", 100);
  NEW_ITEM(store, "Adidas Pants",     "the Classic Adidas Pants", 200);
  NEW_ITEM(store, "Used Jacket",      "From the hermes 1999 Summer Collection", 8000);
  NEW_ITEM(store, "Sandals",          "Simple sandals.", 120);

  SET_ITEM_LOC(store, "Apple",    "F12", 128); 
  SET_ITEM_LOC(store, "Orange",   "F12", 64);
  SET_ITEM_LOC(store, "Mandarin", "F12", 16);
  SET_ITEM_LOC(store, "Coconut",  "F12", 16);
  SET_ITEM_LOC(store, "Mountain Bike", "M01", 28); 
  SET_ITEM_LOC(store, "LADA (Car)",    "M01", 4);
  SET_ITEM_LOC(store, "Electric Bike", "M01", 10);
  SET_ITEM_LOC(store, "Skateboard",    "M02", 160);
  SET_ITEM_LOC(store, "Fake Rolex Watch", "C00", 18); 
  SET_ITEM_LOC(store, "Adidas Pants",     "C01", 6);
  SET_ITEM_LOC(store, "Used Jacket",      "C99", 1);
  SET_ITEM_LOC(store, "Sandals",          "C00", 36);

}

