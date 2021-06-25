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

struct cgcs_listnode {
    voidptr m_data;
    struct cgcs_listnode *m_next;
    struct cgcs_listnode *m_prev;
};

#define CGCS_LISTNODE_INITIALIZER { NULL, (struct cgcs_listnode *)(0), (struct cgcs_listnode *)(0) }

static void cgcs_lninit(struct cgcs_listnode *self, const void *data);
static void cgcs_lndeinit(struct cgcs_listnode *self);

static void cgcs_lnhook(struct cgcs_listnode *x, struct cgcs_listnode *y);
static void cgcs_lnunhook(struct cgcs_listnode *y);

static struct cgcs_listnode *cgcs_lnnew(const void *data);
static struct cgcs_listnode *cgcs_lnnew_allocfn(const void *data, void *(*allocfn)(size_t));

static void cgcs_lndelete(struct cgcs_listnode *node);
static void cgcs_lndelete_freefn(struct cgcs_listnode *node, void (*freefn)(void *));

static struct cgcs_listnode *
cgcs_lninsert(struct cgcs_listnode *x, const void *data);

static struct cgcs_listnode *
cgcs_lninsert_allocfn(struct cgcs_listnode *x, const void *data, void *(*allocfn)(size_t));

static struct cgcs_listnode *
cgcs_lnerase(struct cgcs_listnode *x);

static struct cgcs_listnode *
cgcs_lnerase_freefn(struct cgcs_listnode *x, void (*freefn)(void *));

struct cgcs_listnode *
cgcs_lnclear(struct cgcs_listnode *x, struct cgcs_listnode *y);

struct cgcs_listnode *
cgcs_lnclear_freefn(struct cgcs_listnode *x, struct cgcs_listnode *y, void (*freefn)(void *));

struct cgcs_listnode *
cgcs_lnfindfwd(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (*cmpfn)(const void *, const void *));

struct cgcs_listnode *
cgcs_lnfindfwd_b(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (^cmp_b)(const void *, const void *));

struct cgcs_listnode *
cgcs_lnfindbkw(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (*cmpfn)(const void *, const void *));

struct cgcs_listnode *
cgcs_lnfindbkw_b(struct cgcs_listnode *x, struct cgcs_listnode *y, const void *data, int (^cmpfn)(const void *, const void *));

struct cgcs_listnode *
cgcs_lnadvance(struct cgcs_listnode **x, int index);

struct cgcs_listnode *
cgcs_lngetnode(struct cgcs_listnode *x, int index);

void cgcs_lnswap(struct cgcs_listnode *x, struct cgcs_listnode *y);
void cgcs_lnreverse(struct cgcs_listnode *x);
void cgcs_lntransfer(struct cgcs_listnode *x, struct cgcs_listnode *first, struct cgcs_listnode *last);

static inline void cgcs_lninit(struct cgcs_listnode *self, const void *data) {
    self->m_next = self->m_prev = (struct cgcs_listnode *)(0);
    memcpy(&(self->m_data), data, sizeof(void *));
}

static inline void cgcs_lndeinit(struct cgcs_listnode *self) {
    self->m_next = self->m_prev = (struct cgcs_listnode *)(0);
}

static inline void cgcs_lnhook(struct cgcs_listnode *x, struct cgcs_listnode *y) {
    // x is new node, y is the desired position
    x->m_next = y;
    x->m_prev = y->m_prev;

    y->m_prev->m_next = x;
    y->m_prev = x;
}

static inline void cgcs_lnunhook(struct cgcs_listnode *y) {
    y->m_prev->m_next = y->m_next;
    y->m_next->m_prev = y->m_prev;
}

static inline
struct cgcs_listnode *cgcs_lnnew(const void *data) {
    struct cgcs_listnode *node = malloc(sizeof *node);
    assert(node);

    cgcs_lninit(node, data);
    return node;
}

static inline
struct cgcs_listnode *cgcs_lnnew_allocfn(const void *data, void *(*allocfn)(size_t)) {
    struct cgcs_listnode *node = allocfn(sizeof *node);
    assert(node);

    cgcs_lninit(node, data);
    return node;
}

static inline 
void cgcs_lndelete(struct cgcs_listnode *node) {
    cgcs_lndeinit(node);
    free(node);
}

static inline 
void cgcs_lndelete_freefn(struct cgcs_listnode *node, void (*freefn)(void *)) {
    cgcs_lndeinit(node);
    freefn(node);
}

static inline struct cgcs_listnode *
cgcs_lninsert(struct cgcs_listnode *x, const void *data) {
    struct cgcs_listnode *new_node = cgcs_lnnew(data);
    cgcs_lnhook(new_node, x);
    return x;
}

static inline struct cgcs_listnode *
cgcs_lninsert_allocfn(struct cgcs_listnode *x, const void *data, void *(*allocfn)(size_t)) {
    struct cgcs_listnode *new_node = cgcs_lnnew_allocfn(data, allocfn);
    cgcs_lnhook(new_node, x);
    return x;
}

