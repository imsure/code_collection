/*
  Recursive algorithm for selection sort:
    Basis: 
      If i = n-1, then only the last element
      of the array remains to be sorted. Since any
      one element is already sorted, we need not do anyting.
    Indution: 
      If i < n-1, then find the smallest element
      in A[i...n-1], swap it with A[i], and recursively
      sort A[i+1...n-1].
    This is called backward induction:
      Start with a high basis and by the inductive rule
      solve instances with smaller value of the parameter
      in terms of instances with higher value.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void rec_sel_sort (int a[], int N, int basis);

int main () 
{
  int i, N;
  int a[MAX];

  printf ("Enter the numbers you want to sort:\n");
  for (N = 0; N < MAX && scanf("%d", &a[N]) != EOF; N++) 
    ;

  printf ("Unsorted list:\n");
  for (i = 0; i < N; i++) {
    printf ("%d ", a[i]);
  }
  printf ("\n");

  rec_sel_sort (a, N, 0);
  printf ("Sorted list:\n");
  for (i = 0; i < N; i++) {
    printf ("%d ", a[i]);
  }
  printf ("\n");

  return 0;
}

/*
   a[] : array needs to be sorted
   N : number of elements in array
   basis : index of first element
           in the unsorted tail of array
*/
void rec_sel_sort (int a[], int N, int basis) 
{
  int small, temp, i;
  
  /* boudary check */
  if (basis < 0 || basis >= N) {
    printf ("Wrong basis value!\
            It should be between 0 and %d\n", N-1);
    exit (-1);
  }

  /* base case is basis == N-1 in which we need do nothing */
  if (basis < N-1) { /* recursion */
    /* select the smallest element among a[basis..N-1] */
    small = basis;
    for (i=basis+1; i<N; i++) {
      if (a[i] < a[small]) {
	small = i;
      }
    }
    /* swap */
    temp = a[basis];
    a[basis] = a[small];
    a[small] = temp;
    /* recursively sort a[basis+1..N-1] */
    rec_sel_sort (a, N, ++basis);
  }
}
