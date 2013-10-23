/*
  Implementation of the generic stack in C.
*/

#include "generic_stack.h"

/* Create a new stack with elements size of 'elem_size'.
 'free_func' is a user defined callback function used
 to free dynamical allocated memory pointed by stack
 elements if necessary. */
void stack_new(stack *s, int elem_size,
	       void (*free_func)(void *))
{
  assert(elem_size > 0);
  s->elem_size = elem_size;
  s->logical_len = 0; /* no element on stack yet */
  s->alloc_len = PRE_ALLOC_SZ; /* 4 is an arbitrary value */
  s->free_func = free_func;
  s->sp = malloc(PRE_ALLOC_SZ * elem_size);
  assert(s->sp);
}

/* Dispose a stack */
void stack_dispose(stack *s)
{
  /* To see if there is memory holded by elements that
     needs to be free. */
  if (s->free_func) {
    for (int i = 0; i < s->logical_len; i++) {
      void *obj = (char *)s->sp + i * s->elem_size;
      s->free_func(obj);
    }
  }
  free(s->sp);
}

/* Return ture if stack is empty, else false. */
bool stack_empty(const stack *s)
{
  return (s->logical_len == 0);
}

static void stack_grow(stack *s)
{
  /* Out of space. Double the orginal allocation. */
  s->alloc_len <<= 1; /* double strategy is popular since it
			 grows stack size by power of 2
			 each time. You don't have to call
			 realloc often since realloc is kind
			 of time consuming. */
  s->sp = realloc(s->sp, s->alloc_len * s->elem_size);
  assert(s->sp != NULL);
}

/* Push an element whose address is 'elem_addr' onto stack */
void stack_push(stack *s, const void *elem_addr)
{
  assert(elem_addr != NULL);
  
  if (s->logical_len == s->alloc_len) {
    stack_grow(s);
  }

  void *target = (char *)s->sp + s->logical_len * s->elem_size;
  memcpy(target, elem_addr, s->elem_size);
  s->logical_len++;
}

/* Pop up an element from stack and
   store it at address 'elem_addr' */
void stack_pop(stack *s, void *elem_addr)
{
  if (stack_empty(s)) {
    printf("Empty Stack!\n");
  } else {
    void *src = (char *)s->sp +
      (s->logical_len-1) * s->elem_size;
    memcpy(elem_addr, src, s->elem_size);
    s->logical_len--;
  }
}
