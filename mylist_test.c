// #include <check.h>
// #include <stdint.h>
// #include "mylist.h"
// #include <stdio.h>
// #include <stdlib.h>

// LinkedList* list;
// void* value = (void*)1;

// int main(){
//     list = list_create();

//     return 0;
// }

#include <check.h>
#include <stdint.h>
#include "mylist.h"
#include <stdio.h>
#include <stdlib.h>

LinkedList* list;
void* value = (void*)1;

void setup() {
    list = list_create();
}

void teardown() {
    list_destroy(list);
}

START_TEST (test_init)
{
    ck_assert(list->head == NULL);
}
END_TEST

START_TEST (test_insert)
{
    ck_assert(list_insert(list, value));
    ck_assert(list_contains(list, value));
}
END_TEST

START_TEST (test_contains)
{
    list_insert(list, value);
    ck_assert(list_contains(list, value));
    ck_assert(!list_contains(list, (void*)2));
}
END_TEST

START_TEST (test_remove)
{
    list_insert(list, value);
    ck_assert(list_contains(list, value));
    ck_assert(list_remove(list, value));
    ck_assert(!list_contains(list, value));
}
END_TEST

START_TEST (test_remove_not_in_list)
{
    list_insert(list, value);
    ck_assert(list_contains(list, value));
    ck_assert(!list_remove(list, (void*)2));
    ck_assert(list_contains(list, value));
}
END_TEST

START_TEST (test_empty)
{
    ck_assert(list_empty(list));
    list_insert(list, value);
    ck_assert(!list_empty(list));
}
END_TEST

START_TEST (test_size)
{
    ck_assert_int_eq(0, list_size(list));
    list_insert(list, (void*)1);
    ck_assert_int_eq(1, list_size(list));
    list_insert(list, (void*)2);
    ck_assert_int_eq(2, list_size(list));
    list_insert(list, (void*)3);
    ck_assert_int_eq(3, list_size(list));
    list_remove(list, (void*)1);
    list_remove(list, (void*)2);
    list_remove(list, (void*)3);
    ck_assert_int_eq(0, list_size(list));
}
END_TEST

START_TEST (test_create)
{
    LinkedList* heapList = list_create();
    ck_assert(heapList != NULL);
    list_destroy(heapList);
}
END_TEST

Suite* suite(void) {
    Suite* s = suite_create("queue");
    TCase *tc_core = tcase_create("core");
    tcase_add_checked_fixture (tc_core, setup, teardown);
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_insert);
    tcase_add_test(tc_core, test_contains);
    tcase_add_test(tc_core, test_remove);
    tcase_add_test(tc_core, test_remove_not_in_list);
    tcase_add_test(tc_core, test_empty);
    tcase_add_test(tc_core, test_size);
    tcase_add_test(tc_core, test_create);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int numberFailed;
    Suite* s = suite();
    SRunner *sr = srunner_create(s);
    // Don't fork so we can actually use gdb
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    numberFailed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (numberFailed == 0) ? 0 : 1;
}