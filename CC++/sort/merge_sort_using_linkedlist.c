/*
  Merge Sort :
  an application of Divide-and-Conquer.
  Using linked list instead of array to provide a thorough
  recursive implementation.
  
  Algorithm:
  1. Split list into sub-lists with only one element
     which is sorted by default.
  2. Repeatedly merge these sub-lists to produce new
     sub-lists until there is only one list remaining
     (this will be sorted list).
  
  Pseudo Code (from: http://en.wikipedia.org/wiki/Merge_sort):
  function merge_sort(list m)
    // if list size is 1, consider it sorted and return it
    if length(m) <= 1
        return m
    // else list size is > 1, so split the list into two sublists
    var list left, right
    var integer middle = length(m) / 2
    for each x in m before middle
         add x to left
    for each x in m after or equal middle
         add x to right
    // recursively call merge_sort() to further split each sublist
    // until sublist size is 1
    left = merge_sort(left)
    right = merge_sort(right)
    // merge the sublists returned from prior calls to merge_sort()
    // and return the resulting merged sublist
    return merge(left, right)
*/

#include <stdio.h>
#include <stdlib.h>

#define DefCell(elm_type, cell_type, list_type) \
  typedef struct cell_type cell_type;		\
  typedef struct cell_type * list_type;         \
  struct cell_type {                            \
    elm_type element;                           \
    list_type next;                             \
  };

DefCell(int, Cell, List);

List make_list ();
void print_list (List l);
List merge_sort (List l);
List split (List l);
List merge (List left, List right);

int main ()
{
  printf ("Enter integer elements for the list.\n"
	  "Separate them by Carriage Return.\n"
	  "End input with EOF (Crtl + d)\n");
  List l = make_list ();
  printf ("Unsorted List:\n");
  print_list (l);
  printf ("Sorted List:\n");
  print_list (merge_sort(l));
  
  return 0;
}

/*
  produce a list recursively
*/
List make_list ()
{
  int x;

  if (scanf("%d", &x) == EOF) return NULL;
  else {
    List p_new_cell = (List) malloc (sizeof(Cell));
    p_new_cell->element = x;
    p_new_cell->next = make_list ();
    return p_new_cell;
  }
}

void print_list (List l)
{
  if (l == NULL) printf ("Empty List!!!");
  else {
    while (l != NULL) {
      printf ("%d ", l->element);
      l = l->next;
    }
  }
  printf ("\n");
}

/*
  Split then merge.
  All by recursion.
*/
List merge_sort (List l)
{
  List half;
    
  if (l == NULL) return l; /* length is 0 */
  else if (l->next == NULL) return l; /* length is 1 */
  else { 
    half = split (l);
    return merge (merge_sort(l), merge_sort(half));
  }
}

/*
  split list with length greater than 1 into two recursively.
  One consists of elements at the even position.
  The other one consists of elements at the odd position.
*/
List split (List l)
{
  if (l == NULL) return NULL;
  else if (l->next == NULL) return NULL;
  else { /* has more than 1 element in the list */
    List temp = l->next;
    l->next = temp->next;
    temp->next = split (temp->next);
    return temp;
  }
}

/*
  Here, we assume that tow lists have already been
  sorted well. We just need to merge them together.
*/
List merge (List left, List right)
{
  if (left == NULL) return right;
  else if (right == NULL) return left;
  else { /* both are not empty */
    if (left->element <= right->element) {
      /* recursively merge remaining of left
	 and the entire right */
      left->next = merge (left->next, right);
      return left;
    } else {
      /* recursively merge remaining of right
	 and the entire left */
      right->next = merge (left, right->next);
      return right;
    }
  }
}
