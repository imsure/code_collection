/*
 * Implementation of in-place quick sort.
 */

import java.util.ArrayList;
import java.util.Random;
import java.util.Collections;

public class QuickSort extends IntegerSort {

    private Random generator;
    private final boolean DEBUG = false;

    public QuickSort() {
	generator = new Random();
    }

    /*
     * A in-place partition function which partitions portion of array 'data'
     * between indexes 'left' and 'right' by removing all elements less than
     * pivot which is data[pivot_index] before the pivot and greater or equal
     * elements after it. It also returns the final index of pivot.
     */
    private int partition(ArrayList<Integer> data,
			  int left, int right, int pivot_index) {

	// Temperary pivot index used to keep track
	// of moving pivot position. Begins with 'left'.
	int temp_pivot_index = left;

	int pivot = data.get(pivot_index);

	// First swap pivot with the rightmost element.
	Collections.swap(data, pivot_index, right);

	for (int i = left; i < right; i++) {
	    super.incNumOfComparison(1);
	    if (data.get(i) < pivot) {
		// Find an element less than pivot which means
		// it is time to update pivot position to put smaller element
		// to the left of pivot.
		Collections.swap(data, temp_pivot_index, i);
		temp_pivot_index++;
	    }
	    /*Note that if the data element is greater than or equal with pivot, do nothing
	      since this is just what we want: larger or equal elements are on the right side
	      of current pivot position indicated by 'temp_pivot_index'. */
	}
	// Finally, 'temp_pivot_index' is the final position where pivot
	// should be.
	int final_pivot_index = temp_pivot_index;
	Collections.swap(data, final_pivot_index, right);

	if (DEBUG) {
	    System.out.println("Left side:");
	    for (int i = left; i < final_pivot_index; i++) {
		System.out.println(data.get(i));
	    }

	    System.out.println("Pivot: " + data.get(final_pivot_index));

	    System.out.println("Right side:");
	    for (int i = final_pivot_index+1; i <= right; i++) {
		System.out.println(data.get(i));
	    }
	}

	return final_pivot_index;
    }

    /*
     * Implementation of in-place quick sort algorithm.
     * It recursively sorts the array 'data' between indexes
     * 'left' and 'right' in-place.
     */
    private void qsort(ArrayList<Integer> data, int left, int right) {

	// Base case (left >= right, with zero or one element in array
	// which is sorted by default.)
	if (left < right) { 

	    /* Randomly pick up a pivot index. left <= pivot_index <= right. */
	    int pivot_index = left + generator.nextInt(right-left+1);
	    int new_pivot_index = partition(data, left, right, pivot_index);

	    qsort(data, left, new_pivot_index-1);
	    qsort(data, new_pivot_index+1, right);
	}
    }
	
    public ArrayList<Integer> sort(ArrayList<Integer> array) {
	long start = System.currentTimeMillis();
	qsort(array, 0, array.size()-1);
	long end = System.currentTimeMillis();

	long exectime = end - start;
        super.setExecTimeInMills(exectime);

	long secs = exectime / 1000;
	long mills = exectime - 1000 * secs;
	System.out.println("Sorting time: " + secs + " seconds and " + mills + " millisecs");
	System.out.println("Number of Comparison: " + super.getNumOfComparison());

	return array;
    }

    public static void main(String[] args) {
	ArrayList<Integer> test = new ArrayList<Integer>();
	test.add(3);
	test.add(5);
	test.add(4);
	test.add(1);
	test.add(2);
	test.add(99);
	test.add(84);
	test.add(0);
	test.add(-1);
	test.add(-999);

	QuickSort qs = new QuickSort();
	qs.sort(test);

	System.out.println("Sort results:");
	for (int d : test) {
	    System.out.println(d);
	}
	System.out.print('\n');
    }
}