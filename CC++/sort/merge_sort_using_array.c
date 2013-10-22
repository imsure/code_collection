/*
  Merge Sort :
  an application of Divide-and-Conquer.
  Implemented using array. It is not en elegant
  way of implementation, since when passing an array 
  to a function, it doesn't know array's size, and
  array itself cannot dynamically grow. Using a linked
  list is a good idea as shown in the book which provides
  an thorough way of recursive implementation.
  
  Algorithm:
  1. Split list into sub-lists with only one element
     which is sorted by default.
  2. Repeatedly merge these sub-lists to produce new
     sub-lists until there is only one list remaining
     (this will be sorted list).
  
  Pseudo Code:
  list L[0..N-1] # list with N elements to be sorted
  FUNC merge_sort(list L, index start, index end)
    IF (size of L is one) THEN return a copy of L
    ELSE // split the list into two sublists
      middle = (end - begin) / 2
      list left = merge_sort (L, start, start+middle)
      list right = merge_sort (L, start+middle+1, end)
    RETURN merge (left, right)
*/

#define MAX 100

#include <stdio.h>
#include <stdlib.h>

int * merge_sort (int a[], int begin, int end);

int main ()
{
  int i, N;
  int a[MAX], *b;

  printf ("Enter the numbers you want to sort:\n");
  for (N = 0; N < MAX && scanf("%d", &a[N]) != EOF; N++) 
    ;

  printf ("Unsorted list:\n");
  for (i = 0; i < N; i++) {
    printf ("%d ", a[i]);
  }
  printf ("\n");
  
  b = merge_sort (a, 0, N-1);

  printf ("\nsorted list:\n");
  for (i = 0; i < N; i++) {
    printf ("%d ", b[i]);
  }
  printf ("\n");

  free (b);
  return 0;
}

int * merge_sort (int a[], int begin, int end)
{
  if (begin == end) { /* only one element in array
			 (thus sorted) */
    int * temp = (int *) malloc (sizeof(int));
    temp[0] = a[begin];
    return temp;
  } else { /* split */
    int * left, * right, * temp;
    int i = 0; /* keep track of left side of array */
    int j = 0; /* keep track of right side of array */
    int k = 0; /* keep track of the temp array */
    int mid = (end - begin) / 2;

    left = merge_sort (a, begin, begin + mid); /* split left side */
    right = merge_sort (a, begin + mid + 1, end); /* split right side */

    temp = (int *) malloc ((end-begin+1) * sizeof(int));
    /* merge */
    while (i <= mid) {
      if (j <= (end-begin-mid-1)) {
	if (left[ i ] <= right[ j ]) {
	  temp[ k++ ] = left[ i++ ];
	} else {
	  temp[ k++ ] = right[ j++ ];
	}
      } else { 
	temp[ k++ ] = left[ i++ ];
      }
    }
    while (j <= (end-begin-mid-1)) {
      temp[ k++ ] = right[ j++ ];
    }
    free (left);
    free (right);
    return temp;
  }
}
