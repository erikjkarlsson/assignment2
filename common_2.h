#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
//// RGB COLOR (HOW TO USE BELOW COLORS)
// (R/G/B) Color code aswell as no_color
// to turn off the color.
// Works by printing a color before text
// wanted to be colored, and at the end
// print no_color. The actual color comes
// form the console/vterminal interpreting
// the codes and translates that into color.
extern int errno;
#define RED       "\033[0;31m"
#define BLUE      "\033[0;34m"
#define GREEN     "\033[0;32m"
#define NO_COLOR  "\033[0m"

// String for printf format to color a String (R/G/B)
#define R_STR_STR "\033[0;31m%s\033[0m"
#define G_STR_STR "\033[0;32m%s\033[0m"
#define B_STR_STR "\033[0;34m%s\033[0m"

// String for printf format to color a Int (R/G/B)
#define R_INT_STR "\033[0;31m%d\033[0m"
#define G_INT_STR "\033[0;32m%d\033[0m"
#define B_INT_STR "\033[0;34m%d\033[0m"

// Colored printout of STR to mark start of section
// (the same function as the old "FORM FEED" char (0x0A)
#define SECTION(STR)\
  putc_str("  [", RED, false);\
  printf("%s%s%s", GREEN, STR, NO_COLOR);\
  putc_str("]\n", RED, true);

// Print (R/G/B) String
#define PUT_RSTR(STR) putc_str(STR, RED,   false);
#define PUT_BSTR(STR) putc_str(STR, BLUE,  false);
#define PUT_GSTR(STR) putc_str(STR, GREEN, false);

// Print (R/G/B) Int
#define PUT_RINT(INT) putc_int(INT, RED);
#define PUT_GINT(INT) putc_int(INT, GREEN);
#define PUT_BINT(INT) putc_int(INT, BLUE);

// Newline
#define NL printf("\n");

// Max Amount of "Subtests" (length of list)
#define SUBTEST_LEN 20

// Size of list
#define LIST_LEN 2000

typedef unsigned int uint;
typedef union elem elem_t;

// Errno definitions
#define Error() (errno = EINVAL)
#define Successful() (errno = 0)
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
union elem
{
  /// TODO: update the names of the fields to something better?
  int   i;
  uint  u;
  bool  b;
  float f;
  void *p;
  char *c;
  bool  error;
  char *error_msg;
  char *error_fun;
};
//// elem
// error:     If set to True, then the returning function had a error.
// error_msg: A description of the error.
// error_fun: A description of where the error is (what function).
typedef bool(*ioopm_eq_function)(elem_t key, elem_t value);
typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t value, void *extra);
typedef int(*ioopm_hash_function)(elem_t key);
void putc_str(char *str, char *color, bool nl);
void putc_int(int num, char *color);
void print_cont(char *prefix, char* container, char *arrow,
		elem_t new_elem, int index, int list_size);


typedef struct arg_opt arg_opt_t;



struct arg_opt {
  int *list[LIST_LEN];
  int list_length;

  int *subtest[SUBTEST_LEN];
  int  subtest_length;
  int  subtest_length_max;


  bool use_list_p;
  bool debug_p;
  bool tests_p;
  bool deep_debug_p;

  bool exit;
};


typedef bool (*debug_print)(char *str);
typedef bool (*debug_fineprint)(char *str);
//arg_opt_t *create_arg_opt();

void destroy_arg_opt(arg_opt_t *opt);

void arg_parse(int argc, char **argv, arg_opt_t *opt);

arg_opt_t *create_arg_opt();
/*int extract_int_hash_key(elem_t key)
{
  return key.i;} // following a very terse naming scheme -- improve?
*/
#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }
#define char_elem(x) (elem_t) {.c=(x) }
int extract_int_hash_key(elem_t key);
bool elem_cmp(elem_t fst, elem_t snd);
unsigned long string_knr_hash(elem_t key);
bool eq_elem_int(elem_t a, elem_t b);
bool eq_elem_string(elem_t a, elem_t b);