static inline struct cgcs_listnode *
cgcs_lnerase(struct cgcs_listnode *x) {
    struct cgcs_listnode *next = x->m_next;
    cgcs_lnunhook(x);
    cgcs_lndelete(x);
    return next;
}

static inline struct cgcs_listnode *
cgcs_lnerase_freefn(struct cgcs_listnode *x, void (*freefn)(void *)) {
    struct cgcs_listnode *next = x->m_next;
    cgcs_lnunhook(x);
    cgcs_lndelete_freefn(x, freefn);
    return next;
}

typedef struct cgcs_list cgcs_list;
typedef struct cgcs_listnode *cgcs_list_iterator;

struct cgcs_list {
    struct cgcs_listnode m_impl;
};

#define CGCS_LIST_INITIALIZER { CGCS_LISTNODE_INITIALIZER }

static void cgcs_linit(cgcs_list *self);

void cgcs_ldeinit(cgcs_list *self);
void cgcs_ldeinit_freefn(cgcs_list *self, void (*freefn)(void *));

static voidptr cgcs_lfront(cgcs_list *self);
static voidptr cgcs_lback(cgcs_list *self);

static bool cgcs_lempty(cgcs_list *self);

static cgcs_list_iterator cgcs_lbegin(cgcs_list *self);
static cgcs_list_iterator cgcs_lbefend(cgcs_list *self);
static cgcs_list_iterator cgcs_lend(cgcs_list *self);

cgcs_list_iterator cgcs_linsert(cgcs_list *self, cgcs_list_iterator it, const void *data);
cgcs_list_iterator cgcs_linsert_allocfn(cgcs_list *self, cgcs_list_iterator it, const void *data, void *(*allocfn)(size_t));
cgcs_list_iterator cgcs_lerase(cgcs_list *self, cgcs_list_iterator it);
cgcs_list_iterator cgcs_lerase_freefn(cgcs_list *self, cgcs_list_iterator it, void (*freefn)(void *));

static void cgcs_lpushf(cgcs_list *self, const void *data);
static void cgcs_lpushf_allocfn(cgcs_list *self, const void *data, void *(*allocfn)(size_t));
static void cgcs_lpopf(cgcs_list *self);
static void cgcs_lpopf_freefn(cgcs_list *self, void (*freefn)(void *));

static void cgcs_lpushb(cgcs_list *self, const void *data);
static void cgcs_lpushb_allocfn(cgcs_list *self, const void *data, void *(*allocfn)(size_t));
static void cgcs_lpopb(cgcs_list *self);
static void cgcs_lpopb_freefn(cgcs_list *self, void (*freefn)(void *));

void cgcs_lforeach(cgcs_list *self, void (*func)(void *));
void cgcs_lforeach_range(cgcs_list *self, void (*func)(void *), cgcs_list_iterator beg, cgcs_list_iterator end);

cgcs_list_iterator cgcs_lfind(cgcs_list *self, int (*cmpfn)(const void *, const void *), const void *data);
cgcs_list_iterator cgcs_lfind_range(cgcs_list *self, int (*cmpfn)(const void *, const void *), const void *data, cgcs_list_iterator beg, cgcs_list_iterator end);

cgcs_list *cgcs_lnew();
cgcs_list *cgcs_lnew_allocfn(void *(*allocfn)(size_t));

void cgcs_ldelete(cgcs_list *l);
void cgcs_ldelete_freefn(cgcs_list *l, void (*freefn)(void *));

static inline void cgcs_linit(cgcs_list *self) {
    self->m_impl.m_next = &(self->m_impl);
    self->m_impl.m_prev = &(self->m_impl);
}

static inline voidptr cgcs_lfront(cgcs_list *self) {
    return cgcs_lbegin(self)->m_data;
}

static inline voidptr cgcs_lback(cgcs_list *self) {
    return self->m_impl.m_prev->m_data;
}

static inline bool cgcs_lempty(cgcs_list *self) {
    return cgcs_lbegin(self) == cgcs_lend(self);
}

static inline cgcs_list_iterator cgcs_lbegin(cgcs_list *self) {
    return self->m_impl.m_next;
}

static inline cgcs_list_iterator cgcs_lbefend(cgcs_list *self) {
    return self->m_impl.m_prev;
}

static inline cgcs_list_iterator cgcs_lend(cgcs_list *self) {
    return &(self->m_impl);
}

static inline void cgcs_lpushf(cgcs_list *self, const void *data) {
    cgcs_linsert(self, cgcs_lbegin(self), data);
}

static inline void cgcs_lpushf_allocfn(cgcs_list *self, const void *data, void *(*allocfn)(size_t)) {
    cgcs_linsert_allocfn(self, cgcs_lbegin(self), data, allocfn);
}

static inline void cgcs_lpopf(cgcs_list *self) {
    cgcs_lerase(self, cgcs_lbegin(self));
}

