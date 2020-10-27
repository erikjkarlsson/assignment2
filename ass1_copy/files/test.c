

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "iterator.h"
#include "list_linked.h"
#include <CUnit/Basic.h>

#define LIST_LEN_1 20

#define ELEM_INT(NUM)                                                          \
  (elem_t) { .i = NUM }
#define ELEM_STR(STR)                                                          \
  (elem_t) { .s = STR }

#define GET_ELEM_STR(STR) (char *)STR.s

#define GET_ELEM_INT(INT) (int)INT.i

// Parse arguments

int init_suite(void) { return 0; }

int clean_suite(void) { return 0; }

void test1(void) {
  ioopm_list_t *food = ioopm_linked_list_create(ioopm_elem_cmp);
  // This test adds four foods, and their respective popularity (on
  // a fictional alien planet) in a linked list.
  // And proceeds to calculate and assert the most popular food.

  // ("Large Hamburgers", 100)
  ioopm_linked_list_append(food, ELEM_STR("Large Hamburgers"));
  ioopm_linked_list_append(food, ELEM_INT(100));

  // ("Mustard Hotdogs", 2)
  ioopm_linked_list_append(food, ELEM_STR("Mustard Hotdogs"));
  ioopm_linked_list_append(food, ELEM_INT(2));

  // ("Meat Sallad", 1000)
  ioopm_linked_list_append(food, ELEM_STR("Meat Sallad"));
  ioopm_linked_list_append(food, ELEM_INT(1000));

  // ("Cucumber Burger", 4)
  ioopm_linked_list_append(food, ELEM_STR("Cucumber Burger"));
  ioopm_linked_list_append(food, ELEM_INT(4));

  int tastiest = 0;
  int index = 0;
  int tmp = 0;

  for (int i = 0; i < 4; i++) {
    // Go through all pairs comparing all integer values
    // for the largest one (corresponding to the tastiest).
    // And save that index.
    tmp = GET_ELEM_INT(ioopm_linked_list_get(food, 2 * i + 1));
    if (tmp > tastiest) {
      tastiest = tmp;
      index = i;
    }
  }

  // index corresponding to;
  // "Meat Sallad" which is is the most tasty of them all
  CU_ASSERT(2 * index == 4);

  ioopm_linked_list_destroy(food);
}

void test2(void) {

  int t = 2 * LIST_LEN_1;
  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_cmp);

  // Insert elements (0 --> 2•)
  for (int j = 0; j < t; j++) {
    CU_ASSERT(j < t);
    ioopm_linked_list_insert(list, j, ELEM_INT(j));
  }

  // Assure the insertion of element
  for (int j = 0; j < t; j++) {
    CU_ASSERT(ioopm_linked_list_contains(list, ELEM_INT(j)));
  }

  // list->size starts counting from zero
  // hence in respect to times it is one "behind".
  CU_ASSERT(list->size == t);

  ioopm_linked_list_destroy(list);
}

void test3(void) {

  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_cmp);
  int length = 100;

  elem_t e;
  elem_t f;
  // Insert the amount of the LENGTH variable of elements,
  // constantly in the last position of the list.
  for (int j = 0; j < length; j++) {
    e = (elem_t){.i = j}; // [1]
    ioopm_linked_list_insert(list, list->size - 1, e);
  }

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  // Iterate through a list only using the iterator,
  // get the current element through the iterator, at the same
  // time use the linked list get function to get the same value.
  // The values are then compared.

  for (int i = 0; i < length; i++) {
    // Check if it is possible to iterate

    // needs to be done outside of =iter_next=
    // since the last element has no "next" attatched.
    e = ioopm_iterator_current(iter);
    f = ioopm_linked_list_get(list, i);

    CU_ASSERT((e.i == f.i));

    if (ioopm_iterator_has_next(iter)) {
      // Get element pointed on by the iterator

      // Iterate fwd...
      ioopm_iterator_next(iter);

    } else {
      // Get current elem, ASSERT and break loop.
      break;
    }
  }

  // Assert the expected list size
  CU_ASSERT(list->size == length);

  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iter);
}

void test4(void) {

  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_cmp);

  // Insert Integer and assure list contains it
  ioopm_linked_list_append(list, ELEM_INT(100));
  CU_ASSERT(ioopm_linked_list_contains(list, ELEM_INT(100)));

  // Insert String and assure list contains it
  ioopm_linked_list_prepend(list, ELEM_STR("200"));
  CU_ASSERT(ioopm_linked_list_contains(list, ELEM_STR("200")));

  // Insert Integer and assure list contains it
  ioopm_linked_list_insert(list, 1, ELEM_INT(200));
  CU_ASSERT(ioopm_linked_list_contains(list, ELEM_INT(200)));

  // Use the compare function and use it to assert the last
  // inserted value at the correct position
  CU_ASSERT(ioopm_elem_cmp(ELEM_INT(200), ioopm_linked_list_get(list, 1)));

  ioopm_linked_list_destroy(list);
}

