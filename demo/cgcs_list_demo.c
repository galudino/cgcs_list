/*!
    \file       cgcs_list_demo.c
    \brief      Client source file for cgcs_list

    \author     Gemuele Aludino
    \date       23 Jun 2021
 */

#define USE_CGCS_LIST
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
    cgcs_list list, *l = &list;
    linit(l);

    const char *items[] = {
        "Beta", "Delta", "Alpha", "Charlie", "Foxtrot", "Golf", "Echo"
    };

    const size_t size = sizeof(items) / sizeof *items;

    for (int i = 0; i < size; i++) {
        char *str = strdup(items[i]);
        lpushb(l, &str);
    }

    printf("{");
    lforeach(l, print_cstr);
    printf("}\n");

    cgcs_list_iterator it = lbegin(l);
    for (int i = 0; i < 3; i++) {
        it = it->m_next;
    }

    char *str = strdup("Inserted");

    linsert(l, it, &str);

    printf("{");
    lforeach(l, print_cstr);
    printf("}\n");
    
    str = "Foxtrot";
    it = lfind(l, cmp_cstr, &str);
    
    printf("key searched for: %s, found: %s\n", str, (char *)(it->m_data));
    free(it->m_data);
    lerase(l, it);

    printf("{");
    lforeach(l, print_cstr);
    printf("}\n");

    lforeach(l, free_cstr);
    ldeinit(l);

    cgcs_list intlist, *il = &intlist;
    linit(il);

    for (int i = 0; i < 10; i++) {
        lpushb(il, &i);
    }

    printf("{");
    lforeach(il, print_int);
    printf("}\n");

    int key = 6;
    cgcs_list_iterator iter = lfind(il, cmp_int, &key);

    lerase(il, iter);

    printf("{");
    lforeach(il, print_int);
    printf("}\n");

    ldeinit(il);

    return 0;
}

