#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>

//// RGB COLOR (HOW TO USE BELOW COLORS)
// (R/G/B) Color code aswell as no_color
// to turn off the color.
// Works by printing a color before text
// wanted to be colored, and at the end
// print no_color. The actual color comes
// form the console/vterminal interpreting
// the codes and translates that into color.

#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"
#define NO_COLOR "\033[0m"

// Print (R/G/B) String
#define PUT_RSTR(STR) putc_str(STR, RED, false);
#define PUT_BSTR(STR) putc_str(STR, BLUE, false);
#define PUT_GSTR(STR) putc_str(STR, GREEN, false);

// Print (R/G/B) Int
#define PUT_RINT(INT) putc_int(INT, RED);
#define PUT_GINT(INT) putc_int(INT, GREEN);
#define PUT_BINT(INT) putc_int(INT, BLUE);

// Do "DO" if the debugging flag is active
#define DBG(FLAG, DO)                                                          \
  if (FLAG) {                                                                  \
    DO;                                                                        \
  }

// Log a string to standard output if FLAG is true
#define LOG(FLAG, STR)                                                         \
  if (FLAG)                                                                    \
    printf("%sLOG%s: %s%s%s\n", GREEN, NO_COLOR, RED, STR, NO_COLOR);

// String equality test
#define STR_EQ(STR1, STR2) (!strcmp(STR2, STR1))

// If X do "DO"
#define IIF(X, DO)                                                             \
  if (X) {                                                                     \
    DO;                                                                        \
  }

// Newline
#define NL printf("\n");

// Max Amount of "Subtests" (length of list)
#define SUBTEST_LEN 20

// Size of list
#define LIST_LEN 2000

typedef union elem elem_t;
typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);

typedef unsigned int uint;

typedef struct arg_opt arg_opt_t;

void destroy_arg_opt(arg_opt_t *opt);
void arg_parse(int argc, char **argv, arg_opt_t *opt);
arg_opt_t *create_arg_opt();

union elem {
  /// TODO: update the names of the fields to something better?
  int i;
  uint u;
  bool b;
  float f;
  void *p;
  char *s;

  bool error;
  char *error_msg;
  char *error_fun;
};
//// elem
// error:     If set to True, then the returning function had a error.
// error_msg: A description of the error.
// error_fun: A description of where the error is (what function).

bool test_active(int test_number, arg_opt_t *opt);
void putc_str(char *str, char *color, bool nl);
void putc_int(int num, char *color);
void print_cont(char *prefix, char *container, char *arrow, elem_t new_elem,
                int index, int list_size);

int abs(int);

struct arg_opt {
  int *list[LIST_LEN];
  int list_length;

  int *subtest;
  int subtest_length;
  int subtest_length_max;

  bool use_list_p;
  bool debug_p;
  bool tests_p;
  bool deep_debug_p;

  bool exit;
};

// arg_opt_t *create_arg_opt();
