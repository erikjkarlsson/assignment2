#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>

#define STR_EQ(STR1, STR2) (!strcmp(STR2, STR1))

void putc_str(char *str, char *color, bool nl) {
  printf("%s%s%s%s", color, str, NO_COLOR, nl ? "\n" : "");
}

void putc_int(int num, char *color) { printf("%s%d%s", color, num, NO_COLOR); }

void print_cont(char *prefix, char *container, char *arrow, elem_t new_elem,
                int index, int list_size) {
  printf("%s%s%s %s%s%s[%s%d/%d%s] %s%s%s %s%d%s\n", GREEN, prefix, NO_COLOR,
         BLUE, container, NO_COLOR, RED, index, list_size, NO_COLOR, BLUE,
         NO_COLOR, arrow, RED, new_elem.i, NO_COLOR);
}

int abs(int number) { return (number > 0) ? number : -number; }

bool elem_cmp(elem_t fst, elem_t snd) {
  return ((fst.i == snd.i) || (fst.u == snd.u) || (fst.b == snd.b) ||
          (fst.f == snd.f) || (fst.p == snd.p));
}

void list_active_tests(arg_opt_t *opt) {
  for (int i = 0; i < opt->subtest_length_max; i++)
    if (test_active(i, opt))
      printf("Test Number: %d\nStatus: Active\n", i);
}

arg_opt_t *create_arg_opt() {
  arg_opt_t *opt = calloc(1, sizeof(arg_opt_t));
  opt->subtest = calloc(SUBTEST_LEN, sizeof(int));

  for (int i = 0; i < LIST_LEN; i++) {
    opt->list[i] = 0;
  }
  opt->list_length = 10;

  for (int i = 0; i < SUBTEST_LEN; i++) {
    opt->subtest[i] = 0;
  }

  opt->subtest_length = 0;
  opt->subtest_length_max = SUBTEST_LEN;

  opt->use_list_p = false;
  opt->debug_p = false;
  opt->tests_p = false;

  opt->deep_debug_p = false;

  return opt;
}

void destroy_arg_opt(arg_opt_t *opt) {
  //  free(opt->list);
  free(opt->subtest);
  free(opt);
}

bool test_active(int test_number, arg_opt_t *opt) {
  for (int i = 0; i < SUBTEST_LEN; i++) {
    if (opt->subtest[i] == test_number) {
      return true;
    }
  }
  return false;
}

void subtest_enable(int test_number, arg_opt_t *opt) {

  if (opt->subtest_length < SUBTEST_LEN) {

    opt->subtest[opt->subtest_length++] = test_number;

    if (opt->debug_p)
      printf("ENABLED SUBTEST (KEY: %d)\n", opt->subtest[opt->subtest_length]);

  } else {
    printf("ERROR: Invalid test\n");
    exit(-1);
  }
}

void print_argv(int argc, char **argv) {
  for (int i = 0; i < argc; i++)
    printf("%s", argv[i]);
  printf("\n");
}

void print_help() {

  printf("Usage (--help or -h will show this message):\n");

  putc_str("-h --help                 (Show this message)", BLUE, true);
  putc_str("-d --debug (-v)           (Enable debug/verbose output)", BLUE,
           true);
  putc_str("-t --run-tests            (Run tests)", BLUE, true);
  putc_str("-D --deep-debug           (More detailed debugging)", BLUE, true);
  putc_str("-t --enable-test          (Enable part of a test or a test)", BLUE,
           true);
  putc_str("-s --set list_length <length>", BLUE, true);

  printf("       (Length of linked list <length> < 100)\n");

  putc_str("-e --enable-test  (Run separate tests (subtests) that checks "
           "diffrent things.",
           BLUE, true);
  putc_str("-s --set list <size> <e_0> ... <e_(size - 1)>", BLUE, true);

  printf("       (Length and individual elements of the linked list)\n");

  putc_str("-s --set span <lower> <upper> (set option)", BLUE, true);

  printf("       (All integers between <lower> and <upper> will be used in the "
         "linked list)\n");

  putc_str("-s --set list_length <length>  (Set the length of the list)", BLUE,
           true);

  printf("       (This changes how long the used list is, but it must be lower "
         "than\n");
  printf("        the allocated list size (%d), and the values of the list "
         "defaults to\n",
         LIST_LEN);
  printf("        all natural numbers from 0 --> <length>)\n");
}

