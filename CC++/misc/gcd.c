/*
  Recursively compute the greatest common divisor
  (GCD) of two integer i and j.

  Recursive definition of GCD (assume i > j):
  Basis:
  If j divides i evenly, then j is the GCD.
  Induction:
  If j does not divide i evenly, let k be the remainder
  of i/j, then gcd(i,j) equals with gcd(j,k).
*/

#include <stdio.h>

int gcd (int i, int j);

int main () 
{
  int i, j, g;

  printf ("Program to compute " 
           "Greatest Common Divisor recursivly\n");
  printf ("Input two integers:\n");

  scanf ("%d %d", &i, &j);
  printf ("Greatest Common Divisor is: %d\n",
	   i >= j ? gcd (i, j) : gcd (j, i));

  return 0;
}

/*
  Function to compute GCD recursively.
  Assume the i >= j
*/
int gcd (int i, int j) 
{
  int g;
  if (j == 0) return i;
  else {
    g = gcd (j, i%j);
    return g;
  }
}
