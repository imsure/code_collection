/*
 * Insertion Sort Implementation
 */

#include <stdio.h>

void insertion_sort (int *a, int size)
{
	int i,j, key;

	for (j = 1; j < size; j++) {
		key = a[j];
		i = j - 1;
		while (i >= 0 && a[i] > key) {
			a[i + 1] = a[i];
			i--;
		}
		a[i + 1] = key;	// replace key
	}
}

int main (int argc, char *argv[])
{
	int i;
	int a[] = {5, 8, 3, 11, 2, 1, 9};
	
	insertion_sort (a, sizeof(a)/sizeof(int));
	for (i = 0; i < 7; i++) printf ("%i ", a[i]);
	printf ("\n");

	return 0;
}

