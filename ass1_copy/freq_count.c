#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "hash_table.h"
#include "common.h"
#include "list_linked.h"
#include "iterator.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(* (char * const *) p1, * (char * const *) p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht, size_t count) // Insert word into hashtable
{
  ioopm_hash_table_insert(ht, ptr_elem(word), int_elem(count));
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true) 
    { char *duplicated_word = NULL;
      char *buf = NULL;
      size_t len = 0;
      getline(&buf, &len, f);

      if (feof(f))
        {
          free(buf);
          break;
        }
      
      for (char *word = strtok(buf, Delimiters);
           word && *word;
           word = strtok(NULL, Delimiters)
           )
      {
          elem_t count = ioopm_hash_table_lookup(ht, ptr_elem(word));
          if (No_error()) /// If the current word was not found in the hashtable, add the word to it
          {  
              duplicated_word = strdup(word);
              process_word (duplicated_word,ht, 1);

          }
          else
          {   
              process_word(word, ht, (count.i = count.i + 1)); /// If it was found, add 1 to the current words counter
          }
        }

      free(buf);
    }
  
  fclose(f);
}

int main(int argc, char *argv[])
{
  if (argc<=1)
  {
      printf("Please enter a text file\n");
      return -1;
  }
  
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, eq_elem_void, eq_elem_int); 
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }
      ioopm_list_t *keys = ioopm_hash_table_keys(ht);
      ioopm_list_iterator_t *iterator = ioopm_list_iterator(keys);

  size_t htsize = ioopm_hash_table_size(ht);
  printf("size: %d", htsize);
  char **words = calloc(htsize, sizeof(char*));
  size_t i = 0;
  elem_t current;
  
  for (size_t i = 0; i < htsize; i++)
   {
       current = ioopm_iterator_current(iterator);
       words[i] = current.p;
       current = ioopm_iterator_next(iterator);
   }

  sort_keys(words, htsize);

  char *current_word;
  for (i = 0; i < htsize; i++) /// Prints all the words in the file, together with their count
  {
    current_word = words[i];
    printf("%s: %d\n", current_word, ioopm_hash_table_lookup(ht, ptr_elem(current_word)).i);
  }
  printf("The sum of all unique words: %zu\n", htsize);
  for (i = 0 ; i < htsize; i++) {
    free(words[i]);
  }

  free(words);

  ioopm_iterator_destroy(iterator);
  ioopm_linked_list_destroy(keys);
  ioopm_hash_table_destroy(ht);
    }