void arg_parse(int argc, char **argv, arg_opt_t *opt) {
  ///~}~~~~~~~~~~~~~~~~~~~~~~~~}~~~~~~~~~~~~~~~~~~~~}~~~~~~~~~~~~~~~~~~~~~~~~}
  ///{    (    (    (    (    { ARGUMENT HANTERING {    (    (    (    (    {
  ///~}~~~~~~~~~~~~~~~~~~~~~~~~}~~~~~~~~~~~~~~~~~~~~}~~~~~~~~~~~~~~~~~~~~~~~~}

  // Need to provide atleast =one= argument.
  if (argc == 1) {
    print_argv(argc, argv);
    print_help();
    destroy_arg_opt(opt);
    exit(-1);
  } else {

    // Print help message
    for (int i = 1; i < argc; i++) {
      if ((STR_EQ(argv[i], "--help") || STR_EQ(argv[i], "-h"))) {
        print_argv(argc, argv);
        print_help();
        destroy_arg_opt(opt);
        exit(-1);

        // PARSE: options (and set them)
        // STX: --set <option> <value>
      } else if (STR_EQ(argv[i], "--use-test") || STR_EQ(argv[i], "-e")) {

        subtest_enable(atoi(argv[i + 1]), opt);
        i++;

      } else if (STR_EQ(argv[i], "--set") || STR_EQ(argv[i], "-s")) {

        // Compare towards allowed options
        if (STR_EQ(argv[i + 1], "length")) {
          // OPTION OF SET: Set the list to all numbers up to a number
          opt->list_length = atoi(argv[i + 2]);

          printf("Setting: %s = %d\n", argv[i + 1], atoi(argv[i + 2]));

          // (Skip <option> and <value>) i.e jump two strings forward.
          i = i + 1;

          // STX: -s <init_list> <n> <e_1> ... <e_n>
        } else if (STR_EQ(argv[i + 1], "list")) {
          // OPTION OF SET: use provided arguments as a list
          int n = atoi(argv[i + 2]);

          // Grab the next n argument
          for (int k = (i + 3); k <= n + (i + 2); k++) {
            *opt->list[0 + k - (i + 3)] = atoi(argv[k]);
          }
          // Skip past the rest of elements
          i += n + 2;
          // Save length
          opt->use_list_p = true;
          opt->list_length = n;

        } else if (STR_EQ(argv[i + 1], "span")) {
          // OPTION OF SET: span  (numbers from a -> b as list)
          int from = atoi(argv[i + 2]);
          int to = atoi(argv[i + 3]);

          if (-from + to > LIST_LEN) {
            printf("%sERROR: Span can be a max of %d%s", RED, LIST_LEN,
                   NO_COLOR);
            destroy_arg_opt(opt);
            exit(-1);
          }
          // Grab the next n arguments and store them in =in_list=
          for (int k = from; k <= to; k++) {
            *opt->list[k - from] = k;
          }
          // Skip past the rest of elements
          i += 3;

          // Save length
          opt->use_list_p = true;
          opt->list_length = -from + to;
        }

        // PARSE: Enable experimental features (mostly a lot of logs,..)
      } else if (STR_EQ(argv[i], "--deep-debug") || STR_EQ(argv[i], "-D")) {
        opt->deep_debug_p = true; // Switch to enabled

        // PARSE: Debugging options, (-d, --debug and -v)
      } else if ((STR_EQ(argv[i], "-d") || STR_EQ(argv[i], "--debug") ||
                  STR_EQ(argv[i], "-v"))) {

        printf("Debugging enabled\n");
        opt->debug_p = true; // Switch to enabled

      } else if ((STR_EQ(argv[i], "-w") || STR_EQ(argv[i], "--working"))) {

        printf("Working?\n");
        printf("Next: %d", atoi(argv[i + 1]));
        i++;

        // PARSE: Enable running of test on start.
      } else if ((STR_EQ(argv[i], "--run-tests") || STR_EQ(argv[i], "-t"))) {

        opt->tests_p = true; // Switch to enabled

      } else {
        // PARSE: Invalid / Non-Matching Flags
        printf("%sERROR: The flag '%s' is invalid%s\n", RED, argv[i], NO_COLOR);
        printf("LEN: %d\n", opt->list_length);
        // Display help screen and return failure
        print_argv(argc, argv);
        print_help();
        destroy_arg_opt(opt);
        exit(-1);
      }
    }
  }
}
