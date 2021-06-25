/*!
    \file       cgcs_list.c
    \brief      Source file for doubly-linked list

    \author     Gemuele Aludino
    \date       23 June 2021
 */

#include "cgcs_list.h"

#include <stdio.h>

struct cgcs_listnode *
cgcs_lnclear(struct cgcs_listnode *x, struct cgcs_listnode *y) {
    while (x != y) {
        x = cgcs_lnerase(x);
    }

    return y;
}

struct cgcs_listnode *
cgcs_lnclear_freefn(struct cgcs_listnode *x, struct cgcs_listnode *y, void (*freefn)(void *)) {
    while (x != y) {
        x = cgcs_lnerase_freefn(x, freefn);
    }

    return y;
}

struct cgcs_listnode *
cgcs_lnfindfwd(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (*cmpfn)(const void *, const void *)) {
    for (struct cgcs_listnode *curr = x; curr != y; curr = curr->m_next) {
        if (cmpfn(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_listnode *
cgcs_lnfindfwd_b(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (^cmp_b)(const void *, const void *)) {
    for (struct cgcs_listnode *curr = x; curr != y; curr = curr->m_next) {
        if (cmp_b(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_listnode *
cgcs_lnfindbkw(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (*cmpfn)(const void *, const void *)) {
    for (struct cgcs_listnode *curr = x; curr != y; curr = curr->m_prev) {
        if (cmpfn(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_listnode *
cgcs_lnfindbkw_b(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (^cmp_b)(const void *, const void *)) {
    for (struct cgcs_listnode *curr = x; curr != y; curr = curr->m_prev) {
        if (cmp_b(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_listnode *
cgcs_lnadvance(struct cgcs_listnode **x, int index) {
    if (index > 0) {
        for (int i = 0; i < index; i++) {
            (*x) = (*x)->m_next;
        }
    } else {
        for (int i = index; i >= 0; i--) {
            (*x) = (*x)->m_prev;
        }
    }

    return (*x);
}

struct cgcs_listnode *
cgcs_lngetnode(struct cgcs_listnode *x, int index) {
    return cgcs_lnadvance(&x, index);
}

void cgcs_lnswap(struct cgcs_listnode *x, struct cgcs_listnode *y) {
    if (x->m_next != x) {
        if (y->m_next != y) {
            // both x and y are not empty
            struct cgcs_listnode *temp_x_next = x->m_next;
            struct cgcs_listnode *temp_x_prev = x->m_prev;

            x->m_next = y->m_next;
            y->m_next = temp_x_next;

            x->m_prev = y->m_prev;
            y->m_prev = temp_x_prev;
        } else {
            // x is not empty, y is empty
            y->m_next = x->m_next;
            y->m_prev = x->m_prev;

            y->m_next->m_prev = y;
            y->m_prev->m_next = y;

            x->m_next = x;
            x->m_prev = x;
        }
    } else if (y->m_next != y) {
        // x is empty, y is not empty
        x->m_next = y->m_next;
        x->m_prev = y->m_prev;
        x->m_next->m_prev = x->m_prev->m_next = x;
        y->m_next = y->m_prev = y;
    }
}

void cgcs_lnreverse(struct cgcs_listnode *x) {
    struct cgcs_listnode *temp = x;

    do {
        struct cgcs_listnode *temp_next = temp->m_next;
        temp->m_next = temp->m_prev;
        temp->m_prev = temp_next;

        temp = temp->m_prev;
    } while (temp != x);
}

void cgcs_lntransfer(struct cgcs_listnode *x, struct cgcs_listnode *first, struct cgcs_listnode *last) {
    if (x != last) {
        // remove [first, last) from its old position
        last->m_prev->m_next = x;
        first->m_prev->m_next = last;
        x->m_prev->m_next = first;

        // splice [first, last) into its new position
        struct cgcs_listnode *temp_x_prev = x->m_prev;
        x->m_prev = last->m_prev;
        last->m_prev = first->m_prev;
        first->m_prev = temp_x_prev;
    }
}

void cgcs_ldeinit(cgcs_list *self) {
    cgcs_list_iterator it = cgcs_lbegin(self);

    while (it != cgcs_lend(self)) {
        it = cgcs_lerase(self, it);
    }

    self->m_impl.m_next = &(self->m_impl);
    self->m_impl.m_prev = &(self->m_impl);
}

void cgcs_ldeinit_freefn(cgcs_list *self, void (*freefn)(void *)) {
    cgcs_list_iterator it = cgcs_lbegin(self);

    while (it != cgcs_lend(self)) {
        it = cgcs_lerase_freefn(self, it, freefn);
    }

    self->m_impl.m_next = &(self->m_impl);
    self->m_impl.m_prev = &(self->m_impl);
}

cgcs_list_iterator cgcs_linsert(cgcs_list *self, cgcs_list_iterator it, const void *data) {
    struct cgcs_listnode *new_node = cgcs_lnnew(data);
    cgcs_lnhook(new_node, it);
    return it;
}

cgcs_list_iterator cgcs_linsert_allocfn(cgcs_list *self, cgcs_list_iterator it, const void *data, void *(*allocfn)(size_t)) {
    struct cgcs_listnode *new_node = cgcs_lnnew_allocfn(data, allocfn);
    cgcs_lnhook(new_node, it);
    return it;
}

cgcs_list_iterator cgcs_lerase(cgcs_list *self, cgcs_list_iterator it) {
    cgcs_list_iterator next = it->m_next;
    cgcs_lnunhook(it);
    cgcs_lndelete(it);
    return next;
}

cgcs_list_iterator cgcs_lerase_freefn(cgcs_list *self, cgcs_list_iterator it, void (*freefn)(void *)) {
    cgcs_list_iterator next = it->m_next;
    cgcs_lnunhook(it);
    cgcs_lndelete_freefn(it, freefn);
    return next;
}

void cgcs_lforeach(cgcs_list *self, void (*func)(void *)) {
    for (cgcs_list_iterator it = cgcs_lbegin(self); it != cgcs_lend(self); it = it->m_next) {
        func(&(it->m_data));
    }
}

void cgcs_lforeach_range(cgcs_list *self, void (*func)(void *), cgcs_list_iterator beg, cgcs_list_iterator end) {
    for (cgcs_list_iterator it = beg; it != end; it = it->m_next) {
        func(&(it->m_data));
    }
}

cgcs_list_iterator cgcs_lfind(cgcs_list *self, int (*cmpfn)(const void *, const void *), const void *data) {
    for (cgcs_list_iterator it = cgcs_lbegin(self); it != cgcs_lend(self); it = it->m_next) {
        if (cmpfn(data, &(it->m_data)) == 0) {
            return it;
        }
    }

    return cgcs_lend(self);
}

cgcs_list_iterator cgcs_lfind_range(cgcs_list *self, int (*cmpfn)(const void *, const void *), const void *data, cgcs_list_iterator beg, cgcs_list_iterator end) {
    for (cgcs_list_iterator it = beg; it != end; it = it->m_next) {
        if (cmpfn(data, &(it->m_data)) == 0) {
            return it;
        }
    }

    return cgcs_lend(self);
}

cgcs_list *cgcs_lnew() {
    cgcs_list *list = malloc(sizeof *list);
    assert(list);

    cgcs_linit(list);

    return list;
}

cgcs_list *cgcs_lnew_allocfn(void *(*allocfn)(size_t)) {
    cgcs_list *list = allocfn(sizeof *list);
    assert(list);

    cgcs_linit(list);

    return list;
}

void cgcs_ldelete(cgcs_list *l) {
    cgcs_ldeinit(l);
    free(l);
}

void cgcs_ldelete_freefn(cgcs_list *l, void (*freefn)(void *)) {
    cgcs_ldeinit_freefn(l, freefn);
    freefn(l);
}
