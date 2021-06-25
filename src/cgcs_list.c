/*!
    \file       cgcs_list.c
    \brief      Source file for doubly-linked list

    \author     Gemuele Aludino
    \date       23 June 2021
 */

#include "cgcs_list.h"

#include <stdio.h>

struct cgcs_list_node *
list_node_clear(struct cgcs_list_node *x, struct cgcs_list_node *y) {
    while (x != y) {
        x = list_node_erase(x);
    }

    return y;
}

struct cgcs_list_node *
list_node_clear_free_fn(struct cgcs_list_node *x, struct cgcs_list_node *y, void (*freefn)(void *)) {
    while (x != y) {
        x = list_node_erase_free_fn(x, freefn);
    }

    return y;
}

struct cgcs_list_node *
list_node_find_forward(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (*cmpfn)(const void *, const void *)) {
    for (struct cgcs_list_node *curr = x; curr != y; curr = curr->m_next) {
        if (cmpfn(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_list_node *
list_node_find_forward_b(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (^cmp_b)(const void *, const void *)) {
    for (struct cgcs_list_node *curr = x; curr != y; curr = curr->m_next) {
        if (cmp_b(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_list_node *
list_node_find_backward(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (*cmpfn)(const void *, const void *)) {
    for (struct cgcs_list_node *curr = x; curr != y; curr = curr->m_prev) {
        if (cmpfn(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_list_node *
list_node_find_backward_b(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (^cmp_b)(const void *, const void *)) {
    for (struct cgcs_list_node *curr = x; curr != y; curr = curr->m_prev) {
        if (cmp_b(curr->m_data, data) == 0) {
            return curr;
        }
    }

    return NULL;
}

struct cgcs_list_node *
list_node_advance(struct cgcs_list_node **x, int index) {
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

struct cgcs_list_node *
list_node_get(struct cgcs_list_node *x, int index) {
    return list_node_advance(&x, index);
}

void list_node_swap(struct cgcs_list_node *x, struct cgcs_list_node *y) {
    if (x->m_next != x) {
        if (y->m_next != y) {
            // both x and y are not empty
            struct cgcs_list_node *temp_x_next = x->m_next;
            struct cgcs_list_node *temp_x_prev = x->m_prev;

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

void list_node_reverse(struct cgcs_list_node *x) {
    struct cgcs_list_node *temp = x;

    do {
        struct cgcs_list_node *temp_next = temp->m_next;
        temp->m_next = temp->m_prev;
        temp->m_prev = temp_next;

        temp = temp->m_prev;
    } while (temp != x);
}

void list_node_transfer(struct cgcs_list_node *x, struct cgcs_list_node *first, struct cgcs_list_node *last) {
    if (x != last) {
        // remove [first, last) from its old position
        last->m_prev->m_next = x;
        first->m_prev->m_next = last;
        x->m_prev->m_next = first;

        // splice [first, last) into its new position
        struct cgcs_list_node *temp_x_prev = x->m_prev;
        x->m_prev = last->m_prev;
        last->m_prev = first->m_prev;
        first->m_prev = temp_x_prev;
    }
}

void list_deinit(list_t *self) {
    list_iterator_t it = list_begin(self);

    while (it != list_end(self)) {
        it = list_erase(self, it);
    }

    self->m_impl.m_next = &(self->m_impl);
    self->m_impl.m_prev = &(self->m_impl);
}

void list_deinit_free_fn(list_t *self, void (*freefn)(void *)) {
    list_iterator_t it = list_begin(self);

    while (it != list_end(self)) {
        it = list_erase_free_fn(self, it, freefn);
    }

    self->m_impl.m_next = &(self->m_impl);
    self->m_impl.m_prev = &(self->m_impl);
}

list_iterator_t list_insert(list_t *self, list_iterator_t it, const void *data) {
    struct cgcs_list_node *new_node = list_node_new(data);
    list_node_hook(new_node, it);
    return it;
}

list_iterator_t list_insert_alloc_fn(list_t *self, list_iterator_t it, const void *data, void *(*allocfn)(size_t)) {
    struct cgcs_list_node *new_node = list_node_alloc_fn(data, allocfn);
    list_node_hook(new_node, it);
    return it;
}

list_iterator_t list_erase(list_t *self, list_iterator_t it) {
    list_iterator_t next = it->m_next;
    list_node_unhook(it);
    list_node_delete(it);
    return next;
}

list_iterator_t list_erase_free_fn(list_t *self, list_iterator_t it, void (*freefn)(void *)) {
    list_iterator_t next = it->m_next;
    list_node_unhook(it);
    list_node_delete_free_fn(it, freefn);
    return next;
}

void list_foreach(list_t *self, void (*func)(void *)) {
    for (list_iterator_t it = list_begin(self); it != list_end(self); it = it->m_next) {
        func(&(it->m_data));
    }
}

void list_foreach_range(list_t *self, void (*func)(void *), list_iterator_t beg, list_iterator_t end) {
    for (list_iterator_t it = beg; it != end; it = it->m_next) {
        func(&(it->m_data));
    }
}

list_iterator_t list_find(list_t *self, int (*cmpfn)(const void *, const void *), const void *data) {
    for (list_iterator_t it = list_begin(self); it != list_end(self); it = it->m_next) {
        if (cmpfn(data, &(it->m_data)) == 0) {
            return it;
        }
    }

    return list_end(self);
}

list_iterator_t list_find_range(list_t *self, int (*cmpfn)(const void *, const void *), const void *data, list_iterator_t beg, list_iterator_t end) {
    for (list_iterator_t it = beg; it != end; it = it->m_next) {
        if (cmpfn(data, &(it->m_data)) == 0) {
            return it;
        }
    }

    return list_end(self);
}

list_t *list_new() {
    list_t *list = malloc(sizeof *list);
    assert(list);

    list_init(list);

    return list;
}

list_t *list_new_alloc_fn(void *(*allocfn)(size_t)) {
    list_t *list = allocfn(sizeof *list);
    assert(list);

    list_init(list);

    return list;
}

void list_delete(list_t *l) {
    list_deinit(l);
    free(l);
}

void list_delete_free_fn(list_t *l, void (*freefn)(void *)) {
    list_deinit_free_fn(l, freefn);
    freefn(l);
}
