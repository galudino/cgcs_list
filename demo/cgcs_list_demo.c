/*!
    \file       cgcs_list_demo.c
    \brief      Client source file for cgcs_list

    \author     Gemuele Aludino
    \date       23 Jun 2021
 */

#include "cgcs_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Callback functions for (char *) type
static inline void print_cstr(void *arg) { printf(" %s ", *(char **)(arg)); }
static inline void free_cstr(void *arg) { free(*(char **)(arg)); } 

static inline int cmp_cstr(const void *lhs, const void *rhs) {
    return strcmp(*(char **)(lhs), *(char **)(rhs));
}

// Callback functions for (int) type
static inline void print_int(void *arg) { printf(" %d ", *(int *)(arg)); }

static inline int cmp_int(const void *lhs, const void *rhs) {
    return *(int *)(lhs) - *(int *)(rhs);
}

int main(int argc, const char *argv[]) {
    list_t list, *l = &list;
    list_init(l);

    const char *items[] = {
        "Beta", "Delta", "Alpha", "Charlie", "Foxtrot", "Golf", "Echo"
    };

    const size_t size = sizeof(items) / sizeof *items;

    for (int i = 0; i < size; i++) {
        char *str = strdup(items[i]);
        list_push_back(l, &str);
    }

    printf("{");
    list_foreach(l, print_cstr);
    printf("}\n");

    list_iterator_t it = list_begin(l);
    for (int i = 0; i < 3; i++) {
        it = it->m_next;
    }

    char *str = strdup("Inserted");

    list_insert(l, it, &str);

    printf("{");
    list_foreach(l, print_cstr);
    printf("}\n");
    
    str = "Foxtrot";
    it = list_find(l, cmp_cstr, &str);
    
    printf("key searched for: %s, found: %s\n", str, (char *)(it->m_data));
    free(it->m_data);
    list_erase(l, it);

    printf("{");
    list_foreach(l, print_cstr);
    printf("}\n");

    list_foreach(l, free_cstr);
    list_deinit(l);

    list_t intlist, *il = &intlist;
    list_init(il);

    for (int i = 0; i < 10; i++) {
        list_push_back(il, &i);
    }

    printf("{");
    list_foreach(il, print_int);
    printf("}\n");

    int key = 6;
    list_iterator_t iter = list_find(il, cmp_int, &key);

    list_erase(il, iter);

    printf("{");
    list_foreach(il, print_int);
    printf("}\n");

    list_deinit(il);

    return 0;
}

