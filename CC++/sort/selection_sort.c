/*
  -- implementation of selection sort

  -- description of selection sort:
  From wikipedia:
  It is specially an in-place comparison,
  with O(n²) time complexity, make it inefficient for large lists,
  and generally performs worse than the similar insertion sort.
  However, it is noted for its simplicity, and also has performance
  advantages over more complicated algorithms in certain situations,
  particularly where auxiliary memory is limited.

  It outperforms bubble sort.

  -- algorithm:
  N := number of elements need to sort
  A := array in which elements are stored
  For i from 0 to N-2 Do
    Find the smallest element among [i, N-1]
    Swap positions between element i and the smallest element
  END
 */

#include <stdio.h>

#define MAX 100

void selection_sort (int array[], int N);

int main () 
{
  //int a[] = {4, 1, 0, 7, 9, 5, 4, 1, 2, 9};
  int i, N;
  int a[MAX];

  //N = sizeof(a) / sizeof(int);
  printf ("Enter the numbers you want to sort:\n");
  for (N = 0; N < MAX && scanf("%d", &a[N]) != EOF; N++) 
    ;

  printf ("Unsorted list:\n");
  for (i = 0; i < N; i++) {
    printf ("%d ", a[i]);
  }

  selection_sort (a, N);

  printf ("\nsorted list:\n");
  for (i = 0; i < N; i++) {
    printf ("%d ", a[i]);
  }
  printf ("\n");

  return 0;
}

/*
  Function to perform selection sort
  Parameters:
    -- array: array need to sort
    -- N: number of element in the array
 */
void selection_sort (int array[], int N)
{
  int i, j, smallest, temp;

  for (i = 0 ; i < N-1; i++) {
    /* set smallest to the index of the first
       occurrence of the smallest element remaining */
    smallest = i;
    for (j = i+1; j < N; j++) {
      if (array[smallest] > array[j]) {
	smallest = j;
      }
    }
    /* when reaching here, smallest is the index
       of the smallest element in array[i, N-1].
       Now exchange array[i] with array[smallest] */
    temp = array[i];
    array[i] = array[smallest];
    array[smallest] = temp;
  }
}