static inline void cgcs_lpopf_freefn(cgcs_list *self, void (*freefn)(void *)) {
    cgcs_lerase_freefn(self, cgcs_lbegin(self), freefn);
}

static inline void cgcs_lpushb(cgcs_list *self, const void *data) {
    cgcs_linsert(self, cgcs_lend(self), data);
}

static inline void cgcs_lpushb_allocfn(cgcs_list *self, const void *data, void *(*allocfn)(size_t)) {
    cgcs_linsert_allocfn(self, cgcs_lend(self), data, allocfn);
}

static inline void cgcs_lpopb(cgcs_list *self) {
    cgcs_lerase(self, cgcs_lbefend(self));
}

static inline void cgcs_lpopb_freefn(cgcs_list *self, void (*freefn)(void *)) {
    cgcs_lerase_freefn(self, cgcs_lbefend(self), freefn);
}

#ifdef USE_CGCS_LIST
# define USE_CGCS_LIST

#define lninit(self, data)          cgcs_lninit(self, data)
#define lndeinit(self)              cgcs_lndeinit(self)

#define lnnew(data)                     cgcs_lnnew(data)
#define lnnew_allocfn(data, allocfn)    cgcs_lnnew_allocfn(data, allocfn)

#define lndelete(node)                  cgcs_lndelete(node)
#define lndelete_freefn(node, freefn)   cgcs_lndelete_freefn(node, freefn)

#define lninsert(x, data)                   cgcs_lninsert(x, data)
#define lninsert_allocfn(x, data, allocfn)  cgcs_lninsert_allocfn(x, data, allocfn)

#define lnerase(x)                  cgcs_lnerase(x)
#define lnerase_freefn(x, freefn)   cgcs_lnerase_freefn(x)

#define lnclear(x, y)                   cgcs_lnclear(x, y)
#define lnclear_freefn(x, y, freefn)    cgcs_lnclear_freefn(x, y, freefn)

#define lnfindfwd(x, y, data, cmpfn) cgcs_lnfindfwd(x, y, data, cmpfn)
#define lnfindfwd_b(x, y, data, cmp_b) cgcs_lnfindfwd(x, y, data, cmp_b)
#define lnfindbkw(x, y, data, cmpfn) cgcs_lnfindbkw(x, y, data, cmpfn)
#define lnfindbkw_b(x, y, data, cmp_b) cgcs_lnfindbkw(x, y, data, cmp_b)

#define lnadvance(x, index) cgcs_lnadvance(x, index)
#define lngetnode(x, index) cgcs_lngetnode(x, index)

#define lnswap(x, y)    cgcs_lnswap(x, y)
#define lnreverse(x)    cgcs_lnreverse(x)
#define lntransfer(x, first, last) cgcs_lntransfer(x, first, last)

#define linit(self) cgcs_linit(self)
#define ldeinit(self) cgcs_ldeinit(self)
#define ldeinit_freefn(self, freefn) cgcs_ldeinit_freefn(self, freefn)

#define lfront(self) cgcs_lfront(self)
#define lback(self) cgcs_lback(self)

#define lbegin(self) cgcs_lbegin(self)
#define lbefend(self) cgcs_lbefend(self)
#define lend(self) cgcs_lend(self)

#define linsert(self, it, data) cgcs_linsert(self, it, data)
#define linsert_allocfn(self, it, data, allocfn) cgcs_linsert_allocfn(self, it, data, allocfn)
#define lerase(self, it) cgcs_lerase(self, it)
#define lerase_freefn(self, it, freefn) cgcs_lerase_freefn(self, it, freefn)

#define lpushf(self, data) cgcs_lpushf(self, data)
#define lpushf_allocfn(self, data, allocfn) cgcs_lpushf_allocfn(self, data, allocfn)
#define lpopf(self) cgcs_lpopf(self)
#define lpopf_freefn(self, freefn) cgcs_lpopf_freefn(self, freefn)

#define lpushb(self, data) cgcs_lpushb(self, data)
#define lpushb_allocfn(self, data, allocfn) cgcs_lpushb_allocfn(self, data, allocfn)
#define lpopb(self) cgcs_lpopb(self)
#define lpopb_freefn(self, freefn) cgcs_lpopb_freefn(self, freefn)

#define lforeach(self, func) cgcs_lforeach(self, func)
#define lforeach_range(self, func, beg, end) cgcs_lforeach_range(self, func, beg, end)

#define lfind(self, cmpfn, data) cgcs_lfind(self, cmpfn, data)
#define lfind_range(self, cmpfn, data, beg, end) cgcs_lfind_range(self, cmpfn, data, beg, end)

#define lnew() cgcs_lnew()
#define lnew_allocfn(allocfn) cgcs_lnew_allocfn(allocfn)

#define ldelete(l) cgcs_ldelete(l)
#define ldelete_freefn(l, freefn) cgcs_ldelete_freefn(l, freefn)

#endif /* USE_CGCS_LIST */

#endif /* CGCS_LIST_H */
