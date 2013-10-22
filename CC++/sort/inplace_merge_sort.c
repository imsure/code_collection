/*
 * Implementation of a in-place merge sort algorithm.
 */

#include <stdio.h>

/*
 * Implementation of in-place merge.
 * Other than an integer used for temporary storage, no extra spaces needed.
 * 'array' is split into two parts, one from 'left' to 'mid',
 * the other from 'mid'+1 to 'right'. Two parts have been sorted.
 */
void merge(int *array, int left, int mid, int right)
{
  while (left <= mid && mid < right) {
    if (array[left] > array[mid+1]) {
      // Save the (mid+1)th element.
      int temp = array[mid+1];
      // Shift all elements from 'left' to 'mid' one position to the right.
      int i;
      for (i = mid+1; i > left; --i) {
	array[i] = array[i-1];
      }
      // Put saved (mid+1)th element to the right place.
      array[left] = temp;

      // Update index
      left++;
      mid++;
    } else {
      // Keep 'mid' as the same in this case.
      // We must ensure 'mid' split the array into two sorted part.
      left++; 
    }
  }
}

/*
 * Function performs merge sort.
 * 'array' is an unsorted array, starts from 'low', ends at 'high'.
 */
void merge_sort(int *array, int low, int high)
{
  if (low < high) { // array has not been split to one element
    int mid = (high + low) / 2;
    merge_sort(array, low, mid);
    merge_sort(array, mid+1, high);
    merge(array, low, mid, high);
  }
}


int main()
{
  int i;
  int a[] = {5, 4, 3, 2, 1, 9, 7, -9};
  //int a[] = {1, 2, 3, 4, 5, -9};

  merge_sort(a, 0, sizeof(a)/sizeof(int) - 1);
  for (i = 0; i < sizeof(a)/sizeof(int); ++i) {
    printf("%d\n", a[i]);
  }

  return 0;
}
