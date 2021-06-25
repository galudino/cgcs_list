/*!
    \file       cgcs_list.h
    \brief      Header file for doubly-linked list

    \author     Gemuele Aludino
    \date       23 June 2021
 */

#ifndef CGCS_LIST_H
#define CGCS_LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef void *voidptr;

struct cgcs_list_node {
    voidptr m_data;
    
    struct cgcs_list_node *m_next;
    struct cgcs_list_node *m_prev;
};

#define CGCS_LISTNODE_INITIALIZER { NULL, (struct cgcs_listnode *)(0), (struct cgcs_listnode *)(0) }

static void list_node_init(struct cgcs_list_node *self, const void *data);
static void list_node_deinit(struct cgcs_list_node *self);

static void list_node_hook(struct cgcs_list_node *x, struct cgcs_list_node *y);
static void list_node_unhook(struct cgcs_list_node *y);

static struct cgcs_list_node *list_node_new(const void *data);
static struct cgcs_list_node *list_node_alloc_fn(const void *data, void *(*allocfn)(size_t));

static void list_node_delete(struct cgcs_list_node *node);
static void list_node_delete_free_fn(struct cgcs_list_node *node, void (*freefn)(void *));

static struct cgcs_list_node *
list_node_insert(struct cgcs_list_node *x, const void *data);

static struct cgcs_list_node *
list_node_insert_alloc_fn(struct cgcs_list_node *x, const void *data, void *(*allocfn)(size_t));

static struct cgcs_list_node *
list_node_erase(struct cgcs_list_node *x);

static struct cgcs_list_node *
list_node_erase_free_fn(struct cgcs_list_node *x, void (*freefn)(void *));

struct cgcs_list_node *
list_node_clear(struct cgcs_list_node *x, struct cgcs_list_node *y);

struct cgcs_list_node *
list_node_clear_free_fn(struct cgcs_list_node *x, struct cgcs_list_node *y, void (*freefn)(void *));

struct cgcs_list_node *
list_node_find_forward(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (*cmpfn)(const void *, const void *));

struct cgcs_list_node *
list_node_find_forward_b(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (^cmp_b)(const void *, const void *));

struct cgcs_list_node *
list_node_find_backward(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (*cmpfn)(const void *, const void *));

struct cgcs_list_node *
list_node_find_backward_b(struct cgcs_list_node *x, struct cgcs_list_node *y, const void *data, int (^cmpfn)(const void *, const void *));

struct cgcs_list_node *
list_node_advance(struct cgcs_list_node **x, int index);

struct cgcs_list_node *
list_node_get(struct cgcs_list_node *x, int index);

void list_node_swap(struct cgcs_list_node *x, struct cgcs_list_node *y);
void list_node_reverse(struct cgcs_list_node *x);
void list_node_transfer(struct cgcs_list_node *x, struct cgcs_list_node *first, struct cgcs_list_node *last);

static inline void list_node_init(struct cgcs_list_node *self, const void *data) {
    self->m_next = self->m_prev = (struct cgcs_list_node *)(0);
    memcpy(&(self->m_data), data, sizeof(void *));
}

static inline void list_node_deinit(struct cgcs_list_node *self) {
    self->m_next = self->m_prev = (struct cgcs_list_node *)(0);
}

static inline void list_node_hook(struct cgcs_list_node *x, struct cgcs_list_node *y) {
    // x is new node, y is the desired position
    x->m_next = y;
    x->m_prev = y->m_prev;

    y->m_prev->m_next = x;
    y->m_prev = x;
}

static inline void list_node_unhook(struct cgcs_list_node *y) {
    y->m_prev->m_next = y->m_next;
    y->m_next->m_prev = y->m_prev;
}

static inline
struct cgcs_list_node *list_node_new(const void *data) {
    struct cgcs_list_node *node = malloc(sizeof *node);
    assert(node);

    list_node_init(node, data);
    return node;
}

static inline
struct cgcs_list_node *list_node_alloc_fn(const void *data, void *(*allocfn)(size_t)) {
    struct cgcs_list_node *node = allocfn(sizeof *node);
    assert(node);

    list_node_init(node, data);
    return node;
}

static inline 
void list_node_delete(struct cgcs_list_node *node) {
    list_node_deinit(node);
    free(node);
}

static inline 
void list_node_delete_free_fn(struct cgcs_list_node *node, void (*freefn)(void *)) {
    list_node_deinit(node);
    freefn(node);
}

static inline struct cgcs_list_node *
list_node_insert(struct cgcs_list_node *x, const void *data) {
    struct cgcs_list_node *new_node = list_node_new(data);
    list_node_hook(new_node, x);
    return x;
}

