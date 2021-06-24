/*!
    \file       cgcs_list.c
    \brief      Source file for doubly-linked list

    \author     Gemuele Aludino
    \date       23 June 2021
 */

#include "cgcs_list.h"

#include <stdio.h>

struct cgcs_listnode *cgcs_lnnew(const void *data) {
    struct cgcs_listnode *node = malloc(sizeof *node);
    assert(node);

    cgcs_lninit(node, data);
    return node;
}

struct cgcs_listnode *cgcs_lnnew_allocfn(const void *data, void *(*allocfn)(size_t)) {
    struct cgcs_listnode *node = allocfn(sizeof *node);
    assert(node);

    cgcs_lninit(node, data);
    return node;
}

void cgcs_lndelete(struct cgcs_listnode *node) {
    cgcs_lndeinit(node);
    free(node);
}

void cgcs_lndelete_freefn(struct cgcs_listnode *node, void (*freefn)(void *)) {
    cgcs_lndeinit(node);
    freefn(node);
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
