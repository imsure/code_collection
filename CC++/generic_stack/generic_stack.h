#ifndef _GENERIC_STACK_H
#define _GENERIC_STACK_H

/*
  Interface of the generic stack in C.
*/

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRE_ALLOC_SZ 4

/* User should ignore the interals of exposed
   data structure. Though this is gloable visible. */
typedef struct {
  void *sp; /* stack pointer */
  int elem_size;
  /* insertion index */
  int logical_len; /* how many elements actually on stack. */
  int alloc_len; /* number of elements allocated */
  void (*free_func)(void *);
}stack;

/* Interface for manipulating stack */
void stack_new(stack *s, int elem_size,
	       void (*free_func)(void *));
void stack_dispose(stack *s);
bool stack_empty(const stack *s);
void stack_push(stack *s, const void *elem_addr);
void stack_pop(stack *s, void *elem_addr);

#endif