static inline struct cgcs_list_node *
list_node_insert_alloc_fn(struct cgcs_list_node *x, const void *data, void *(*allocfn)(size_t)) {
    struct cgcs_list_node *new_node = list_node_alloc_fn(data, allocfn);
    list_node_hook(new_node, x);
    return x;
}

static inline struct cgcs_list_node *
list_node_erase(struct cgcs_list_node *x) {
    struct cgcs_list_node *next = x->m_next;
    list_node_unhook(x);
    list_node_delete(x);
    return next;
}

static inline struct cgcs_list_node *
list_node_erase_free_fn(struct cgcs_list_node *x, void (*freefn)(void *)) {
    struct cgcs_list_node *next = x->m_next;
    list_node_unhook(x);
    list_node_delete_free_fn(x, freefn);
    return next;
}

typedef struct cgcs_list list_t;
typedef struct cgcs_list_node *list_iterator_t;

struct cgcs_list {
    struct cgcs_list_node m_impl;
};

#define CGCS_LIST_INITIALIZER { CGCS_LISTNODE_INITIALIZER }

static void list_init(list_t *self);

void list_deinit(list_t *self);
void list_deinit_free_fn(list_t *self, void (*freefn)(void *));

static voidptr list_front(list_t *self);
static voidptr list_back(list_t *self);

static bool list_empty(list_t *self);

static list_iterator_t list_begin(list_t *self);
static list_iterator_t list_before_end(list_t *self);
static list_iterator_t list_end(list_t *self);

list_iterator_t list_insert(list_t *self, list_iterator_t it, const void *data);
list_iterator_t list_insert_alloc_fn(list_t *self, list_iterator_t it, const void *data, void *(*allocfn)(size_t));
list_iterator_t list_erase(list_t *self, list_iterator_t it);
list_iterator_t list_erase_free_fn(list_t *self, list_iterator_t it, void (*freefn)(void *));

static void list_push_front(list_t *self, const void *data);
static void list_push_front_alloc_fn(list_t *self, const void *data, void *(*allocfn)(size_t));
static void list_pop_front(list_t *self);
static void list_pop_front_free_fn(list_t *self, void (*freefn)(void *));

static void list_push_back(list_t *self, const void *data);
static void list_push_back_alloc_fn(list_t *self, const void *data, void *(*allocfn)(size_t));
static void list_pop_back(list_t *self);
static void list_pop_back_free_fn(list_t *self, void (*freefn)(void *));

void list_foreach(list_t *self, void (*func)(void *));
void list_foreach_range(list_t *self, void (*func)(void *), list_iterator_t beg, list_iterator_t end);

list_iterator_t list_find(list_t *self, int (*cmpfn)(const void *, const void *), const void *data);
list_iterator_t list_find_range(list_t *self, int (*cmpfn)(const void *, const void *), const void *data, list_iterator_t beg, list_iterator_t end);

list_t *list_new();
list_t *list_new_alloc_fn(void *(*allocfn)(size_t));

void list_delete(list_t *l);
void list_delete_free_fn(list_t *l, void (*freefn)(void *));

static inline void list_init(list_t *self) {
    self->m_impl.m_next = &(self->m_impl);
    self->m_impl.m_prev = &(self->m_impl);
}

static inline voidptr list_front(list_t *self) {
    return list_begin(self)->m_data;
}

static inline voidptr list_back(list_t *self) {
    return self->m_impl.m_prev->m_data;
}

static inline bool list_empty(list_t *self) {
    return list_begin(self) == list_end(self);
}

static inline list_iterator_t list_begin(list_t *self) {
    return self->m_impl.m_next;
}

static inline list_iterator_t list_before_end(list_t *self) {
    return self->m_impl.m_prev;
}

static inline list_iterator_t list_end(list_t *self) {
    return &(self->m_impl);
}

static inline void list_push_front(list_t *self, const void *data) {
    list_insert(self, list_begin(self), data);
}

static inline void list_push_front_alloc_fn(list_t *self, const void *data, void *(*allocfn)(size_t)) {
    list_insert_alloc_fn(self, list_begin(self), data, allocfn);
}

static inline void list_pop_front(list_t *self) {
    list_erase(self, list_begin(self));
}

static inline void list_pop_front_free_fn(list_t *self, void (*freefn)(void *)) {
    list_erase_free_fn(self, list_begin(self), freefn);
}

static inline void list_push_back(list_t *self, const void *data) {
    list_insert(self, list_end(self), data);
}

static inline void list_push_back_alloc_fn(list_t *self, const void *data, void *(*allocfn)(size_t)) {
    list_insert_alloc_fn(self, list_end(self), data, allocfn);
}

static inline void list_pop_back(list_t *self) {
    list_erase(self, list_before_end(self));
}

static inline void list_pop_back_free_fn(list_t *self, void (*freefn)(void *)) {
    list_erase_free_fn(self, list_before_end(self), freefn);
}

#endif /* CGCS_LIST_H */