void test5(void) {

  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_cmp);

  // Inject a value in the zeroth position
  // resulting in a reversed list
  for (int i = 1; i <= 10; i++) {
    ioopm_linked_list_insert(list, 0, ELEM_INT(i));
  }

  // Assert the reversed order of the list
  for (int i = 9; i >= 0; i--) {
    CU_ASSERT(10 - i == GET_ELEM_INT(ioopm_linked_list_get(list, i)));
  }

  ioopm_linked_list_destroy(list);
}

void test6(void) {
  // ## Testing the Iterator ##
  // Insert values into a empty list, assuring
  // that the size and the inserted values
  // are indeed inserted.
  // # ENABLE DEBUG #
  // Use the -d flag to enable debug
  // messages.

  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_cmp);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

  // Debug str of max size 255 chars,
  // which will be reused for all debug print out.
  // char *debug_str = calloc(255, sizeof(char));
  //
  int sum = 0;
  int sum_assert = 0;

  ioopm_iterator_reset(iter);

  for (int i = 0; i < 10; i++) {
    ioopm_iterator_insert(iter, ELEM_INT(i));
    sum_assert += i;
  }

  ioopm_iterator_reset(iter);
  int tmp = 0;
  tmp = GET_ELEM_INT(ioopm_iterator_current(iter)); // ==> 1
  sum += tmp;

  // First element since iterator next NEVER returns
  // the first element.

  for (int i = 1; ioopm_iterator_has_next(iter); i++) {
    tmp = GET_ELEM_INT(ioopm_iterator_next(iter));
    sum += tmp;
  }

  // (+ 2 3 4 5 6 7 8 9 0) ==> 44
  // Note: The current (0) moves towards the end
  //       since insert makes the current element
  //       its next on insertion.

  // Assure the size is as expected
  // and that the items inserted
  // is correctly retrieved.

  CU_ASSERT(list->size == 10);
  CU_ASSERT(sum == sum_assert);

  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iter);
}

void test7(void) {
  // Insert values into a empty list, assuring
  // that the size and the inserted values
  // are indeed inserted.

  ioopm_list_t *list = ioopm_linked_list_create(ioopm_elem_cmp);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

  ioopm_iterator_reset(iter);

  for (int i = 0; i < 10; i++) {
    ioopm_iterator_insert(iter, ELEM_INT(i));
  }

  ioopm_iterator_reset(iter);

  for (int i = 0; i < 10; i++) {
    ioopm_iterator_remove(iter);
  }

  // Assure that the list is empty
  CU_ASSERT(list->size == 0);

  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iter);
}

int main(int argc, char **argv) {

  CU_pSuite test_suite1 = NULL;
  arg_opt_t *options;
  options = create_arg_opt();

  arg_parse(argc, argv, options);

  // Lists tests enabled from the command line
  if (CUE_SUCCESS != CU_initialize_registry()) {
    destroy_arg_opt(options);
    return CU_get_error();
  }

  test_suite1 = CU_add_suite("Test Suite 1", init_suite, clean_suite);

  if (NULL == test_suite1) {
    destroy_arg_opt(options);
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (options->tests_p) {
    // Run tests using the (-t --run-tests) flag
    // using -e <test number> to enable specfic tests
    // Ex: ./test -t -e 1 -e 4    Will enable test 1 and 4
    if ((test_active(1, options)
             ? (NULL ==
                CU_add_test(test_suite1,
                            "Linked list: Mixing different types in one list.",
                            test1))
             : false) ||
        (test_active(2, options)
             ? (NULL ==
                CU_add_test(test_suite1,
                            "Linked List: Assert any items lost on insertion.",
                            test2))
             : false) ||
        (test_active(3, options)
             ? (NULL == CU_add_test(test_suite1,
                                    "Iterator: Insertion into Linkedlist.",
                                    test3))
             : false) ||
        (test_active(4, options)
             ? (NULL ==
                CU_add_test(test_suite1,
                            "Linked list: Assure list contains inserted item.",
                            test4))
             : false) ||
        (test_active(5, options)
             ? (NULL ==
                CU_add_test(test_suite1,
                            "Linked list: Insertion, Injective traversal.",
                            test5))
             : false) ||
        (test_active(6, options)
             ? (NULL ==
                CU_add_test(test_suite1,
                            "Iterator: Assure list contains inserted item",
                            test6))
             : false) ||
        (test_active(7, options)
             ? (NULL ==
                CU_add_test(test_suite1,
                            "Iterator: Inserting items and then removing them",
                            test7))
             : false)) {

      destroy_arg_opt(options);

      CU_cleanup_registry();
      return CU_get_error();
    }
  }
  destroy_arg_opt(options);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
