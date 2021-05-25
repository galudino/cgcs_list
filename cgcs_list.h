/*!
    \file       cgcs_list.h
    \brief      Header file for singly/doubly linked lists

    \author     Gemuele Aludino
    \date       24 May 2021

    TODO: Documentation, more comments
 */

#ifndef CGCS_LIST_H
#define CGCS_LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef void *voidptr;

struct cgcs_snode {
    voidptr m_data;
    struct cgcs_snode *m_next;
};

#define CGCS_SNODE_INITIALIZER { ((struct cgcs_snode *)(0)), NULL }

static void cgcs_sninit(struct cgcs_snode *self, const void *data);
static void cgcs_sndeinit(struct cgcs_snode *self);

static void cgcs_snhookaft(struct cgcs_snode *self, struct cgcs_snode *pos);
static void cgcs_snunhookaft(struct cgcs_snode *self);

struct cgcs_snode *cgcs_snnew(const void *data);
struct cgcs_snode *cgcs_snnew_allocfn(const void *data,
                                          void *(*allocfn)(size_t));
static void cgcs_sndelete(struct cgcs_snode *node);
static void cgcs_sndelete_freefn(struct cgcs_snode *node,
                                     void (*freefn)(void *));

static inline void
cgcs_sninit(struct cgcs_snode *self, const void *data) {
    self->m_next = (struct cgcs_snode *)(0);
    // data is the address of a (T *).
    // Therefore, we pass &(self->m_data) to memcpy --
    // which is the address of a (void *).
    memcpy(&(self->m_data), data, sizeof(void *));
}

static inline void
cgcs_sndeinit(struct cgcs_snode *self) {
    self->m_next = (struct cgcs_snode *)(0);
}

static inline void
cgcs_snhookaft(struct cgcs_snode *self,
                struct cgcs_snode *pos) {
    // "Hook" node self after node pos.
    self->m_next = pos->m_next;
    pos->m_next = self;
    // Node self's new neighbor is now pos's former neighbor.
    // pos's current neighbor is now self.
}

static inline void
cgcs_snunhookaft(struct cgcs_snode *self) {
    self->m_next = self->m_next->m_next;
    // The node formerly after self (formerly self->m_next) 
    // is now "unhooked" from the list.
    // The caller should save self->m_next prior to this code.
}

static inline void
cgcs_sndelete(struct cgcs_snode *node) {
    // Destruction of node->m_data should be handled by the caller
    // prior to calling this function.
    cgcs_sndeinit(node);
    free(node);
}

static inline void
cgcs_sndelete_freefn(struct cgcs_snode *node, void (*freefn)(void *)) {
    cgcs_sndeinit(node);
    freefn(node);
}

typedef struct cgcs_slist cgcs_slist;
typedef struct cgcs_snode *cgcs_slist_iterator;

struct cgcs_slist {
    struct cgcs_snode m_impl;
};

#define CGCS_SLIST_INITIALIZER { CGCS_SNODE_INITIALIZER }

static void cgcs_slinit(cgcs_slist *self);

void cgcs_sldeinit(cgcs_slist *self);
void cgcs_sldeinit_freefn(cgcs_slist *self, 
                          void (*freefn)(void *));

static voidptr cgcs_slfront(cgcs_slist *self);
static bool cgcs_slempty(cgcs_slist *self);

static cgcs_slist_iterator cgcs_slbefbegin(cgcs_slist *self);
static cgcs_slist_iterator cgcs_slbegin(cgcs_slist *self);
static cgcs_slist_iterator cgcs_slend(cgcs_slist *self);

cgcs_slist_iterator cgcs_slinsertaft(cgcs_slist *self,
                                     cgcs_slist_iterator it,
                                     const void *data);

cgcs_slist_iterator cgcs_slinsertaft_allocfn(cgcs_slist *self,
                                             cgcs_slist_iterator it,
                                             const void *data,
                                             void *(*allocfn)(size_t));

cgcs_slist_iterator cgcs_sleraseaft(cgcs_slist *self,
                                    cgcs_slist_iterator it);

cgcs_slist_iterator cgcs_sleraseaft_freefn(cgcs_slist *self,
                                           cgcs_slist_iterator it,
                                           void (*freefn)(void *));

static void cgcs_slpushf(cgcs_slist *self, const void *data);

static void cgcs_slpushf_allocfn(cgcs_slist *self, 
                                 const void *data,
                                 void *(*allocfn)(size_t));

static void cgcs_slpopf(cgcs_slist *self);
static void cgcs_slpopf_freefn(cgcs_slist *self, void (*freefn)(void *));

void cgcs_slforeach(cgcs_slist *self, void (*func)(void *));

void cgcs_slforeach_range(cgcs_slist *self, void (*func)(void *),
                          cgcs_slist_iterator beg,
                          cgcs_slist_iterator end);

cgcs_slist_iterator cgcs_slfind(cgcs_slist *self,
                                  int (*cmpfn)(const void *, const void *),
                                  const void *data);

cgcs_slist_iterator cgcs_slfind_range(cgcs_slist *self,
                                        int (*cmpfn)(const void *, const void *),
                                        const void *data,
                                        cgcs_slist_iterator beg,
                                        cgcs_slist_iterator end);

cgcs_slist *cgcs_slnew();
cgcs_slist *cgcs_slnew_allocfn(void *(*allocfn)(size_t));

void cgcs_sldelete(cgcs_slist *sl);
void cgcs_sldelete_freefn(cgcs_slist *sl, void (*freefn)(void *));

static inline void
cgcs_slinit(cgcs_slist *self) {
    self->m_impl.m_next = cgcs_slend(self);
    self->m_impl.m_data = NULL;
}

static inline voidptr
cgcs_slfront(cgcs_slist *self) {
    return &(cgcs_slbegin(self)->m_data);
}

static inline bool cgcs_slempty(cgcs_slist *self) {
    return cgcs_slbegin(self) == cgcs_slend(self);
}

static inline cgcs_slist_iterator
cgcs_slbefbegin(cgcs_slist *self) {
    return ((cgcs_slist_iterator)(&(self->m_impl)));
}

static inline cgcs_slist_iterator
cgcs_slbegin(cgcs_slist *self) {
    return ((cgcs_slist_iterator)(self->m_impl.m_next));
}

static inline cgcs_slist_iterator
cgcs_slend(cgcs_slist *self) {
    return ((cgcs_slist_iterator)(0));
}

static inline void
cgcs_slpushf(cgcs_slist *self, const void *data) {
    cgcs_slinsertaft(self, cgcs_slbefbegin(self), data);
}

static inline void
cgcs_slpushf_allocfn(cgcs_slist *self, 
                     const void *data,
                     void *(*allocfn)(size_t)) {
    cgcs_slinsertaft_allocfn(self, cgcs_slbefbegin(self), data, allocfn);
}

static inline void
cgcs_slpopf(cgcs_slist *self) {
    cgcs_sleraseaft(self, cgcs_slbefbegin(self));
}

static inline void
cgcs_slpopf_freefn(cgcs_slist *self, void (*freefn)(void *)) {
    cgcs_sleraseaft_freefn(self, cgcs_slbefbegin(self), freefn);
}

#ifdef USE_CGCS_SLIST
# define USE_CGCS_SLIST

typedef cgcs_slist slist;
typedef cgcs_slist_iterator slist_iterator;

// TODO: More shorthand notation, as per cgcs_vector

#endif /* USE_CGCS_SLIST */

#endif /* CGCS_LIST_H */
