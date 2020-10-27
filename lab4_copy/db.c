#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

struct item {
  char *name;
  char *desc;
  int   price; // Öre
  char *shelf;
}; 

typedef struct item item_t;

void print_item(item_t *i){
  printf("Name: %s\n", i->name);
  printf("Desc: %s\n", i->desc);
  printf("Price: %d.%d\n", i->price / 100, i->price % 100);
  printf("Shelf: %s\n", i->shelf);
}

item_t make_item(char *name, char* desc, int price, char *shelf){
  item_t i = { .name = name,
               .desc = desc,
               .price = price,
               .shelf = shelf };

  return i;
}

item_t input_item(){
  char *name  = ask_question_string("Enter Name: ");
  char *desc  = ask_question_string("Enter Desc: ");
  int price   = ask_question_int("Enter Price: ");
  char *shelf = ask_question_shelf("Enter Shelf: ");
    
  return make_item(name, desc, price, shelf);
}

char *magick(char **arr1, char **arr2, char **arr3, int arr_len){
  char str[255];
  strcpy(str, arr1[random_number(arr_len)]);
  strcat(str, "-");
  strcat(str, arr2[random_number(arr_len)]);
  strcat(str, " ");
  strcat(str, arr3[random_number(arr_len)]);
  
  return strdup(str);
}


void edit_db(item_t *items, int db_siz){
  while (1){
    int index = ask_question_int("Number to Edit: ");
    
    if (!(index < 1 || index > db_siz)){
	print_item(&items[index - 1]);
	items[index - 1] = input_item();
	break;	
    }else {
      printf("Non item!\n");
    }
  }
}

void list_db(item_t *items, int no_items){
  for (int i = 0; i < no_items; ++i){
    printf("%d. %s\n", (i + 1), items[i].name);
  }
}


int main(int argc, char *argv[])
{
  seed_random();
  char *array1[] = { "ultra",    "super",    "micro",     "nano"};
  char *array2[] = { "smakande", "luktande", "smörjande", "glittrande"};
  char *array3[] = { "kolor",    "pizzor",   "godisar",   "chips" }; 

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, 4); 
        char *desc = magick(array1, array2, array3, 4); 
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, shelf);

        db[db_siz] = item;
        ++db_siz;
      }
    list_db(db, db_siz);
    edit_db(db, db_siz);
    list_db(db, db_siz);
  }
  return 0;
}
