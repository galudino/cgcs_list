/*!
    \file       cgcs_list_ptr_test.c
    \brief      Client source file for cgcs_slist and cgcs_list

    \author     Gemuele Aludino
    \date       24 May 2021
 */

#include "cgcs_list.h"

#include <stdio.h>
#include <string.h>

// Callback functions for (char *) type
void print_cstr(void *arg);
void free_cstr(void *arg);
int cmp_cstr(const void *lhs, const void *rhs);

void populate_strings(cgcs_slist *sl);
void deinitialize(cgcs_slist *sl);

int main(int argc, const char *argv[]) {
    // Handle to our slist(s). We will have two separate examples.
    cgcs_slist *sl = (cgcs_slist *)(0);

    cgcs_slist slist_strings = CGCS_SLIST_INITIALIZER;
    sl = &slist_strings;

    populate_strings(sl);

    printf("\n{");
    cgcs_slforeach(sl, print_cstr);
    printf("}\n");
    
    const char *charlie = "Charlie";
    cgcs_slist_iterator it = cgcs_slend(sl);

    if ((it = cgcs_slfind(sl, cmp_cstr, &charlie)) != cgcs_slend(sl)) {
        printf("[slfind]: '%s'\n", *(char **)(it));
    } else {
        printf("[slfind]: '(failed)'\n");
    }

    deinitialize(sl);

    // Create an slist of int, populate it from the bottom up.
    cgcs_slist slist_ints = CGCS_SLIST_INITIALIZER;
    sl = &slist_ints;
    for (int i = 1; i < 100; i += 10) {
        cgcs_slpushf(sl, &i);
    }

    while (!cgcs_slempty(sl)) {
        int front = *(int *)(cgcs_slfront(sl));
        printf("ints: %d\n", front);
        cgcs_slpopf(sl);
    }

    return 0;
}

void print_cstr(void *arg) {
    printf(" %s \t %p\n", *(char **)(arg), arg);
}

void free_cstr(void *arg) {
    free(*(char **)(arg));
}

int cmp_cstr(const void *lhs, const void *rhs) {
    return strcmp(*(char **)(lhs), *(char **)(rhs));
}

void populate_strings(cgcs_slist *sl) {
    const char *arr[] = {
        "Beta",
        "Delta",
        "Alpha",
        "Foxtrot",
        "Charlie",
        "Echo",
        "Hotel",
        "Golf"
    };

    // You can only insert after/erase after
    // your current position (node) in the slist.
    // befbegin (before_begin) is the node
    // before the true head of the list.
    cgcs_slist_iterator it = cgcs_slbefbegin(sl);
    const size_t count = sizeof(arr) / sizeof *arr;

    for (size_t i = 0; i < count; i++) {
        char *str = malloc(strlen(arr[i]) + 1);
        strcpy(str, arr[i]);

        it = cgcs_slinsertaft(sl, it, &str);
    }
}

void deinitialize(cgcs_slist *sl) {
    // Go through each node and:
        // - Free the memory 
        //   addressed by the (void *) field within the node
        // - Free the memory addressed by the node itself
    while (!cgcs_slempty(sl)) {
        char *front = *(char **)(cgcs_slfront(sl));

        printf("popping front: %s\n", front);

        free_cstr(&front); 
        cgcs_slpopf(sl);
    }
}
