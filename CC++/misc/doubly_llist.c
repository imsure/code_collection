/*
  A simple test for doubly linked list.
  Especially focus on finding the addres of struct that
  embed list element.
*/

#include <stdio.h>
#include <stddef.h> /* offsetof, size_t, etc */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h> /* uint8_t, etc */

/*
  Get the address of the instance of struct 'name'
  in which 'member' is embedded in given the address
  of 'member', that is, 'ptr'.
*/
#define list_entry_v1(ptr, name, member) \
  ((struct name *)((uint8_t *)ptr - offsetof(struct name, member)))

/* v2 and v3 seem to be trival */
#define list_entry_v2(ptr, name, member) \
  ((struct name *)((uint8_t *)&(ptr)->next - offsetof(struct name, member.next)))

#define list_entry_v3(ptr, name, member) \
  ((struct name *)((uint8_t *)&(ptr)->prev - offsetof(struct name, member.prev)))

/*
  Get the address of the object 'obj' in which
  'member' is embedded in given the address of
  'member' which is 'ptr'.
*/
#define container_of(ptr, obj, member) \
  ({ typeof(obj) *_obj = (typeof(obj) *)(0); \
  uint8_t *off = (uint8_t *)&(_obj)->member; \
  (typeof(obj) *)((uint8_t *)ptr - off); })

struct list_elem {
  struct list_elem *prev;
  struct list_elem *next;
};

struct list {
  struct list_elem head;
  struct list_elem tail;
};

/* return the head of the list */
inline struct list_elem *
list_head(struct list *list)
{
  assert(list != NULL);
  return &(list->head);
}

/* return the tail of the list */
inline struct list_elem *
list_tail(struct list *list)
{
  assert(list != NULL);
  return &(list->tail);
}

/* return TRUE if 'elem' is the head of a list */
inline bool
is_head(struct list_elem *elem)
{
  return elem->prev == NULL;
}

inline bool
is_tail(struct list_elem *elem)
{
  return elem->next == NULL;
}

inline bool
is_interior(struct list_elem *elem)
{
  return (elem->prev != NULL && elem->next != NULL);
}

/* return the next element of 'elem' */
inline struct list_elem *
list_next(struct list_elem *elem)
{
  assert(is_head(elem) || is_interior(elem));
  return elem->next;
}

inline struct list_elem *
list_prev(struct list_elem *elem)
{
  assert(is_tail(elem) || is_interior(elem));
  return elem->prev;
}

void
list_init(struct list *list)
{
  assert(list != NULL);

  list->head.prev = NULL;
  list->head.next = &(list->tail);
  list->tail.next = NULL;
  list->tail.prev = &(list->head);
}

void
list_insert(struct list *list,
	    struct list_elem *elem)
{
  assert(list != NULL && elem != NULL);

  /* back up pointer to the element before the tail */
  struct list_elem *prev = list->tail.prev;

  /* link new element 'elem' with 'prev' */ 
  prev->next = elem;
  elem->prev = prev;
  /* link new element 'elem' with the tail */
  elem->next = &(list->tail);
  list->tail.prev = elem;
}

void
list_print_address(struct list *list, bool reverse)
{
  assert(list != NULL);

  if (!reverse) { /* forward iteration */
    struct list_elem *e = list_head(list);
    printf("head:\n%p", e);
    for (; e != list_tail(list); e = list_next(e)) {
      printf("-->%p", e->next);
    }
    printf("\ntail\n");
  } else { /* backward iteration */
    struct list_elem *e = list_tail(list);
    printf("tail:\n%p", e);
    for (; e != list_head(list); e = list_prev(e)) {
      printf("-->%p", e->prev);
    }
    printf("\nhead\n");
  }
}

struct list_elem b;

int
main(int argc, char *argv[])
{
  struct list list;
  list_init(&list);

  struct list_elem a;
  static struct list_elem c;
  list_insert(&list, &a);
  list_insert(&list, &b);
  list_insert(&list, &c);

  list_print_address(&list, false);
  list_print_address(&list, true);

  struct foo {
    int bar;
    struct list_elem elem;
  };
  
  struct list list_foo;
  list_init(&list_foo);

  struct foo f1, f2;
  f1.bar = 5; f2.bar = 10;
  list_insert(&list_foo, &f1.elem);
  list_insert(&list_foo, &f2.elem);
  list_print_address(&list_foo, false);
  
  struct foo *tmp = list_entry_v1(&f1.elem, foo, elem);
  printf("tmp: %p, bar: %d\n", tmp, tmp->bar);
  tmp = list_entry_v2(&f1.elem, foo, elem);
  printf("tmp: %p, bar: %d\n", tmp, tmp->bar);
  tmp = list_entry_v3(&f1.elem, foo, elem);
  printf("tmp: %p, bar: %d\n", tmp, tmp->bar);

  tmp = container_of(&f1.elem, f1, elem);
  printf("tmp: %p, bar: %d\n", tmp, tmp->bar);

  tmp = container_of(&f2.elem, f2, elem);
  printf("tmp: %p, bar: %d\n", tmp, tmp->bar);

  return 0;
}
