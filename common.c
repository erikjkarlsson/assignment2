#include <string.h>
#include "common_2.h"


bool eq_elem_void(elem_t a, elem_t b){
  char *pointer1 = a.p;
  char *pointer2 = b.p;

  if (pointer1 == pointer2 || pointer1 == NULL) {
    return pointer1 == pointer2;
  }

  return strcmp(pointer1, pointer2) == 0;
} 

bool eq_elem_int(elem_t a, elem_t b) {
  return a.i == b.i;
}

unsigned long string_knr_hash(elem_t key) {
  unsigned long result = 0;
  char *str = (char*)key.p;

  do {
    result = result * 31 + *str;
  } while (*++str != '\0');

  return result;
}

int extract_int_hash_key(elem_t key){

  return key.i;}


bool elem_cmp(elem_t fst, elem_t snd){
  return ((fst.i == snd.i) ||
	  (fst.u == snd.u) ||
	  (fst.b == snd.b) ||
	  (fst.f == snd.f) ||
	  (fst.p == snd.p));
}

